#include <bulletlua/BulletLuaManager.hpp>
#include <bulletlua/BulletLua.hpp>

#include <bulletlua/Utils/Rng.hpp>
#include <bulletlua/Utils/Math.hpp>

BulletLuaManager::BulletLuaManager(int left, int top, int width, int height, const BulletLuaUtils::Rect& playerp)
    : current{0},
      player(playerp),
      rank{0.8},
      rng{}
{
    position.reserve(BLOCK_SIZE);
    attributes.reserve(BLOCK_SIZE);
    lifeData.reserve(BLOCK_SIZE);
    functions.reserve(BLOCK_SIZE);
}

// Create a root bullet from an external script.
void BulletLuaManager::createBulletFromFile(const std::string& filename)
{
    std::shared_ptr<sol::state> luaState{initLua()};
    luaState->open_file(filename);

    positions.emplace_back({320.0f, 120.0f});
    attributes.emplace_back({0.0f, 0.0f});
    lifeData.emplace_back({1.0f, 0});
    functions.emplace_back({luaState, luaState->get<sol::function>("main")});
}

// Create a root bullet from an embedded script.
void BulletLuaManager::createBulletFromScript(const std::string& script,
                                              Bullet* origin)
{
    std::shared_ptr<sol::state> luaState{initLua()};
    luaState->script(script);

    positions.emplace_back({320.0f, 120.0f});
    attributes.emplace_back({0.0f, 0.0f});
    lifeData.emplace_back({1.0f, 0});
    functions.emplace_back({luaState, luaState->get<sol::function>("main")});
}

// Create Child Bullet
void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua,
                                    const sol::function& func,
                                    float x, float y, float vx, float vy)
{
    positions.emplace_back({x, y});
    attributes.emplace_back({vx, vy});
    lifeData.emplace_back({1.0f, 0});
    functions.emplace_back({lua, luaState->get<sol::function>("main")});
}

void BulletLuaManager::tick()
{
    float numDead{0.0f};
    std::size_t size = lifeData.size();

    for (std::size_t i = 0; i < size; ++i)
    {
        BulletLifeData& life = lifeData[i];
        BulletPosition& pos  = positions[i];
        BulletFunction& func = functions[i];

        current = i;

        
    }
}

// Move all bullets to the free stack
void BulletLuaManager::clear()
{
    position.resize(0);
    attributes.resize(0);
    lifeData.resize(0);
    functions.resize(0);
}

// void BulletLuaManager::vanishAll()
// {
//     for (BulletLua* b : bullets)
//     {
//         b->vanish();
//     }
// }

// unsigned int BulletLuaManager::bulletCount() const
// {
//     return bullets.size();
// }

// unsigned int BulletLuaManager::freeCount() const
// {
//     return freeBullets.size();
// }

// unsigned int BulletLuaManager::blockCount() const
// {
//     return blocks.size();
// }

// // Returns an unused bullet. Allocates more data blocks if there none are available
// BulletLua* BulletLuaManager::getFreeBullet()
// {
//     BulletLua* bullet = freeBullets.top();
//     freeBullets.pop();

//     if (freeBullets.empty())
//     {
//         increaseCapacity();
//     }

//     return bullet;
// }

// void BulletLuaManager::increaseCapacity(unsigned int blockSize)
// {
//     blocks.push_back(new BulletLua[blockSize]);

//     // Throw all bullets into free stack
//     for (unsigned int i = 0; i < blockSize; ++i)
//     {
//         freeBullets.push(&blocks.back()[i]);
//     }

//     // Subclasses should override this method if their extensions depends on block size.
//     // E.g. allocation of vertices or bookkeeping of vertices in a VBO.
//     // Keep in mind that this original version will be called in the default constructor.
// }

std::shared_ptr<sol::state> BulletLuaManager::initLua()
{
    std::shared_ptr<sol::state> luaState(new sol::state);

    luaState->open_libraries(sol::lib::base);
    luaState->open_libraries(sol::lib::math);
    luaState->open_libraries(sol::lib::table);

    luaState->set_function("nullfunc",
                           [&]()
                           {
                           });

    luaState->set_function("getPosition",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return std::make_tuple(c->position.x, c->position.y);
                           });

    luaState->set_function("getTargetPosition",
                           [&]()
                           {
                               // BulletLua* c = this->current;
                               return std::make_tuple(player.x, player.y);
                           });

    luaState->set_function("getVelocity",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return std::make_tuple(c->vx, c->vy);
                           });

    luaState->set_function("getSpeed",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return c->getSpeed();
                           });

    luaState->set_function("getDirection",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return Math::radToDeg(c->getDirection());
                           });

    luaState->set_function("setCollision",
                           [&](bool collision)
                           {
                               BulletLua* c = this->current;
                               c->collisionCheck = collision;
                           });

    luaState->set_function("getLife",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return c->life;
                           });

    luaState->set_function("getTurn",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return c->getTurn();
                           });

    luaState->set_function("resetTurns",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               c->turn = 0;
                           });

    luaState->set_function("getRank",
                           [&]()
                           {
                               return BulletLuaManager::rank;
                           });

    luaState->set_function("randFloat",
                           [&]()
                           {
                               return rng.float_01();
                           });

    luaState->set_function("randFloatRange",
                           [&](float min, float max)
                           {
                               return rng.floatRange(min, max);
                           });

    luaState->set_function("randInt",
                           [&](int max)
                           {
                               return rng.int_64(0, max);
                           });

    luaState->set_function("randIntRange",
                           [&](int min, int max)
                           {
                               return rng.int_64(min, max);
                           });

    luaState->set_function("setPosition",
                           [&](float x, float y)
                           {
                               BulletLua* c = this->current;
                               c->setPosition(x, y);
                           });

    luaState->set_function("setVelocity",
                           [&](float vx, float vy)
                           {
                               BulletLua* c = this->current;
                               c->setVelocity(vx, vy);
                           });

    luaState->set_function("setDirection",
                           [&](float dir)
                           {
                               BulletLua* c = this->current;
                               c->setDirection(Math::degToRad(dir));
                           });

    luaState->set_function("setDirectionRelative",
                           [&](float dir)
                           {
                               BulletLua* c = this->current;
                               c->setDirectionRelative(Math::degToRad(dir));
                           });

    luaState->set_function("aimTarget",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               c->aimAtPoint(player.x, player.y);
                           });

    luaState->set_function("aimPoint",
                           [&](float x, float y)
                           {
                               BulletLua* c = this->current;
                               c->aimAtPoint(x, y);
                           });

    luaState->set_function("setSpeed",
                           [&](float s)
                           {
                               BulletLua* c = this->current;
                               c->setSpeed(s);
                           });

    luaState->set_function("setSpeedRelative",
                           [&](float s)
                           {
                               BulletLua* c = this->current;
                               c->setSpeedRelative(s);
                           });

    luaState->set_function("linearInterpolate",
                           [&](float x, float y, unsigned int steps)
                           {
                               BulletLua* c = this->current;
                               c->vx = (x - c->position.x) / steps;
                               c->vy = (y - c->position.y) / steps;
                           });

    luaState->set_function("setFunction",
                           [&](const sol::function& func)
                           {
                               BulletLua* c = this->current;
                               c->setFunction(func);
                           });

    luaState->set_function("fire",
                           [&](float d, float s,
                               const sol::function& func)
                           {
                               BulletLua* c = this->current;
                               if (c->dying)
                                   return;

                               this->createBullet(c->luaState, func,
                                                  c->position.x, c->position.y,
                                                  Math::degToRad(d), s);
                           });

    luaState->set_function("fireAtTarget",
                           [&](float s,
                               const sol::function& func)
                           {
                               BulletLua* c = this->current;
                               if (c->dying)
                                   return;

                               this->createBullet(c->luaState, func,
                                                  c->position.x, c->position.y,
                                                  c->getAimDirection(player.x,
                                                                     player.y),
                                                  s);
                           });

    luaState->set_function("fireCircle",
                           [&](int segments, float s,
                               const sol::function& func)
                           {
                               BulletLua* c = this->current;
                               if (c->dying)
                                   return;

                               float segRad = Math::PI * 2 / segments;
                               for (int i = 0; i < segments; ++i)
                               {
                                   this->createBullet(c->luaState, func,
                                                      c->position.x, c->position.y,
                                                      segRad * i, s);
                               }
                           });

    luaState->set_function("setColor",
                           [&](unsigned char r, unsigned char g, unsigned char b)
                           {
                               BulletLua* c = this->current;
                               c->setColor(r, g, b);
                           });

    luaState->set_function("getColor",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               return std::make_tuple(c->r, c->g, c->b);
                           });

    luaState->set_function("vanish",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               c->vanish();
                           });

    luaState->set_function("kill",
                           [&]()
                           {
                               BulletLua* c = this->current;
                               c->kill();
                           });

    return luaState;
}

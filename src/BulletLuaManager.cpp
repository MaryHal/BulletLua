#include <bulletlua/BulletLuaManager.hpp>
#include <bulletlua/BulletLua.hpp>

#include <bulletlua/Utils/Rng.hpp>
#include <bulletlua/Utils/Math.hpp>

#include <cassert>

BulletLuaManager::BulletLuaManager(int left, int top, int width, int height, const BulletLuaUtils::Rect& playerp)
    : current{nullptr},
      player(playerp),
      rank{0.8},
      collision{BulletLuaUtils::Rect{float(left), float(top), float(width), float(height)}},
      rng{}
{
    // Initialize our vector like an array. Ensure we have at least BLOCK_SIZE elements.
    bullets.resize(BLOCK_SIZE);

    // Setup our linked list.
    clear();
}

BulletLuaManager::~BulletLuaManager()
{
}

// Create a root bullet from an external script.
void BulletLuaManager::createBulletFromFile(const std::string& filename,
                                            Bullet* origin)
{
    BulletLua* b = getFreeBullet();

    std::shared_ptr<sol::state> luaState = initLua();
    luaState->open_file(filename);

    b->set(luaState,
           luaState->get<sol::function>("main"),
           origin);

    // bullets.push_back(b);
}

// Create a root bullet from an embedded script.
void BulletLuaManager::createBulletFromScript(const std::string& script,
                                              Bullet* origin)
{
    BulletLua* b = getFreeBullet();

    std::shared_ptr<sol::state> luaState = initLua();
    luaState->script(script);

    b->set(luaState,
           luaState->get<sol::function>("main"),
           origin);

    // bullets.push_back(b);
}

// Create Child Bullet
void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua,
                                    const sol::function& func,
                                    float x, float y, float d, float s)
{
    BulletLua* b = getFreeBullet();

    b->set(lua, func, x, y, d, s);
    // bullets.push_back(b);
}

bool BulletLuaManager::checkCollision()
{
    return collision.checkCollision(player);
}

void BulletLuaManager::tick()
{
    // Reset containers inside collision detection object.
    // Since bullets are dynamic and are most likely unpredictable,
    // we must repopulate the containers each frame.
    collision.reset();

    for (unsigned int i = 0; i < BLOCK_SIZE; ++i)
    {
        // Must be set so lua knows which bullet to modify.
        current = &bullets[i];
        current->run(collision);

        if (current->isDead())
        {
            current->setNext(firstAvailable);
            firstAvailable = current;
            continue;
        }

        collision.addBullet(current);

        // if ((*iter)->collisionCheck)
        // {
        //     collision.addBullet(*iter);
        // }
    }
}

// Move all bullets to the free stack
void BulletLuaManager::clear()
{
    // Setup our linked list.
    firstAvailable = bullets.data();
    for (unsigned int i = 0; i < BLOCK_SIZE - 1; ++i)
    {
        bullets[i].setNext(&bullets[i + 1]);
    }

    // Terminate our list
    bullets[BLOCK_SIZE - 1].setNext(nullptr);
}

void BulletLuaManager::vanishAll()
{
    for (auto b : bullets)
    {
        b.vanish();
    }
}

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

// Returns an unused bullet. Allocates more data blocks if there none are available
BulletLua* BulletLuaManager::getFreeBullet()
{
    if (firstAvailable == nullptr)
    {
        bullets.resize(bullets.capacity() + BLOCK_SIZE);
    }

    BulletLua* b = firstAvailable;
    firstAvailable = (BulletLua*)b->getNext();

    return b;
}

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
                               return std::make_tuple(c->state.live.x, c->state.live.y);
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
                               return std::make_tuple(c->state.live.vx, c->state.live.vy);
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

    // luaState->set_function("setCollision",
    //                        [&](bool collision)
    //                        {
    //                            BulletLua* c = this->current;
    //                            c->collisionCheck = collision;
    //                        });

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
                               c->state.live.vx = (x - c->state.live.x) / steps;
                               c->state.live.vy = (y - c->state.live.y) / steps;
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
                               if (c->isDying())
                                   return;

                               this->createBullet(c->luaState, func,
                                                  c->state.live.x, c->state.live.y,
                                                  Math::degToRad(d), s);
                           });

    luaState->set_function("fireAtTarget",
                           [&](float s,
                               const sol::function& func)
                           {
                               BulletLua* c = this->current;
                               if (c->isDying())
                                   return;

                               this->createBullet(c->luaState, func,
                                                  c->state.live.x, c->state.live.y,
                                                  c->getAimDirection(player.x,
                                                                     player.y),
                                                  s);
                           });

    luaState->set_function("fireCircle",
                           [&](int segments, float s,
                               const sol::function& func)
                           {
                               BulletLua* c = this->current;
                               if (c->isDying())
                                   return;

                               float segRad = Math::PI * 2 / segments;
                               for (int i = 0; i < segments; ++i)
                               {
                                   this->createBullet(c->luaState, func,
                                                      c->state.live.x, c->state.live.y,
                                                      segRad * i, s);
                               }
                           });

    // luaState->set_function("setColor",
    //                        [&](unsigned char r, unsigned char g, unsigned char b)
    //                        {
    //                            BulletLua* c = this->current;
    //                            c->setColor(r, g, b);
    //                        });

    // luaState->set_function("getColor",
    //                        [&]()
    //                        {
    //                            BulletLua* c = this->current;
    //                            return std::make_tuple(c->r, c->g, c->b);
    //                        });

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

#include <bulletlua/BulletLuaManager.hpp>

#include <bulletlua/Utils/Rng.hpp>
#include <bulletlua/Utils/Math.hpp>

#include <iostream>

const std::size_t BulletLuaManager::MAX_BULLETS;

BulletLuaManager::BulletLuaManager()
    : firstAvailable{nullptr},
      activeCount{0},
      current{0},
      playerX{0.0f}, playerY{0.0f},
      rank{0.8},
      collision{BulletLuaUtils::Rect{0.0f, 0.0f, 640.0f, 480.0f}},
      rng{}
{
    bullets.reserve(MAX_BULLETS << 2);
    bullets.resize(MAX_BULLETS);

    firstAvailable = &bullets[0];

    // Create our free list
    for (std::size_t i = 0; i < bullets.size() - 1; ++i)
    {
        bullets[i].setNext(&bullets[i + 1]);
    }

    // Terminate our linked list
    bullets[bullets.size() - 1].setNext(nullptr);
}

// Create a root bullet from an external script.
void BulletLuaManager::createBulletFromFile(const std::string& filename,
                                            float originX, float originY)
{
    std::shared_ptr<sol::state> luaState{initLua()};
    luaState->open_file(filename);

    Bullet* b = getFreeBullet();

    b->setPosition(originX, originY);
    b->luaState = luaState;
    b->func = luaState->get<sol::function>("main");
}

// Create a root bullet from an embedded script.
void BulletLuaManager::createBulletFromScript(const std::string& script,
                                              float originX, float originY)
{
    std::shared_ptr<sol::state> luaState{initLua()};
    luaState->script(script);

    Bullet* b = getFreeBullet();

    b->setPosition(originX, originY);
    b->luaState = luaState;
    b->func = luaState->get<sol::function>("main");
}

// Create Child Bullet
void BulletLuaManager::createBullet(std::shared_ptr<sol::state> luaState,
                                    const sol::function& func,
                                    float x, float y, float vx, float vy)
{
    Bullet* b = getFreeBullet();

    b->setPosition(x, y);
    b->setVelocity(vx, vy);
    b->luaState = luaState;
    b->func = func;
}

bool BulletLuaManager::checkCollision()
{
    return collision.checkCollision(BulletLuaUtils::Rect{playerX - 2.0f, playerY - 2.0f, 4.0f, 4.0f});
}

void BulletLuaManager::tick()
{
    // Reset containers inside collision detection object.
    // Since bullets are dynamic and are most likely unpredictable,
    // we must repopulate the containers each frame.
    collision.reset();

    int killed{0};
    for (std::size_t i = 0; i < activeCount; ++i)
    {
        // Must be set so our lua functions know which bullet to modify.
        current = i;

        bullets[i].func.call();
        bullets[i].update();

        if (collision.checkOutOfBounds(BulletLuaUtils::Rect{bullets[i].state.live.x - 2.0f, bullets[i].state.live.y - 2.0f, 4.0f, 4.0f}))
        {
            bullets[i].setNext(firstAvailable);
            firstAvailable = &bullets[i];

            killed++;
        }

        if (bullets[i].isDead())
        {
            bullets[i].setNext(firstAvailable);
            firstAvailable = &bullets[i];

            killed++;
        }

        collision.addBullet(&bullets[i]);
    }
    activeCount -= killed;
    current = 0;
}

const Bullet* BulletLuaManager::yield()
{
    if (current < activeCount)
    {
        return &bullets[current++];
    }

    return nullptr;
}

void BulletLuaManager::setPlayerPosition(float x, float y)
{
    playerX = x;
    playerY = y;
}

// Move all bullets to the free stack
void BulletLuaManager::clear()
{
    bullets.clear();
    bullets.resize(MAX_BULLETS);

    firstAvailable = &bullets[0];

    // Create our free list
    for (std::size_t i = 0; i < bullets.size() - 1; ++i)
    {
        bullets[i].setNext(&bullets[i + 1]);
    }

    // Terminate our linked list
    bullets[bullets.size() - 1].setNext(nullptr);
}

// void BulletLuaManager::vanishAll()
// {
//     for (BulletLua* b : bullets)
//     {
//         b->vanish();
//     }
// }

std::size_t BulletLuaManager::getContainerSize() const
{
    return bullets.size();
}

std::size_t BulletLuaManager::getActiveBulletCount() const
{
    return activeCount;
}

// Returns an unused bullet. Allocates more data blocks if there none are available
Bullet* BulletLuaManager::getFreeBullet()
{
    // Move ahead in our linked list
    Bullet* b = firstAvailable;
    firstAvailable = b->getNext();

    activeCount++;

    // If we don't have anymore bullets, resize our container.
    if (firstAvailable == nullptr)
    {
        bullets.reserve(MAX_BULLETS * 2);

        std::size_t oldSize = bullets.size();
        bullets.resize(oldSize * 2);

        firstAvailable = &bullets[oldSize];

        // Create our free list
        for (std::size_t i = oldSize; i < bullets.size() - 1; ++i)
        {
            bullets[i].setNext(&bullets[i + 1]);
        }

        // Terminate our linked list
        bullets[bullets.size() - 1].setNext(nullptr);
    }

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
                               return std::make_tuple(bullets[current].state.live.x, bullets[current].state.live.y);
                           });

    luaState->set_function("getTargetPosition",
                           [&]()
                           {
                               return std::make_tuple(playerX, playerY);
                           });

    luaState->set_function("getVelocity",
                           [&]()
                           {
                               return std::make_tuple(bullets[current].state.live.vx, bullets[current].state.live.vy);
                           });

    luaState->set_function("getSpeed",
                           [&]()
                           {
                               return bullets[current].getSpeed();
                           });

    luaState->set_function("getDirection",
                           [&]()
                           {
                               return Math::radToDeg(bullets[current].getDirection());
                           });

    // luaState->set_function("setCollision",
    //                        [&](bool collision)
    //                        {
    //                            bullets[current].collisionCheck = collision;
    //                        });

    luaState->set_function("getLife",
                           [&]()
                           {
                               return bullets[current].life;
                           });

    luaState->set_function("getTurn",
                           [&]()
                           {
                               return bullets[current].getTurn();
                           });

    luaState->set_function("resetTurns",
                           [&]()
                           {
                               bullets[current].turn = 0;
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
                               bullets[current].setPosition(x, y);
                           });

    luaState->set_function("setVelocity",
                           [&](float vx, float vy)
                           {
                               bullets[current].setVelocity(vx, vy);
                           });

    luaState->set_function("setDirection",
                           [&](float dir)
                           {
                               bullets[current].setDirection(Math::degToRad(dir));
                           });

    luaState->set_function("setDirectionRelative",
                           [&](float dir)
                           {
                               bullets[current].setDirectionRelative(Math::degToRad(dir));
                           });

    luaState->set_function("aimTarget",
                           [&]()
                           {
                               bullets[current].aimAtPoint(playerX, playerY);
                           });

    luaState->set_function("aimPoint",
                           [&](float x, float y)
                           {
                               bullets[current].aimAtPoint(x, y);
                           });

    luaState->set_function("setSpeed",
                           [&](float s)
                           {
                               bullets[current].setSpeed(s);
                           });

    luaState->set_function("setSpeedRelative",
                           [&](float s)
                           {
                               bullets[current].setSpeedRelative(s);
                           });

    luaState->set_function("linearInterpolate",
                           [&](float x, float y, unsigned int steps)
                           {
                               bullets[current].setVelocity((x - bullets[current].state.live.x) / steps,
                                                            (y - bullets[current].state.live.y) / steps);
                           });

    luaState->set_function("setFunction",
                           [&](const sol::function& func)
                           {
                               // bullets[current].setFunction(func);
                               bullets[current].func = func;
                           });

    luaState->set_function("fire",
                           [&](float d, float s,
                               const sol::function& func)
                           {
                               if (bullets[current].isDying())
                                   return;

                               this->createBullet(bullets[current].luaState, func,
                                                  bullets[current].state.live.x, bullets[current].state.live.y,
                                                  Math::degToRad(d), s);
                           });

    luaState->set_function("fireAtTarget",
                           [&](float s,
                               const sol::function& func)
                           {
                               if (bullets[current].isDying())
                                   return;

                               this->createBullet(bullets[current].luaState, func,
                                                  bullets[current].state.live.x, bullets[current].state.live.y,
                                                  bullets[current].getAimDirection(playerX, playerY),
                                                  s);
                           });

    luaState->set_function("fireCircle",
                           [&](int segments, float s,
                               const sol::function& func)
                           {
                               if (bullets[current].isDying())
                                   return;

                               float segRad = Math::PI * 2 / segments;
                               for (int i = 0; i < segments; ++i)
                               {
                                   this->createBullet(bullets[current].luaState, func,
                                                      bullets[current].state.live.x, bullets[current].state.live.y,
                                                      segRad * i, s);
                               }
                           });

    // luaState->set_function("setColor",
    //                        [&](unsigned char r, unsigned char g, unsigned char b)
    //                        {
    //                            bullets[current].setColor(r, g, b);
    //                        });

    // luaState->set_function("getColor",
    //                        [&]()
    //                        {
    //                            return std::make_tuple(bullets[current].r, bullets[current].g, bullets[current].b);
    //                        });

    luaState->set_function("vanish",
                           [&]()
                           {
                               bullets[current].vanish();
                           });

    luaState->set_function("kill",
                           [&]()
                           {
                               bullets[current].kill();
                           });

    return luaState;
}

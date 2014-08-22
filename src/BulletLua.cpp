#include "BulletLua.hpp"
#include "BulletLuaManager.hpp"

#include <sol.hpp>

#include "Math.hpp"

// This pointer should point to the currently "processing" bullet.
// The reason for this is that the lambda functions generated for our lua state
// are generated at compile time. This means that while many objects can
// control the lua state, the lua state can only reference the "this" pointer
// of a single bullet. The lua functions now act on this pointer instead of "this".
BulletLua* BulletLua::current = nullptr;

BulletLua::BulletLua()
    : Bullet(0.0, 0.0, 0.0, 0.0),
      target(nullptr),
      funcName(""),
      turn(0)
{
}

void BulletLua::reset()
{
    target = nullptr;

    dead = true;
    dying = false;
    life = 255;

    luaState = nullptr;
    funcName = "";
    turn = 0;

    mOwner = nullptr;
}

void BulletLua::set(const std::string& filename,
                    Bullet* origin, Bullet* target,
                    BulletLuaManager* owner)
{
    // Copy Movers
    this->x = origin->x;
    this->y = origin->y;
    this->vx = origin->vx;
    this->vy = origin->vy;

    this->target = target;

    dead = false;
    dying = false;
    life = 255;

    luaState = std::make_shared<sol::state>();
    luaState->open_file(filename);
    funcName = "main";
    turn = 0;

    mOwner = owner;

    initLua();
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const std::string& func,
                    Bullet* origin, Bullet* target,
                    BulletLuaManager* owner)
{
    // Copy Movers
    this->x = origin->x;
    this->y = origin->y;
    this->vx = origin->vx;
    this->vy = origin->vy;

    this->target = target;

    dead = false;
    dying = false;
    life = 255;

    luaState = lua;
    funcName = func;
    turn = 0;

    mOwner = owner;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const std::string& func,
                    float x, float y, float d, float s,
                    Bullet* target,
                    BulletLuaManager* owner)
{
    // Copy Movers
    this->x = x;
    this->y = y;
    this->setSpeedAndDirection(s, d);
    /* this->vx = vx; */
    /* this->vy = vy; */

    this->target = target;

    dead = false;
    dying = false;
    life = 255;

    luaState = lua;
    funcName = func;
    turn = 0;

    mOwner = owner;
}

std::shared_ptr<sol::state> BulletLua::getLuaState()
{
    return luaState;
}

int BulletLua::getTurn() const
{
    return turn;
}

bool BulletLua::isDead() const
{
    return dead;
}

void BulletLua::run()
{
    BulletLua::current = this;

    // Run lua function
    if (!funcName.empty())
        luaState->get<sol::function>(funcName).call();

    x += vx;
    y += vy;

    if (x < -100.0 || x > 700.0 ||
        y < -100.0 || y > 600.0)
    {
        dead = true;
    }

    if (dying)
    {
        // Fade out over 30 frames
        life -= 255/30;
        if (life < 0)
        {
            life = 0;
            dead = true;
        }
    }

    turn++;
}

void BulletLua::__debugRun(const std::string& code)
{
    BulletLua::current = this;
    luaState->script(code);
}

void BulletLua::initLua()
{
    // luaState->open_libraries(sol::lib::base);
    luaState->open_libraries(sol::lib::math);

    luaState->set_function("getPosition",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->x, c->y);
                           });

    luaState->set_function("getVelocity",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->vx, c->vy);
                           });

    luaState->set_function("getSpeed",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->getSpeed();
                           });

    luaState->set_function("getDirection",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return Math::radToDeg(c->getDirection());
                           });

    luaState->set_function("getTurn",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->getTurn();
                           });

    luaState->set_function("getRank",
                           []()
                           {
                               return BulletLuaManager::rank;
                           });

    luaState->set_function("randFloat",
                           [&]()
                           {
                               return mOwner->randFloat();
                           });

    luaState->set_function("randFloatRange",
                           [&](float min, float max)
                           {
                               return mOwner->randFloat(min, max);
                           });

    luaState->set_function("randInt",
                           [&](int max)
                           {
                               return mOwner->randInt(max);
                           });

    luaState->set_function("randIntRange",
                           [&](int min, int max)
                           {
                               return mOwner->randInt(min, max);
                           });

    luaState->set_function("setPosition",
                           [](float x, float y)
                           {
                               BulletLua* c = BulletLua::current;
                               c->x = x;
                               c->y = y;
                           });

    luaState->set_function("setVelocity",
                           [](float vx, float vy)
                           {
                               BulletLua* c = BulletLua::current;
                               c->vx = vx;
                               c->vy = vy;
                           });

    luaState->set_function("setDirection",
                           [](float dir)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setDirection(Math::degToRad(dir));
                           });

    luaState->set_function("setDirectionRelative",
                           [](float dir)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setDirectionRelative(Math::degToRad(dir));
                           });

    luaState->set_function("aimTarget",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               c->setDirectionAim(c->target->x, c->target->y);
                           });

    luaState->set_function("aimPoint",
                           [](float x, float y)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setDirectionAim(x, y);
                           });

    luaState->set_function("setSpeed",
                           [](float s)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setSpeed(s);
                           });

    luaState->set_function("setSpeedRelative",
                           [](float s)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setSpeedRelative(s);
                           });

    luaState->set_function("setFunction",
                           [](const std::string& funcName)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setFunctionName(funcName);
                           });

    // luaState->set_function("fire",
    //                        [](float x, float y, float d, float s,
    //                           const std::string& funcName)
    //                        {
    //                            BulletLua* c = BulletLua::current;
    //                            c->mOwner->createBullet(c->luaState, funcName,
    //                                                    x, y,
    //                                                    Math::degToRad(d), s,
    //                                                    c->target);
    //                        });

    luaState->set_function("fire",
                           [](float d, float s,
                              const std::string& funcName)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mOwner->createBullet(c->luaState, funcName,
                                                       c->x, c->y,
                                                       Math::degToRad(d), s,
                                                       c->target);
                           });

    luaState->set_function("fireCircle",
                           [](int segments, float s,
                              const std::string& funcName)
                           {
                               BulletLua* c = BulletLua::current;

                               float segRad = Math::PI * 2 / segments;
                               for (int i = 0; i < segments; ++i)
                               {
                                   c->mOwner->createBullet(c->luaState, funcName,
                                                           c->x, c->y,
                                                           segRad * i, s,
                                                           c->target);
                               }
                           });

    luaState->set_function("vanish",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               c->vanish();
                           });

    luaState->set_function("kill",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               c->kill();
                           });
}

void BulletLua::setFunctionName(const std::string& funcName)
{
    this->funcName = funcName;
}

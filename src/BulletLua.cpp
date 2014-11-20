#include "BulletLua.hpp"
#include "BulletLuaManager.hpp"

#include "Utils/Math.hpp"
#include "Utils/Rng.hpp"

#include "SpacialPartition.hpp"

// This pointer should point to the currently "processing" bullet.
// The reason for this is that the lambda functions generated for our lua state
// are generated at compile time. This means that while many objects can
// control the lua state, the lua state can only reference the "this" pointer
// of a single bullet. The lua functions now act on this pointer instead of "this".
BulletLua* BulletLua::current = nullptr;

BulletLua::BulletLua()
    : Bullet(0.0, 0.0, 0.0, 0.0),
      target(nullptr)
{
}

void BulletLua::makeReusable(Bullet* target, BulletLuaManager* owner)
{
    this->target = target;

    this->dead  = false;
    this->dying = false;
    this->life = 255;

    this->r = 255;
    this->g = 255;
    this->b = 255;

    this->turn = 0;

    this->mOwner = owner;
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

    makeReusable(target, owner);

    // Load Lua file/functions
    luaState = std::make_shared<sol::state>();
    luaState->open_file(filename);

    func = luaState->get<sol::function>("main");

    initLua();
}

// void BulletLua::set(std::shared_ptr<sol::state> lua,
//                     const sol::function& func,
//                     Bullet* origin, Bullet* target,
//                     BulletLuaManager* owner)
// {
//     // Copy Movers
//     this->x = origin->x;
//     this->y = origin->y;
//     this->vx = origin->vx;
//     this->vy = origin->vy;

//     this->target = target;

//     dead = false;
//     dying = false;
//     life = 255;

//     luaState = lua;
//     this->func = func;
//     turn = 0;

//     mOwner = owner;
// }

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const sol::function& func,
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

    makeReusable(target, owner);

    luaState = lua;
    this->func = func;
}

void BulletLua::run(const SpacialPartition& collision)
{
    BulletLua::current = this;

    // Run lua function
    if (!dead)
    {
        func.call();
    }

    x += vx;
    y += vy;

    if (collision.checkOutOfBounds(*this))
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

void BulletLua::setFunction(const sol::function& func)
{
    this->func = func;
}

void BulletLua::initLua()
{
    luaState->open_libraries(sol::lib::base);
    luaState->open_libraries(sol::lib::math);
    luaState->open_libraries(sol::lib::table);

    luaState->set_function("nullfunc",
                           []()
                           {
                           });

    luaState->set_function("getPosition",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->x, c->y);
                           });

    luaState->set_function("getTargetPosition",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->target->x, c->target->y);
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

    luaState->set_function("getLife",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->life;
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
                           []()
                           {
                               return BulletLuaUtils::Random::genFloat();
                           });

    luaState->set_function("randFloatRange",
                           [](float min, float max)
                           {
                               return BulletLuaUtils::Random::genFloat(min, max);
                           });

    luaState->set_function("randInt",
                           [](int max)
                           {
                               return BulletLuaUtils::Random::genInt(max);
                           });

    luaState->set_function("randIntRange",
                           [](int min, int max)
                           {
                               return BulletLuaUtils::Random::genInt(min, max);
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
                           [](const sol::function& func)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setFunction(func);
                           });

    luaState->set_function("fire",
                           [](float d, float s,
                              const sol::function& func)
                           {
                               BulletLua* c = BulletLua::current;
                               if (c->dying)
                                   return;

                               c->mOwner->createBullet(c->luaState, func,
                                                       c->x, c->y,
                                                       Math::degToRad(d), s,
                                                       c->target);
                           });

    luaState->set_function("fireCircle",
                           [](int segments, float s,
                              const sol::function& func)
                           {
                               BulletLua* c = BulletLua::current;
                               if (c->dying)
                                   return;

                               float segRad = Math::PI * 2 / segments;
                               for (int i = 0; i < segments; ++i)
                               {
                                   c->mOwner->createBullet(c->luaState, func,
                                                           c->x, c->y,
                                                           segRad * i, s,
                                                           c->target);
                               }
                           });

    luaState->set_function("setColor",
                           [](int r, int g, int b)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setColor(r, g, b);
                           });

    luaState->set_function("getColor",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->r, c->g, c->b);
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

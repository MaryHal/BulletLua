#include "BulletLua.hpp"
#include "BulletLuaManager.hpp"

#include <sol.hpp>

#include "Utils/Math.hpp"
#include "Utils/Rng.hpp"

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

int BulletLua::getTurn() const
{
    return turn;
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
}

void BulletLua::setFunctionName(const std::string& funcName)
{
    this->funcName = funcName;
}

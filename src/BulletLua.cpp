#include <bulletlua/BulletLua.hpp>

#include <bulletlua/SpacialPartition.hpp>

BulletLua::BulletLua()
    : Bullet{0.0, 0.0, 0.0, 0.0}
{
}

void BulletLua::makeReusable()
{
    this->life = 255;

    // this->r = 255;
    // this->g = 255;
    // this->b = 255;

    this->turn = 0;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
         const sol::function& func,
         Bullet* origin)
{
    // Copy Movers
    this->x  = origin->x;
    this->y  = origin->y;
    this->vx = origin->vx;
    this->vy = origin->vy;

    makeReusable();

    luaState = lua;
    this->func = func;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const sol::function& func,
                    float originX, float originY)
{
    // Copy Movers
    this->x = originX;
    this->y = originY;
    // this->setSpeedAndDirection(s, d);
    /* this->vx = vx; */
    /* this->vy = vy; */

    makeReusable();

    luaState = lua;
    this->func = func;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
         const sol::function& func,
         float x, float y, float d, float s)
{
    // Copy Movers
    this->x = x;
    this->y = y;
    this->setSpeedAndDirection(s, d);
    /* this->vx = vx; */
    /* this->vy = vy; */

    makeReusable();

    luaState = lua;
    this->func = func;
}

void BulletLua::run(const SpacialPartition& collision)
{
    // Run lua function
    if (!dead)
    {
        func.call();
    }

    x += vx;
    y += vy;

    if (collision.checkOutOfBounds(BulletLuaUtils::Rect{x - 2.0f, y - 2.0f, 4.0f, 4.0f}))
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

void BulletLua::setFunction(const sol::function& func)
{
    this->turn = 0;
    this->func = func;
}

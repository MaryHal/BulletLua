#include <bulletlua/BulletLua.hpp>

#include <bulletlua/SpacialPartition.hpp>

BulletLua::BulletLua()
    : Bullet{0.0, 0.0, 0.0, 0.0},
      target{nullptr}
{
}

void BulletLua::makeReusable(Bullet* target)
{
    this->target = target;

    this->dead  = false;
    this->dying = false;
    this->life = 255;

    this->r = 255;
    this->g = 255;
    this->b = 255;

    this->turn = 0;

    this->collisionCheck = true;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
         const sol::function& func,
         Bullet* origin, Bullet* target)
{
    // Copy Movers
    this->position = origin->position;
    this->vx = origin->vx;
    this->vy = origin->vy;

    makeReusable(target);

    luaState = lua;
    this->func = func;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
         const sol::function& func,
         float x, float y, float d, float s,
         Bullet* target)
{
    // Copy Movers
    this->position.x = x;
    this->position.y = y;
    this->setSpeedAndDirection(s, d);
    /* this->vx = vx; */
    /* this->vy = vy; */

    makeReusable(target);

    luaState = lua;
    this->func = func;
}

void BulletLua::run(const SpacialPartition& collision)
{
    // BulletLua::current = this;

    // Run lua function
    if (!dead)
    {
        func.call();
    }

    position.x += vx;
    position.y += vy;

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

void BulletLua::setFunction(const sol::function& func)
{
    this->turn = 0;
    this->func = func;
}

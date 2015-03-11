#include <bulletlua/BulletLua.hpp>

#include <bulletlua/SpacialPartition.hpp>

BulletLua::BulletLua()
    // : Bullet{0.0, 0.0, 0.0, 0.0}
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
    this->setPosition(origin->state.live.x, origin->state.live.y);
    this->setVelocity(origin->state.live.x, origin->state.live.y);

    makeReusable();

    luaState = lua;
    this->func = func;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const sol::function& func,
                    float x, float y, float d, float s)
{
    // Copy Movers
    this->setPosition(x, y);
    this->setSpeedAndDirection(s, d);

    makeReusable();

    luaState = lua;
    this->func = func;
}

void BulletLua::run(const SpacialPartition& collision)
{
    // Run lua function
    if (isDead())
    {
        return false;
    }
    else
    {
        func.call();
    }

    // x += vx;
    // y += vy;
    update();

    if (collision.checkOutOfBounds(BulletLuaUtils::Rect{state.live.x, state.live.y, 4.0f, 4.0f}))
    {
        kill();
    }

    if (isDying())
    {
        // Fade out over 30 frames
        life -= 255/30;
        if (life < 0)
        {
            life = 0;
            kill();
        }
    }

    turn++;

    return isDead();
}

void BulletLua::setFunction(const sol::function& func)
{
    this->turn = 0;
    this->func = func;
}

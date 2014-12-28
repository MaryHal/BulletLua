#ifndef _BulletLua_hpp_
#define _BulletLua_hpp_

#include <memory>

#include <sol.hpp>

#include "Bullet.hpp"
#include "SpacialPartition.hpp"

class BulletLua : public Bullet
{
    public:
        BulletLua()
            : Bullet(0.0, 0.0, 0.0, 0.0),
              target(nullptr)
        {
        }

        void makeReusable(Bullet* target)
        {
            this->target = target;

            this->dead  = false;
            this->dying = false;
            this->life = 255;

            this->r = 255;
            this->g = 255;
            this->b = 255;

            this->turn = 0;
        }

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 Bullet* origin, Bullet* target)
        {
            // Copy Movers
            this->x = origin->x;
            this->y = origin->y;
            this->vx = origin->vx;
            this->vy = origin->vy;

            makeReusable(target);

            luaState = lua;
            this->func = func;
        }

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 float x, float y, float d, float s,
                 Bullet* target)
        {
            // Copy Movers
            this->x = x;
            this->y = y;
            this->setSpeedAndDirection(s, d);
            /* this->vx = vx; */
            /* this->vy = vy; */

            makeReusable(target);

            luaState = lua;
            this->func = func;
        }

        void run(const SpacialPartition& collision)
        {
            // BulletLua::current = this;

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

        void setFunction(const sol::function& func)
        {
            this->turn = 0;
            this->func = func;
        }

    public:
        Bullet* target;

        std::shared_ptr<sol::state> luaState;
        sol::function func;
};

#endif // _BulletLua_hpp_

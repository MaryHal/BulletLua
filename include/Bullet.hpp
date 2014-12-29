#ifndef _Bullet_hpp_
#define _Bullet_hpp_

#include "Utils/Math.hpp"
#include <cfloat>

struct Bullet
{
    public:
        float x, y;
        float vx, vy;
        bool dead;

        unsigned char r, g, b;

        bool dying;
        int life;
        int turn;

        bool collisionCheck;

        Bullet(float x, float y, float vx, float vy)
            : x(x), y(y), vx(vx), vy(vy), dead(true),
              r(255), g(255), b(255),
              dying(true), life(0), turn(0), collisionCheck(false)
        {
            fixSpeed();
        }

        // void setBullet(float x, float y, float vx, float vy);
        void setSpeedAndDirection(float speed, float dir)
        {
            vx = speed * std::sin(dir);
            vy = -speed * std::cos(dir);

            fixSpeed();
        }

        void setSpeed(float speed)
        {
            float mag = getSpeed();

            vx = (vx * speed) / mag;
            vy = (vy * speed) / mag;

            fixSpeed();
        }

        void setSpeedRelative(float speed)
        {
            float mag = getSpeed();

            vx = (vx * (speed + mag)) / mag;
            vy = (vy * (speed + mag)) / mag;

            fixSpeed();
        }

        float getSpeed() const
        {
            return std::sqrt(vx * vx + vy * vy);
        }

        void setDirection(float dir)
        {
            float speed = getSpeed();
            vx = speed * std::sin(dir);
            vy = -speed * std::cos(dir);
        }

        void setDirectionRelative(float dir)
        {
            setDirection(dir + getDirection());
        }

        void setDirectionAim(float tx, float ty)
        {
            setDirection(Math::PI - std::atan2(tx - x, ty - y));
        }

        float getDirectionAim(float tx, float ty)
        {
            return Math::PI - std::atan2(tx - x, ty - y);
        }

        float getDirection() const
        {
            return Math::PI - std::atan2(vx, vy);
        }

        void vanish()
        {
            dying = true;
        }

        void kill()
        {
            dead = true;
        }

        bool isDead() const
        {
            return dead;
        }

        bool isDying() const
        {
            return dying;
        }

        int getTurn() const
        {
            return turn;
        }

        void setColor(unsigned char newR, unsigned char newG, unsigned char newB)
        {
            r = newR;
            g = newG;
            b = newB;
        }

    private:
        // Adjust speed if near zero as setDirection depends on at least one component
        // of our velocity vector is non-zero.
        void fixSpeed()
        {
            // See https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
            // Setting direction (alone) is dependent on components, so if speed is set to 0.0f,
            // setting a direction will not do anything.
            if (std::abs(vy) < FLT_EPSILON)
            {
                vy = FLT_EPSILON;
            }
        }
};

#endif // _Bullet_hpp_

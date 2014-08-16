#ifndef _Bullet_hpp_
#define _Bullet_hpp_

#include <SFML/Graphics.hpp>

struct Bullet
{
    public:
        float x, y;
        float vx, vy;
        bool dead;

        bool dying;
        int life;

        Bullet(float x, float y, float vx, float vy);

        void setSpeedAndDirection(float speed, float dir);

        void setSpeed(float speed);
        void setSpeedRelative(float speed);
        float getSpeed() const;

        void setDirection(float dir);
        void setDirectionRelative(float dir);
        void setDirectionAim(float tx, float ty);
        float getDirection() const;

        void vanish();
        void kill();
};

#endif // _Bullet_hpp_

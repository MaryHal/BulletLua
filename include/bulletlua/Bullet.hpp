#ifndef _Bullet_hpp_
#define _Bullet_hpp_

#include <bulletlua/Utils/Rect.hpp>

class Bullet
{
    public:
        BulletLuaUtils::Rect position;
        float vx, vy;
        bool dead;

        unsigned char r, g, b;

        bool dying;
        int life;
        int turn;

        bool collisionCheck;

    public:
        Bullet(float x, float y, float vx, float vy);

        // void setBullet(float x, float y, float vx, float vy);
        void setSpeedAndDirection(float speed, float dir);
        void setSpeed(float speed);
        void setSpeedRelative(float speed);
        float getSpeed() const;

        void setDirection(float dir);
        void setDirectionRelative(float dir);

        void aimAtPoint(float tx, float ty);
        float getAimDirection(float tx, float ty);

        float getDirection() const;

        void vanish();
        void kill();

        bool isDead() const;
        bool isDying() const;

        int getTurn() const;
        void setColor(unsigned char newR, unsigned char newG, unsigned char newB);

        void update();

    private:
        // Adjust speed if near zero as setDirection depends on at least one component
        // of our velocity vector is non-zero.
        void fixSpeed();
};

#endif // _Bullet_hpp_

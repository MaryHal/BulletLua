#ifndef _Bullet_hpp_
#define _Bullet_hpp_

#include <bulletlua/Utils/Rect.hpp>

struct BulletPosition
{
    public:
        float x, y;
        float vx, vy;
};

struct BulletAttribute
{
    public:
        float r, g, b;
};

struct BulletLifeData
{
    public:
        float life;
        int turn;
};

class Bullet
{
    public:
        const int DYING = -1;
        const int DEAD  = -2;
        union
        {
                struct
                {
                        float x, y;
                        float vx, vy;
                } live;

                Bullet* next;
        } state;

        // unsigned char r, g, b;

        int life;
        int turn;

        // bool collisionCheck;

    public:
        Bullet(float x, float y, float vx, float vy);

        Bullet* getnext() const;
        void setNext(Bullet* next);

        // void setBullet(float x, float y, float vx, float vy);

        void setPosition(float cx, float cy);
        void setVelocity(float nvx, float nvy);

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
        // void setColor(unsigned char newR, unsigned char newG, unsigned char newB);

        void update();

    private:
        // Adjust speed if near zero as setDirection depends on at least one component
        // of our velocity vector is non-zero.
        void fixSpeed();
};

#endif // _Bullet_hpp_

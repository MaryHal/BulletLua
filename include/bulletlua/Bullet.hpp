#ifndef _Bullet_hpp_
#define _Bullet_hpp_

#include <memory>
#include <sol.hpp>

class Bullet
{
    public:
        static const int DYING = -1;
        static const int DEAD  = -2;

        union
        {
                struct
                {
                        float x, y;
                        float vx, vy;
                } live;

                Bullet* next;
        } state;

        int life;
        int turn;

        std::shared_ptr<sol::state> luaState;
        sol::function func;

    public:
        Bullet();
        Bullet(float x, float y, float vx, float vy);

        Bullet* getNext() const;
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

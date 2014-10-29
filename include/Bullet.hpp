#ifndef _Bullet_hpp_
#define _Bullet_hpp_

struct Bullet
{
    public:
        static constexpr float zero = 0.0001;

    private:
        static constexpr float tolerance = 0.0001f;

    public:
        float x, y;
        float vx, vy;
        bool dead;

        bool dying;
        int life;
        int wait;

        Bullet(float x, float y, float vx, float vy);

        void setBullet(float x, float y, float vx, float vy);

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

        bool isDead() const;
        bool isDying() const;
};

#endif // _Bullet_hpp_

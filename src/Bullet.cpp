#include "Bullet.hpp"

#include "Math.hpp"

Bullet::Bullet(float x, float y, float vx, float vy)
    : x(x), y(y), vx(vx), vy(vy), dead(true), dying(true), life(0)
{
}

void Bullet::setSpeedAndDirection(float speed, float dir)
{
    vx = speed * std::sin(dir);
    vy = -speed * std::cos(dir);
}

void Bullet::setSpeed(float speed)
{
    float mag = getSpeed();

    vx = (vx * speed) / mag;
    vy = (vy * speed) / mag;
}

void Bullet::setSpeedRelative(float speed)
{
    float mag = getSpeed();

    vx = (vx * (speed + mag)) / mag;
    vy = (vy * (speed + mag)) / mag;
}

float Bullet::getSpeed() const
{
    return std::sqrt(vx * vx + vy * vy);
}

void Bullet::setDirection(float dir)
{
    float speed = getSpeed();
    vx = speed * std::sin(dir);
    vy = -speed * std::cos(dir);
}

void Bullet::setDirectionRelative(float dir)
{
    setDirection(dir + getDirection());
}

void Bullet::setDirectionAim(float tx, float ty)
{
    setDirection(Math::PI - std::atan2(tx - x, ty - y));
}

float Bullet::getDirection() const
{
    return Math::PI - std::atan2(vx, vy);
}

void Bullet::vanish()
{
    dying = true;
}

void Bullet::kill()
{
    dead = true;
}


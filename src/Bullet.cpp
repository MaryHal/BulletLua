#include "Bullet.hpp"
#include "Utils/Math.hpp"

#include <cfloat>

Bullet::Bullet(float x, float y, float vx, float vy)
    : x(x), y(y), vx(vx), vy(vy), dead(true), dying(true), life(0), turn(0)
{
    fixSpeed();
}

void Bullet::setSpeedAndDirection(float speed, float dir)
{
    vx = speed * std::sin(dir);
    vy = -speed * std::cos(dir);

    fixSpeed();
}

void Bullet::setSpeed(float speed)
{
    float mag = getSpeed();

    vx = (vx * speed) / mag;
    vy = (vy * speed) / mag;

    fixSpeed();
}

void Bullet::setSpeedRelative(float speed)
{
    float mag = getSpeed();

    vx = (vx * (speed + mag)) / mag;
    vy = (vy * (speed + mag)) / mag;

    fixSpeed();
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

bool Bullet::isDead() const
{
    return dead;
}

bool Bullet::isDying() const
{
    return dying;
}

int Bullet::getTurn() const
{
    return turn;
}

void Bullet::fixSpeed()
{
    // See https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
    // Setting direction (alone) is dependent on components, so if speed is set to 0.0f,
    // setting a direction will not do anything.
    if (std::abs(vy) < FLT_EPSILON)
    {
        vy = FLT_EPSILON;
    }
}

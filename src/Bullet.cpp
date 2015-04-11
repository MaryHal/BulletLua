#include <bulletlua/Bullet.hpp>

#include <bulletlua/Utils/Math.hpp>
#include <cfloat>

Bullet::Bullet()
    : life{0}, turn{0}, luaState{nullptr}
{
    state.next = nullptr;
}

Bullet::Bullet(float x, float y, float vx, float vy)
    : life{0}, turn{0}, luaState{nullptr}
{
    state.live.x  = x;
    state.live.y  = y;
    state.live.vx = vx;
    state.live.vy = vy;

    fixSpeed();
}

Bullet* Bullet::getNext() const
{
    return state.next;
}

void Bullet::setNext(Bullet* next)
{
    state.next = next;
}

void Bullet::setPosition(float cx, float cy)
{
    state.live.x = cx;
    state.live.y = cy;
}

void Bullet::setVelocity(float nvx, float nvy)
{
    state.live.vx = nvx;
    state.live.vy = nvy;
}

void Bullet::setSpeedAndDirection(float speed, float dir)
{
    state.live.vx = speed * std::sin(dir);
    state.live.vy = -speed * std::cos(dir);

    fixSpeed();
}

void Bullet::setSpeed(float speed)
{
    float mag = getSpeed();

    state.live.vx = (state.live.vx * speed) / mag;
    state.live.vy = (state.live.vy * speed) / mag;

    fixSpeed();
}

void Bullet::setSpeedRelative(float speed)
{
    float mag = getSpeed();

    state.live.vx = (state.live.vx * (speed + mag)) / mag;
    state.live.vy = (state.live.vy * (speed + mag)) / mag;

    fixSpeed();
}

float Bullet::getSpeed() const
{
    return std::sqrt(state.live.vx * state.live.vx +
                     state.live.vy * state.live.vy);
}


void Bullet::setDirection(float dir)
{
    float speed = getSpeed();
    state.live.vx = speed * std::sin(dir);
    state.live.vy = -speed * std::cos(dir);
}


void Bullet::setDirectionRelative(float dir)
{
    setDirection(dir + getDirection());
}


void Bullet::aimAtPoint(float tx, float ty)
{
    // TODO: use getDirectionAim
    setDirection(Math::PI -
                 std::atan2(tx - state.live.x,
                            ty - state.live.y));
}

float Bullet::getAimDirection(float tx, float ty)
{
    return Math::PI - std::atan2(tx - state.live.x, ty - state.live.y);
}


float Bullet::getDirection() const
{
    return Math::PI - std::atan2(state.live.vx, state.live.vy);
}


void Bullet::vanish()
{
    turn = DYING;
}


void Bullet::kill()
{
    turn = DEAD;
}


bool Bullet::isDead() const
{
    return turn == DEAD;
}


bool Bullet::isDying() const
{
    return turn == DYING;
}


int Bullet::getTurn() const
{
    return turn;
}


// void Bullet::setColor(unsigned char newR, unsigned char newG, unsigned char newB)
// {
//     r = newR;
//     g = newG;
//     b = newB;
// }

void Bullet::update()
{
    state.live.x += state.live.vx;
    state.live.y += state.live.vy;

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
}

void Bullet::fixSpeed()
{
    // See https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
    // Setting direction (alone) is dependent on components, so if speed is set to 0.0f,
    // setting a direction will not do anything.
    if (std::abs(state.live.vy) < FLT_EPSILON)
    {
        state.live.vy = FLT_EPSILON;
    }
}

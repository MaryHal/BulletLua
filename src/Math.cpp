#include "Math.hpp"

#include <cmath>

unsigned int Math::nextPow2(register unsigned int n)
{
    // Flip final bit
    --n;

    // Shift bits then OR data
    for (unsigned int i = 1; i < sizeof(unsigned int); i <<= 1)
        n |= (n >> i);

    // flip the last bit back over.
    return (n + 1);
}

bool Math::checkPow2(unsigned int n)
{
    return ((n & (n - 1)) == 0) ? true : false;
}

float Math::radToDeg(float rad)
{
    return 180 / PI * rad;
}

float Math::degToRad(float deg)
{
    return PI / 180 * deg;
}

float Math::sin(float deg)
{
    return std::sin(degToRad(deg));
}

float Math::cos(float deg)
{
    return std::cos(degToRad(deg));
}

float Math::tan(float deg)
{
    return std::tan(degToRad(deg));
}

float Math::arcSin(float value)
{
    return radToDeg(std::asin(value));
}

float Math::arcCos(float value)
{
    return radToDeg(std::acos(value));
}

float Math::arcTan2(float x, float y)
{
    return radToDeg(std::atan2(y, x));
}

float Math::sqrt(float value)
{
    return std::sqrt(value);
}


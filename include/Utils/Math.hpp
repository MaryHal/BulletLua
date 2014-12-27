#ifndef _Math_hpp_
#define _Math_hpp_

#include <cmath>

namespace Math
{
    constexpr float PI = 3.14159265f;
    constexpr float TWO_PI = (PI * 2.0f);

    inline float radToDeg(float rad)
    {
        return 180 / PI * rad;
    }

    inline float degToRad(float deg)
    {
        return PI / 180 * deg;
    }

    inline float sin(float deg)
    {
        return std::sin(degToRad(deg));
    }

    inline float cos(float deg)
    {
        return std::cos(degToRad(deg));
    }

    inline float tan(float deg)
    {
        return std::tan(degToRad(deg));
    }

    inline float arcSin(float value)
    {
        return radToDeg(std::asin(value));
    }

    inline float arcCos(float value)
    {
        return radToDeg(std::acos(value));
    }

    inline float arcTan2(float x, float y)
    {
        return radToDeg(std::atan2(y, x));
    }

    inline float sqrt(float value)
    {
        return std::sqrt(value);
    }

    /* inline float getX(float d, float m); */
    /* inline float getY(float d, float m); */
}

#endif // _Math_hpp_


#ifndef _Math_hpp_
#define _Math_hpp_

#include <cmath>

namespace Math
{
    unsigned int nextPow2(register unsigned int n);
    bool checkPow2(unsigned int n);

    static constexpr float PI = 3.14159265f;
    static constexpr float TWO_PI = (PI * 2.0f);

    float radToDeg(float rad);
    float degToRad(float deg);

    float sin(float deg);
    float cos(float deg);
    float tan(float deg);
    float arcSin(float value);
    float arcCos(float value);
    float arcTan2(float x, float y);
    float sqrt(float value);

    /* float getX(float d, float m); */
    /* float getY(float d, float m); */
};

#endif // _Math_hpp_


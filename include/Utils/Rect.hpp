#ifndef _Rect_hpp_
#define _Rect_hpp_

#include <algorithm>

namespace BulletLuaUtils
{
    class Rect
    {
        public:
            float left;
            float top;
            float width;
            float height;

            Rect()
                : left(0.0f), top(0.0f),
                  width(0.0f), height(0.0f)
            {
            }

            Rect(float left, float top, float width, float height)
                : left(left), top(top), width(width), height(height)
            {
            }

            bool intersects(const Rect& that) const
            {
                // Normalize coordinates/sizes
                float r1MinX = std::min(left, left + width);
                float r1MaxX = std::max(left, left + width);
                float r1MinY = std::min(top,  top  + height);
                float r1MaxY = std::max(top,  top  + height);

                float r2MinX = std::min(that.left, that.left + that.width);
                float r2MaxX = std::max(that.left, that.left + that.width);
                float r2MinY = std::min(that.top,  that.top  + that.height);
                float r2MaxY = std::max(that.top,  that.top  + that.height);

                // Compute the intersection boundaries
                float interLeft   = std::max(r1MinX, r2MinX);
                float interTop    = std::max(r1MinY, r2MinY);
                float interRight  = std::min(r1MaxX, r2MaxX);
                float interBottom = std::min(r1MaxY, r2MaxY);

                return (interLeft < interRight) && (interTop < interBottom);
            }
    };

    inline bool operator==(const Rect& r1, const Rect& r2)
    {
        return (r1.left == r2.left && r1.top == r2.top) &&
            (r1.width == r2.width && r1.height == r2.height);
    }

    inline bool operator!=(const Rect& r1, const Rect& r2)
    {
        return !(r1 == r2);
    }
}

#endif /* _Rect_hpp_ */

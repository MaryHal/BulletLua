#include <bulletlua/Utils/Rect.hpp>

#include <algorithm>

namespace BulletLuaUtils
{
    Rect::Rect()
        : left{0.0f}, top{0.0f},
          width{0.0f}, height{0.0f}
    {
    }

    Rect::Rect(float left, float top, float width, float height)
        : left{left}, top{top}, width{width}, height{height}
    {
    }

    bool Rect::intersects(const Rect& that) const
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

    bool operator==(const Rect& r1, const Rect& r2)
    {
        return (r1.left == r2.left && r1.top == r2.top) &&
            (r1.width == r2.width && r1.height == r2.height);
    }

    bool operator!=(const Rect& r1, const Rect& r2)
    {
        return !(r1 == r2);
    }
} // namespace BulletLuaUtils


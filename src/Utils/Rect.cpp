#include <bulletlua/Utils/Rect.hpp>

#include <algorithm>

namespace BulletLuaUtils
{
    Rect::Rect()
        : x{0.0f}, y{0.0f},
          w{0.0f}, h{0.0f}
    {
    }

    Rect::Rect(float left, float top, float width, float height)
        : x{left}, y{top}, w{width}, h{height}
    {
    }

    bool Rect::intersects(const Rect& that) const
    {
        // Normalize coordinates/sizes
        float r1MinX = std::min(x, x + w);
        float r1MaxX = std::max(x, x + w);
        float r1MinY = std::min(y, y + h);
        float r1MaxY = std::max(y, y + h);

        float r2MinX = std::min(that.x, that.x + that.w);
        float r2MaxX = std::max(that.x, that.x + that.w);
        float r2MinY = std::min(that.y, that.y + that.h);
        float r2MaxY = std::max(that.y, that.y + that.h);

        // Compute the intersection boundaries
        float interLeft   = std::max(r1MinX, r2MinX);
        float interTop    = std::max(r1MinY, r2MinY);
        float interRight  = std::min(r1MaxX, r2MaxX);
        float interBottom = std::min(r1MaxY, r2MaxY);

        return (interLeft < interRight) && (interTop < interBottom);
    }

    float Rect::getCenterX() const
    {
        return x + (w / 2);
    }

    float Rect::getCenterY() const
    {
        return y + (h / 2);
    }

    bool operator==(const Rect& r1, const Rect& r2)
    {
        return (r1.x == r2.x && r1.y == r2.y) &&
            (r1.w == r2.w && r1.h == r2.h);
    }

    bool operator!=(const Rect& r1, const Rect& r2)
    {
        return !(r1 == r2);
    }
} // namespace BulletLuaUtils

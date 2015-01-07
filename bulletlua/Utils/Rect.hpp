#ifndef _Rect_hpp_
#define _Rect_hpp_

namespace BulletLuaUtils
{
    class Rect
    {
        public:
            float left;
            float top;
            float width;
            float height;

            Rect();
            Rect(float left, float top, float width, float height);

            bool intersects(const Rect& that) const;
    };

    bool operator==(const Rect& r1, const Rect& r2);
    bool operator!=(const Rect& r1, const Rect& r2);
}

#endif /* _Rect_hpp_ */

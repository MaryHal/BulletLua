#ifndef _Rect_hpp_
#define _Rect_hpp_

namespace BulletLuaUtils
{
    class Rect
    {
        public:
            float x;
            float y;
            float w;
            float h;

            Rect();
            Rect(float left, float top, float width, float height);

            bool intersects(const Rect& that) const;

            void setCenter(float cx, float cy);
            float getCenterX() const;
            float getCenterY() const;
    };

    bool operator==(const Rect& r1, const Rect& r2);
    bool operator!=(const Rect& r1, const Rect& r2);
}

#endif /* _Rect_hpp_ */

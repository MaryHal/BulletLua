#ifndef _BulletModel_hpp
#define _BulletModel_hpp

struct BulletModel
{
    public:
        // Bullet Metrics. Hitboxes at the moment are assumed to be in the center
        // of the graphic.
        float width;
        float height;
        float hitboxWidth;
        float hitboxHeight;

        // Image filename and texture coordinates.
        // It's up to the user to write the code to draw a BulletModel since it varies
        // greatly between user to user.
        // std::string filename;
        float texCoordX;
        float texCoordY;
        float texCoordWidth;
        float texCoordHeight;
};

#endif /* _BulletModel_hpp */

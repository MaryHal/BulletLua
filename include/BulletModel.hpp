#ifndef _BulletModel_hpp
#define _BulletModel_hpp

struct BulletModel
{
    public:
        std::string filename;
        float texCoordX;
        float texCoordY;
        float texCoordWidth;
        float texCoordHeight;

        float xSize;
        float ySize;
        float hitboxWidth;
        float hitboxHeight;
};

#endif /* _BulletModel_hpp */

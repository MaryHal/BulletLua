#ifndef _BulletManager_hpp_
#define _BulletManager_hpp_

#include <BulletLuaManager.hpp>

class BulletManager : public BulletLuaManager
{
    public:
        BulletManager(int left, int top, int width, int height);
        ~BulletManager() final;

        void tick() final;
        void draw() const;

        unsigned int getVertexCount() const;

    private:
        // void increaseCapacity(unsigned int blockSize=BLOCK_SIZE) final;
        // void increaseVertexCount(unsigned int blockSize=BLOCK_SIZE);

    private:
        static constexpr unsigned int MAX_BULLETS = 4096;

        unsigned int vao;
        unsigned int vertexVbo;
        unsigned int colorVbo;
        float vertexArray[MAX_BULLETS * 4 * 2];
        float colorArray [MAX_BULLETS * 4 * 4];
        // float textureArray[MAX_BULLETS * 4 * 2];
        unsigned int bulletCount;

        unsigned int tex;
};

#endif // _BulletManager_hpp_

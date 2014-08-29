#ifndef _BulletLuaManager_hpp_
#define _BulletLuaManager_hpp_

#include "BulletLua.hpp"
#include "SpacialPartition.hpp"

#include <vector>
#include <list>
#include <stack>

#include <string>

#include <random>

class BulletLuaManager
{
    public:
        // Rank [0.0, 1.0] represents the requested difficulty of a bullet pattern.
        static float rank;

        // Amount of bullets to allocate at once.
        static constexpr unsigned int BLOCK_SIZE = 2048;

    public:
        BulletLuaManager();
        ~BulletLuaManager();

        // Root Bullet
        void createBullet(const std::string& filename,
                          Bullet* origin,
                          Bullet* target);

        // Child Bullets
        // void createBullet(std::shared_ptr<sol::state> lua,
        //                   const std::string& func,
        //                   Bullet* origin,
        //                   Bullet* target);

        void createBullet(std::shared_ptr<sol::state> lua,
                          const std::string& func,
                          float x, float y, float d, float s,
                          Bullet* target);

        bool checkCollision(Bullet& b);
        virtual void tick();

        // Draw function.
        // void draw()

        // Move all bullets to the free stack
        void clear();
        void vanishAll();

        unsigned int bulletCount() const;
        unsigned int freeCount() const;
        unsigned int blockCount() const;

    protected:
        BulletLua* getFreeBullet();

        virtual void increaseCapacity(unsigned int blockSize=BLOCK_SIZE);

        // Allocate a block of BLOCK_SIZE(default) Bullets and add it to freeBullets
        void allocateBlock(unsigned int blockSize);

    protected:
        std::list<BulletLua*> bullets;
        std::stack<BulletLua*> freeBullets;

        std::list<BulletLua*> blocks;

        SpacialPartition collision;
};

#endif /* _BulletLuaManager_hpp_ */

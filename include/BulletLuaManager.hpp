#ifndef _BulletLuaManager_hpp_
#define _BulletLuaManager_hpp_

#include "BulletLua.hpp"
#include "SpacialPartition.hpp"

#include <vector>
#include <list>
#include <stack>

#include <string>

class BulletLuaManager
{
    public:
        static float rank;
        static constexpr unsigned int BLOCK_SIZE = 2048;

    public:
        BulletLuaManager();
        ~BulletLuaManager();

        // Root Bullet
        void createBullet(const std::string& filename, Bullet* origin, Bullet* target);

        // Child Bullets
        void createBullet(std::shared_ptr<sol::state> lua, const std::string& func, Bullet* origin, Bullet* target);
        void createBullet(std::shared_ptr<sol::state> lua, const std::string& func,
                          double x, double y, double d, double s, Bullet* target);

        bool checkCollision(Bullet& b);
        virtual void tick();
        // Draw function.
        // void draw()

        // Move all bullets to the free stack
        void clear();

        unsigned int bulletCount() const;
        unsigned int freeCount() const;
        unsigned int blockCount() const;

    protected:
        BulletLua* getFreeBullet();

        // Allocate a block of BLOCK_SIZE Bullets and add it to freeBullets
        virtual void increaseCapacity(unsigned int blockSize=BLOCK_SIZE);

    protected:
        std::list<BulletLua*> bullets;
        std::stack<BulletLua*> freeBullets;

        std::list<BulletLua*> blocks;

        SpacialPartition collision;
};

#endif /* _BulletLuaManager_hpp_ */

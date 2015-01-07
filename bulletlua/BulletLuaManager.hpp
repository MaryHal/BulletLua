#ifndef _BulletLuaManager_hpp_
#define _BulletLuaManager_hpp_

// #include "BulletModel.hpp"
#include "SpacialPartition.hpp"

#include <list>
#include <stack>

#include <string>
#include <memory>

#include <sol.hpp>

namespace
{
    // Amount of bullets to allocate at once.
    const unsigned int BLOCK_SIZE = 2048;
}

namespace BulletLuaUtils
{
    class Random;
}

class Bullet;
class BulletLua;

class BulletLuaManager
{
    protected:
        // Pointer to the bullet that is to-be-processed.
        BulletLua* current;

        // Rank [0.0, 1.0] represents the requested difficulty of a bullet pattern.
        float rank;

        std::list<BulletLua*> bullets;
        std::stack<BulletLua*> freeBullets;

        std::list<BulletLua*> blocks;

        // std::vector<BulletModel> models;

        SpacialPartition collision;
        std::unique_ptr<BulletLuaUtils::Random> rng;

    public:
        BulletLuaManager(int left, int top, int width, int height);
        virtual ~BulletLuaManager();

        // Non-copyable
        BulletLuaManager(const BulletLuaManager&) = delete;
        BulletLuaManager& operator=(const BulletLuaManager&) = delete;

        // TODO: Explicit copy constructor?

        // // TODO: Finish bullet model methods.
        // void registerModel(const BulletModel& model);
        // const BulletModel& getModel(int index) const;

        // Root Bullet
        void createBullet(const std::string& filename,
                          Bullet* origin,
                          Bullet* target);

        // // Create Child Bullet
        // void createBullet(std::shared_ptr<sol::state> lua,
        //                   const sol::function& func,
        //                   Bullet* origin,
        //                   Bullet* target);

        // Create Child Bullet
        void createBullet(std::shared_ptr<sol::state> lua,
                          const sol::function& func,
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
        // Returns an unused bullet. Allocates more data blocks if there none are available
        BulletLua* getFreeBullet();

        // Allocate a new block of Bullet data.
        virtual void increaseCapacity(unsigned int blockSize=BLOCK_SIZE);

        // Create a BulletLua lua state with the necessary functions.
        std::shared_ptr<sol::state> initLua();
};

#endif /* _BulletLuaManager_hpp_ */

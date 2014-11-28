#ifndef _BulletLuaManager_hpp_
#define _BulletLuaManager_hpp_

#include "BulletLua.hpp"
#include "BulletModel.hpp"
#include "SpacialPartition.hpp"

#include <list>
#include <stack>

#include <string>

class BulletLuaManager
{
    public:
        // Rank [0.0, 1.0] represents the requested difficulty of a bullet pattern.
        static float rank;

        // Amount of bullets to allocate at once.
        static constexpr unsigned int BLOCK_SIZE = 2048;

    public:
        BulletLuaManager(int left, int top, int width, int height);
        virtual ~BulletLuaManager();

        // Non-copyable
        BulletLuaManager(const BulletLuaManager&) = delete;
        BulletLuaManager& operator=(const BulletLuaManager&) = delete;

        // TODO: Explicit copy constructor?

        void registerModel(const BulletModel& model);
        const BulletModel& getModel(int index) const;

        // Root Bullet
        void createBullet(const std::string& filename,
                          Bullet* origin,
                          Bullet* target);

        // Create Child Bullet
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

        virtual void increaseCapacity(unsigned int blockSize=BLOCK_SIZE);

    protected:
        std::list<BulletLua*> bullets;
        std::stack<BulletLua*> freeBullets;

        std::list<BulletLua*> blocks;

        std::vector<BulletModel> models;

        SpacialPartition collision;
};

#endif /* _BulletLuaManager_hpp_ */

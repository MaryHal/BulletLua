#ifndef _BulletLuaManager_hpp_
#define _BulletLuaManager_hpp_

#include <list>
#include <stack>

#include <string>
#include <memory>

#include <sol.hpp>

// #include <bulletlua/BulletModel.hpp>
#include <bulletlua/SpacialPartition.hpp>
#include <bulletlua/Utils/Rng.hpp>
#include <bulletlua/Utils/Rect.hpp>

namespace
{
    // Amount of bullets to allocate at once.
    const unsigned int BLOCK_SIZE = 2048;
}

class Bullet;
class BulletLua;

class BulletLuaManager
{
    protected:
        // Pointer to the bullet that is to-be-processed.
        BulletLua* current;

        // Collision object that bullets can "aim" at (i.e. the player).
        const BulletLuaUtils::Rect& player;

        // Rank [0.0, 1.0] represents the requested difficulty of a bullet pattern.
        float rank;

        std::list<BulletLua*> bullets;
        std::stack<BulletLua*> freeBullets;

        std::list<BulletLua*> blocks;

        // std::vector<BulletModel> models;

        SpacialPartition collision;
        BulletLuaUtils::MTRandom rng;

    public:
        BulletLuaManager(int left, int top, int width, int height, const BulletLuaUtils::Rect& player);
        virtual ~BulletLuaManager();

        // Non-copyable
        BulletLuaManager(const BulletLuaManager&) = delete;
        BulletLuaManager& operator=(const BulletLuaManager&) = delete;

        // // TODO: Finish bullet model methods.
        // void registerModel(const BulletModel& model);
        // const BulletModel& getModel(int index) const;

        // Create a root bullet from an external script.
        void createBulletFromFile(const std::string& filename,
                                  Bullet* origin);

        // Create a root bullet from an embedded script.
        void createBulletFromScript(const std::string& script,
                                    Bullet* origin);

        // Create child bullet
        void createBullet(std::shared_ptr<sol::state> lua,
                          const sol::function& func,
                          float x, float y, float d, float s);

        bool checkCollision();
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

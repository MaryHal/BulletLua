#include "catch.hpp"

#include "BulletLuaManager.hpp"
#include "BulletLua.hpp"

#include <memory>
#include <iostream>

class BulletTester : public BulletLuaManager
{
    public:
        std::unique_ptr<Bullet> origin;
        std::unique_ptr<Bullet> destination;

    public:
        BulletTester()
            : BulletLuaManager(0.0f, 0.0f, 640.0f, 480.0f)
            , origin(new Bullet(320.0f, 120.0f, 0.0f, 0.0f))
            , destination(new Bullet(320.0f, 240.0f, 0.0f, 0.0f))
        {
        }

        // Create bullet from string.
        void createBullet(const std::string& script)
        {
            BulletLua* b = getFreeBullet();

            std::shared_ptr<sol::state> luaState = initLua();
            luaState->script(script);

            b->set(luaState,
                   luaState->get<sol::function>("main"),
                   origin.get(), destination.get());

            bullets.push_back(b);
        }

        void createEmptyBullet()
        {
            BulletLua* b = getFreeBullet();
            bullets.push_back(b);
        }

        void createEmptyBullets(unsigned int n)
        {
            for (unsigned int i = 0; i < n; ++i)
            {
                createEmptyBullet();
            }
        }
};

TEST_CASE("Space Allocation", "[Space]")
{
    BulletTester manager;

    SECTION("Initial Size")
    {
        REQUIRE(manager.bulletCount() == 0);
        REQUIRE(manager.freeCount() == BLOCK_SIZE);
        REQUIRE(manager.blockCount() == 1);
    }

    SECTION("Add a handful")
    {
        const unsigned int a_handful = 100;
        const unsigned int expected_blocks = (a_handful / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_handful);

        REQUIRE(manager.bulletCount() == a_handful);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_handful);
        REQUIRE(manager.blockCount() == expected_blocks);
    }

    // Overflow one (default-sized) block
    SECTION("Add a metric ton")
    {
        const unsigned int a_metric_ton = 2200;
        const unsigned int expected_blocks = (a_metric_ton / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_metric_ton);

        REQUIRE(manager.bulletCount() == a_metric_ton);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_metric_ton);
        REQUIRE(manager.blockCount() == expected_blocks);
    }

    // Overflow many (default-sized) blocks.
    SECTION("Add a lot")
    {
        const unsigned int a_lot = 1 << 16;
        const unsigned int expected_blocks = (a_lot / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_lot);

        REQUIRE(manager.bulletCount() == a_lot);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_lot);
        REQUIRE(manager.blockCount() == expected_blocks);
    }
}

TEST_CASE("Out of Bounds Check", "[Boundary]")
{
    const char* script =
        "function main()"
        "    setPosition(-4, 0)"
        "end";

    BulletTester manager;

    SECTION("Single Bullet")
    {
        manager.createBullet(script);
        REQUIRE(manager.bulletCount() == 1);

        manager.tick();

        REQUIRE(manager.bulletCount() == 0);
    }
}

TEST_CASE("Collision Check", "[Collision]")
{
    const char* script =
        "function main()"
        "    setPosition(100, 100)"
        "end";

    BulletTester manager;

    SECTION("Single Bullet")
    {
        // Create a bullet, script will move it to (100, 100)
        manager.createBullet(script);
        manager.tick();

        // Move our "ship" to (100, 100) as well.
        manager.destination->x = 100;
        manager.destination->y = 100;

        // Is there a collision? There better be.
        REQUIRE(manager.checkCollision(*manager.destination) == true);
    }
}

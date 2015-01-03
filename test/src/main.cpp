#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "BulletLuaManager.hpp"

#include <memory>
#include <iostream>

class BulletTester : public BulletLuaManager
{
    public:
        std::unique_ptr<Bullet> origin;
        std::unique_ptr<Bullet> destination;

    public:
        BulletTester()
            : BulletLuaManager(0, 0, 640, 480)
        {
        }

        // Create bullet from string.
        void createBullet(const std::string& script,
                          Bullet* origin,
                          Bullet* target)
        {
            BulletLua* b = getFreeBullet();

            std::shared_ptr<sol::state> luaState = initLua();
            luaState->script(script);

            b->set(luaState,
                   luaState->get<sol::function>("main"),
                   origin, target);

            bullets.push_back(b);
        }

        void createBullet()
        {
            BulletLua* b = getFreeBullet();
            bullets.push_back(b);
        }
};

TEST_CASE("Space Allocation", "[space]")
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

        for (unsigned int i = 0; i < a_handful; ++i)
        {
            manager.createBullet();
        }

        REQUIRE(manager.bulletCount() == a_handful);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_handful);
        REQUIRE(manager.blockCount() == expected_blocks);
    }

    SECTION("Add a metric ton")
    {
        const unsigned int a_metric_ton = 2200;
        const unsigned int expected_blocks = (a_metric_ton / BLOCK_SIZE) + 1;

        for (unsigned int i = 0; i < a_metric_ton; ++i)
        {
            manager.createBullet();
        }

        REQUIRE(manager.bulletCount() == a_metric_ton);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_metric_ton);
        REQUIRE(manager.blockCount() == expected_blocks);
    }

    SECTION("Add a lot")
    {
        const unsigned int a_lot = 1 << 16;
        const unsigned int expected_blocks = (a_lot / BLOCK_SIZE) + 1;

        for (unsigned int i = 0; i < a_lot; ++i)
        {
            manager.createBullet();
        }

        REQUIRE(manager.bulletCount() == a_lot);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_lot);
        REQUIRE(manager.blockCount() == expected_blocks);
    }
}

#include <catch.hpp>

#include <bulletlua/BulletLuaManager.hpp>
#include <bulletlua/BulletLua.hpp>
#include <bulletlua/Utils/Rect.hpp>

#include <memory>
#include <iostream>

class BulletTester : public BulletLuaManager
{
    public:
        std::unique_ptr<Bullet> origin;

    public:
        BulletTester(BulletLuaUtils::Rect& player)
            : BulletLuaManager{0, 0, 640, 480, player}
            , origin{new Bullet{320.0f, 120.0f, 0.0f, 0.0f}}
        {
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
    BulletLuaUtils::Rect player{320.0f, 240.0f, 4.0f, 4.0f};
    BulletTester manager{player};

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

    // Overflow one (default-size) block.
    SECTION("Add a metric ton")
    {
        const unsigned int a_metric_ton = 2200;
        const unsigned int expected_blocks = (a_metric_ton / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_metric_ton);

        REQUIRE(manager.bulletCount() == a_metric_ton);
        REQUIRE(manager.freeCount() == expected_blocks * BLOCK_SIZE - a_metric_ton);
        REQUIRE(manager.blockCount() == expected_blocks);
    }

    // Overflow many (default-size) blocks.
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
    BulletLuaUtils::Rect player{320.0f, 240.0f, 4.0f, 4.0f};
    BulletTester manager{player};

    const char* script =
        "function main()"
        "    setPosition(-4, 0)"
        "end";

    SECTION("Single Bullet")
    {
        manager.createBulletFromScript(script, manager.origin.get());
        REQUIRE(manager.bulletCount() == 1);

        manager.tick();

        REQUIRE(manager.bulletCount() == 0);
    }
}

TEST_CASE("Collision Check", "[Collision]")
{
    BulletLuaUtils::Rect player{320.0f, 240.0f, 4.0f, 4.0f};
    BulletTester manager{player};

    const char* script =
        "function main()"
        "    setPosition(100, 100)"
        "end";

    SECTION("Single Bullet")
    {
        // Create a bullet, script will move it to (100, 100)
        manager.createBulletFromScript(script, manager.origin.get());
        manager.tick();

        // Move our "ship" to (100, 100) as well.
        player.x = 100;
        player.y = 100;

        // Is there a collision? There better be.
        REQUIRE(manager.checkCollision() == true);
    }
}

#include <catch.hpp>

#include <bulletlua/BulletLuaManager.hpp>
#include <bulletlua/BulletLua.hpp>
#include <bulletlua/Utils/Rect.hpp>

#include <memory>
#include <iostream>

class BulletTester : public BulletLuaManager
{
    public:
        // std::unique_ptr<Bullet> origin;

    public:
        BulletTester()
            : BulletLuaManager{0, 0, 640, 480}
            // , origin{new Bullet{320.0f, 120.0f, 0.0f, 0.0f}}
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
    BulletTester manager;
    manager.setPlayerPosition(320.0f, 240.0f);

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
    BulletTester manager;
    manager.setPlayerPosition(320.0f, 240.0f);

    const char* script =
        "function main()"
        "    setPosition(-4, 0)"
        "end";

    SECTION("Single Bullet")
    {
        manager.createBulletFromScript(script, 320.0f, 120.0f);
        REQUIRE(manager.bulletCount() == 1);

        manager.tick();

        REQUIRE(manager.bulletCount() == 0);
    }
}

// TEST_CASE("Collision Check", "[Collision]")
// {
//     BulletTester manager;
//     manager.setPlayerPosition(320.0f, 240.0f);

//     const char* script =
//         "function main()"
//         "    setPosition(100, 100)"
//         "end";

//     SECTION("Single Bullet")
//     {
//         // Create a bullet, script will move it to (100, 100)
//         manager.createBulletFromScript(script, manager.origin.get());
//         manager.tick();

//         // Move our "ship" to (100, 100) as well.
//         manager.setPlayerPosition(100.0f, 100.0f);

//         // std::cout << player.x << " " << player.y << " " << player.w << " " << player.h << std::endl;

//         // Is there a collision? There better be.
//         REQUIRE(manager.checkCollision() == true);

//         // Offset by one. Since our "ship" is 4 units wide and 4 units tall, this should still be
//         // considered a collision.
//         manager.setPlayerPosition(99.0f, 99.0f);
//         REQUIRE(manager.checkCollision() == true);

//         manager.setPlayerPosition(101.0f, 99.0f);
//         REQUIRE(manager.checkCollision() == true);

//         manager.setPlayerPosition(101.0f, 101.0f);
//         REQUIRE(manager.checkCollision() == true);

//         manager.setPlayerPosition(99.0f, 101.0f);
//         REQUIRE(manager.checkCollision() == true);

//         // // No longer a collision.
//         // manager.setPlayerPosition(102.0f, 102.0f);
//         // REQUIRE(manager.checkCollision() == false);
//     }
// }

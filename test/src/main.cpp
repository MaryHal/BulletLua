#include <catch.hpp>

#include <bulletlua/BulletLuaManager.hpp>

#include <iostream>

class BulletTester : public BulletLuaManager
{
    public:
        // std::unique_ptr<Bullet> origin;

    public:
        BulletTester()
        {
        }

        void createEmptyBullet()
        {
            getFreeBullet();
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
        REQUIRE(manager.getContainerSize() == BulletTester::MAX_BULLETS);
        REQUIRE(manager.getActiveBulletCount() == 0);
    }

    SECTION("Add a handful")
    {
        const unsigned int a_handful = 100;
        // const unsigned int expected_blocks = (a_handful / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_handful);

        REQUIRE(manager.getContainerSize() == BulletTester::MAX_BULLETS);
        REQUIRE(manager.getActiveBulletCount() == a_handful);
    }

    // Overflow one (default-size) block.
    SECTION("Add a metric ton")
    {
        const unsigned int a_metric_ton = 2200;
        // const unsigned int expected_blocks = (a_metric_ton / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_metric_ton);

        REQUIRE(manager.getContainerSize() == BulletTester::MAX_BULLETS);
        REQUIRE(manager.getActiveBulletCount() == a_metric_ton);
    }

    // Overflow many (default-size) blocks.
    SECTION("Add a lot")
    {
        const unsigned int a_lot = 8000;
        // const unsigned int expected_blocks = (a_lot / BLOCK_SIZE) + 1;

        manager.createEmptyBullets(a_lot);

        REQUIRE(manager.getContainerSize() == BulletTester::MAX_BULLETS * 2);
        REQUIRE(manager.getActiveBulletCount() == a_lot);
    }
}

TEST_CASE("Out of Bounds Check", "[Boundary]")
{
    BulletTester manager;
    manager.setPlayerPosition(320.0f, 240.0f);

    const char* script =
        "function main()"
        "    setPosition(-10, -10)"
        "end";

    SECTION("Single Bullet")
    {
        manager.createBulletFromScript(script, 320.0f, 120.0f);
        REQUIRE(manager.getActiveBulletCount() == 1);

        manager.tick();

        REQUIRE(manager.getActiveBulletCount() == 0);
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
//         manager.createBulletFromScript(script, 320.0f, 120.0f);
//         manager.tick();

//         // Move our "ship" to (100, 100) as well.
//         manager.setPlayerPosition(100.0f, 100.0f);

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

#ifndef _SpacialPartition_hpp_
#define _SpacialPartition_hpp_

#include <bulletlua/Utils/Rect.hpp>

class Bullet;

// Very simple collision detection.
// Cuts a region into a fixed amount of tiles so collision detection only
// needs to compare against elements located in the same tile.
// While the worst-case runtime is O(n^2) , where ALL bullets are located in
// the same tile (O(n) to add all bullets, O(n) to check against all bullets in a tile),
// this simple solution suits my needs for now.
class SpacialPartition
{
    private:
        static constexpr unsigned int tileSize = 50;
        static constexpr int WIDTH  = 640 / tileSize;
        static constexpr int HEIGHT = 480 / tileSize;
        static constexpr int CAP = 100;

        // Out of bounds error waiting to happen...
        const Bullet* space[WIDTH][HEIGHT][CAP];
        int bulletCount[WIDTH][HEIGHT];

        BulletLuaUtils::Rect screenArea;

    public:
        SpacialPartition(const BulletLuaUtils::Rect& area);
        // SpacialPartition(int width, int height);

        SpacialPartition(const SpacialPartition&) = delete;
        SpacialPartition& operator=(const SpacialPartition&) = delete;

        void addBullet(const Bullet* bullet);

        // Lazy delete all bullets.
        void reset();

        // Checks if bullet is still in the testable area
        bool checkOutOfBounds(const BulletLuaUtils::Rect& b) const;

        // Point-to-point collision test.
        bool checkCollision(const BulletLuaUtils::Rect& b) const;

        // Point-to-rect collision test.
        bool checkCollision(int x, int y, int width, int height) = delete;
};

#endif // _SpacialPartition_hpp_

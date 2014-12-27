#ifndef _SpacialPartition_hpp_
#define _SpacialPartition_hpp_

#include "Bullet.hpp"
#include "Utils/Rect.hpp"

// Very simple collision detection.
// Cuts a region into a fixed amount of tiles so collision detection only
// needs to compare against elements located in the same tile.
// While the worst-case runtime is O(n^2) , where ALL bullets are located in
// the same tile (O(n) to add all bullets, O(n) to check against all bullets in a tile),
// this simple solution suits my needs for now.
class SpacialPartition
{
    public:
        SpacialPartition(const BulletLuaUtils::Rect& area)
            : screenArea(area)
        {
            reset();
        }
        // SpacialPartition(int width, int height);

        SpacialPartition(const SpacialPartition&) = delete;
        SpacialPartition& operator=(const SpacialPartition&) = delete;

        void addBullet(const Bullet* bullet)
        {
            if (bullet->dying || bullet->dead)
                return;

            // Abuse integer division to determine which array cell this bullet belongs to.
            int x = bullet->x / tileSize;
            int y = bullet->y / tileSize;

            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
                return;

            if (bulletCount[x][y] < CAP)
            {
                space[x][y][bulletCount[x][y]] = bullet;
                ++bulletCount[x][y];
            }
        }
        void reset()
        {
            // More or less "lazy-deletion" of our bullet pointers.
            std::memset(bulletCount, 0, sizeof(int) * WIDTH * HEIGHT);
            // std::memset(space, 0, sizeof(Bullet*) * WIDTH * HEIGHT * CAP);
        }

        // Checks if bullet is still in the testable area
        bool checkOutOfBounds(const Bullet& b) const
        {
            BulletLuaUtils::Rect thisBullet(b.x, b.y, 4, 4);
            return !screenArea.intersects(thisBullet);
        }

        // Point-to-point collision test.
        bool checkCollision(const Bullet& b) const
        {
            int x = b.x / tileSize;
            int y = b.y / tileSize;

            // If the position of the bullet passed as an argument is outside our defined space,
            // don't bother
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
                return false;

            BulletLuaUtils::Rect thisBullet(b.x - 2, b.y - 2, 4, 4);
            BulletLuaUtils::Rect thatBullet(0.0f, 0.0f, 4, 4);

            for (int i = 0; i < bulletCount[x][y]; i++)
            {
                thatBullet.left = space[x][y][i]->x - 2;
                thatBullet.top  = space[x][y][i]->y - 2;

                if (thisBullet.intersects(thatBullet))
                    return true;
            }

            return false;
        }

        // Point-to-rect collision test.
        bool checkCollision(int x, int y, int width, int height)
        {
            int xMin = x / tileSize;
            int xMax = (x + width) / tileSize;

            int yMin = y / tileSize;
            int yMax = (y + height) / tileSize;

            for (int j = yMin; j < yMax; ++j)
            {
                for (int i = xMin; i < xMax; ++i)
                {
                    return false;
                }
            }
            return false;
        }

    private:
        static constexpr unsigned int tileSize = 50;
        static constexpr int WIDTH  = 640 / tileSize;
        static constexpr int HEIGHT = 480 / tileSize;
        static constexpr int CAP = 100;

        // Out of bounds error waiting to happen...
        const Bullet* space[WIDTH][HEIGHT][CAP];
        int bulletCount[WIDTH][HEIGHT];

        BulletLuaUtils::Rect screenArea;
};

#endif // _SpacialPartition_hpp_

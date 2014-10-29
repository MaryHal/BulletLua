#ifndef _SpacialPartition_hpp_
#define _SpacialPartition_hpp_

#include "Bullet.hpp"
#include "Utils/Rect.hpp"

// Very simple collision detection
class SpacialPartition
{
    public:
        SpacialPartition(const BulletLuaUtils::Rect& area);
        // SpacialPartition(int width, int height);

        void addBullet(const Bullet* bullet);
        void reset();

        // Checks if bullet is still in the testable area
        bool checkOutOfBounds(const Bullet& b) const;

        // Checks collision for a bullet against all other bullets
        bool checkCollision(const Bullet& b) const;

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

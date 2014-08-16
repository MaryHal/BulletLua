#ifndef _SpacialPartition_hpp_
#define _SpacialPartition_hpp_

#include "Bullet.hpp"

class SpacialPartition
{
    public:
        SpacialPartition();

        void addBullet(const Bullet* bullet);
        void reset();

        bool checkCollision(Bullet& b);

    private:
        static constexpr unsigned int tileSize = 100;
        static constexpr int WIDTH  = 640 / tileSize + 3;
        static constexpr int HEIGHT = 480 / tileSize + 3;
        static constexpr int CAP = 1000;

        const Bullet* space[WIDTH][HEIGHT][CAP];
        int bulletCount[WIDTH][HEIGHT];
};

#endif // _SpacialPartition_hpp_

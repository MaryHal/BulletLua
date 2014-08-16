#include "SpacialPartition.hpp"

#include <cstring>

SpacialPartition::SpacialPartition()
{
    reset();
}

void SpacialPartition::addBullet(const Bullet* bullet)
{
    int x = bullet->x / tileSize;
    int y = bullet->y / tileSize;

    if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
        return;

    space[x][y][bulletCount[x][y]] = bullet;
    bulletCount[x][y]++;
}

// Call once per frame
void SpacialPartition::reset()
{
    std::memset(space, 0, sizeof(Bullet*) * WIDTH * HEIGHT * CAP);
    std::memset(bulletCount, 0, sizeof(int) * WIDTH * HEIGHT);
}

bool SpacialPartition::checkCollision(Bullet& b)
{
    int x = b.x / tileSize;
    int y = b.y / tileSize;

    sf::FloatRect thisBullet(b.x, b.y, 4, 4);
    sf::FloatRect thatBullet(0.0f, 0.0f, 4, 4);

    for (int i = 0; i < bulletCount[x][y]; i++)
    {
        thatBullet.left = space[x][y][i]->x - 2;
        thatBullet.top  = space[x][y][i]->y - 2;

        if (thisBullet.intersects(thatBullet))
            return true;
    }

    return false;
}

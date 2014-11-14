#include "SpacialPartition.hpp"

#include <cstring>

SpacialPartition::SpacialPartition(const BulletLuaUtils::Rect& area)
    : screenArea(area)
{
    reset();
}

// SpacialPartition::SpacialPartition(int width, int height)
// {
//     reset();
// }

void SpacialPartition::addBullet(const Bullet* bullet)
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

// Call once per frame
void SpacialPartition::reset()
{
    // More or less "lazy-deletion" of our bullet pointers.
    std::memset(bulletCount, 0, sizeof(int) * WIDTH * HEIGHT);
    // std::memset(space, 0, sizeof(Bullet*) * WIDTH * HEIGHT * CAP);
}

bool SpacialPartition::checkOutOfBounds(const Bullet& b) const
{
    BulletLuaUtils::Rect thisBullet(b.x, b.y, 4, 4);
    return !screenArea.intersects(thisBullet);
}

bool SpacialPartition::checkCollision(const Bullet& b) const
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

#include <bulletlua/SpacialPartition.hpp>
#include <bulletlua/Bullet.hpp>

#include <cstring>
#include <iostream>

SpacialPartition::SpacialPartition(const BulletLuaUtils::Rect& area)
    : screenArea{area}
{
    reset();
}

void SpacialPartition::addBullet(const Bullet* bullet)
{
    if (bullet->dying || bullet->dead)
        return;

    // Abuse integer division to determine which array cell this bullet belongs to.
    int x = bullet->position.x / tileSize;
    int y = bullet->position.y / tileSize;

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    if (bulletCount[x][y] < CAP)
    {
        space[x][y][bulletCount[x][y]] = bullet;
        ++bulletCount[x][y];
    }
}

void SpacialPartition::reset()
{
    // More or less "lazy-deletion" of our bullet pointers.
    std::memset(bulletCount, 0, sizeof(int) * WIDTH * HEIGHT);
    // std::memset(space, 0, sizeof(Bullet*) * WIDTH * HEIGHT * CAP);
}

// Checks if bullet is still in the testable area
bool SpacialPartition::checkOutOfBounds(const BulletLuaUtils::Rect& b) const
{
    // BulletLuaUtils::Rect thisBullet(b.position.x, b.position.y, 4.0f, 4.0f);
    return !screenArea.intersects(b);
}

// Point-to-point collision test.
bool SpacialPartition::checkCollision(const BulletLuaUtils::Rect& b) const
{
    int x = b.x / tileSize;
    int y = b.y / tileSize;

    // If the position of the bullet passed as an argument is outside our defined space, don't
    // bother.
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return false;

    BulletLuaUtils::Rect thisBullet{b};

    for (int i = 0; i < bulletCount[x][y]; i++)
    {
        BulletLuaUtils::Rect thatBullet{space[x][y][i]->position};
        // thatBullet.x = space[x][y][i]->position.x;
        // thatBullet.x = space[x][y][i]->position.y;

        if (thisBullet.intersects(thatBullet))
            return true;
    }

    return false;
}

// // Point-to-rect collision test.
// bool SpacialPartition::checkCollision(int x, int y, int width, int height)
// {
//     int xMin = x / tileSize;
//     int xMax = (x + width) / tileSize;
//
//     int yMin = y / tileSize;
//     int yMax = (y + height) / tileSize;
//
//     for (int j = yMin; j < yMax; ++j)
//     {
//         for (int i = xMin; i < xMax; ++i)
//         {
//             return false;
//         }
//     }
//     return false;
// }

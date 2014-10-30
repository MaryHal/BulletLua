#include "BulletLuaManager.hpp"

float BulletLuaManager::rank = 0.8;

BulletLuaManager::BulletLuaManager(const BulletLuaUtils::Rect& area)
    : collision(area)
{
    // This only calls this class' version of this function, not any subclass'.
    increaseCapacity();
}

BulletLuaManager::~BulletLuaManager()
{
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter)
    {
        delete [] *iter;
    }
}

void BulletLuaManager::createBullet(const std::string& filename,
                                    Bullet* origin,
                                    Bullet* target)
{
    BulletLua* b = getFreeBullet();
    b->set(filename, origin, target, this);
    bullets.push_back(b);
}

// void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua,
//                                     const sol::function& func,
//                                     Bullet* origin,
//                                     Bullet* target)
// {
//     BulletLua* b = getFreeBullet();
//     b->set(lua, func, origin, target, this);
//     bullets.push_back(b);
// }

void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua,
                                    const sol::function& func,
                                    float x, float y, float d, float s,
                                    Bullet* target)
{
    BulletLua* b = getFreeBullet();
    b->set(lua, func, x, y, d, s, target, this);
    bullets.push_back(b);
}

bool BulletLuaManager::checkCollision(Bullet& b)
{
    return collision.checkCollision(b);
}

void BulletLuaManager::tick()
{
    // Reset containers inside collision detection object.
    // Since bullets are dynamic and are most likely unpredictable,
    // we must repopulate the containers each frame.
    collision.reset();

    for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
    {
        if ((*iter)->isDead())
        {
            freeBullets.push(*iter);
            iter = bullets.erase(iter);
        }
        (*iter)->run(collision);
        collision.addBullet(*iter);
    }
}

void BulletLuaManager::clear()
{
    while (!bullets.empty())
    {
        freeBullets.push(bullets.front());
        bullets.pop_front();
    }
}

void BulletLuaManager::vanishAll()
{
    for (BulletLua* b : bullets)
    {
        b->vanish();
    }
}

unsigned int BulletLuaManager::bulletCount() const
{
    return bullets.size();
}

unsigned int BulletLuaManager::freeCount() const
{
    return freeBullets.size();
}

unsigned int BulletLuaManager::blockCount() const
{
    return blocks.size();
}

BulletLua* BulletLuaManager::getFreeBullet()
{
    if (freeBullets.empty())
    {
        increaseCapacity();
    }

    BulletLua* bullet = freeBullets.top();
    freeBullets.pop();

    return bullet;
}

void BulletLuaManager::increaseCapacity(unsigned int blockSize)
{
    blocks.push_back(new BulletLua[blockSize]);

    // Throw all bullets into free stack
    for (unsigned int i = 0; i < blockSize; ++i)
    {
        freeBullets.push(&blocks.back()[i]);
    }

    // Subclasses should override this method if their extensions depends on block size.
    // E.g. allocation of vertices or bookkeeping of vertices in a VBO.
    // Keep in mind that this original version will be called in the default constructor.
}

#include "BulletManager.hpp"

BulletManager::BulletManager(const BulletLuaUtils::Rect& area)
    : BulletLuaManager(area),
      bulletTexture(nullptr),
      vertices(),
      vertexCount(0)
{
    // Superclass constructor(BulletLuaManager) has no arguments, so it's called implicitly

    // Calculate and create room for initial set of vertices.
    vertices.setPrimitiveType(sf::Quads);
    increaseVertexCount();
}

BulletManager::~BulletManager()
{
}

void BulletManager::setTexture(sf::Texture& tex)
{
    bulletTexture = &tex;
}

void BulletManager::tick()
{
    vertices.clear();
    collision.reset();

    for (auto iter = bullets.begin(); iter != bullets.end();)
    {
        if ((*iter)->isDead())
        {
            freeBullets.push(*iter);
            iter = bullets.erase(iter);
        }

        // If the next bullet is dead, we are at the end of the list, so iter will be
        // invalid.
        if ((*iter)->isDead())
        {
            continue;
        }

        (*iter)->run(collision);

        const Bullet* b = *iter;
        float rad = std::sqrt(8*8 + 8*8);
        float dir = b->getDirection();

        sf::Color color(255, 255, 255, b->life);
        sf::Vector2f texCoords[4] = {
            {0.0f, 0.0f},
            {32.0f, 0.0f},
            {32.0f, 32.0f},
            {0.0f, 32.0f}
        };

        // Rotate coordinates around center
        sf::Vector2f position[4] = {
            {b->x +  rad * (float)sin(dir - 3.1415f/4),
             b->y + -rad * (float)cos(dir - 3.1415f/4)},

            {b->x +  rad * (float)sin(dir + 3.1415f/4),
             b->y + -rad * (float)cos(dir + 3.1415f/4)},

            {b->x +  rad * (float)sin(dir + 3 * 3.1415f/4),
             b->y + -rad * (float)cos(dir + 3 * 3.1415f/4)},

            {b->x +  rad * (float)sin(dir + 5 * 3.1415f/4),
             b->y + -rad * (float)cos(dir + 5 * 3.1415f/4)},
        };

        vertices.append({position[0], color, texCoords[0]});
        vertices.append({position[1], color, texCoords[1]});
        vertices.append({position[2], color, texCoords[2]});
        vertices.append({position[3], color, texCoords[3]});

        collision.addBullet(*iter);

        ++iter;
    }
}

void BulletManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (bulletTexture != nullptr)
        states.texture = bulletTexture;
    target.draw(vertices, states);
}

unsigned int BulletManager::getVertexCount() const
{
    return vertexCount;
}

void BulletManager::increaseCapacity(unsigned int blockSize)
{
    BulletLuaManager::increaseCapacity(blockSize);

    increaseVertexCount(blockSize);
}

void BulletManager::increaseVertexCount(unsigned int blockSize)
{
    vertexCount += blockSize * 4;
    vertices.resize(vertexCount);
}

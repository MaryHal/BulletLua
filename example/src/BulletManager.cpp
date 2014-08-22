#include "BulletManager.hpp"

BulletManager::BulletManager()
{
    // Superclass constructor(BulletLuaManager) has no arguments, so it's called implicitly
    vertexCount = 4 * BLOCK_SIZE;
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(vertexCount);
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

    for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
    {
        BulletLua* bullet = *iter;

        if (bullet->isDead())
        {
            freeBullets.push(bullet);
            iter = bullets.erase(iter);
        }
        else
        {
            bullet->run();

            const Bullet* b = bullet;
            float rad = std::sqrt(8*8 + 8*8);
            float dir = b->getDirection();

            sf::Color color(255, 255, 255, b->life);
            sf::Vector2f texCoords[] = {
                {0.0f, 0.0f},
                {32.0f, 0.0f},
                {32.0f, 32.0f},
                {0.0f, 32.0f}
            };

            sf::Vertex v1(sf::Vector2f(b->x +  rad * sin(dir - 3.1415f/4),
                                       b->y + -rad * cos(dir - 3.1415f/4)),
                          color,
                          texCoords[0]);

            sf::Vertex v2(sf::Vector2f(b->x +  rad * sin(dir + 3.1415f/4),
                                       b->y + -rad * cos(dir + 3.1415f/4)),
                          color,
                          texCoords[1]);

            sf::Vertex v3(sf::Vector2f(b->x +  rad * sin(dir + 3 * 3.1415f/4),
                                       b->y + -rad * cos(dir + 3 * 3.1415f/4)),
                          color,
                          texCoords[2]);

            sf::Vertex v4(sf::Vector2f(b->x +  rad * sin(dir + 5 * 3.1415f/4),
                                       b->y + -rad * cos(dir + 5 * 3.1415f/4)),
                          color,
                          texCoords[3]);

            vertices.append(v1);
            vertices.append(v2);
            vertices.append(v3);
            vertices.append(v4);

            collision.addBullet(bullet);
        }
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

    vertexCount += blockSize * 4;
    vertices.resize(vertexCount);
}

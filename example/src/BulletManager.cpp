#include "BulletManager.hpp"
#include <bulletlua/BulletLua.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>

BulletManager::BulletManager(int left, int top, int width, int height,
                             const BulletLuaUtils::Rect& player)
    : BulletLuaManager(left, top, width, height, player),
      vbo(0),
      bulletCount(0),
      tex(0)
{
    // Superclass constructor(BulletLuaManager) has no arguments, so it's called implicitly

    // // Calculate and create room for initial set of vertices.
    // vertices.setPrimitiveType(sf::Quads);
    // increaseVertexCount();

    // Load image texture
    int w, h, comp;
    unsigned char* imageData = stbi_load("bullet2.png", &w, &h, &comp, STBI_rgb_alpha);

    if(imageData == nullptr)
        throw(std::string("Failed to load texture"));

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    else if(comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);

    // Generate vertex buffer object buffer.
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

BulletManager::~BulletManager()
{
}

void BulletManager::tick()
{
    collision.reset();

    unsigned int i = 0;
    for (auto iter = bullets.begin(); iter != bullets.end();)
    {
        // Must be set so lua knows which bullet to modify.
        current = *iter;

        (*iter)->run(collision);

        // If the current bullet is dead, push it onto the free stack.
        // Keep in mind `erase` increments our iterator and returns a valid iterator.
        if ((*iter)->isDead())
        {
            freeBullets.push(*iter);
            iter = bullets.erase(iter);
            continue;
        }

        // Our vertex arrays are not dynamic (for now), so we gotta cap the number of entries.
        if (i > MAX_BULLETS)
            continue;

        const Bullet* b = *iter;
        float rad = std::sqrt(8*8 + 8*8);
        float dir = b->getDirection();

        // Unrolled loops!

        float red   = b->r / 256.0f;
        float green = b->g / 256.0f;
        float blue  = b->b / 256.0f;
        float alpha = b->life / 256.0f;

        colorArray[i * 16 + 0]  = red;
        colorArray[i * 16 + 1]  = green;
        colorArray[i * 16 + 2]  = blue;
        colorArray[i * 16 + 3]  = alpha;

        colorArray[i * 16 + 4]  = red;
        colorArray[i * 16 + 5]  = green;
        colorArray[i * 16 + 6]  = blue;
        colorArray[i * 16 + 7]  = alpha;

        colorArray[i * 16 + 8]  = red;
        colorArray[i * 16 + 9]  = green;
        colorArray[i * 16 + 10] = blue;
        colorArray[i * 16 + 11] = alpha;

        colorArray[i * 16 + 12] = red;
        colorArray[i * 16 + 13] = green;
        colorArray[i * 16 + 14] = blue;
        colorArray[i * 16 + 15] = alpha;

        textureArray[i * 8 + 0] = 0.0f;
        textureArray[i * 8 + 1] = 0.0f;

        textureArray[i * 8 + 2] = 1.0f;
        textureArray[i * 8 + 3] = 0.0f;

        textureArray[i * 8 + 4] = 1.0f;
        textureArray[i * 8 + 5] = 1.0f;

        textureArray[i * 8 + 6] = 0.0f;
        textureArray[i * 8 + 7] = 1.0f;

        // Rotate coordinates around center
        vertexArray[i * 8 + 0] = b->position.x +  rad * (float)sin(dir - 3.1415f/4);
        vertexArray[i * 8 + 1] = b->position.y + -rad * (float)cos(dir - 3.1415f/4);

        vertexArray[i * 8 + 2] = b->position.x +  rad * (float)sin(dir + 3.1415f/4);
        vertexArray[i * 8 + 3] = b->position.y + -rad * (float)cos(dir + 3.1415f/4);

        vertexArray[i * 8 + 4] = b->position.x +  rad * (float)sin(dir + 3 * 3.1415f/4);
        vertexArray[i * 8 + 5] = b->position.y + -rad * (float)cos(dir + 3 * 3.1415f/4);

        vertexArray[i * 8 + 6] = b->position.x +  rad * (float)sin(dir + 5 * 3.1415f/4);
        vertexArray[i * 8 + 7] = b->position.y + -rad * (float)cos(dir + 5 * 3.1415f/4);

        if ((*iter)->collisionCheck)
        {
            collision.addBullet(*iter);
        }

        ++i;
        ++iter;
    }

    bulletCount = i;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertexArray) + sizeof(colorArray) + sizeof(textureArray),
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(vertexArray),
                    vertexArray);

    glBufferSubData(GL_ARRAY_BUFFER,
                    sizeof(vertexArray),
                    sizeof(colorArray),
                    colorArray);

    glBufferSubData(GL_ARRAY_BUFFER,
                    sizeof(vertexArray) + sizeof(colorArray),
                    sizeof(textureArray),
                    textureArray);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BulletManager::draw() const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glColorPointer(4, GL_FLOAT, 0, (void*)(sizeof(vertexArray)));
    glTexCoordPointer(2, GL_FLOAT, 0, (void*)(sizeof(vertexArray) + sizeof(colorArray)));
    glDrawArrays(GL_QUADS, 0, bulletCount * 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_TEXTURE_2D);
}

void BulletManager::drawCollision() const
{
    for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
    {
        if (!(*iter)->isDying())
        {
            float x = (*iter)->position.x;
            float y = (*iter)->position.y;
            float w = (*iter)->position.w;
            float h = (*iter)->position.h;

            glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_QUADS);
            {
                glVertex2f(x    , y);
                glVertex2f(x + w, y);
                glVertex2f(x + w, y + h);
                glVertex2f(x    , y + h);
            }
            glEnd();
        }
    }
}

unsigned int BulletManager::getVertexCount() const
{
    return bulletCount;
}

// void BulletManager::increaseCapacity(unsigned int blockSize)
// {
//     BulletLuaManager::increaseCapacity(blockSize);

//     increaseVertexCount(blockSize);
// }

// void BulletManager::increaseVertexCount(unsigned int blockSize)
// {
//     vertexCount += blockSize * 4;
//     vertices.resize(vertexCount);
// }

#ifndef _BulletManager_hpp_
#define _BulletManager_hpp_

#include <SFML/Graphics.hpp>

#include "BulletLuaManager.hpp"

class BulletManager : public BulletLuaManager,
                      public sf::Drawable,
                      public sf::Transformable
{
    public:
        BulletManager();
        ~BulletManager();

        void setTexture(sf::Texture& tex);

        virtual void tick();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        unsigned int getVertexCount() const
        {
            return vertexCount;
        }

    private:
        virtual void increaseCapacity(unsigned int blockSize=BLOCK_SIZE);

    private:
        sf::Texture* bulletTexture;
        sf::VertexArray vertices;
        unsigned int vertexCount;
};

#endif // _BulletManager_hpp_


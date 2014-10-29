#ifndef _BulletManager_hpp_
#define _BulletManager_hpp_

#include <SFML/Graphics.hpp>

#include "Utils/Rect.hpp"
#include "BulletLuaManager.hpp"

class BulletManager : public BulletLuaManager,
                      public sf::Drawable,
                      public sf::Transformable
{
    public:
        BulletManager(const BulletLuaUtils::Rect& area);
        ~BulletManager() final;

        void setTexture(sf::Texture& tex);

        void tick() final;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        unsigned int getVertexCount() const;

    private:
        void increaseCapacity(unsigned int blockSize=BLOCK_SIZE) final;
        void increaseVertexCount(unsigned int blockSize=BLOCK_SIZE);

    private:
        sf::Texture* bulletTexture;
        sf::VertexArray vertices;
        unsigned int vertexCount;
};

#endif // _BulletManager_hpp_


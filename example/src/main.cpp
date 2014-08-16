#include <SFML/Graphics.hpp>

#include "Bullet.hpp"
#include "BulletManager.hpp"

#include <cstdio>

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "BulletLua Example", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Bullet origin(320.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(320.0f, 320.0f, 0.0f, 0.0f);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("bullet2.png");

    BulletManager manager;
    manager.setTexture(bulletTexture);
    /* printf("%d %d %d %d\n", manager.bulletCount(), manager.freeCount(), manager.blockCount(), manager.getVertexCount()); */

    manager.createBullet("test.lua", &origin, &destination);

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        manager.tick();
        /* manager.checkCollision(&destination); */
        window.draw(manager);

        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>

#include "Bullet.hpp"
#include "BulletManager.hpp"

#include <cstdio>

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(320, 240), "BulletLua Example", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Bullet origin(160.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(160.0f, 200.0f, 0.0f, 0.0f);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("bullet2.png");

    BulletManager manager;
    manager.setTexture(bulletTexture);

    manager.createBullet("test2.lua", &origin, &destination);

    // Run the program as long as the window is open
    int frame = 0;
    sf::Image image;
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

        window.clear(sf::Color(246, 246, 246));

        manager.tick();
        /* manager.checkCollision(&destination); */
        window.draw(manager);

        window.display();

        // Save frame to file
        {
            char buffer[32];
            sprintf(buffer, "ss/frame%03d.png", frame);

            image = window.capture();
            image.saveToFile(buffer);

            frame++;
        }
    }

    return 0;
}

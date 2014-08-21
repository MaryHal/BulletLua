#include <SFML/Graphics.hpp>

#include "Bullet.hpp"
#include "BulletManager.hpp"

#include <cstdio>
#include <cstring>

int main(int argc, char* argv[])
{
    bool capture = false;
    if (argc > 1 && strcmp(argv[1], "-c") == 0)
    {
        capture = true;
    }

    sf::RenderWindow window(sf::VideoMode(320, 240), "BulletLua Example", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Bullet origin(160.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(160.0f, 200.0f, 0.0f, 0.0f);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("bullet2.png");

    BulletManager manager;
    manager.setTexture(bulletTexture);

    manager.createBullet("test3.lua", &origin, &destination);

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

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        destination.x = mousePos.x;
        destination.y = mousePos.y;

        manager.tick();
        if (manager.checkCollision(destination))
        {
            manager.vanishAll();
        }

        window.draw(manager);

        window.display();

        // Save frame to file
        if (capture)
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

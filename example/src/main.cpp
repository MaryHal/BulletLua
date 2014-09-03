#include <SFML/Graphics.hpp>

#include "Bullet.hpp"
#include "BulletManager.hpp"

#include <cstdio>
#include <cstring>
#include <string>

int main(int argc, char* argv[])
{
    bool capture = false;
    std::string filename = "script/test.lua";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
            capture = true;
        else
            filename = argv[i];
    }

    sf::RenderWindow window(sf::VideoMode(640, 480), "BulletLua Example", sf::Style::Close);
    window.setPosition({100, 100});
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Bullet origin(320.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(320.0f, 240.0f, 0.0f, 0.0f);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("bullet2.png");

    BulletManager manager;
    manager.setTexture(bulletTexture);

    printf("Running Script: %s\n", filename.c_str());
    manager.createBullet(filename, &origin, &destination);

    // Run the program as long as the window is open
    int frame = 0;
    while (window.isOpen())
    {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    manager.clear();
                    manager.createBullet(filename, &origin, &destination);
                }
            }
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

            sf::Image image = window.capture();
            image.saveToFile(buffer);

            frame++;
        }
    }

    return 0;
}

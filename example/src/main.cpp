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
    window.setPosition({32, 32});
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("DroidSansFallback.ttf");

    sf::Text infoText("", font, 11);
    infoText.setColor(sf::Color(30, 30, 30, 255));
    infoText.setPosition(8.0f, 8.0f);

    Bullet origin(320.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(320.0f, 240.0f, 0.0f, 0.0f);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("bullet2.png");

    // Create a new bullet manager and make it govern the window + 100px padding
    BulletManager manager(-100, -100, 840, 680);
    manager.setTexture(bulletTexture);

    printf("Running Script: %s\n", filename.c_str());
    manager.createBullet(filename, &origin, &destination);

    // Run the program as long as the window is open
    sf::Time updateTime;
    std::size_t frameCount = 0;
    unsigned int fps = 0;

    sf::Clock frameClock;
    frameClock.restart();

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
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }

            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
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

        sf::Time elapsedTime = frameClock.restart();
        updateTime += elapsedTime;
        frameCount += 1;

        // Calculate FPS
        if (updateTime >= sf::seconds(1.0f))
        {
            fps = frameCount;
            updateTime -= sf::seconds(1.0f);
            frameCount = 0;
        }

        // Setup string for infoText
        char infoBuffer[128];
        sprintf(infoBuffer,
                "fps: %d\nBulletCount: %d\nFreeCount: %d\nBlockCount: %d",
                fps, manager.bulletCount(), manager.freeCount(), manager.blockCount());
        infoText.setString(infoBuffer);

        // Draw everything
        window.draw(manager);
        window.draw(infoText);

        window.display();

        // Save frame to file
        if (capture)
        {
            // Build filename
            char buffer[32];
            sprintf(buffer, "ss/frame%03d.png", frame);

            sf::Image image = window.capture();
            image.saveToFile(buffer);

            frame++;
        }
    }

    return 0;
}

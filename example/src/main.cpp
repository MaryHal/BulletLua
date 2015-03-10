// BulletLua SDL2 / Opengl(2.0) example

#include <iostream>
#include <string>

#include <bulletlua/Bullet.hpp>
#include <bulletlua/Utils/Rect.hpp>
#include "BulletManager.hpp"

#include "Stopwatch.hpp"

#include "Font.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <stdarg.h>

std::string format(const char* format, ...)
{
    char buf[1024];

    va_list marker;
    va_start(marker, format);
    vsprintf(buf, format, marker);
    va_end(marker);

    return buf;
}

int main(int argc, char *argv[])
{
    std::string filename = "script/test.lua";

    if (argc > 1)
    {
        filename = argv[1];
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("My Game Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        std::cout << "Error creating OpenGL context." << std::endl;
        return 1;
    }

    const unsigned char *version = glGetString(GL_VERSION);
    if (version == nullptr)
    {
        std::cout << "Error creating OpenGL context." << std::endl;
        return 1;
    }

    // SDL_GL_MakeCurrent(window, glContext);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

    // MUST make a context AND make it current BEFORE glewInit()!
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != 0)
    {
        std::cout << "Error initializing glew: " << glewGetErrorString(glewStatus) << std::endl;
        return 1;
    }

    // OpenGL 2d perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);

    // Initialize modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    Bullet origin{320.0f, 120.0f, 0.0f, 0.0f};
    BulletLuaUtils::Rect player{320.0f, 240.0f, 4.0f, 4.0f};

    // Create a new bullet manager and make it govern the window + 100px padding
    BulletManager manager{-100, -100, 840, 680, player};
    bool collision{false};
    bool frameAdvanceMode{false};

    float bestTime{0.0f};
    Stopwatch timer;

    // Create Font
    Font font{"DroidSansFallback.ttf"};

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    timer.start();
                    manager.clear();
                    manager.createBulletFromFile(filename, &origin);
                }
                else if (e.key.keysym.sym == SDLK_c)
                {
                    collision = !collision;
                }
                else if (e.key.keysym.sym == SDLK_f)
                {
                    frameAdvanceMode = !frameAdvanceMode;
                }
                else if (e.key.keysym.sym == SDLK_a)
                {
                    if (frameAdvanceMode)
                        manager.tick();
                }
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!frameAdvanceMode)
            manager.tick();

        int x, y;
        SDL_GetMouseState(&x, &y);
        player.x = x;
        player.y = y;

        if (manager.checkCollision())
        {
            manager.vanishAll();

            if (timer.getFloatTime() > bestTime)
                bestTime = timer.getFloatTime();
            timer.stop();
        }

        glBegin(GL_QUADS);
        {
            glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            glVertex2f(player.x           , player.y);
            glVertex2f(player.x + player.w, player.y);
            glVertex2f(player.x + player.w, player.y  + player.h);
            glVertex2f(player.x           , player.y  + player.h);
        }
        glEnd();

        manager.draw();

        if (collision)
            manager.drawCollision();

        glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
        font.draw(10.0f, 20.0f, format("Current %.3f", timer.getFloatTime()));
        font.draw(10.0f, 40.0f, format("Best %.3f", bestTime));
        font.draw(10.0f, 470.0f, "Press Space to Begin");
        font.draw(460.0f, 430.0f, "c = toggle collision boxes");
        font.draw(460.0f, 450.0f, "f = frame advance mode");
        font.draw(460.0f, 470.0f, "a = advance frame");

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}

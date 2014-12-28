// #include "Bullet.hpp"
// #include "BulletManager.hpp"

#include <iostream>
#include <string>

#include "Bullet.hpp"
#include "BulletManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>

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
    if (glContext == NULL)
    {
        std::cout << "Error creating OpenGL context." << std::endl;
        return 1;
    }

    const unsigned char *version = glGetString(GL_VERSION);
    if (version == NULL)
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

    //MUST make a context AND make it current BEFORE glewInit()!
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

    Bullet origin(320.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(320.0f, 240.0f, 0.0f, 0.0f);

    // Create a new bullet manager and make it govern the window + 100px padding
    BulletManager manager(-100, -100, 840, 680);
    manager.createBullet(filename, &origin, &destination);

    bool running = true;
    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_KEYUP)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    manager.clear();
                    manager.createBullet(filename, &origin, &destination);
                }
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        manager.tick();

        int x, y;
        SDL_GetMouseState(&x, &y);
        destination.x = x;
        destination.y = y;
        if (manager.checkCollision(destination))
        {
            manager.vanishAll();
        }

        manager.draw();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}

// BulletLua SDL2 / Opengl(2.0) example

#include <iostream>
#include <string>

#include "Bullet.hpp"
#include "BulletManager.hpp"

#include "Stopwatch.hpp"

// I'm a huge set-top box fan.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

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

const unsigned int BITMAP_W = 512;
const unsigned int BITMAP_H = 512;

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[BITMAP_H][BITMAP_W];
stbtt_packedchar pdata[256*2];
unsigned int ftex;

void my_stbtt_initfont(void)
{
    fread(ttf_buffer, 1, 1 << 20, fopen("DroidSansFallback.ttf", "rb"));

    stbtt_pack_context pc;
    stbtt_pack_range pr[1];
    stbtt_PackBegin(&pc, temp_bitmap[0], BITMAP_W, BITMAP_H, 0, 1, NULL);

    pr[0].chardata_for_range = pdata;
    pr[0].first_unicode_char_in_range = 32;
    pr[0].num_chars_in_range = 95;
    pr[0].font_size = 20.0f;

    stbtt_PackSetOversampling(&pc, 2, 2);
    stbtt_PackFontRanges(&pc, ttf_buffer, 0, pr, 1);
    stbtt_PackEnd(&pc);

    // stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!

    // can free ttf_buffer at this point
    glGenTextures(1, &ftex);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);

    // can free temp_bitmap at this point
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void my_stbtt_print(float x, float y, const std::string& text)
{
    glEnable(GL_TEXTURE_2D);

    // assume orthographic projection with units = screen pixels, origin at top left
    glBindTexture(GL_TEXTURE_2D, ftex);

    glBegin(GL_QUADS);
    for (char c : text)
    {
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(pdata, BITMAP_W, BITMAP_H, c-32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9

        glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
        glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
        glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
        glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
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

    Bullet origin(320.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(320.0f, 240.0f, 0.0f, 0.0f);

    // Create a new bullet manager and make it govern the window + 100px padding
    BulletManager manager(-100, -100, 840, 680);

    // Create Font
    my_stbtt_initfont();

    float bestTime = 0.0f;
    Stopwatch timer;

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
            else if (e.type == SDL_KEYUP)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    timer.start();
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

            if (timer.getFloatTime() > bestTime)
                bestTime = timer.getFloatTime();
            timer.stop();
        }

        manager.draw();

        glColor4f(0.2f, 0.2f, 0.2f, 1.0f);

        std::string curTimeStr  = format("Current %.3f", timer.getFloatTime());
        std::string bestTimeStr = format("Best %.3f", bestTime);
        my_stbtt_print(10.0f, 20.0f, curTimeStr);
        my_stbtt_print(10.0f, 40.0f, bestTimeStr);
        my_stbtt_print(10.0f, 460.0f, "Press Space to Begin");

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}

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

#define GLSL(version, shader)  "#version " #version "\n" #shader
void printShaderLog( GLuint shader );

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

    // Shader Program
    GLuint prog = glCreateProgram();

    // Create Shader program
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Get vertex source
        const GLchar* vertexShaderSource =
            GLSL(130,
                 in vec2 vertex_position;
                 // in vec4 vertex_color;

                 // out vec4 color;

                 void main () {
                     // color = vertex_color;
                     gl_Position = vec4(vertex_position, 0.0, 1.0);
                 }
                );

        // Get fragment source
        const GLchar* fragmentShaderSource =
            GLSL(130,
                 precision highp float;

                 // in vec4 color;
                 out vec4 frag_color;

                 void main () {
                     // frag_color = color;
                     frag_color = vec4(1.0, 1.0, 1.0, 1.0);
                 }
                );

        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLint vShaderCompiled = GL_FALSE;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
        if (vShaderCompiled != GL_TRUE)
        {
            std::cout << "Unable to compile vertex shader " << vertexShader << std::endl;
            printShaderLog(vertexShader);
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        // Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE)
        {
            std::cout << "Unable to compile fragment shader " << fragmentShader << std::endl;
            printShaderLog(fragmentShader);
        }

        glAttachShader(prog, vertexShader);
        glAttachShader(prog, fragmentShader);

        glBindAttribLocation(prog, 0, "vertex_position");
        glBindAttribLocation(prog, 1, "vertex_color");

        glLinkProgram(prog);

        GLint programSuccess = GL_TRUE;
        glGetProgramiv(prog, GL_LINK_STATUS, &programSuccess);
        if (programSuccess != GL_TRUE)
        {
            std::cout << "Unable to link program " << prog << std::endl;
        }
    }

    float points[] = {
        0.0f,   0.0f,
        10.0f,  5.0f,
        0.0f,   10.0f
    };

    GLuint vbo = 0;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 6 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    Bullet origin(320.0f, 120.0f, 0.0f, 0.0f);
    Bullet destination(320.0f, 240.0f, 0.0f, 0.0f);

    // Create a new bullet manager and make it govern the window + 100px padding
    BulletManager manager(-100, -100, 840, 680);
    manager.createBullet(filename, &origin, &destination);

    bool collision = false;
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
                    manager.clear();
                    manager.createBullet(filename, &origin, &destination);
                }
                else if (e.key.keysym.sym == SDLK_c)
                {
                    collision = !collision;
                }
            }
        }

        // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // manager.tick();

        // int x, y;
        // SDL_GetMouseState(&x, &y);
        // destination.x = x;
        // destination.y = y;

        // if (collision)
        // {
        //     if (manager.checkCollision(destination))
        //     {
        //         manager.vanishAll();
        //     }
        // }


        // Drawing!
        glUseProgram(prog);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // manager.draw();

        glUseProgram(0);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}

void printShaderLog(GLuint shader)
{
    //Make sure name is shader
    if (glIsShader(shader))
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //  Print Log
            std::cout << infoLog << std::endl;
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        std::cout << "Name " << shader << " is not a shader" << std::endl;
    }
}

BulletLua
=========

This C++ project aims to create a sane scriptable interface to define bullet patterns in shoot 'em up games. These patterns, while difficult to dodge in game and hard to appreciate while in the heat of the moment, can be beautiful to spectate.

Bullet Barrage Examples:

[![Pattern1](https://www.youtube.com/watch?v=yCOYrMgnNGE/0.jpg)](http://youtu.be/yCOYrMgnNGE?t=11m39s)

[![Pattern2](https://www.youtube.com/watch?v=VABlU6nh_2k/0.jpg)](http://youtu.be/VABlU6nh_2k?t=30m24s)

BulletLua is not mature enough at the moment to recreate the complex patterns in the videos above, but it's getting there.

Usage
=========
This project depends on [Lua](http://www.lua.org/), so make sure your compiler can find correct files (library, header files, etc).
For ease of use, [sol](https://github.com/Rapptz/sol.git) is used as a modern C++ binding to Lua. Remember to

    git submodule update --init --recursive

after you clone this repository!

Both sol and BulletLua use C++11 features, so make sure you have a C++11 capable compiler.

A Makefile is provided, a simple

    make

(or if you want to build without debugging symbols)

    make config=release

in the terminal should be enough to build the project. I use [premake](http://industriousone.com/what-premake) to generate my makefiles, which can also output other project types. So look into that if you wish to build this outside the terminal.

Link the library generated in the `lib` directory and also make sure the headers can be found, and you're halfway there.

Because there are so many use cases out there, BulletLua does not do any drawing of sprites. It simply runs lua scripts, manages the generated bullets (BulletLuaManager.hpp), and provides a simple method for collision detection. As such, you'll need to produce your own code to draw the bullets. This can be as simple as creating a class to inherit from BulletLuaManager and creating a draw method. Example:

    class BulletManager : public BulletLuaManager
    {
        public:
            void draw()
            {
                for (Bullet* b : bullets)
                {
                    drawPoint(b->x, b->y);
                }
            }
    };

Then to run a script...

    BulletManager manager;
    Bullet origin(x, y, vx, vy);
    Bullet destination(x, y, vx, vy);

    manager.createBullet("MyFile.lua", &origin, &destination);

    // Game Loop
    while (running)
    {
        manager.tick();
        manager.draw();
    }

A moderately complex example can be found in the `example` directory.

An example BulletLua script can be found in `example/bin/test.lua`. More examples to come.


BulletLua [![build](https://travis-ci.org/sanford1/BulletLua.svg)](https://travis-ci.org/sanford1/BulletLua/builds)
=========

This C++ project aims to create a sane scriptable interface to define bullet patterns in shoot 'em up games. These patterns, while difficult to dodge in game and hard to appreciate while in the heat of the moment, can be beautiful to spectate.

Bullet Barrage Examples:

[Example 1](http://youtu.be/yCOYrMgnNGE?t=11m39s)

[Example 2](http://youtu.be/v4m08AeGFWU?t=33m22s)

BulletLua is not quite mature enough at the moment to recreate the complex patterns in the videos above, but it's getting there.


![test.lua](http://zippy.gfycat.com/PeriodicUnnaturalHartebeest.gif)

![test2.lua](http://giant.gfycat.com/AmbitiousSpecificAracari.gif)

![test3.lua](http://giant.gfycat.com/BelatedFakeBoilweevil.gif)


Usage
=========
This project depends on [Lua](http://www.lua.org/), so make sure your compiler can find the correct Lua files (static library, header files, etc).

For ease of use, [sol](https://github.com/Rapptz/sol.git) is used as a modern C++ binding to Lua. Remember to

    git submodule update --init --recursive

after you clone this repository!

Both sol and BulletLua use C++11 features, so make sure you have a C++11-capable compiler.

BulletLua is now a header-only library, so you just need to make sure your project can find the headers in the `include` directory.

Because there are so many use cases out there, BulletLua doesn't actually draw any sprites. It simply runs lua scripts, manages the generated bullets, and provides a simple method for collision detection. As such, you'll need to produce your own code to draw the bullets. This can be as simple as creating a class to inherit from BulletLuaManager and creating a draw method. Example:

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

A moderately complex example (using [SDL2](http://libsdl.org/) and OpenGL) can be found in the `example` directory. To build it easily, use the [ninja](https://martine.github.io/ninja/) script. The source code for the older example that uses [SFML](http://www.sfml-dev.org/) still exists in the `example` directory as well.

Lua Binding
=========

C++ Functions visible in BulletLua scripts:

    -- Get Position (as a tuple).
    x, y = getPosition()

    -- Get Velocity Components (as a tuple).
    vx, vy = getVelocity()

    -- Get Bullet Speed and Direction.
    getSpeed()
    getDir()

    -- Get the amount of frames since this bullet's creation.
    getTurn()

    -- Get the current barrage "difficulty", From [0.0, 1.0]
    getRank()

    -- Generate random floating point numbers
    randFloat()
    randFloat(float min, float max)

    -- Generate random integers
    randInt(int max)
    randInt(int min, int max)

    -- Set Bullet Position.
    setPosition(float x, float y)

    -- Set Bullet Velocity.
    setVelocity(float vx, float vy)

    -- Set Bullet Direction
    setDirection(double dir)

    -- Set Bullet Direction (relatively), adds to current direction.
    setDirectionRelative(double dir)

    -- Set the current bullet to aim at the "player"
    aimTarget()

    -- Set the current bullet to aim at a point
    aimPoint(float x, float y)

    -- Set Speed
    setSpeed(float s)
    setSpeedRelative(float s)

    -- Switch current running function
    setFunction(const sol::function& funcName)

    -- Shoot a bullet at (x, y) moving in direction (d) at speed (s) running function (func).
    fire(double d, double s, const sol::function& funcName)

    -- Shoot (segments) bullets in a circle at speed (s) running function (func).
    fireCircle(int segments, float s, const sol::function& funcName)

    -- Fade out the current bullet. Kill it slowly.
    vanish()

    -- Immediately destroy the bullet.
    kill()

An example BulletLua script can be found in `example/bin/script/test.lua`. More examples to come.

Examples
=========

This script should produce something similar to the first gif above:

    -- BulletLua Test Script

    dir = 0

    function main()
        local turn = getTurn()
        local rank = getRank()

        if (math.fmod(turn, 15) == 0) then
            fire(dir, 1.5, explode)
            dir = dir + 34
        end
    end

    function explode()
        local turn = getTurn()
        if (turn == 60) then
            fireCircle(40, 4.0, homeIn)

            kill()
        end
    end

    function homeIn()
        local turn = getTurn()
        if (turn == 20) then
            setSpeed(1)
        elseif (turn == 25) then
            aimTarget()
            setSpeed(10)
        elseif (turn == 50) then
            vanish()
        end
    end

Right now, BulletLua associates a bullet with a function and runs that function every frame. However, since it's difficult to dynamically save arbitrary function arguments in a single container, when you set a bullet to run a function, that function cannot have parameters. A way to bypass this is by using a `bind` function. `bind` will create a wrapper function for an existing function and forward an argument to it. So effectively, you can transform any function with parameters into a (new) function with no parameters. This example (`example/bin/script/test5.lua`) should show a simple use of `bind`.

![test5.lua](http://giant.gfycat.com/IncompatibleBruisedIbisbill.gif)

    -- BulletLua Test Script 5

    cycles = 0
    gwait = 20

    function bind(f,...)
       local args = {...}
       return function(...)
          return f(unpack(args),...)
       end
    end

    function main()
       circle = bind(fireCircle, 40)
       setPosition(320, 240)
       setFunction(bind(go, 10))
    end

    function go(wait)
       local turn = getTurn()
       if (math.fmod(turn, wait) == 0) then
          -- speeds = {4, 5, 6}
          circle(21 - gwait, curve)

          setFunction(bind(go, gwait))
          gwait = gwait - 1
          if gwait < 18 then
             gwait = 20
          end

          cycles = cycles + 1
       end
       if (cycles == 15) then
          vanish()
       end
    end

    function curve()
       local turn = getTurn()
       setDirectionRelative(5)
       setSpeedRelative(0.1)

       if (turn > 180) then
          vanish()
       end
    end

A second, perhaps more useful, example of `bind` can be found in `example/bin/script/test6.lua`

Future Plans
=========

* Smooth out the C++ interface and then introduce a mechanism to convert BulletLua scripts to C++.
* Be able to desribe collision detection area.
* More robust collision detection.
* Be able to agnostically define different types of bullets.

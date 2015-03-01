#ifndef _BulletLua_hpp_
#define _BulletLua_hpp_

#include <memory>
#include <sol.hpp>

#include <bulletlua/Bullet.hpp>

class SpacialPartition;

class BulletLua : public Bullet
{
    public:
        BulletLua();

        void makeReusable(Bullet* target);

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 Bullet* origin, Bullet* target);

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 float x, float y, float d, float s,
                 Bullet* target);

        void run(const SpacialPartition& collision);
        void setFunction(const sol::function& func);

    public:
        Bullet* target;

        std::shared_ptr<sol::state> luaState;
        sol::function func;
};

#endif // _BulletLua_hpp_

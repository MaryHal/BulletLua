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

        void makeReusable();

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 Bullet* origin);

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 float x, float y, float d, float s);

        bool run(const SpacialPartition& collision);
        void setFunction(const sol::function& func);

    public:
        std::shared_ptr<sol::state> luaState;
        sol::function func;
};

#endif // _BulletLua_hpp_

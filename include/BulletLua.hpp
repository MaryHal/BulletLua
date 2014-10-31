#ifndef _BulletLua_hpp_
#define _BulletLua_hpp_

#include <string>
#include <memory>

#include <sol.hpp>

#include "Bullet.hpp"

class BulletLuaManager;
class SpacialPartition;

class BulletLua : public Bullet
{
    private:
        static BulletLua* current;

    public:
        BulletLua();

        void makeReusable(Bullet* target, BulletLuaManager* owner);

        void set(const std::string& filename,
                 Bullet* origin, Bullet* target,
                 BulletLuaManager* owner);

        // void set(std::shared_ptr<sol::state> lua,
        //          const sol::function& func,
        //          Bullet* origin, Bullet* target,
        //          BulletLuaManager* owner);

        void set(std::shared_ptr<sol::state> lua,
                 const sol::function& func,
                 float x, float y, float d, float s,
                 Bullet* target,
                 BulletLuaManager* owner);

        void run(const SpacialPartition& collision);

        void __debugRun(const std::string& code);

    private:
        void setFunction(const sol::function& func);
        void initLua();

    private:
        Bullet* target;

        std::shared_ptr<sol::state> luaState;
        sol::function func;

        BulletLuaManager* mOwner;
};

#endif // _BulletLua_hpp_

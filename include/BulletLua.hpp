#ifndef _BulletLua_hpp_
#define _BulletLua_hpp_

#include <string>
#include <memory>

#include "Bullet.hpp"

class BulletLuaManager;

namespace sol
{
    class state;
};

class BulletLua : public Bullet
{
    private:
        static BulletLua* current;

    public:
        BulletLua();

        void reset();

        void set(const std::string& filename,
                 Bullet* origin, Bullet* target,
                 BulletLuaManager* owner);

        void set(std::shared_ptr<sol::state> lua,
                 const std::string& func,
                 Bullet* origin, Bullet* target,
                 BulletLuaManager* owner);

        void set(std::shared_ptr<sol::state> lua,
                 const std::string& func,
                 float x, float y, float d, float s,
                 Bullet* target,
                 BulletLuaManager* owner);

        std::shared_ptr<sol::state> getLuaState();
        int getTurn() const;
        bool isDead() const;

        void run();

        void __debugRun(const std::string& code);

    private:
        void initLua();
        void setFunctionName(const std::string& funcName);

    private:
        Bullet* target;

        std::shared_ptr<sol::state> luaState;
        std::string funcName;

        int turn;

        BulletLuaManager* mOwner;
};

#endif // _BulletLua_hpp_

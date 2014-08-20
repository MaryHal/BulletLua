#include "BulletLua.hpp"
#include "BulletLuaManager.hpp"

#include <sol.hpp>

#include "Math.hpp"

// This pointer should point to the currently "processing" bullet.
// The reason for this is that the lambda functions generated for our lua state
// are generated at compile time. This means that while many objects can
// control the lua state, the lua state can only reference the "this" pointer
// of a single bullet. The lua functions now act on this pointer instead of "this".
BulletLua* BulletLua::current = nullptr;

BulletLua::BulletLua()
    : mMover(0.0, 0.0, 0.0, 0.0),
      mTarget(nullptr),
      funcName(""),
      turn(0)
{
}

void BulletLua::reset()
{
    mTarget = nullptr;

    mMover.dead = true;
    mMover.dying = false;
    mMover.life = 255;

    luaState = nullptr;
    funcName = "";
    turn = 0;

    mOwner = nullptr;
}

void BulletLua::set(const std::string& filename,
                    Bullet* origin, Bullet* target,
                    BulletLuaManager* owner)
{
    // Copy Movers
    mMover = *origin;
    mTarget = target;

    mMover.dead = false;
    mMover.dying = false;
    mMover.life = 255;

    luaState = std::make_shared<sol::state>();
    luaState->open_file(filename);
    funcName = "main";
    turn = 0;

    mOwner = owner;

    initLua();
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const std::string& func,
                    Bullet* origin, Bullet* target,
                    BulletLuaManager* owner)
{
    // Copy Movers
    mMover = *origin;
    mTarget = target;

    mMover.dead = false;
    mMover.dying = false;
    mMover.life = 255;

    luaState = lua;
    funcName = func;
    turn = 0;

    mOwner = owner;
}

void BulletLua::set(std::shared_ptr<sol::state> lua,
                    const std::string& func,
                    double x, double y, double d, double s,
                    Bullet* target,
                    BulletLuaManager* owner)
{
    // Copy Movers
    mMover.x = x;
    mMover.y = y;
    mMover.setSpeedAndDirection(s, d);
    /* mMover.vx = vx; */
    /* mMover.vy = vy; */

    mTarget = target;

    mMover.dead = false;
    mMover.dying = false;
    mMover.life = 255;

    luaState = lua;
    funcName = func;
    turn = 0;

    mOwner = owner;
}

std::shared_ptr<sol::state> BulletLua::getLuaState()
{
    return luaState;
}

int BulletLua::getTurn() const
{
    return turn;
}

bool BulletLua::isDead() const
{
    return mMover.dead;
}

void BulletLua::run()
{
    BulletLua::current = this;

    // Run lua function
    if (!funcName.empty())
        luaState->get<sol::function>(funcName).call();

    mMover.x += mMover.vx;
    mMover.y += mMover.vy;

    /* mMover.sprite.setPosition(mMover.x, mMover.y); */

    if (mMover.x < -100.0 || mMover.x > 700.0 ||
        mMover.y < -100.0 || mMover.y > 600.0)
    {
        mMover.dead = true;
    }

    if (mMover.dying)
    {
        // Fade out over 30 frames
        mMover.life -= 255/30;
        if (mMover.life < 0)
            mMover.life = 0;
    }
    if (mMover.life == 0)
    {
        mMover.dead = true;
    }

    turn++;
}

const Bullet& BulletLua::getMover() const
{
    return mMover;
}

void BulletLua::__debugRun(const std::string& code)
{
    BulletLua::current = this;
    luaState->script(code);
}

void BulletLua::initLua()
{
    // luaState->open_libraries(sol::lib::base);
    luaState->open_libraries(sol::lib::math);

    luaState->set_function("getPosX",
                           []()
                           {
                               return BulletLua::current->mMover.x;
                           });

    luaState->set_function("getPosY",
                           []()
                           {
                               return BulletLua::current->mMover.y;
                           });

    luaState->set_function("getPosition",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->mMover.x, c->mMover.y);
                           });

    luaState->set_function("getVelX",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->mMover.vx;
                           });

    luaState->set_function("getVelY",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->mMover.vy;
                           });

    luaState->set_function("getVelocity",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return std::make_tuple(c->mMover.vx, c->mMover.vy);
                           });

    luaState->set_function("getSpeed",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->mMover.getSpeed();
                           });

    luaState->set_function("getDir",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return Math::radToDeg(c->mMover.getDirection());
                           });

    luaState->set_function("getTurn",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               return c->getTurn();
                           });

    luaState->set_function("getRank",
                           []()
                           {
                               return BulletLuaManager::rank;
                           });

    luaState->set_function("genRand",
                           [&]()
                           {
                               return mOwner->genRand();
                           });

    luaState->set_function("setPos",
                           [](double x, double y)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.x = x;
                               c->mMover.y = y;
                           });

    luaState->set_function("setVel",
                           [](double vx, double vy)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.vx = vx;
                               c->mMover.vy = vy;
                           });

    luaState->set_function("setDir",
                           [](double dir)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.setDirection(Math::degToRad(dir));
                           });

    luaState->set_function("setDirRel",
                           [](double dir)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.setDirectionRelative(Math::degToRad(dir));
                           });

    luaState->set_function("aimTarget",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.setDirectionAim(c->mTarget->x, c->mTarget->y);
                           });

    luaState->set_function("aimPoint",
                           [](float x, float y)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.setDirectionAim(x, y);
                           });

    luaState->set_function("setSpeed",
                           [](double s)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.setSpeed(s);
                           });

    luaState->set_function("setSpeedRel",
                           [](double s)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.setSpeedRelative(s);
                           });

    luaState->set_function("setFunction",
                           [](const std::string& funcName)
                           {
                               BulletLua* c = BulletLua::current;
                               c->setFunctionName(funcName);
                           });

    // luaState->set_function("fire",
    //                        [](double x, double y, double d, double s,
    //                           const std::string& funcName)
    //                        {
    //                            BulletLua* c = BulletLua::current;
    //                            c->mOwner->createBullet(c->luaState, funcName,
    //                                                    x, y,
    //                                                    Math::degToRad(d), s,
    //                                                    c->mTarget);
    //                        });

    luaState->set_function("fire",
                           [](double d, double s,
                              const std::string& funcName)
                           {
                               BulletLua* c = BulletLua::current;
                               c->mOwner->createBullet(c->luaState, funcName,
                                                       c->mMover.x, c->mMover.y,
                                                       Math::degToRad(d), s,
                                                       c->mTarget);
                           });

    luaState->set_function("fireCircle",
                           [](int segments, double s,
                              const std::string& funcName)
                           {
                               BulletLua* c = BulletLua::current;

                               float segRad = Math::PI * 2 / segments;
                               for (int i = 0; i < segments; ++i)
                               {
                                   c->mOwner->createBullet(c->luaState, funcName,
                                                           c->mMover.x, c->mMover.y,
                                                           segRad * i, s,
                                                           c->mTarget);
                               }
                           });

    luaState->set_function("vanish",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.vanish();
                           });

    luaState->set_function("kill",
                           []()
                           {
                               BulletLua* c = BulletLua::current;
                               c->mMover.kill();
                           });
}

void BulletLua::setFunctionName(const std::string& funcName)
{
    this->funcName = funcName;
}

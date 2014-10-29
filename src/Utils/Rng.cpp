#include "Rng.hpp"

namespace BulletLuaUtils
{
    std::random_device Random::rd;
    std::mt19937 Random::rng(Random::rd());

    void Random::seed(uint32_t new_seed)
    {
        rng.seed(new_seed);
    }

    float Random::genFloat()
    {
        return genFloat(0.0f, 1.0f);
    }

    float Random::genFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }

    int Random::genInt(int max)
    {
        return genInt(0, max);
    }

    int Random::genInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    uint32_t Random::gen()
    {
        return rng();
    }
};

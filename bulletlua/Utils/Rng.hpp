#ifndef _Rng_hpp_
#define _Rng_hpp_

#include <random>

namespace BulletLuaUtils
{
    class Random
    {
        public:
            std::random_device rd;
            std::mt19937 rng;

        public:
            Random();

            void seed(uint32_t new_seed=std::mt19937::default_seed);

            float genFloat();
            float genFloat(float min, float max);

            int genInt(int max);
            int genInt(int min, int max);

            uint32_t gen();
    };
}


#endif /* _Rng_hpp_ */

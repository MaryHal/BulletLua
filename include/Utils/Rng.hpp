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
            Random()
                : rd(), rng(rd())
            {
            }

            void seed(uint32_t new_seed=std::mt19937::default_seed)
            {
                rng.seed(new_seed);
            }

            float genFloat()
            {
                return genFloat(0.0f, 1.0f);
            }
            float genFloat(float min, float max)
            {
                std::uniform_real_distribution<float> dist(min, max);
                return dist(rng);
            }

            int genInt(int max)
            {
                return genInt(0, max);
            }
            int genInt(int min, int max)
            {
                std::uniform_int_distribution<int> dist(min, max);
                return dist(rng);
            }

            uint32_t gen()
            {
                return rng();
            }
    };
}


#endif /* _Rng_hpp_ */

#ifndef _Rng_hpp_
#define _Rng_hpp_

#include <random>

class Random
{
    public:
        static std::random_device rd;
        static std::mt19937 rng;

    public:
        static void seed(uint32_t new_seed=std::mt19937::default_seed);

        static float genFloat();
        static float genFloat(float min, float max);

        static int genInt(int max);
        static int genInt(int min, int max);

        static uint32_t gen();
};


#endif /* _Rng_hpp_ */

#ifndef _Rng_hpp_
#define _Rng_hpp_

// Adapted from https://gist.github.com/jrandom/64c8972b438bf8f1d0dd

#include <random>
#include <cstdint>
#include <limits>
#include <utility>

namespace BulletLuaUtils
{
    using int8_pair_t = std::pair< int_fast8_t, int_fast8_t >;
    using int16_pair_t = std::pair< int_fast16_t, int_fast16_t >;
    using int32_pair_t = std::pair< int_fast32_t, int_fast32_t >;
    using int64_pair_t = std::pair< int_fast64_t, int_fast64_t >;

    using uint8_pair_t = std::pair< uint_fast8_t, uint_fast8_t >;
    using uint16_pair_t = std::pair< uint_fast16_t, uint_fast16_t >;
    using uint32_pair_t = std::pair< uint_fast32_t, uint_fast32_t >;
    using uint64_pair_t = std::pair< uint_fast64_t, uint_fast64_t >;

    using size_t_pair_t = std::pair< std::size_t, std::size_t >;

    using float_pair_t = std::pair< float, float >;
    using double_pair_t = std::pair< double, double >;

    template <typename E>
    class Random
    {
        public:
            E engine;
            std::uniform_real_distribution<float> floatDistribution;
            std::uniform_real_distribution<double> doubleDistribution;

        public:
            explicit Random(uint_fast64_t seed)
                : engine{seed},
                  floatDistribution{0.0f, 1.0f},
                  doubleDistribution{0.0, 1.0}
            {
            }

            Random()
                : Random{ uint_fast64_t(std::random_device{}()) << 32
                    | uint_fast64_t(std::random_device{}()) }
            {
            }

            void seed(uint32_t new_seed)
            {
                engine.seed(new_seed);
            }

            uint_fast8_t bits_8 () { return std::uniform_int_distribution< uint8_t >(0, std::numeric_limits< uint8_t >::max())(engine); }
            uint_fast16_t bits_16() { return std::uniform_int_distribution< uint16_t >(0, std::numeric_limits< uint16_t >::max())(engine); }
            uint_fast32_t bits_32() { return std::uniform_int_distribution< uint32_t >(0, std::numeric_limits< uint32_t >::max())(engine); }
            uint_fast64_t bits_64() { return std::uniform_int_distribution< uint64_t >(0, std::numeric_limits< uint64_t >::max())(engine); }

            // -------------------------------------------------------------------- Integers
            int_fast8_t int_8 ( int8_t low, int8_t high ) { return std::uniform_int_distribution< int8_t >(low, high)(engine); }
            int_fast16_t int_16 ( int16_t low, int16_t high ) { return std::uniform_int_distribution< int16_t >(low, high)(engine); }
            int_fast32_t int_32 ( int32_t low, int32_t high ) { return std::uniform_int_distribution< int32_t >(low, high)(engine); }
            int_fast64_t int_64 ( int64_t low, int64_t high ) { return std::uniform_int_distribution< int64_t >(low, high)(engine); }
            uint_fast8_t uint_8 ( uint8_t low, uint8_t high ) { return std::uniform_int_distribution< uint8_t >(low, high)(engine); }
            uint_fast16_t uint_16 ( uint16_t low, uint16_t high ) { return std::uniform_int_distribution< uint16_t >(low, high)(engine); }
            uint_fast32_t uint_32 ( uint32_t low, uint32_t high ) { return std::uniform_int_distribution< uint32_t >(low, high)(engine); }
            uint_fast64_t uint_64 ( uint64_t low, uint64_t high ) { return std::uniform_int_distribution< uint64_t >(low, high)(engine); }
            std::size_t size_t ( std::size_t low, std::size_t high ) { return std::uniform_int_distribution< std::size_t >(low, high)(engine); }

            // -------------------------------------------------------------------- Integers (pairs)
            int_fast8_t int_8 ( const int8_pair_t range ) { return int_8 ( range.first, range.second ); }
            int_fast16_t int_16 ( const int16_pair_t range ) { return int_16 ( range.first, range.second ); }
            int_fast32_t int_32 ( const int32_pair_t range ) { return int_32 ( range.first, range.second ); }
            int_fast64_t int_64 ( const int64_pair_t range ) { return int_64 ( range.first, range.second ); }
            uint_fast8_t uint_8 ( const uint8_pair_t range ) { return uint_8 ( range.first, range.second ); }
            uint_fast16_t uint_16 ( const uint16_pair_t range ) { return uint_16( range.first, range.second ); }
            uint_fast32_t uint_32 ( const uint32_pair_t range ) { return uint_32( range.first, range.second ); }
            uint_fast64_t uint_64 ( const uint64_pair_t range ) { return uint_64( range.first, range.second ); }
            std::size_t size_t ( const size_t_pair_t range ) { return size_t ( range.first, range.second ); }

            // -------------------------------------------------------------------- Reals
            float float_01 () { return floatDistribution (engine); }
            double double_01 () { return doubleDistribution(engine); }
            float floatRange ( float low, float high ) { return std::uniform_real_distribution<float >(low, high)(engine); }
            double doubleRange ( double low, double high ) { return std::uniform_real_distribution<double>(low, high)(engine); }

            // -------------------------------------------------------------------- Reals (pairs)
            float floatRange ( const float_pair_t range ) { return floatRange ( range.first, range.second ); }
            double doubleRange ( const double_pair_t range ) { return doubleRange( range.first, range.second ); }

            // -------------------------------------------------------------------- Overloaded
            int_fast8_t number ( int8_t low, int8_t high ) { return int_8 (low, high); }
            int_fast16_t number ( int16_t low, int16_t high ) { return int_16(low, high); }
            int_fast32_t number ( int32_t low, int32_t high ) { return int_32(low, high); }
            int_fast64_t number ( int64_t low, int64_t high ) { return int_64(low, high); }
            uint_fast8_t number ( uint8_t low, uint8_t high ) { return uint_8 (low, high); }
            uint_fast16_t number ( uint16_t low, uint16_t high ) { return uint_16(low, high); }
            uint_fast32_t number ( uint32_t low, uint32_t high ) { return uint_32(low, high); }
            uint_fast64_t number ( uint64_t low, uint64_t high ) { return uint_64(low, high); }
            // std::size_t number ( std::size_t low, std::size_t high ) { return Size_T (low, high); }

            float number ( float low, float high ) { return floatRange (low, high); }
            double number ( double low, double high ) { return doubleRange (low, high); }

            // -------------------------------------------------------------------- Templated
            template < typename value_t >
            value_t number( std::pair<value_t, value_t> range ) { return number(range.first, range.second); }

            // -------------------------------------------------------------------- Utility
            bool chance ( float probability ) { return float_01() < probability; }
    };

    using MTRandom = Random<std::mt19937_64>;
    using LCRandom = Random<std::minstd_rand>;
    using SWCRandom = Random<std::ranlux48_base>;
}

#endif /* _Rng_hpp_ */

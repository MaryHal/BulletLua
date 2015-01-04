#ifndef _Stopwatch_hpp_
#define _Stopwatch_hpp_

#include <chrono>

class Stopwatch
{
    public:
        Stopwatch() = default;
        Stopwatch(const Stopwatch& s);
        Stopwatch& operator=(const Stopwatch& s);

        unsigned int getTime();
        float getFloatTime();

        void start();
        void stop();

        void restart();

        bool isStarted();

    private:
        std::chrono::high_resolution_clock::time_point startTime;
        bool started;
};

#endif /* _Stopwatch_hpp_ */

#include "Stopwatch.hpp"

Stopwatch::Stopwatch(const Stopwatch& s)
    : startTime(s.startTime)
{
}

Stopwatch& Stopwatch::operator=(const Stopwatch& s)
{
    startTime = s.startTime;
    return *this;
}

unsigned int Stopwatch::getTime()
{
    if (started)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

    return 0;
}

float Stopwatch::getFloatTime()
{
    if (started)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() / 1000.0f;
    }

    return 0;
}

void Stopwatch::start()
{
    started = true;
    startTime = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop()
{
    started = false;
}

void Stopwatch::restart()
{
    startTime = std::chrono::high_resolution_clock::now();
}

bool Stopwatch::isStarted()
{
    return started;
}

#pragma once

#include <chrono>

namespace dante
{
    template <typename TimeUnit>
    class Timer
    {
    public:
        using TimeRatio = TimeUnit;
        Timer();
        ~Timer() = default;

        Timer(const Timer& other) = default;
        Timer& operator=(const Timer& other) = default;
        Timer& operator=(Timer&& other) = default;

        inline double getElapsedTime();
        inline double restart();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _prevTimePoint;
    };

    template <typename TimeUnit>
    Timer<TimeUnit>::Timer()
        : _prevTimePoint(std::chrono::high_resolution_clock::now())
    {
    }

    template <typename TimeUnit>
    double Timer<TimeUnit>::getElapsedTime()
    {
        return std::chrono::duration<double, TimeUnit>(std::chrono::high_resolution_clock::now() - _prevTimePoint).count();
    }

    template <typename TimeUnit>
    double Timer<TimeUnit>::restart()
    {
        auto current = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double, TimeUnit>(current - _prevTimePoint).count();
        _prevTimePoint = current;

        return elapsed;
    }
}
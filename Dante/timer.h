#pragma once

#include <chrono>
#include <type_traits>

namespace dante
{
    class Timer
    {
    public:
        using ClockType = std::chrono::steady_clock;

        Timer();
        ~Timer() = default;

        Timer(const Timer& other) = default;
        Timer& operator=(const Timer& other) = default;
        Timer& operator=(Timer&& other) = default;

        template <typename TimeUnit = std::chrono::duration<double>>
        inline TimeUnit::rep getElapsedTime();

        template <typename TimeUnit = std::chrono::duration<double>>
        inline TimeUnit::rep restart();

    private:
        std::chrono::time_point<ClockType> _prevTimePoint;
    };

    Timer::Timer()
        : _prevTimePoint(ClockType::now())
    {
    }

    template <typename TimeUnit>
    TimeUnit::rep Timer::getElapsedTime()
    {
        return TimeUnit(ClockType::now() - _prevTimePoint).count();
    }

    template <typename TimeUnit>
    TimeUnit::rep Timer::restart()
    {
        const auto current = ClockType::now();
        const double elapsed = TimeUnit(current - _prevTimePoint).count();
        _prevTimePoint = current;

        return elapsed;
    }
}
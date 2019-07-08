#pragma once

#include "api.h"
#include <chrono>
#include <vector>

namespace pluto
{
    class PLUTO_API StopWatch
    {
    private:
        struct Interval
        {
            std::chrono::time_point<std::chrono::steady_clock> begin, end;
        };

        bool isRunning;
        std::vector<Interval> intervals;

    public:
        StopWatch();
        ~StopWatch();

        StopWatch(const StopWatch& other);
        StopWatch(StopWatch&& other) noexcept;

        StopWatch& operator=(const StopWatch& rhs);
        StopWatch& operator=(StopWatch&& rhs) noexcept;

        uint64_t GetElapsedNanoseconds() const;
        uint64_t GetElapsedMilliseconds() const;

        void Start();
        void Stop();
        void Reset();
        void Restart();
    };
}

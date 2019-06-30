#include <pluto/stop_watch.h>

namespace pluto
{
    StopWatch::StopWatch() : isRunning(false)
    {
    };

    StopWatch::~StopWatch() = default;

    StopWatch::StopWatch(const StopWatch& other) : isRunning(other.isRunning), intervals(other.intervals)
    {
    }

    StopWatch::StopWatch(StopWatch&& other) noexcept : isRunning(other.isRunning), intervals(std::move(other.intervals))
    {
    }

    StopWatch& StopWatch::operator=(const StopWatch& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        isRunning = rhs.isRunning;
        intervals = rhs.intervals;
        return *this;
    }

    StopWatch& StopWatch::operator=(StopWatch&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        isRunning = rhs.isRunning;
        intervals = std::move(rhs.intervals);
        return *this;
    }

    uint64_t StopWatch::GetElapsedNanoseconds() const
    {
        uint64_t result = 0;
        for (auto& interval : intervals)
        {
            result += std::chrono::duration_cast<std::chrono::nanoseconds>(interval.end - interval.begin).count();
        }
        return result;
    }

    uint64_t StopWatch::GetElapsedMilliseconds() const
    {
        uint64_t result = 0;
        for (auto& interval : intervals)
        {
            result += std::chrono::duration_cast<std::chrono::milliseconds>(interval.end - interval.begin).count();
        }
        return result;
    }

    void StopWatch::Start()
    {
        if (isRunning)
        {
            return;
        }

        const auto now = std::chrono::steady_clock::now();
        intervals.push_back({now, now});
        isRunning = true;
    }

    void StopWatch::Stop()
    {
        if (!isRunning)
        {
            return;
        }

        intervals[intervals.size() - 1].end = std::chrono::steady_clock::now();
        isRunning = false;
    }

    void StopWatch::Reset()
    {
        isRunning = false;
        intervals.clear();
    }

    void StopWatch::Restart()
    {
        Reset();
        Start();
    }
}

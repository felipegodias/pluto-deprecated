#include "pluto/debug/assert.h"
#include "pluto/exception.h"
#include <stdexcept>

namespace pluto
{
#ifdef NDEBUG

    constexpr void Assert::IsTrue(const bool condition, const std::string& message)
    {
    }

    constexpr void Assert::IsFalse(const bool condition, const std::string& message)
    {
    }

#else

    constexpr void Assert::IsTrue(const bool condition, const std::string& message)
    {
        if (condition)
        {
            return;
        }

        abort();
        Exception::Throw(std::runtime_error(message));
    }

    constexpr void Assert::IsFalse(const bool condition, const std::string& message)
    {
        if (!condition)
        {
            return;
        }

        abort();
        Exception::Throw(std::runtime_error(message));
    }

#endif
}

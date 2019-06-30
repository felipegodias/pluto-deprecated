#include <pluto/stack_trace.h>
#include <boost/stacktrace.hpp>

namespace pluto
{
    StackTrace::StackTrace(const int skipCount)
    {
        const boost::stacktrace::stacktrace st(skipCount, 999);
        stackTrace = boost::stacktrace::detail::to_string(&st[0], st.size() - 6);
    }

    StackTrace::StackTrace(const StackTrace& other) = default;

    StackTrace::StackTrace(StackTrace&& other) noexcept : stackTrace(std::move(other.stackTrace))
    {
    }

    StackTrace::~StackTrace() = default;

    StackTrace& StackTrace::operator=(const StackTrace& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        stackTrace = rhs.stackTrace;
        return *this;
    }

    StackTrace& StackTrace::operator=(StackTrace&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        stackTrace = std::move(rhs.stackTrace);
        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const StackTrace& stackTrace)
    {
        os << stackTrace.stackTrace;
        return os;
    }

    const std::string& StackTrace::Str() const
    {
        return stackTrace;
    }
}

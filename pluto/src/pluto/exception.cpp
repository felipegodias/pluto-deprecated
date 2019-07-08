#include <pluto/exception.h>
#include <pluto/stack_trace.h>

#include <sstream>

namespace pluto
{
    class Exception::Impl
    {
    private:
        std::exception innerException;
        StackTrace stackTrace;

    public:
        explicit Impl(const std::exception& innerException) : innerException(innerException), stackTrace(5)
        {
        }

        Impl(const std::exception& innerException, StackTrace stackTrace) : innerException(innerException),
                                                                            stackTrace(std::move(stackTrace))
        {
        }

        char const* What() const
        {
            return innerException.what();
        }

        const StackTrace& GetStackTrace() const
        {
            return stackTrace;
        }
    };

    Exception::Exception(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    Exception::Exception(Exception&& other) noexcept : impl(std::move(other.impl))
    {
    }

    Exception::~Exception() noexcept = default;

    Exception& Exception::operator=(Exception&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    char const* Exception::what() const
    {
        return impl->What();
    }

    const StackTrace& Exception::GetStackTrace() const
    {
        return impl->GetStackTrace();
    }

    void Exception::Throw(const std::exception& e)
    {
        throw Exception(std::make_unique<Impl>(e));
    }
}

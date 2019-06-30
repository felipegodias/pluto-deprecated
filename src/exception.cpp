#include <pluto/exception.h>
#include <boost/stacktrace.hpp>

namespace pluto
{
    class Exception::Impl
    {
    private:
        std::exception innerException;
        std::string stackTrace;

    public:
        explicit Impl(const std::exception& innerException) : innerException(innerException)
        {
            const boost::stacktrace::stacktrace st;
            stackTrace = std::string(st.begin(), st.end());
        }

        char const* What() const
        {
            return innerException.what();
        }

        const std::string& GetStackTrace() const
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

    const std::string& Exception::GetStackTrace() const
    {
        return impl->GetStackTrace();
    }

    void Exception::Throw(const std::exception& e)
    {
        throw Exception(std::make_unique<Impl>(e));
    }
}

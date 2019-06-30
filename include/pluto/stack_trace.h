#pragma once

#include "api.h"
#include <string>

namespace pluto
{
    class PLUTO_API StackTrace
    {
    private:
        std::string stackTrace;

    public:
        explicit StackTrace(int skipCount);
        StackTrace(const StackTrace& other);
        StackTrace(StackTrace&& other) noexcept;
        ~StackTrace();

        StackTrace& operator=(const StackTrace& rhs);
        StackTrace& operator=(StackTrace&& rhs) noexcept;

        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const StackTrace& stackTrace);

        const std::string& Str() const;
    };
}

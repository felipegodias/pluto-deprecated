#pragma once

#include "api.h"
#include <array>
#include <ostream>

namespace pluto
{
    class PLUTO_API Guid
    {
    private:
        std::array<uint8_t, 16> data;

    public:
        Guid();
        explicit Guid(const std::array<uint8_t, 16>& data);

        Guid(const Guid& other);
        Guid(Guid&& other) noexcept;

        Guid& operator=(const Guid& rhs);
        Guid& operator=(Guid&& rhs) noexcept;
        bool operator==(const Guid& rhs) const;
        bool operator!=(const Guid& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const Guid& guid);
    };
}

namespace std
{
    template <>
    struct PLUTO_API hash<pluto::Guid>
    {
        size_t operator()(const pluto::Guid& guid) const noexcept;
    };
}

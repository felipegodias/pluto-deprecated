#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class PLUTO_API Vector4I
    {
    public:
        static const Vector4I ZERO;
        static const Vector4I ONE;

        int x, y, z, w;

        Vector4I();
        explicit Vector4I(int scalar);
        Vector4I(int x, int y, int z, int w);

        bool operator==(const Vector4I& rhs) const;
        bool operator!=(const Vector4I& rhs) const;
        int operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector4I& vector);

        std::string Str() const;
        int* Data();
        const int* Data() const;
    };
}

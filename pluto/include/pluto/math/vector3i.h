#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class PLUTO_API Vector3I
    {
    public:
        static const Vector3I ZERO;
        static const Vector3I ONE;
        static const Vector3I RIGHT;
        static const Vector3I LEFT;
        static const Vector3I UP;
        static const Vector3I DOWN;
        static const Vector3I FORWARD;
        static const Vector3I BACK;

        int x, y, z;

        Vector3I();
        explicit Vector3I(int scalar);
        Vector3I(int x, int y, int z);

        bool operator==(const Vector3I& rhs) const;
        bool operator!=(const Vector3I& rhs) const;
        int operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector3I& vector);

        std::string Str() const;
        int* Data();
        const int* Data() const;
    };
}

#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class PLUTO_API Vector2I
    {
    public:
        static const Vector2I ZERO;
        static const Vector2I ONE;
        static const Vector2I RIGHT;
        static const Vector2I LEFT;
        static const Vector2I UP;
        static const Vector2I DOWN;

        int x, y;

        Vector2I();
        explicit Vector2I(int scalar);
        Vector2I(int x, int y);

        bool operator==(const Vector2I& rhs) const;
        bool operator!=(const Vector2I& rhs) const;
        int operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector2I& vector);

        std::string Str() const;
        int* Data();
        const int* Data() const;
    };
}

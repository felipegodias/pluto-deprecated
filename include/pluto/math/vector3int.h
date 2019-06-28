#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class PLUTO_API Vector3Int
    {
    public:
        static const Vector3Int ZERO;
        static const Vector3Int ONE;
        static const Vector3Int RIGHT;
        static const Vector3Int LEFT;
        static const Vector3Int UP;
        static const Vector3Int DOWN;
        static const Vector3Int FORWARD;
        static const Vector3Int BACK;

        int x, y, z;

        Vector3Int();
        explicit Vector3Int(int scalar);
        Vector3Int(int x, int y, int z);
        Vector3Int(const Vector3Int& other);
        Vector3Int(Vector3Int&& other) noexcept;

        ~Vector3Int();

        Vector3Int& operator=(const Vector3Int& rhs);
        Vector3Int& operator=(Vector3Int&& rhs) noexcept;

        bool operator==(const Vector3Int& rhs) const;
        bool operator!=(const Vector3Int& rhs) const;
        int operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector3Int& vector);
        std::string Str() const;
    };
}

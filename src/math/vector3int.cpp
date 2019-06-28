#include <pluto/math/vector3int.h>

#include <sstream>

namespace pluto
{
    Vector3Int::Vector3Int() : Vector3Int(0)
    {
    }

    Vector3Int::Vector3Int(const int scalar) : Vector3Int(scalar, scalar, scalar)
    {
    }

    Vector3Int::Vector3Int(const int x, const int y, const int z) : x(x), y(y), z(z)
    {
    }

    Vector3Int::Vector3Int(const Vector3Int& other) : Vector3Int(other.x, other.y, other.z)
    {
    }

    Vector3Int::Vector3Int(Vector3Int&& other) noexcept : Vector3Int(other.x, other.y, other.z)
    {
    }

    Vector3Int::~Vector3Int() = default;

    Vector3Int& Vector3Int::operator=(const Vector3Int& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector3Int& Vector3Int::operator=(Vector3Int&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    bool Vector3Int::operator==(const Vector3Int& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool Vector3Int::operator!=(const Vector3Int& rhs) const
    {
        return !(*this == rhs);
    }

    int Vector3Int::operator[](const int index) const
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            throw std::out_of_range("");
        }
    }

    std::ostream& operator<<(std::ostream& os, const Vector3Int& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "]";
        return os;
    }

    std::string Vector3Int::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
}

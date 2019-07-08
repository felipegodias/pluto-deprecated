#include <pluto/math/vector3i.h>

#include <sstream>

namespace pluto
{
    Vector3I::Vector3I() : Vector3I(0)
    {
    }

    Vector3I::Vector3I(const int scalar) : Vector3I(scalar, scalar, scalar)
    {
    }

    Vector3I::Vector3I(const int x, const int y, const int z) : x(x), y(y), z(z)
    {
    }

    Vector3I::Vector3I(const Vector3I& other) : Vector3I(other.x, other.y, other.z)
    {
    }

    Vector3I::Vector3I(Vector3I&& other) noexcept : Vector3I(other.x, other.y, other.z)
    {
    }

    Vector3I::~Vector3I() = default;

    Vector3I& Vector3I::operator=(const Vector3I& rhs)

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

    Vector3I& Vector3I::operator=(Vector3I&& rhs) noexcept
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

    bool Vector3I::operator==(const Vector3I& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool Vector3I::operator!=(const Vector3I& rhs) const
    {
        return !(*this == rhs);
    }

    int Vector3I::operator[](const int index) const
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

    std::ostream& operator<<(std::ostream& os, const Vector3I& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "]";
        return os;
    }

    std::string Vector3I::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
}

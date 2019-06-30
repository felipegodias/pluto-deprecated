#include <pluto/math/vector4i.h>

#include <sstream>

namespace pluto
{
    Vector4I::Vector4I() : Vector4I(0)
    {
    }

    Vector4I::Vector4I(const int scalar) : Vector4I(scalar, scalar, scalar, scalar)
    {
    }

    Vector4I::Vector4I(const int x, const int y, const int z, const int w) : x(x), y(y), z(z), w(w)
    {
    }

    Vector4I::Vector4I(const Vector4I& other) : Vector4I(other.x, other.y, other.z, other.w)
    {
    }

    Vector4I::Vector4I(Vector4I&& other) noexcept : Vector4I(other.x, other.y, other.z, other.w)
    {
    }

    Vector4I::~Vector4I() = default;

    Vector4I& Vector4I::operator=(const Vector4I& rhs)

    {
        if (this == &rhs)
        {
            return *this;
        }

        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    Vector4I& Vector4I::operator=(Vector4I&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    bool Vector4I::operator==(const Vector4I& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }

    bool Vector4I::operator!=(const Vector4I& rhs) const
    {
        return !(*this == rhs);
    }

    int Vector4I::operator[](const int index) const
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 4:
            return w;
        default:
            throw std::out_of_range("");
        }
    }

    std::ostream& operator<<(std::ostream& os, const Vector4I& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << "]";
        return os;
    }
     
    std::string Vector4I::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
}

#include <pluto/math/vector4.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>
#include <stdexcept>

namespace pluto
{
    const Vector4 Vector4::ZERO = Vector4(0);
    const Vector4 Vector4::ONE = Vector4(1);

    Vector4::Vector4() : x(0), y(0), z(0), w(0)
    {
    }

    Vector4::Vector4(const float scalar) : x(scalar), y(scalar), z(scalar), w(scalar)
    {
    }

    Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
    {
    }

    Vector4::Vector4(const Vector2& other) : x(other.x), y(other.y), z(0), w(0)
    {
    }

    Vector4::Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0)
    {
    }

    Vector4::Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }

    Vector4::Vector4(Vector4&& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }

    Vector4::~Vector4() = default;

    Vector4& Vector4::operator=(const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = 0;
        w = 0;
        return *this;
    }

    Vector4& Vector4::operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = 0;
        return *this;
    }

    Vector4& Vector4::operator=(const Vector4& rhs)
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

    Vector4& Vector4::operator=(Vector4&& rhs) noexcept
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

    Vector4& Vector4::operator+=(const Vector4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4& Vector4::operator-=(const Vector4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4& Vector4::operator*=(const float d)
    {
        x *= d;
        y *= d;
        z *= d;
        w *= d;
        return *this;
    }

    Vector4& Vector4::operator/=(const float d)
    {
        x /= d;
        y /= d;
        z /= d;
        w /= d;
        return *this;
    }

    Vector4 Vector4::operator+(const Vector4& rhs) const
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector4 Vector4::operator-(const Vector4& rhs) const
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector4 Vector4::operator*(const float d) const
    {
        return Vector4(x * d, y * d, z * d, w * d);
    }

    Vector4 Vector4::operator/(const float d) const
    {
        return Vector4(x / d, y / d, z / d, w / d);
    }

    bool Vector4::operator==(const Vector4& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }

    bool Vector4::operator!=(const Vector4& rhs) const
    {
        return !(*this == rhs);
    }

    float Vector4::operator[](const int index) const
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        default:
            throw std::out_of_range("");
        }
    }

    std::ostream& operator<<(std::ostream& os, const Vector4& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << "]";
        return os;
    }
}

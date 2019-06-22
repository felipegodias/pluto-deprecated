#include <pluto/math/vector3.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector4.h>
#include <stdexcept>

namespace pluto
{
    const Vector3 Vector3::ZERO = Vector3(0);
    const Vector3 Vector3::ONE = Vector3(1);
    const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
    const Vector3 Vector3::LEFT = Vector3(-1, 0, 0);
    const Vector3 Vector3::UP = Vector3(0, 1, 0);
    const Vector3 Vector3::DOWN = Vector3(0, -1, 0);
    const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
    const Vector3 Vector3::BACK = Vector3(0, 0, -1);

    Vector3::Vector3() : x(0), y(0), z(0)
    {
    }

    Vector3::Vector3(const float scalar) : x(scalar), y(scalar), z(scalar)
    {
    }

    Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z)
    {
    }

    Vector3::Vector3(const Vector2& other) : x(other.x), y(other.y), z(0)
    {
    }

    Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z)
    {
    }

    Vector3::Vector3(const Vector4& other) : x(other.x), y(other.y), z(other.z)
    {
    }

    Vector3::Vector3(Vector3&& other) noexcept : x(other.x), y(other.y), z(other.z)
    {
    }

    Vector3::~Vector3() = default;

    Vector3& Vector3::operator=(const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = 0;
        return *this;
    }

    Vector3& Vector3::operator=(const Vector3& rhs)
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

    Vector3& Vector3::operator=(const Vector4& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector3& Vector3::operator=(Vector3&& rhs) noexcept
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

    Vector3& Vector3::operator+=(const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3& Vector3::operator*=(const float d)
    {
        x *= d;
        y *= d;
        z *= d;
        return *this;
    }

    Vector3& Vector3::operator/=(const float d)
    {
        x /= d;
        y /= d;
        z /= d;
        return *this;
    }

    Vector3 Vector3::operator+(const Vector3& rhs) const
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector3 Vector3::operator-(const Vector3& rhs) const
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector3 Vector3::operator*(const float d) const
    {
        return Vector3(x * d, y * d, z * d);
    }

    Vector3 Vector3::operator/(const float d) const
    {
        return Vector3(x / d, y / d, z / d);
    }

    bool Vector3::operator==(const Vector3& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool Vector3::operator!=(const Vector3& rhs) const
    {
        return !(*this == rhs);
    }

    float Vector3::operator[](const int index) const
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

    std::ostream& operator<<(std::ostream& os, const Vector3& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "]";
        return os;
    }
}

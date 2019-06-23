#include <pluto/math/vector3.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector4.h>
#include <stdexcept>
#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace pluto
{
    inline glm::vec3& ToGlm(Vector3& v)
    {
        return reinterpret_cast<glm::vec3&>(v);
    }

    inline const glm::vec3& ToGlm(const Vector3& v)
    {
        return reinterpret_cast<const glm::vec3&>(v);
    }

    inline Vector3& FromGlm(glm::vec3& v)
    {
        return reinterpret_cast<Vector3&>(v);
    }

    inline const Vector3& FromGlm(const glm::vec3& v)
    {
        return reinterpret_cast<const Vector3&>(v);
    }

    const Vector3 Vector3::ZERO = Vector3(0);
    const Vector3 Vector3::ONE = Vector3(1);
    const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
    const Vector3 Vector3::LEFT = Vector3(-1, 0, 0);
    const Vector3 Vector3::UP = Vector3(0, 1, 0);
    const Vector3 Vector3::DOWN = Vector3(0, -1, 0);
    const Vector3 Vector3::FORWARD = Vector3(0, 0, 1);
    const Vector3 Vector3::BACK = Vector3(0, 0, -1);

    Vector3::Vector3() : Vector3(ZERO)
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
        constexpr float e = std::numeric_limits<float>::epsilon();
        return abs(x - rhs.x) <= e && abs(y - rhs.y) <= e && abs(z - rhs.z) <= e;
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

    float Vector3::GetMagnitude() const
    {
        return length(ToGlm(*this));
    }

    float Vector3::GetSqrMagnitude() const
    {
        return length2(ToGlm(*this));
    }

    Vector3 Vector3::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    float Vector3::Angle(const Vector3& from, const Vector3& to)
    {
        return angle(normalize(ToGlm(from)), normalize(ToGlm(to)));
    }

    float Vector3::Distance(const Vector3& from, const Vector3& to)
    {
        return distance(ToGlm(from), ToGlm(to));
    }

    Vector3 Vector3::Cross(const Vector3& from, const Vector3& to)
    {
        return FromGlm(cross(ToGlm(from), ToGlm(to)));
    }

    Vector3 Vector3::Lerp(const Vector3& from, const Vector3& to, const float t)
    {
        return FromGlm(lerp(ToGlm(from), ToGlm(to), t));
    }

    Vector3 Vector3::Slerp(const Vector3& from, const Vector3& to, const float t)
    {
        return FromGlm(slerp(ToGlm(from), ToGlm(to), t));
    }

    Vector3 Vector3::Max(const Vector3& lhs, const Vector3& rhs)
    {
        return FromGlm(max(ToGlm(lhs), ToGlm(rhs)));
    }

    Vector3 Vector3::Min(const Vector3& lhs, const Vector3& rhs)
    {
        return FromGlm(min(ToGlm(lhs), ToGlm(rhs)));
    }

    float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Vector3 Vector3::Scale(const Vector3& lhs, const Vector3& rhs)
    {
        return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }

    Vector3 Vector3::ClampMagnitude(const Vector3& vector, const float minLength, const float maxLength)
    {
        const glm::vec3& norm = normalize(ToGlm(vector));
        return FromGlm(clamp(ToGlm(vector), norm * minLength, norm * maxLength));
    }

    Vector3 Vector3::Reflect(const Vector3& direction, const Vector3& normal)
    {
        return FromGlm(reflect(ToGlm(direction), ToGlm(normal)));
    }
}

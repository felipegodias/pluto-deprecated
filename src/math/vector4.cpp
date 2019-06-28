#include <pluto/math/vector4.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>

#include <stdexcept>
#include <limits>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace pluto
{
    inline glm::vec4& ToGlm(Vector4& v)
    {
        return reinterpret_cast<glm::vec4&>(v);
    }

    inline const glm::vec4& ToGlm(const Vector4& v)
    {
        return reinterpret_cast<const glm::vec4&>(v);
    }

    inline Vector4& FromGlm(glm::vec4& v)
    {
        return reinterpret_cast<Vector4&>(v);
    }

    inline const Vector4& FromGlm(const glm::vec4& v)
    {
        return reinterpret_cast<const Vector4&>(v);
    }

    const Vector4 Vector4::ZERO = Vector4(0);
    const Vector4 Vector4::ONE = Vector4(1);

    Vector4::Vector4() : Vector4(ZERO)
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
        constexpr float e = std::numeric_limits<float>::epsilon();
        return abs(x - rhs.x) <= e && abs(y - rhs.y) <= e && abs(z - rhs.z) <= e && abs(w - rhs.w) <= e;
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

    float Vector4::GetMagnitude() const
    {
        return length(ToGlm(*this));
    }

    float Vector4::GetSqrMagnitude() const
    {
        return length2(ToGlm(*this));
    }

    Vector4 Vector4::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    std::string Vector4::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    float Vector4::Distance(const Vector4& from, const Vector4& to)
    {
        return distance(ToGlm(from), ToGlm(to));
    }

    Vector4 Vector4::Lerp(const Vector4& from, const Vector4& to, const float t)
    {
        return FromGlm(lerp(ToGlm(from), ToGlm(to), t));
    }

    Vector4 Vector4::Max(const Vector4& lhs, const Vector4& rhs)
    {
        return FromGlm(max(ToGlm(lhs), ToGlm(rhs)));
    }

    Vector4 Vector4::Min(const Vector4& lhs, const Vector4& rhs)
    {
        return FromGlm(min(ToGlm(lhs), ToGlm(rhs)));
    }

    float Vector4::Dot(const Vector4& lhs, const Vector4& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Vector4 Vector4::Scale(const Vector4& lhs, const Vector4& rhs)
    {
        return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
    }
}

#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>
#include <pluto/math/vector4.h>

#include <stdexcept>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/perpendicular.hpp>

namespace pluto
{
    inline glm::vec2& ToGlm(Vector2& v)
    {
        return reinterpret_cast<glm::vec2&>(v);
    }

    inline const glm::vec2& ToGlm(const Vector2& v)
    {
        return reinterpret_cast<const glm::vec2&>(v);
    }

    inline Vector2& FromGlm(glm::vec2& v)
    {
        return reinterpret_cast<Vector2&>(v);
    }

    inline const Vector2& FromGlm(const glm::vec2& v)
    {
        return reinterpret_cast<const Vector2&>(v);
    }

    const Vector2 Vector2::ZERO = Vector2(0);
    const Vector2 Vector2::ONE = Vector2(1);
    const Vector2 Vector2::RIGHT = Vector2(1, 0);
    const Vector2 Vector2::LEFT = Vector2(-1, 0);
    const Vector2 Vector2::UP = Vector2(0, 1);
    const Vector2 Vector2::DOWN = Vector2(0, -1);

    Vector2::Vector2() : x(0), y(0)
    {
    }

    Vector2::Vector2(const float scalar) : x(scalar), y(scalar)
    {
    }

    Vector2::Vector2(const float x, const float y) : x(x), y(y)
    {
    }

    Vector2::Vector2(const Vector3& other) : x(other.x), y(other.y)
    {
    }

    Vector2::Vector2(const Vector4& other) : x(other.x), y(other.y)
    {
    }

    Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y)
    {
    }

    Vector2::Vector2(Vector2&& other) noexcept : x(other.x), y(other.y)
    {
    }

    Vector2::~Vector2() = default;

    Vector2& Vector2::operator=(const Vector2& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2& Vector2::operator=(const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2& Vector2::operator=(const Vector4& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2& Vector2::operator=(Vector2&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2& Vector2::operator+=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2& Vector2::operator*=(const float d)
    {
        x *= d;
        y *= d;
        return *this;
    }

    Vector2& Vector2::operator/=(const float d)
    {
        x /= d;
        y /= d;
        return *this;
    }

    Vector2 Vector2::operator+(const Vector2& rhs) const
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }

    Vector2 Vector2::operator-(const Vector2& rhs) const
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    Vector2 Vector2::operator*(const float d) const
    {
        return Vector2(x * d, y * d);
    }

    Vector2 Vector2::operator/(const float d) const
    {
        return Vector2(x / d, y / d);
    }

    bool Vector2::operator==(const Vector2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool Vector2::operator!=(const Vector2& rhs) const
    {
        return !(*this == rhs);
    }

    float Vector2::operator[](const int index) const
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            throw std::out_of_range("");
        }
    }

    std::ostream& operator<<(std::ostream& os, const Vector2& vector)
    {
        os << "[" << vector.x << "," << vector.y << "]";
        return os;
    }

    float Vector2::GetMagnitude() const
    {
        return length(ToGlm(*this));
    }

    float Vector2::GetSqrMagnitude() const
    {
        return length2(ToGlm(*this));
    }

    Vector2 Vector2::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    float Vector2::Angle(const Vector2& from, const Vector2& to)
    {
        return angle(normalize(ToGlm(from)), normalize(ToGlm(to)));
    }

    float Vector2::Distance(const Vector2& from, const Vector2& to)
    {
        return distance(ToGlm(from), ToGlm(to));
    }

    Vector2 Vector2::Lerp(const Vector2& from, const Vector2& to, const float t)
    {
        return FromGlm(lerp(ToGlm(from), ToGlm(to), t));
    }

    Vector2 Vector2::Max(const Vector2& lhs, const Vector2& rhs)
    {
        return FromGlm(max(ToGlm(lhs), ToGlm(rhs)));
    }

    Vector2 Vector2::Min(const Vector2& lhs, const Vector2& rhs)
    {
        return FromGlm(min(ToGlm(lhs), ToGlm(rhs)));
    }

    float Vector2::Dot(const Vector2& lhs, const Vector2& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Vector2 Vector2::Scale(const Vector2& lhs, const Vector2& rhs)
    {
        return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    Vector2 Vector2::ClampMagnitude(const Vector2& vector, const float minLength, const float maxLength)
    {
        const glm::vec2& norm = normalize(ToGlm(vector));
        return FromGlm(clamp(ToGlm(vector), norm * minLength, norm * maxLength));
    }

    Vector2 Vector2::Perpendicular(const Vector2& direction)
    {
        return FromGlm(perp(ToGlm(direction), ToGlm(UP)));
    }

    Vector2 Vector2::Reflect(const Vector2& direction, const Vector2& normal)
    {
        return FromGlm(reflect(ToGlm(direction), ToGlm(normal)));
    }
}

#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector4f.h>

#include <stdexcept>
#include <limits>
#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/perpendicular.hpp>

namespace pluto
{
    inline glm::vec2& ToGlm(Vector2F& v)
    {
        return reinterpret_cast<glm::vec2&>(v);
    }

    inline const glm::vec2& ToGlm(const Vector2F& v)
    {
        return reinterpret_cast<const glm::vec2&>(v);
    }

    inline Vector2F& FromGlm(glm::vec2& v)
    {
        return reinterpret_cast<Vector2F&>(v);
    }

    inline const Vector2F& FromGlm(const glm::vec2& v)
    {
        return reinterpret_cast<const Vector2F&>(v);
    }

    const Vector2F Vector2F::ZERO = Vector2F(0);
    const Vector2F Vector2F::ONE = Vector2F(1);
    const Vector2F Vector2F::RIGHT = Vector2F(1, 0);
    const Vector2F Vector2F::LEFT = Vector2F(-1, 0);
    const Vector2F Vector2F::UP = Vector2F(0, 1);
    const Vector2F Vector2F::DOWN = Vector2F(0, -1);

    Vector2F::Vector2F() : Vector2F(ZERO)
    {
    }

    Vector2F::Vector2F(const float scalar) : x(scalar), y(scalar)
    {
    }

    Vector2F::Vector2F(const float x, const float y) : x(x), y(y)
    {
    }

    Vector2F::Vector2F(const Vector3F& other) : x(other.x), y(other.y)
    {
    }

    Vector2F::Vector2F(const Vector4F& other) : x(other.x), y(other.y)
    {
    }

    Vector2F::Vector2F(const Vector2F& other) : x(other.x), y(other.y)
    {
    }

    Vector2F::Vector2F(Vector2F&& other) noexcept : x(other.x), y(other.y)
    {
    }

    Vector2F::~Vector2F() = default;

    Vector2F& Vector2F::operator=(const Vector2F& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2F& Vector2F::operator=(const Vector3F& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2F& Vector2F::operator=(const Vector4F& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2F& Vector2F::operator=(Vector2F&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2F& Vector2F::operator+=(const Vector2F& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2F& Vector2F::operator-=(const Vector2F& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2F& Vector2F::operator*=(const float d)
    {
        x *= d;
        y *= d;
        return *this;
    }

    Vector2F& Vector2F::operator/=(const float d)
    {
        x /= d;
        y /= d;
        return *this;
    }

    Vector2F Vector2F::operator+(const Vector2F& rhs) const
    {
        return Vector2F(x + rhs.x, y + rhs.y);
    }

    Vector2F Vector2F::operator-(const Vector2F& rhs) const
    {
        return Vector2F(x - rhs.x, y - rhs.y);
    }

    Vector2F Vector2F::operator*(const float d) const
    {
        return Vector2F(x * d, y * d);
    }

    Vector2F Vector2F::operator/(const float d) const
    {
        return Vector2F(x / d, y / d);
    }

    bool Vector2F::operator==(const Vector2F& rhs) const
    {
        constexpr float e = std::numeric_limits<float>::epsilon();
        return abs(x - rhs.x) <= e && abs(y - rhs.y) <= e;
    }

    bool Vector2F::operator!=(const Vector2F& rhs) const
    {
        return !(*this == rhs);
    }

    float Vector2F::operator[](const int index) const
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

    std::ostream& operator<<(std::ostream& os, const Vector2F& vector)
    {
        os << "[" << vector.x << "," << vector.y << "]";
        return os;
    }

    float Vector2F::GetMagnitude() const
    {
        return length(ToGlm(*this));
    }

    float Vector2F::GetSqrMagnitude() const
    {
        return length2(ToGlm(*this));
    }

    Vector2F Vector2F::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    std::string Vector2F::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    float Vector2F::Angle(const Vector2F& from, const Vector2F& to)
    {
        return angle(normalize(ToGlm(from)), normalize(ToGlm(to)));
    }

    float Vector2F::Distance(const Vector2F& from, const Vector2F& to)
    {
        return distance(ToGlm(from), ToGlm(to));
    }

    Vector2F Vector2F::Lerp(const Vector2F& from, const Vector2F& to, const float t)
    {
        return FromGlm(lerp(ToGlm(from), ToGlm(to), t));
    }

    Vector2F Vector2F::Max(const Vector2F& lhs, const Vector2F& rhs)
    {
        return FromGlm(max(ToGlm(lhs), ToGlm(rhs)));
    }

    Vector2F Vector2F::Min(const Vector2F& lhs, const Vector2F& rhs)
    {
        return FromGlm(min(ToGlm(lhs), ToGlm(rhs)));
    }

    float Vector2F::Dot(const Vector2F& lhs, const Vector2F& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Vector2F Vector2F::Scale(const Vector2F& lhs, const Vector2F& rhs)
    {
        return Vector2F(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    Vector2F Vector2F::ClampMagnitude(const Vector2F& vector, const float minLength, const float maxLength)
    {
        const glm::vec2& norm = normalize(ToGlm(vector));
        return FromGlm(clamp(ToGlm(vector), norm * minLength, norm * maxLength));
    }

    Vector2F Vector2F::Perpendicular(const Vector2F& direction)
    {
        return FromGlm(perp(ToGlm(direction), ToGlm(UP)));
    }

    Vector2F Vector2F::Reflect(const Vector2F& direction, const Vector2F& normal)
    {
        return FromGlm(reflect(ToGlm(direction), ToGlm(normal)));
    }
}

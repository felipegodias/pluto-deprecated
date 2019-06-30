#include <pluto/math/vector3f.h>
#include <pluto/math/vector2f.h>
#include <pluto/math/vector3i.h>
#include <pluto/math/vector4f.h>

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
    inline glm::vec3& ToGlm(Vector3F& v)
    {
        return reinterpret_cast<glm::vec3&>(v);
    }

    inline const glm::vec3& ToGlm(const Vector3F& v)
    {
        return reinterpret_cast<const glm::vec3&>(v);
    }

    inline Vector3F& FromGlm(glm::vec3& v)
    {
        return reinterpret_cast<Vector3F&>(v);
    }

    inline const Vector3F& FromGlm(const glm::vec3& v)
    {
        return reinterpret_cast<const Vector3F&>(v);
    }

    const Vector3F Vector3F::ZERO = Vector3F(0);
    const Vector3F Vector3F::ONE = Vector3F(1);
    const Vector3F Vector3F::RIGHT = Vector3F(1, 0, 0);
    const Vector3F Vector3F::LEFT = Vector3F(-1, 0, 0);
    const Vector3F Vector3F::UP = Vector3F(0, 1, 0);
    const Vector3F Vector3F::DOWN = Vector3F(0, -1, 0);
    const Vector3F Vector3F::FORWARD = Vector3F(0, 0, 1);
    const Vector3F Vector3F::BACK = Vector3F(0, 0, -1);

    Vector3F::Vector3F() : Vector3F(ZERO)
    {
    }

    Vector3F::Vector3F(const float scalar) : Vector3F(scalar, scalar, scalar)
    {
    }

    Vector3F::Vector3F(const float x, const float y, const float z) : x(x), y(y), z(z)
    {
    }

    Vector3F::Vector3F(const Vector2F& other) : Vector3F(other.x, other.y, 0)
    {
    }

    Vector3F::Vector3F(const Vector3F& other) : Vector3F(other.x, other.y, other.z)
    {
    }

    Vector3F::Vector3F(const Vector3I& other) : Vector3F(static_cast<float>(other.x), static_cast<float>(other.y),
                                                        static_cast<float>(other.z))
    {
    }

    Vector3F::Vector3F(const Vector4F& other) : Vector3F(other.x, other.y, other.z)
    {
    }

    Vector3F::Vector3F(Vector3F&& other) noexcept : Vector3F(other.x, other.y, other.z)
    {
    }

    Vector3F::~Vector3F() = default;

    Vector3F& Vector3F::operator=(const Vector2F& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = 0;
        return *this;
    }

    Vector3F& Vector3F::operator=(const Vector3F& rhs)
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

    Vector3F& Vector3F::operator=(const Vector3I& rhs)
    {
        x = static_cast<float>(rhs.x);
        y = static_cast<float>(rhs.y);
        z = static_cast<float>(rhs.z);
        return *this;
    }

    Vector3F& Vector3F::operator=(const Vector4F& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector3F& Vector3F::operator=(Vector3F&& rhs) noexcept
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

    Vector3F& Vector3F::operator+=(const Vector3F& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3F& Vector3F::operator-=(const Vector3F& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3F& Vector3F::operator*=(const float d)
    {
        x *= d;
        y *= d;
        z *= d;
        return *this;
    }

    Vector3F& Vector3F::operator/=(const float d)
    {
        x /= d;
        y /= d;
        z /= d;
        return *this;
    }

    Vector3F Vector3F::operator+(const Vector3F& rhs) const
    {
        return Vector3F(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vector3F Vector3F::operator-(const Vector3F& rhs) const
    {
        return Vector3F(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vector3F Vector3F::operator*(const float d) const
    {
        return Vector3F(x * d, y * d, z * d);
    }

    Vector3F Vector3F::operator/(const float d) const
    {
        return Vector3F(x / d, y / d, z / d);
    }

    bool Vector3F::operator==(const Vector3F& rhs) const
    {
        constexpr float e = std::numeric_limits<float>::epsilon();
        return abs(x - rhs.x) <= e && abs(y - rhs.y) <= e && abs(z - rhs.z) <= e;
    }

    bool Vector3F::operator!=(const Vector3F& rhs) const
    {
        return !(*this == rhs);
    }

    float Vector3F::operator[](const int index) const
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

    std::ostream& operator<<(std::ostream& os, const Vector3F& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "]";
        return os;
    }

    float Vector3F::GetMagnitude() const
    {
        return length(ToGlm(*this));
    }

    float Vector3F::GetSqrMagnitude() const
    {
        return length2(ToGlm(*this));
    }

    Vector3F Vector3F::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    std::string Vector3F::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    float Vector3F::Angle(const Vector3F& from, const Vector3F& to)
    {
        return angle(normalize(ToGlm(from)), normalize(ToGlm(to)));
    }

    float Vector3F::Distance(const Vector3F& from, const Vector3F& to)
    {
        return distance(ToGlm(from), ToGlm(to));
    }

    Vector3F Vector3F::Cross(const Vector3F& from, const Vector3F& to)
    {
        return FromGlm(cross(ToGlm(from), ToGlm(to)));
    }

    Vector3F Vector3F::Lerp(const Vector3F& from, const Vector3F& to, const float t)
    {
        return FromGlm(lerp(ToGlm(from), ToGlm(to), t));
    }

    Vector3F Vector3F::Slerp(const Vector3F& from, const Vector3F& to, const float t)
    {
        return FromGlm(slerp(ToGlm(from), ToGlm(to), t));
    }

    Vector3F Vector3F::Max(const Vector3F& lhs, const Vector3F& rhs)
    {
        return FromGlm(max(ToGlm(lhs), ToGlm(rhs)));
    }

    Vector3F Vector3F::Min(const Vector3F& lhs, const Vector3F& rhs)
    {
        return FromGlm(min(ToGlm(lhs), ToGlm(rhs)));
    }

    float Vector3F::Dot(const Vector3F& lhs, const Vector3F& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Vector3F Vector3F::Scale(const Vector3F& lhs, const Vector3F& rhs)
    {
        return Vector3F(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }

    Vector3F Vector3F::ClampMagnitude(const Vector3F& vector, const float minLength, const float maxLength)
    {
        const glm::vec3& norm = normalize(ToGlm(vector));
        return FromGlm(clamp(ToGlm(vector), norm * minLength, norm * maxLength));
    }

    Vector3F Vector3F::Reflect(const Vector3F& direction, const Vector3F& normal)
    {
        return FromGlm(reflect(ToGlm(direction), ToGlm(normal)));
    }
}

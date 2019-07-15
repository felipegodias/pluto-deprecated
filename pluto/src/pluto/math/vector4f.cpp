#include <pluto/math/vector4f.h>
#include <pluto/math/vector2f.h>
#include <pluto/math/vector2i.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>
#include <pluto/math/vector4i.h>

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
    inline glm::vec4& ToGlm(Vector4F& v)
    {
        return reinterpret_cast<glm::vec4&>(v);
    }

    inline const glm::vec4& ToGlm(const Vector4F& v)
    {
        return reinterpret_cast<const glm::vec4&>(v);
    }

    inline Vector4F& FromGlm(glm::vec4& v)
    {
        return reinterpret_cast<Vector4F&>(v);
    }

    inline const Vector4F& FromGlm(const glm::vec4& v)
    {
        return reinterpret_cast<const Vector4F&>(v);
    }

    const Vector4F Vector4F::ZERO = Vector4F(0);
    const Vector4F Vector4F::ONE = Vector4F(1);

    Vector4F::Vector4F() : Vector4F(ZERO)
    {
    }

    Vector4F::Vector4F(const float scalar) : Vector4F(scalar, scalar, scalar, scalar)
    {
    }

    Vector4F::Vector4F(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
    {
    }

    Vector4F::Vector4F(const Vector2F& other) : Vector4F(other.x, other.y, 0, 0)
    {
    }

    Vector4F::Vector4F(const Vector2I& other) : Vector4F(static_cast<float>(other.x), static_cast<float>(other.y),
                                                         0, 0)
    {
    }

    Vector4F::Vector4F(const Vector3F& other) : Vector4F(other.x, other.y, other.z, 0)
    {
    }

    Vector4F::Vector4F(const Vector3I& other) : Vector4F(static_cast<float>(other.x), static_cast<float>(other.y),
                                                         static_cast<float>(other.z), 0)
    {
    }

    Vector4F::Vector4F(const Vector4F& other) : Vector4F(other.x, other.y, other.z, other.w)
    {
    }

    Vector4F::Vector4F(const Vector4I& other) : Vector4F(static_cast<float>(other.x), static_cast<float>(other.y),
                                                         static_cast<float>(other.z), static_cast<float>(other.w))
    {
    }

    Vector4F::Vector4F(Vector4F&& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }

    Vector4F::~Vector4F() = default;

    Vector4F& Vector4F::operator=(const Vector2F& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = 0;
        w = 0;
        return *this;
    }

    Vector4F& Vector4F::operator=(const Vector2I& rhs)
    {
        x = static_cast<float>(rhs.x);
        y = static_cast<float>(rhs.y);
        z = 0;
        w = 0;
        return *this;
    }

    Vector4F& Vector4F::operator=(const Vector3F& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = 0;
        return *this;
    }

    Vector4F& Vector4F::operator=(const Vector3I& rhs)
    {
        x = static_cast<float>(rhs.x);
        y = static_cast<float>(rhs.y);
        z = static_cast<float>(rhs.z);
        w = 0;
        return *this;
    }

    Vector4F& Vector4F::operator=(const Vector4F& rhs)
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

    Vector4F& Vector4F::operator=(const Vector4I& rhs)
    {
        x = static_cast<float>(rhs.x);
        y = static_cast<float>(rhs.y);
        z = static_cast<float>(rhs.z);
        w = static_cast<float>(rhs.w);
        return *this;
    }

    Vector4F& Vector4F::operator=(Vector4F&& rhs) noexcept
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

    Vector4F& Vector4F::operator+=(const Vector4F& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4F& Vector4F::operator-=(const Vector4F& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4F& Vector4F::operator*=(const float d)
    {
        x *= d;
        y *= d;
        z *= d;
        w *= d;
        return *this;
    }

    Vector4F& Vector4F::operator/=(const float d)
    {
        x /= d;
        y /= d;
        z /= d;
        w /= d;
        return *this;
    }

    Vector4F Vector4F::operator+(const Vector4F& rhs) const
    {
        return Vector4F(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vector4F Vector4F::operator-(const Vector4F& rhs) const
    {
        return Vector4F(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vector4F Vector4F::operator*(const float d) const
    {
        return Vector4F(x * d, y * d, z * d, w * d);
    }

    Vector4F Vector4F::operator/(const float d) const
    {
        return Vector4F(x / d, y / d, z / d, w / d);
    }

    bool Vector4F::operator==(const Vector4F& rhs) const
    {
        constexpr float e = std::numeric_limits<float>::epsilon();
        return abs(x - rhs.x) <= e && abs(y - rhs.y) <= e && abs(z - rhs.z) <= e && abs(w - rhs.w) <= e;
    }

    bool Vector4F::operator!=(const Vector4F& rhs) const
    {
        return !(*this == rhs);
    }

    float Vector4F::operator[](const int index) const
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

    std::ostream& operator<<(std::ostream& os, const Vector4F& vector)
    {
        os << "[" << vector.x << "," << vector.y << "," << vector.z << "," << vector.w << "]";
        return os;
    }

    float Vector4F::GetMagnitude() const
    {
        return length(ToGlm(*this));
    }

    float Vector4F::GetSqrMagnitude() const
    {
        return length2(ToGlm(*this));
    }

    Vector4F Vector4F::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    std::string Vector4F::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    float* Vector4F::Data()
    {
        return &x;
    }

    float Vector4F::Distance(const Vector4F& from, const Vector4F& to)
    {
        return distance(ToGlm(from), ToGlm(to));
    }

    Vector4F Vector4F::Lerp(const Vector4F& from, const Vector4F& to, const float t)
    {
        return FromGlm(lerp(ToGlm(from), ToGlm(to), t));
    }

    Vector4F Vector4F::Max(const Vector4F& lhs, const Vector4F& rhs)
    {
        return FromGlm(max(ToGlm(lhs), ToGlm(rhs)));
    }

    Vector4F Vector4F::Min(const Vector4F& lhs, const Vector4F& rhs)
    {
        return FromGlm(min(ToGlm(lhs), ToGlm(rhs)));
    }

    float Vector4F::Dot(const Vector4F& lhs, const Vector4F& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Vector4F Vector4F::Scale(const Vector4F& lhs, const Vector4F& rhs)
    {
        return Vector4F(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
    }
}

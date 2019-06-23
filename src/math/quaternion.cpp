#include <pluto/math/quaternion.h>
#include <pluto/math/vector3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

namespace pluto
{
    inline glm::quat& ToGlm(Quaternion& q)
    {
        return reinterpret_cast<glm::quat&>(q);
    }

    inline const glm::quat& ToGlm(const Quaternion& q)
    {
        return reinterpret_cast<const glm::quat&>(q);
    }

    inline const glm::vec3& ToGlm(const Vector3& v)
    {
        return reinterpret_cast<const glm::vec3&>(v);
    }

    inline Quaternion& FromGlm(glm::quat& q)
    {
        return reinterpret_cast<Quaternion&>(q);
    }

    inline const Quaternion& FromGlm(const glm::quat& q)
    {
        return reinterpret_cast<const Quaternion&>(q);
    }

    inline const Vector3& FromGlm(const glm::vec3& v)
    {
        return reinterpret_cast<const Vector3&>(v);
    }

    const Quaternion Quaternion::IDENTITY = Quaternion(0, 0, 0, 1);

    Quaternion::Quaternion() : Quaternion(IDENTITY)
    {
    }

    Quaternion::Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
    {
    }

    Quaternion::Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }

    Quaternion::Quaternion(Quaternion&& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }

    Quaternion::~Quaternion() = default;

    Quaternion& Quaternion::operator=(const Quaternion& rhs)
    {
        if (*this == rhs)
        {
            return *this;
        }

        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        z = rhs.z;
        return *this;
    }

    Quaternion& Quaternion::operator=(Quaternion&& rhs) noexcept
    {
        if (*this == rhs)
        {
            return *this;
        }

        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        z = rhs.z;
        return *this;
    }

    Quaternion& Quaternion::operator*=(const Quaternion& rhs)
    {
        return *this = FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Quaternion Quaternion::operator*(const Quaternion& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Vector3 Quaternion::operator*(const Vector3& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    bool Quaternion::operator==(const Quaternion& rhs) const
    {
        constexpr float e = std::numeric_limits<float>::epsilon();
        return abs(x - rhs.x) <= e && abs(y - rhs.y) <= e && abs(z - rhs.z) <= e && abs(w - rhs.w) <= e;
    }

    bool Quaternion::operator!=(const Quaternion& rhs) const
    {
        return !(*this == rhs);
    }

    float Quaternion::operator[](const int index) const
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

    std::ostream& operator<<(std::ostream& os, const Quaternion& quaternion)
    {
        os << "[" << quaternion.x << "," << quaternion.y << "," << quaternion.z << "," << quaternion.w << "]";
        return os;
    }

    Vector3 Quaternion::GetEulerAngles() const
    {
        return FromGlm(degrees(eulerAngles(ToGlm(*this))));
    }

    void Quaternion::SetEulerAngles(const Vector3& eulerAngles)
    {
        *this = Euler(eulerAngles);
    }

    Quaternion Quaternion::GetNormalized() const
    {
        return FromGlm(normalize(ToGlm(*this)));
    }

    Quaternion Quaternion::GetInverse() const
    {
        return FromGlm(inverse(ToGlm(*this)));
    }

    float Quaternion::Dot(const Quaternion& lhs, const Quaternion& rhs)
    {
        return dot(ToGlm(lhs), ToGlm(rhs));
    }

    Quaternion Quaternion::Euler(const Vector3& eulerAngles)
    {
        return FromGlm(glm::quat(radians(ToGlm(eulerAngles))));
    }

    Quaternion Quaternion::Lerp(const Quaternion& lhs, const Quaternion& rhs, const float t)
    {
        return FromGlm(lerp(ToGlm(lhs), ToGlm(rhs), t));
    }

    Quaternion Quaternion::Slerp(const Quaternion& lhs, const Quaternion& rhs, const float t)
    {
        return FromGlm(slerp(ToGlm(lhs), ToGlm(rhs), t));
    }
}

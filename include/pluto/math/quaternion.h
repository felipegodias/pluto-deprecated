#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Vector3;

    class PLUTO_API Quaternion
    {
    public:
        static const Quaternion IDENTITY;

        float x, y, z, w;

        Quaternion();
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Quaternion& other);
        Quaternion(Quaternion&& other) noexcept;

        ~Quaternion();

        Quaternion& operator=(const Quaternion& rhs);
        Quaternion& operator=(Quaternion&& rhs) noexcept;

        Quaternion& operator*=(const Quaternion& rhs);
        Quaternion operator*(const Quaternion& rhs) const;
        Vector3 operator*(const Vector3& rhs) const;

        bool operator==(const Quaternion& rhs) const;
        bool operator!=(const Quaternion& rhs) const;
        float operator[](int index) const;
        friend std::ostream& operator<<(std::ostream& os, const Quaternion& quaternion);

        Vector3 GetEulerAngles() const;
        void SetEulerAngles(const Vector3& eulerAngles);
        Quaternion GetNormalized() const;
        Quaternion GetInverse() const;

        static float Dot(const Quaternion& lhs, const Quaternion& rhs);
        static Quaternion Euler(const Vector3& eulerAngles);
        static Quaternion Lerp(const Quaternion& lhs, const Quaternion& rhs, float t);
        static Quaternion Slerp(const Quaternion& lhs, const Quaternion& rhs, float t);
    };
}

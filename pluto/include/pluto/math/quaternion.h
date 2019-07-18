#pragma once

#include "../api.h"
#include <ostream>
#include <array>

namespace pluto
{
    class Vector3F;

    class PLUTO_API Quaternion
    {
    public:
        static const Quaternion IDENTITY;

        float x, y, z, w;

        Quaternion();
        Quaternion(float x, float y, float z, float w);

        Quaternion& operator*=(const Quaternion& rhs);
        Quaternion operator*(const Quaternion& rhs) const;
        Vector3F operator*(const Vector3F& rhs) const;

        bool operator==(const Quaternion& rhs) const;
        bool operator!=(const Quaternion& rhs) const;
        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Quaternion& quaternion);

        Vector3F GetEulerAngles() const;
        void SetEulerAngles(const Vector3F& eulerAngles);
        Quaternion GetNormalized() const;
        Quaternion GetInverse() const;
        std::string Str() const;
        float* Data();
        const float* Data() const;

        static float Dot(const Quaternion& lhs, const Quaternion& rhs);
        static Quaternion Euler(const Vector3F& eulerAngles);
        static Quaternion Lerp(const Quaternion& lhs, const Quaternion& rhs, float t);
        static Quaternion Slerp(const Quaternion& lhs, const Quaternion& rhs, float t);
    };
}

#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Vector2;
    class Vector3;

    class PLUTO_API Vector4
    {
    public:
        static const Vector4 ZERO;
        static const Vector4 ONE;

        float x, y, z, w;

        Vector4();
        explicit Vector4(float scalar);
        Vector4(float x, float y, float z, float w);
        Vector4(const Vector2& other);
        Vector4(const Vector3& other);
        Vector4(const Vector4& other);
        Vector4(Vector4&& other) noexcept;

        ~Vector4();

        Vector4& operator=(const Vector2& rhs);
        Vector4& operator=(const Vector3& rhs);
        Vector4& operator=(const Vector4& rhs);
        Vector4& operator=(Vector4&& rhs) noexcept;

        Vector4& operator+=(const Vector4& rhs);
        Vector4& operator-=(const Vector4& rhs);
        Vector4& operator*=(float d);
        Vector4& operator/=(float d);

        Vector4 operator+(const Vector4& rhs) const;
        Vector4 operator-(const Vector4& rhs) const;
        Vector4 operator*(float d) const;
        Vector4 operator/(float d) const;
        bool operator==(const Vector4& rhs) const;
        bool operator!=(const Vector4& rhs) const;
        float operator[](int index) const;
        friend std::ostream& operator<<(std::ostream& os, const Vector4& vector);

        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector4 GetNormalized() const;

        static float Distance(const Vector4& from, const Vector4& to);
        static Vector4 Lerp(const Vector4& from, const Vector4& to, float t);
        static Vector4 Max(const Vector4& lhs, const Vector4& rhs);
        static Vector4 Min(const Vector4& lhs, const Vector4& rhs);
        static float Dot(const Vector4& lhs, const Vector4& rhs);
        static Vector4 Scale(const Vector4& lhs, const Vector4& rhs);
    };
}

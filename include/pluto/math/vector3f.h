#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Vector2F;
    class Vector3I;
    class Vector4F;

    class PLUTO_API Vector3F
    {
    public:
        static const Vector3F ZERO;
        static const Vector3F ONE;
        static const Vector3F RIGHT;
        static const Vector3F LEFT;
        static const Vector3F UP;
        static const Vector3F DOWN;
        static const Vector3F FORWARD;
        static const Vector3F BACK;

        float x, y, z;

        Vector3F();
        explicit Vector3F(float scalar);
        Vector3F(float x, float y, float z);
        Vector3F(const Vector2F& other);
        Vector3F(const Vector3F& other);
        Vector3F(const Vector3I& other);
        Vector3F(const Vector4F& other);
        Vector3F(Vector3F&& other) noexcept;

        ~Vector3F();

        Vector3F& operator=(const Vector2F& rhs);
        Vector3F& operator=(const Vector3F& rhs);
        Vector3F& operator=(const Vector3I& rhs);
        Vector3F& operator=(const Vector4F& rhs);
        Vector3F& operator=(Vector3F&& rhs) noexcept;

        Vector3F& operator+=(const Vector3F& rhs);
        Vector3F& operator-=(const Vector3F& rhs);
        Vector3F& operator*=(float d);
        Vector3F& operator/=(float d);

        Vector3F operator+(const Vector3F& rhs) const;
        Vector3F operator-(const Vector3F& rhs) const;
        Vector3F operator*(float d) const;
        Vector3F operator/(float d) const;
        bool operator==(const Vector3F& rhs) const;
        bool operator!=(const Vector3F& rhs) const;
        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector3F& vector);

        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector3F GetNormalized() const;
        std::string Str() const;

        static float Angle(const Vector3F& from, const Vector3F& to);
        static float Distance(const Vector3F& from, const Vector3F& to);
        static Vector3F Cross(const Vector3F& from, const Vector3F& to);
        static Vector3F Lerp(const Vector3F& from, const Vector3F& to, float t);
        static Vector3F Slerp(const Vector3F& from, const Vector3F& to, float t);
        static Vector3F Max(const Vector3F& lhs, const Vector3F& rhs);
        static Vector3F Min(const Vector3F& lhs, const Vector3F& rhs);
        static float Dot(const Vector3F& lhs, const Vector3F& rhs);
        static Vector3F Scale(const Vector3F& lhs, const Vector3F& rhs);
        static Vector3F ClampMagnitude(const Vector3F& vector, float minLength, float maxLength);
        static Vector3F Reflect(const Vector3F& direction, const Vector3F& normal);
    };
}

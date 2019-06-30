#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Vector3F;
    class Vector4F;

    class PLUTO_API Vector2F
    {
    public:
        static const Vector2F ZERO;
        static const Vector2F ONE;
        static const Vector2F RIGHT;
        static const Vector2F LEFT;
        static const Vector2F UP;
        static const Vector2F DOWN;

        float x, y;

        Vector2F();
        explicit Vector2F(float scalar);
        Vector2F(float x, float y);
        Vector2F(const Vector2F& other);
        Vector2F(const Vector3F& other);
        Vector2F(const Vector4F& other);
        Vector2F(Vector2F&& other) noexcept;

        ~Vector2F();

        Vector2F& operator=(const Vector2F& rhs);
        Vector2F& operator=(const Vector3F& rhs);
        Vector2F& operator=(const Vector4F& rhs);
        Vector2F& operator=(Vector2F&& rhs) noexcept;

        Vector2F& operator+=(const Vector2F& rhs);
        Vector2F& operator-=(const Vector2F& rhs);
        Vector2F& operator*=(float d);
        Vector2F& operator/=(float d);

        Vector2F operator+(const Vector2F& rhs) const;
        Vector2F operator-(const Vector2F& rhs) const;
        Vector2F operator*(float d) const;
        Vector2F operator/(float d) const;

        bool operator==(const Vector2F& rhs) const;
        bool operator!=(const Vector2F& rhs) const;
        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector2F& vector);

        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector2F GetNormalized() const;
        std::string Str() const;

        static float Angle(const Vector2F& from, const Vector2F& to);
        static float Distance(const Vector2F& from, const Vector2F& to);
        static Vector2F Lerp(const Vector2F& from, const Vector2F& to, float t);
        static Vector2F Max(const Vector2F& lhs, const Vector2F& rhs);
        static Vector2F Min(const Vector2F& lhs, const Vector2F& rhs);
        static float Dot(const Vector2F& lhs, const Vector2F& rhs);
        static Vector2F Scale(const Vector2F& lhs, const Vector2F& rhs);
        static Vector2F ClampMagnitude(const Vector2F& vector, float minLength, float maxLength);
        static Vector2F Perpendicular(const Vector2F& direction);
        static Vector2F Reflect(const Vector2F& direction, const Vector2F& normal);
    };
}

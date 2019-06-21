#pragma once

#include "../api.h"

namespace pluto
{
    class Vector3;

    class PLUTO_API Vector2
    {
    public:
        static const Vector2 ZERO;
        static const Vector2 ONE;
        static const Vector2 RIGHT;
        static const Vector2 LEFT;
        static const Vector2 UP;
        static const Vector2 DOWN;

        union
        {
            struct
            {
                float x, y;
            };

            float v[2]{};
        };

        Vector2();
        explicit Vector2(float scalar);
        Vector2(float x, float y);
        Vector2(const Vector2& other);
        Vector2(Vector2&& other) noexcept;

        ~Vector2();

        Vector2& operator=(const Vector2& other);
        Vector2& operator=(Vector2&& other) noexcept;

        Vector2& operator+=(const Vector2& other);
        Vector2& operator-=(const Vector2& other);
        Vector2& operator*=(float d);
        Vector2& operator/=(float d);

        Vector2 operator+(const Vector2& rhs) const;
        Vector2 operator-(const Vector2& rhs) const;
        Vector2 operator*(float d) const;
        Vector2 operator/(float d) const;
        bool operator==(const Vector2& rhs) const;

        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector2 GetNormalized() const;

        static float Angle(const Vector2& from, const Vector2& to);
        static float Distance(const Vector2& from, const Vector2& to);
        static Vector2 Lerp(const Vector2& from, const Vector2& to, float t);
        static Vector2 Max(const Vector2& lhs, const Vector2& rhs);
        static Vector2 Min(const Vector2& lhs, const Vector2& rhs);
        static float Dot(const Vector2& lhs, const Vector2& rhs);
        static Vector2 Scale(const Vector2& lhs, const Vector2& rhs);
        static Vector2 ClampMagnitude(const Vector2& vector, float minLength, float maxLength);
        static Vector2 Perpendicular(const Vector2& direction);
        static Vector2 Reflect(const Vector2& direction, const Vector2& normal);
    };
}

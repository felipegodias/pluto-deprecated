#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Vector2;
    class Vector3Int;
    class Vector4;

    class PLUTO_API Vector3
    {
    public:
        static const Vector3 ZERO;
        static const Vector3 ONE;
        static const Vector3 RIGHT;
        static const Vector3 LEFT;
        static const Vector3 UP;
        static const Vector3 DOWN;
        static const Vector3 FORWARD;
        static const Vector3 BACK;

        float x, y, z;

        Vector3();
        explicit Vector3(float scalar);
        Vector3(float x, float y, float z);
        Vector3(const Vector2& other);
        Vector3(const Vector3& other);
        Vector3(const Vector3Int& other);
        Vector3(const Vector4& other);
        Vector3(Vector3&& other) noexcept;

        ~Vector3();

        Vector3& operator=(const Vector2& rhs);
        Vector3& operator=(const Vector3& rhs);
        Vector3& operator=(const Vector3Int& rhs);
        Vector3& operator=(const Vector4& rhs);
        Vector3& operator=(Vector3&& rhs) noexcept;

        Vector3& operator+=(const Vector3& rhs);
        Vector3& operator-=(const Vector3& rhs);
        Vector3& operator*=(float d);
        Vector3& operator/=(float d);

        Vector3 operator+(const Vector3& rhs) const;
        Vector3 operator-(const Vector3& rhs) const;
        Vector3 operator*(float d) const;
        Vector3 operator/(float d) const;
        bool operator==(const Vector3& rhs) const;
        bool operator!=(const Vector3& rhs) const;
        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector3& vector);

        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector3 GetNormalized() const;
        std::string Str() const;

        static float Angle(const Vector3& from, const Vector3& to);
        static float Distance(const Vector3& from, const Vector3& to);
        static Vector3 Cross(const Vector3& from, const Vector3& to);
        static Vector3 Lerp(const Vector3& from, const Vector3& to, float t);
        static Vector3 Slerp(const Vector3& from, const Vector3& to, float t);
        static Vector3 Max(const Vector3& lhs, const Vector3& rhs);
        static Vector3 Min(const Vector3& lhs, const Vector3& rhs);
        static float Dot(const Vector3& lhs, const Vector3& rhs);
        static Vector3 Scale(const Vector3& lhs, const Vector3& rhs);
        static Vector3 ClampMagnitude(const Vector3& vector, float minLength, float maxLength);
        static Vector3 Reflect(const Vector3& direction, const Vector3& normal);
    };
}

#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Color;
    class Vector2F;
    class Vector2I;
    class Vector3F;
    class Vector3I;
    class Vector4I;

    class PLUTO_API Vector4F
    {
    public:
        static const Vector4F ZERO;
        static const Vector4F ONE;

        float x, y, z, w;

        Vector4F();
        explicit Vector4F(float scalar);
        Vector4F(float x, float y, float z, float w);
        explicit Vector4F(const Color& other);
        explicit Vector4F(const Vector2F& other);
        explicit Vector4F(const Vector2I& other);
        explicit Vector4F(const Vector3F& other);
        explicit Vector4F(const Vector3I& other);
        explicit Vector4F(const Vector4I& other);

        Vector4F& operator=(const Color& rhs);
        Vector4F& operator=(const Vector2F& rhs);
        Vector4F& operator=(const Vector2I& rhs);
        Vector4F& operator=(const Vector3F& rhs);
        Vector4F& operator=(const Vector3I& rhs);
        Vector4F& operator=(const Vector4I& rhs);

        Vector4F& operator+=(const Vector4F& rhs);
        Vector4F& operator-=(const Vector4F& rhs);
        Vector4F& operator*=(float d);
        Vector4F& operator/=(float d);

        Vector4F operator+(const Vector4F& rhs) const;
        Vector4F operator-(const Vector4F& rhs) const;
        Vector4F operator*(float d) const;
        Vector4F operator/(float d) const;
        bool operator==(const Vector4F& rhs) const;
        bool operator!=(const Vector4F& rhs) const;
        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Vector4F& vector);

        float GetMagnitude() const;
        float GetSqrMagnitude() const;
        Vector4F GetNormalized() const;
        std::string Str() const;
        float* Data();
        const float* Data() const;

        static float Distance(const Vector4F& from, const Vector4F& to);
        static Vector4F Lerp(const Vector4F& from, const Vector4F& to, float t);
        static Vector4F Max(const Vector4F& lhs, const Vector4F& rhs);
        static Vector4F Min(const Vector4F& lhs, const Vector4F& rhs);
        static float Dot(const Vector4F& lhs, const Vector4F& rhs);
        static Vector4F Scale(const Vector4F& lhs, const Vector4F& rhs);
    };
}

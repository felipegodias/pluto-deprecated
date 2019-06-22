#pragma once

#include "../api.h"

namespace pluto
{
    class Vector2;
    class Vector3;

    class PLUTO_API Vector4
    {
    public:
        static const Vector4 ZERO;
        static const Vector4 ONE;

        union
        {
            struct
            {
                float x, y, z, w;
            };

            float v[4]{};
        };

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
    };
}

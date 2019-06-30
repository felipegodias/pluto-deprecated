#pragma once

#include "../api.h"
#include <ostream>

namespace pluto
{
    class Vector2F;
    class Vector3F;
    class Vector4F;
    class Quaternion;

    class PLUTO_API Matrix4
    {
    public:
        static const Matrix4 IDENTITY;
        static const Matrix4 ZERO;

        float x0, y0, z0, w0;
        float x1, y1, z1, w1;
        float x2, y2, z2, w2;
        float x3, y3, z3, w3;

        Matrix4();
        Matrix4(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2,
                float z2, float w2, float x3, float y3, float z3, float w3);
        Matrix4(const Vector4F& row0, const Vector4F& row1, const Vector4F& row2, const Vector4F& row3);
        Matrix4(const Matrix4& other);
        Matrix4(Matrix4&& other) noexcept;

        ~Matrix4();

        Matrix4& operator=(const Matrix4& rhs);
        Matrix4& operator=(Matrix4&& rhs) noexcept;
        Matrix4& operator*=(const Matrix4& rhs);
        Matrix4 operator*(const Matrix4& rhs) const;
        Vector4F operator*(const Vector4F& rhs) const;

        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Matrix4& matrix);

        Vector4F GetRow(int index) const;
        void SetRow(int index, const Vector4F& value);
        Vector4F GetColumn(int index) const;
        void SetColumn(int index, const Vector4F& value);

        float GetDeterminant() const;
        Matrix4 GetInverse() const;
        Matrix4 GetTranspose() const;
        Vector2F MultiplyPoint(const Vector2F& point) const;
        Vector3F MultiplyPoint(const Vector3F& point) const;
        std::string Str() const;

        static Matrix4 Frustum(float left, float right, float bottom, float top, float near, float far);
        static Matrix4 LookAt(const Vector3F& from, const Vector3F& to, const Vector3F& up);
        static Matrix4 Ortho(float left, float right, float bottom, float top, float near, float far);
        static Matrix4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4 Translate(const Vector3F& vector);
        static Matrix4 Rotate(const Quaternion& quaternion);
        static Matrix4 Scale(const Vector3F& vector);
        static Matrix4 TSR(const Vector3F& position, const Quaternion& rotation, const Vector3F& scale);
    };
}

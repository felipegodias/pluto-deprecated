#pragma once

#include "../api.h"
#include <ostream>
#include <array>

namespace pluto
{
    class Vector2F;
    class Vector3F;
    class Vector4F;
    class Quaternion;

    class PLUTO_API Matrix4X4
    {
    public:
        static const Matrix4X4 IDENTITY;
        static const Matrix4X4 ZERO;

        std::array<float, 16> data;

        Matrix4X4();
        explicit Matrix4X4(const std::array<float, 16>& data);

        Matrix4X4(float x0, float y0, float z0, float w0, float x1, float y1, float z1, float w1, float x2, float y2,
                  float z2, float w2, float x3, float y3, float z3, float w3);
        Matrix4X4(const Vector4F& row0, const Vector4F& row1, const Vector4F& row2, const Vector4F& row3);
        Matrix4X4(const Matrix4X4& other);
        Matrix4X4(Matrix4X4&& other) noexcept;

        ~Matrix4X4();

        Matrix4X4& operator=(const Matrix4X4& rhs);
        Matrix4X4& operator=(Matrix4X4&& rhs) noexcept;
        Matrix4X4& operator*=(const Matrix4X4& rhs);
        Matrix4X4 operator*(const Matrix4X4& rhs) const;
        Vector4F operator*(const Vector4F& rhs) const;

        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Matrix4X4& matrix);

        Vector4F GetRow(int index) const;
        void SetRow(int index, const Vector4F& value);
        Vector4F GetColumn(int index) const;
        void SetColumn(int index, const Vector4F& value);

        float GetDeterminant() const;
        Matrix4X4 GetInverse() const;
        Matrix4X4 GetTranspose() const;
        Vector2F MultiplyPoint(const Vector2F& point) const;
        Vector3F MultiplyPoint(const Vector3F& point) const;
        std::string Str() const;
        float* Data();

        static Matrix4X4 Frustum(float left, float right, float bottom, float top, float near, float far);
        static Matrix4X4 LookAt(const Vector3F& from, const Vector3F& to, const Vector3F& up);
        static Matrix4X4 Ortho(float left, float right, float bottom, float top, float near, float far);
        static Matrix4X4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4X4 Translate(const Vector3F& vector);
        static Matrix4X4 Rotate(const Quaternion& quaternion);
        static Matrix4X4 Scale(const Vector3F& vector);
        static Matrix4X4 TSR(const Vector3F& position, const Quaternion& rotation, const Vector3F& scale);
    };
}

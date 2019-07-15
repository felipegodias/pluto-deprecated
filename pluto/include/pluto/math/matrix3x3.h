#pragma once

#include "../api.h"
#include <ostream>
#include <array>

namespace pluto
{
    class Vector3F;
    class Quaternion;

    class PLUTO_API Matrix3X3
    {
    public:
        static const Matrix3X3 IDENTITY;
        static const Matrix3X3 ZERO;

        std::array<float, 9> data;

        Matrix3X3();
        explicit Matrix3X3(const std::array<float, 9>& data);

        Matrix3X3(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2);
        Matrix3X3(const Vector3F& row0, const Vector3F& row1, const Vector3F& row2);
        Matrix3X3(const Matrix3X3& other);
        Matrix3X3(Matrix3X3&& other) noexcept;

        ~Matrix3X3();

        Matrix3X3& operator=(const Matrix3X3& rhs);
        Matrix3X3& operator=(Matrix3X3&& rhs) noexcept;
        Matrix3X3& operator*=(const Matrix3X3& rhs);
        Matrix3X3 operator*(const Matrix3X3& rhs) const;
        Vector3F operator*(const Vector3F& rhs) const;

        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Matrix3X3& matrix);

        Vector3F GetRow(int index) const;
        void SetRow(int index, const Vector3F& value);
        Vector3F GetColumn(int index) const;
        void SetColumn(int index, const Vector3F& value);

        float GetDeterminant() const;
        Matrix3X3 GetInverse() const;
        Matrix3X3 GetTranspose() const;
        std::string Str() const;
        float* Data();
    };
}

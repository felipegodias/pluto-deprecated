#pragma once

#include "../api.h"
#include <ostream>
#include <array>

namespace pluto
{
    class Vector2F;
    class Quaternion;

    class PLUTO_API Matrix2X2
    {
    public:
        static const Matrix2X2 IDENTITY;
        static const Matrix2X2 ZERO;

        std::array<float, 4> data;

        Matrix2X2();
        explicit Matrix2X2(const std::array<float, 4>& data);

        Matrix2X2(float x0, float y0, float x1, float y1);
        Matrix2X2(const Vector2F& row0, const Vector2F& row1);
        Matrix2X2(const Matrix2X2& other);
        Matrix2X2(Matrix2X2&& other) noexcept;

        ~Matrix2X2();

        Matrix2X2& operator=(const Matrix2X2& rhs);
        Matrix2X2& operator=(Matrix2X2&& rhs) noexcept;
        Matrix2X2& operator*=(const Matrix2X2& rhs);
        Matrix2X2 operator*(const Matrix2X2& rhs) const;
        Vector2F operator*(const Vector2F& rhs) const;

        float operator[](int index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Matrix2X2& matrix);

        Vector2F GetRow(int index) const;
        void SetRow(int index, const Vector2F& value);
        Vector2F GetColumn(int index) const;
        void SetColumn(int index, const Vector2F& value);

        float GetDeterminant() const;
        Matrix2X2 GetInverse() const;
        Matrix2X2 GetTranspose() const;
        std::string Str() const;
        float* Data();
        const float* Data() const;
    };
}

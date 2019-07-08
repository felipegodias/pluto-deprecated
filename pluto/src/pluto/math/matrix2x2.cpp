#include <pluto/math/matrix2x2.h>
#include <pluto/math/vector2f.h>

#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace pluto
{
    inline glm::mat2& ToGlm(Matrix2X2& m)
    {
        return reinterpret_cast<glm::mat2&>(m);
    }

    inline const glm::mat2& ToGlm(const Matrix2X2& m)
    {
        return reinterpret_cast<const glm::mat2&>(m);
    }

    inline const glm::vec2& ToGlm(const Vector2F& v)
    {
        return reinterpret_cast<const glm::vec2&>(v);
    }

    inline Matrix2X2& FromGlm(glm::mat2& m)
    {
        return reinterpret_cast<Matrix2X2&>(m);
    }

    inline const Matrix2X2& FromGlm(const glm::mat2& m)
    {
        return reinterpret_cast<const Matrix2X2&>(m);
    }

    inline const Vector2F& FromGlm(const glm::vec2& v)
    {
        return reinterpret_cast<const Vector2F&>(v);
    }

    const Matrix2X2 Matrix2X2::IDENTITY = Matrix2X2(std::array<float, 4>{
        1, 0,
        0, 1,
    });

    const Matrix2X2 Matrix2X2::ZERO = Matrix2X2({});

    Matrix2X2::Matrix2X2() : Matrix2X2(IDENTITY)
    {
    }

    Matrix2X2::Matrix2X2(const std::array<float, 4>& data) : data(data)
    {
    }

    Matrix2X2::Matrix2X2(const float x0, const float y0, const float x1, const float y1)
        : Matrix2X2(std::array<float, 4>{
            x0, y0,
            x1, y1
        })
    {
    }

    Matrix2X2::Matrix2X2(const Vector2F& row0, const Vector2F& row1) :
        Matrix2X2(std::array<float, 4>{
            row0.x, row0.y,
            row1.x, row1.y
        })
    {
    }

    Matrix2X2::Matrix2X2(const Matrix2X2& other) : Matrix2X2(other.data)
    {
    }

    Matrix2X2::Matrix2X2(Matrix2X2&& other) noexcept : Matrix2X2(other.data)
    {
    }

    Matrix2X2::~Matrix2X2() = default;

    Matrix2X2& Matrix2X2::operator=(const Matrix2X2& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        data = rhs.data;
        return *this;
    }

    Matrix2X2& Matrix2X2::operator=(Matrix2X2&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        data = rhs.data;
        return *this;
    }

    Matrix2X2& Matrix2X2::operator*=(const Matrix2X2& rhs)
    {
        return *this = FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Matrix2X2 Matrix2X2::operator*(const Matrix2X2& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Vector2F Matrix2X2::operator*(const Vector2F& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    float Matrix2X2::operator[](const int index) const
    {
        if (index > 3)
        {
            throw std::out_of_range("");
        }

        return data[index];
    }

    std::ostream& operator<<(std::ostream& os, const Matrix2X2& matrix)
    {
        return os << "[" << matrix.GetRow(0) << matrix.GetRow(1) << "]";
    }

    Vector2F Matrix2X2::GetRow(const int index) const
    {
        if (index > 1)
        {
            throw std::out_of_range("");
        }

        return Vector2F(data[index * 2], data[index * 2 + 1]);
    }

    void Matrix2X2::SetRow(const int index, const Vector2F& value)
    {
        if (index > 1)
        {
            throw std::out_of_range("");
        }

        data[index * 2] = value.x;
        data[index * 2 + 1] = value.y;
    }

    Vector2F Matrix2X2::GetColumn(const int index) const
    {
        if (index > 1)
        {
            throw std::out_of_range("");
        }

        return Vector2F(data[index * 2], data[(index + 1) * 2]);
    }

    void Matrix2X2::SetColumn(const int index, const Vector2F& value)
    {
        if (index > 1)
        {
            throw std::out_of_range("");
        }

        data[index * 2] = value.x;
        data[(index + 1) * 2] = value.y;
    }

    float Matrix2X2::GetDeterminant() const
    {
        return determinant(ToGlm(*this));
    }

    Matrix2X2 Matrix2X2::GetInverse() const
    {
        return FromGlm(inverse(ToGlm(*this)));
    }

    Matrix2X2 Matrix2X2::GetTranspose() const
    {
        return FromGlm(transpose(ToGlm(*this)));
    }

    std::string Matrix2X2::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
}

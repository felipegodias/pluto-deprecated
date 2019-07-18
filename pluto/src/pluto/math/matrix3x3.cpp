#include <pluto/math/matrix3x3.h>
#include <pluto/math/vector3f.h>

#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace pluto
{
    inline glm::mat3& ToGlm(Matrix3X3& m)
    {
        return reinterpret_cast<glm::mat3&>(m);
    }

    inline const glm::mat3& ToGlm(const Matrix3X3& m)
    {
        return reinterpret_cast<const glm::mat3&>(m);
    }

    inline const glm::vec3& ToGlm(const Vector3F& v)
    {
        return reinterpret_cast<const glm::vec3&>(v);
    }

    inline Matrix3X3& FromGlm(glm::mat3& m)
    {
        return reinterpret_cast<Matrix3X3&>(m);
    }

    inline const Matrix3X3& FromGlm(const glm::mat3& m)
    {
        return reinterpret_cast<const Matrix3X3&>(m);
    }

    inline const Vector3F& FromGlm(const glm::vec3& v)
    {
        return reinterpret_cast<const Vector3F&>(v);
    }

    const Matrix3X3 Matrix3X3::IDENTITY = Matrix3X3(std::array<float, 9>{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    });

    const Matrix3X3 Matrix3X3::ZERO = Matrix3X3({});

    Matrix3X3::Matrix3X3() : Matrix3X3(IDENTITY)
    {
    }

    Matrix3X3::Matrix3X3(const std::array<float, 9>& data) : data(data)
    {
    }

    Matrix3X3::Matrix3X3(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1,
                         const float x2, const float y2, const float z2)
        : Matrix3X3(std::array<float, 9>{
            x0, y0, z0,
            x1, y1, z1,
            x2, y2, z2,
        })
    {
    }

    Matrix3X3::Matrix3X3(const Vector3F& row0, const Vector3F& row1, const Vector3F& row2) :
        Matrix3X3(std::array<float, 9>{
            row0.x, row0.y, row0.z,
            row1.x, row1.y, row1.z,
            row2.x, row2.y, row2.z
        })
    {
    }

    Matrix3X3& Matrix3X3::operator*=(const Matrix3X3& rhs)
    {
        return *this = FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Matrix3X3 Matrix3X3::operator*(const Matrix3X3& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Vector3F Matrix3X3::operator*(const Vector3F& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    float Matrix3X3::operator[](const int index) const
    {
        if (index > 8)
        {
            throw std::out_of_range("");
        }

        return data[index];
    }

    std::ostream& operator<<(std::ostream& os, const Matrix3X3& matrix)
    {
        return os << "[" << matrix.GetRow(0) << matrix.GetRow(1) << matrix.GetRow(2) << "]";
    }

    Vector3F Matrix3X3::GetRow(const int index) const
    {
        if (index > 2)
        {
            throw std::out_of_range("");
        }

        return Vector3F(data[index * 3], data[index * 3 + 1], data[index * 3 + 2]);
    }

    void Matrix3X3::SetRow(const int index, const Vector3F& value)
    {
        if (index > 2)
        {
            throw std::out_of_range("");
        }

        data[index * 3] = value.x;
        data[index * 3 + 1] = value.y;
        data[index * 3 + 2] = value.y;
    }

    Vector3F Matrix3X3::GetColumn(const int index) const
    {
        if (index > 2)
        {
            throw std::out_of_range("");
        }

        return Vector3F(data[index * 3], data[(index + 1) * 3], data[(index + 2) * 3]);
    }

    void Matrix3X3::SetColumn(const int index, const Vector3F& value)
    {
        if (index > 2)
        {
            throw std::out_of_range("");
        }

        data[index * 3] = value.x;
        data[(index + 1) * 3] = value.y;
        data[(index + 2) * 3] = value.z;
    }

    float Matrix3X3::GetDeterminant() const
    {
        return determinant(ToGlm(*this));
    }

    Matrix3X3 Matrix3X3::GetInverse() const
    {
        return FromGlm(inverse(ToGlm(*this)));
    }

    Matrix3X3 Matrix3X3::GetTranspose() const
    {
        return FromGlm(transpose(ToGlm(*this)));
    }

    std::string Matrix3X3::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    float* Matrix3X3::Data()
    {
        return data.data();
    }

    const float* Matrix3X3::Data() const
    {
        return data.data();
    }
}

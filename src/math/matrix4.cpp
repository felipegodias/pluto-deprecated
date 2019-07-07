#include <pluto/math/matrix4.h>
#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector4f.h>

#include <sstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace pluto
{
    inline glm::mat4& ToGlm(Matrix4X4& m)
    {
        return reinterpret_cast<glm::mat4&>(m);
    }

    inline const glm::mat4& ToGlm(const Matrix4X4& m)
    {
        return reinterpret_cast<const glm::mat4&>(m);
    }

    inline const glm::vec3& ToGlm(const Vector3F& v)
    {
        return reinterpret_cast<const glm::vec3&>(v);
    }

    inline const glm::vec4& ToGlm(const Vector4F& v)
    {
        return reinterpret_cast<const glm::vec4&>(v);
    }

    inline const glm::quat& ToGlm(const Quaternion& q)
    {
        return reinterpret_cast<const glm::quat&>(q);
    }

    inline Matrix4X4& FromGlm(glm::mat4& m)
    {
        return reinterpret_cast<Matrix4X4&>(m);
    }

    inline const Matrix4X4& FromGlm(const glm::mat4& m)
    {
        return reinterpret_cast<const Matrix4X4&>(m);
    }

    inline const Vector4F& FromGlm(const glm::vec4& v)
    {
        return reinterpret_cast<const Vector4F&>(v);
    }

    const Matrix4X4 Matrix4X4::IDENTITY = Matrix4X4(std::array<float, 16>{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    });

    const Matrix4X4 Matrix4X4::ZERO = Matrix4X4({});

    Matrix4X4::Matrix4X4() : Matrix4X4(IDENTITY)
    {
    }

    Matrix4X4::Matrix4X4(const std::array<float, 16>& data) : data(data)
    {
    }

    Matrix4X4::Matrix4X4(const float x0, const float y0, const float z0, const float w0, const float x1, const float y1,
                     const float z1, const float w1, const float x2, const float y2, const float z2, const float w2,
                     const float x3, const float y3, const float z3, const float w3)
        : Matrix4X4(std::array<float, 16>{
            x0, y0, z0, w0,
            x1, y1, z1, w1,
            x2, y2, z2, w2,
            x3, y3, z3, w3
        })
    {
    }

    Matrix4X4::Matrix4X4(const Vector4F& row0, const Vector4F& row1, const Vector4F& row2, const Vector4F& row3) :
        Matrix4X4(std::array<float, 16>{
            row0.x, row0.y, row0.z, row0.w,
            row1.x, row1.y, row1.z, row1.w,
            row2.x, row2.y, row2.z, row2.w,
            row3.x, row3.y, row3.z, row3.w
        })
    {
    }

    Matrix4X4::Matrix4X4(const Matrix4X4& other) : Matrix4X4(other.data)
    {
    }

    Matrix4X4::Matrix4X4(Matrix4X4&& other) noexcept : Matrix4X4(other.data)
    {
    }

    Matrix4X4::~Matrix4X4() = default;

    Matrix4X4& Matrix4X4::operator=(const Matrix4X4& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        data = rhs.data;
        return *this;
    }

    Matrix4X4& Matrix4X4::operator=(Matrix4X4&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        data = rhs.data;
        return *this;
    }

    Matrix4X4& Matrix4X4::operator*=(const Matrix4X4& rhs)
    {
        return *this = FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Matrix4X4 Matrix4X4::operator*(const Matrix4X4& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Vector4F Matrix4X4::operator*(const Vector4F& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    float Matrix4X4::operator[](const int index) const
    {
        if (index > 15)
        {
            throw std::out_of_range("");
        }

        return data[index];
    }

    std::ostream& operator<<(std::ostream& os, const Matrix4X4& matrix)
    {
        return os << "[" << matrix.GetRow(0) << matrix.GetRow(1) << matrix.GetRow(2) << matrix.GetRow(3) << "]";
    }

    Vector4F Matrix4X4::GetRow(const int index) const
    {
        if (index > 3)
        {
            throw std::out_of_range("");
        }

        return Vector4F(data[index * 4], data[index * 4 + 1], data[index * 4 + 2], data[index * 4 + 3]);
    }

    void Matrix4X4::SetRow(const int index, const Vector4F& value)
    {
        if (index > 3)
        {
            throw std::out_of_range("");
        }

        data[index * 4] = value.x;
        data[index * 4 + 1] = value.y;
        data[index * 4 + 2] = value.z;
        data[index * 4 + 3] = value.w;
    }

    Vector4F Matrix4X4::GetColumn(const int index) const
    {
        if (index > 3)
        {
            throw std::out_of_range("");
        }

        return Vector4F(data[index * 4], data[(index + 1) * 4], data[(index + 2) * 4], data[(index + 3) * 4]);
    }

    void Matrix4X4::SetColumn(const int index, const Vector4F& value)
    {
        if (index > 3)
        {
            throw std::out_of_range("");
        }

        data[index * 4] = value.x;
        data[(index + 1) * 4] = value.y;
        data[(index + 2) * 4] = value.z;
        data[(index + 3) * 4] = value.w;
    }

    float Matrix4X4::GetDeterminant() const
    {
        return determinant(ToGlm(*this));
    }

    Matrix4X4 Matrix4X4::GetInverse() const
    {
        return FromGlm(inverse(ToGlm(*this)));
    }

    Matrix4X4 Matrix4X4::GetTranspose() const
    {
        return FromGlm(transpose(ToGlm(*this)));
    }

    Vector2F Matrix4X4::MultiplyPoint(const Vector2F& point) const
    {
        return Vector2F(*this * Vector4F(point));
    }

    Vector3F Matrix4X4::MultiplyPoint(const Vector3F& point) const
    {
        return Vector3F(*this * Vector4F(point));
    }

    std::string Matrix4X4::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    Matrix4X4 Matrix4X4::Frustum(const float left, const float right, const float bottom, const float top, const float near,
                             const float far)
    {
        return FromGlm(glm::frustum(left, right, bottom, top, near, far));
    }

    Matrix4X4 Matrix4X4::LookAt(const Vector3F& from, const Vector3F& to, const Vector3F& up)
    {
        return FromGlm(lookAt(ToGlm(from), ToGlm(to), ToGlm(up)));
    }

    Matrix4X4 Matrix4X4::Ortho(const float left, const float right, const float bottom, const float top, const float near,
                           const float far)
    {
        return FromGlm(glm::ortho(left, right, bottom, top, near, far));
    }

    Matrix4X4 Matrix4X4::Perspective(const float fov, const float aspect, const float near, const float far)
    {
        return FromGlm(glm::perspective(fov, aspect, near, far));
    }

    Matrix4X4 Matrix4X4::Translate(const Vector3F& vector)
    {
        return FromGlm(translate(ToGlm(vector)));
    }

    Matrix4X4 Matrix4X4::Rotate(const Quaternion& quaternion)
    {
        return FromGlm(toMat4(ToGlm(quaternion)));
    }

    Matrix4X4 Matrix4X4::Scale(const Vector3F& vector)
    {
        return FromGlm(scale(ToGlm(vector)));
    }

    Matrix4X4 Matrix4X4::TSR(const Vector3F& position, const Quaternion& rotation, const Vector3F& scale)
    {
        return Translate(position) * Rotate(rotation) * Scale(scale);
    }
}

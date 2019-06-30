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
    inline glm::mat4& ToGlm(Matrix4& m)
    {
        return reinterpret_cast<glm::mat4&>(m);
    }

    inline const glm::mat4& ToGlm(const Matrix4& m)
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

    inline Matrix4& FromGlm(glm::mat4& m)
    {
        return reinterpret_cast<Matrix4&>(m);
    }

    inline const Matrix4& FromGlm(const glm::mat4& m)
    {
        return reinterpret_cast<const Matrix4&>(m);
    }

    inline const Vector4F& FromGlm(const glm::vec4& v)
    {
        return reinterpret_cast<const Vector4F&>(v);
    }

    const Matrix4 Matrix4::IDENTITY = Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    const Matrix4 Matrix4::ZERO = Matrix4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    Matrix4::Matrix4() : Matrix4(IDENTITY)
    {
    }

    Matrix4::Matrix4(const float x0, const float y0, const float z0, const float w0, const float x1, const float y1,
                     const float z1, const float w1, const float x2, const float y2, const float z2, const float w2,
                     const float x3, const float y3, const float z3, const float w3) : x0(x0), y0(y0), z0(z0), w0(w0),
                                                                                       x1(x1), y1(y1), z1(z1), w1(w1),
                                                                                       x2(x2), y2(y2), z2(z2), w2(w2),
                                                                                       x3(x3), y3(y3), z3(z3), w3(w3)
    {
    }

    Matrix4::Matrix4(const Vector4F& row0, const Vector4F& row1, const Vector4F& row2, const Vector4F& row3) :
        x0(row0.x), y0(row0.y), z0(row0.z), w0(row0.w),
        x1(row1.x), y1(row1.y), z1(row1.z), w1(row1.w),
        x2(row2.x), y2(row2.y), z2(row2.z), w2(row2.w),
        x3(row3.x), y3(row3.y), z3(row3.z), w3(row3.w)
    {
    }

    Matrix4::Matrix4(const Matrix4& other) : x0(other.x0), y0(other.y0), z0(other.z0), w0(other.w0),
                                             x1(other.x1), y1(other.y1), z1(other.z1), w1(other.w1),
                                             x2(other.x2), y2(other.y2), z2(other.z2), w2(other.w2),
                                             x3(other.x3), y3(other.y3), z3(other.z3), w3(other.w3)
    {
    }

    Matrix4::Matrix4(Matrix4&& other) noexcept : x0(other.x0), y0(other.y0), z0(other.z0), w0(other.w0),
                                                 x1(other.x1), y1(other.y1), z1(other.z1), w1(other.w1),
                                                 x2(other.x2), y2(other.y2), z2(other.z2), w2(other.w2),
                                                 x3(other.x3), y3(other.y3), z3(other.z3), w3(other.w3)
    {
    }

    Matrix4::~Matrix4() = default;

    Matrix4& Matrix4::operator=(const Matrix4& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        *this = FromGlm(ToGlm(rhs));
        return *this;
    }

    Matrix4& Matrix4::operator=(Matrix4&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        *this = FromGlm(ToGlm(rhs));
        return *this;
    }

    Matrix4& Matrix4::operator*=(const Matrix4& rhs)
    {
        return *this = FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Matrix4 Matrix4::operator*(const Matrix4& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    Vector4F Matrix4::operator*(const Vector4F& rhs) const
    {
        return FromGlm(ToGlm(*this) * ToGlm(rhs));
    }

    float Matrix4::operator[](const int index) const
    {
        switch (index)
        {
        case 0:
            return x0;
        case 1:
            return y0;
        case 2:
            return z0;
        case 3:
            return w0;
        case 4:
            return x1;
        case 5:
            return y1;
        case 6:
            return z1;
        case 7:
            return w1;
        case 8:
            return x2;
        case 9:
            return y2;
        case 10:
            return z2;
        case 11:
            return w2;
        case 12:
            return x3;
        case 13:
            return y3;
        case 14:
            return z3;
        case 15:
            return w3;
        default:
            throw std::out_of_range("");
        }
    }

    std::ostream& operator<<(std::ostream& os, const Matrix4& matrix)
    {
        return os << "[" << matrix.GetRow(0) << matrix.GetRow(1) << matrix.GetRow(2) << matrix.GetRow(3) << "]";
    }

    Vector4F Matrix4::GetRow(const int index) const
    {
        switch (index)
        {
        case 0:
            return Vector4F(x0, y0, z0, w0);
        case 1:
            return Vector4F(x1, y1, z1, w1);
        case 2:
            return Vector4F(x2, y2, z2, w2);
        case 3:
            return Vector4F(x3, y3, z3, w3);
        default:
            throw std::out_of_range("");
        }
    }

    void Matrix4::SetRow(const int index, const Vector4F& value)
    {
        switch (index)
        {
        case 0:
            x0 = value.x;
            y0 = value.y;
            z0 = value.z;
            w0 = value.w;
            break;
        case 1:
            x1 = value.x;
            y1 = value.y;
            z1 = value.z;
            w1 = value.w;
        case 2:
            x2 = value.x;
            y2 = value.y;
            z2 = value.z;
            w2 = value.w;
        case 3:
            x3 = value.x;
            y3 = value.y;
            z3 = value.z;
            w3 = value.w;
        default:
            throw std::out_of_range("");
        }
    }

    Vector4F Matrix4::GetColumn(const int index) const
    {
        switch (index)
        {
        case 0:
            return Vector4F(x0, x1, x2, x3);
        case 1:
            return Vector4F(y0, y1, y2, y3);
        case 2:
            return Vector4F(z0, z1, z2, z3);
        case 3:
            return Vector4F(w0, w1, w2, w3);
        default:
            throw std::out_of_range("");
        }
    }

    void Matrix4::SetColumn(const int index, const Vector4F& value)
    {
        switch (index)
        {
        case 0:
            x0 = value.x;
            x1 = value.y;
            x2 = value.z;
            x3 = value.w;
            break;
        case 1:
            y0 = value.x;
            y1 = value.y;
            y2 = value.z;
            y3 = value.w;
        case 2:
            z0 = value.x;
            z1 = value.y;
            z2 = value.z;
            z3 = value.w;
        case 3:
            w0 = value.x;
            w1 = value.y;
            w2 = value.z;
            w3 = value.w;
        default:
            throw std::out_of_range("");
        }
    }

    float Matrix4::GetDeterminant() const
    {
        return determinant(ToGlm(*this));
    }

    Matrix4 Matrix4::GetInverse() const
    {
        return FromGlm(inverse(ToGlm(*this)));
    }

    Matrix4 Matrix4::GetTranspose() const
    {
        return FromGlm(transpose(ToGlm(*this)));
    }

    Vector2F Matrix4::MultiplyPoint(const Vector2F& point) const
    {
        return Vector2F(*this * Vector4F(point));
    }

    Vector3F Matrix4::MultiplyPoint(const Vector3F& point) const
    {
        return Vector3F(*this * Vector4F(point));
    }

    std::string Matrix4::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    Matrix4 Matrix4::Frustum(const float left, const float right, const float bottom, const float top, const float near,
                             const float far)
    {
        return FromGlm(glm::frustum(left, right, bottom, top, near, far));
    }

    Matrix4 Matrix4::LookAt(const Vector3F& from, const Vector3F& to, const Vector3F& up)
    {
        return FromGlm(lookAt(ToGlm(from), ToGlm(to), ToGlm(up)));
    }

    Matrix4 Matrix4::Ortho(const float left, const float right, const float bottom, const float top, const float near,
                           const float far)
    {
        return FromGlm(glm::ortho(left, right, bottom, top, near, far));
    }

    Matrix4 Matrix4::Perspective(const float fov, const float aspect, const float near, const float far)
    {
        return FromGlm(glm::perspective(fov, aspect, near, far));
    }

    Matrix4 Matrix4::Translate(const Vector3F& vector)
    {
        return FromGlm(translate(ToGlm(vector)));
    }

    Matrix4 Matrix4::Rotate(const Quaternion& quaternion)
    {
        return FromGlm(toMat4(ToGlm(quaternion)));
    }

    Matrix4 Matrix4::Scale(const Vector3F& vector)
    {
        return FromGlm(scale(ToGlm(vector)));
    }

    Matrix4 Matrix4::TSR(const Vector3F& position, const Quaternion& rotation, const Vector3F& scale)
    {
        return Translate(position) * Rotate(rotation) * Scale(scale);
    }
}

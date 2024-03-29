#include <pluto/math/vector2i.h>

#include <sstream>

namespace pluto
{
    Vector2I::Vector2I() : Vector2I(0)
    {
    }

    Vector2I::Vector2I(const int scalar) : Vector2I(scalar, scalar)
    {
    }

    Vector2I::Vector2I(const int x, const int y) : x(x), y(y)
    {
    }

    bool Vector2I::operator==(const Vector2I& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool Vector2I::operator!=(const Vector2I& rhs) const
    {
        return !(*this == rhs);
    }

    int Vector2I::operator[](const int index) const
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        default:
            throw std::out_of_range("");
        }
    }

    std::ostream& operator<<(std::ostream& os, const Vector2I& vector)
    {
        os << "[" << vector.x << "," << vector.y << "]";
        return os;
    }

    std::string Vector2I::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    int* Vector2I::Data()
    {
        return &x;
    }

    const int* Vector2I::Data() const
    {
        return &x;
    }
}

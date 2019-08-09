#include "pluto/math/rect.h"
#include "pluto/math/vector2f.h"

namespace pluto
{
    Rect::Rect()
        : x(0),
          y(0),
          width(0),
          height(0)
    {
    }

    Rect::Rect(const float x, const float y, const float width, const float height)
        : x(x),
          y(y),
          width(width),
          height(height)
    {
    }

    Rect::Rect(const Vector2F& center, const Vector2F& size)
        : x(center.x - size.x / 2),
          y(center.y + size.y / 2),
          width(size.x),
          height(size.y)
    {
    }

    float Rect::GetX() const
    {
        return x;
    }

    float Rect::GetY() const
    {
        return y;
    }

    float Rect::GetWidth() const
    {
        return width;
    }

    float Rect::GetHeight() const
    {
        return height;
    }

    Vector2F Rect::GetMin() const
    {
        return {x, y};
    }

    Vector2F Rect::GetMax() const
    {
        return {x + width, y - height};
    }

    Vector2F Rect::GetCenter()
    {
        return {x + width / 2, y - height / 2};
    }

    Vector2F Rect::GetSize()
    {
        return {width, height};
    }
}

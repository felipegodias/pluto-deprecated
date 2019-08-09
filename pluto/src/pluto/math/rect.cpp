#include "pluto/math/rect.h"

namespace pluto
{
    Rect::Rect()
        : min(Vector2F::ZERO),
          max(Vector2F::ZERO)
    {
    }

    Rect::Rect(const Vector2F& center, const Vector2F& size)
        : min(center.x - size.x / 2, center.y - size.y / 2),
          max(center.x + size.x / 2, center.y + size.y / 2)
    {
    }

    Rect::Rect(const float xMin, const float yMin, const float xMax, const float yMax)
        : min(xMin, yMin),
          max(xMax, yMax)
    {
    }

    const Vector2F& Rect::GetMin() const
    {
        return min;
    }

    const Vector2F& Rect::GetMax() const
    {
        return max;
    }
}

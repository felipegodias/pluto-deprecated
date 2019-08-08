#pragma once

#include "pluto/api.h"
#include "pluto/math/vector2f.h"

namespace pluto
{
    class PLUTO_API Rect
    {
        Vector2F min;
        Vector2F max;

    public:
        Rect();
        Rect(const Vector2F& center, const Vector2F& size);
        Rect(float xMin, float yMin, float xMax, float yMax);

        const Vector2F& GetMin() const;
        const Vector2F& GetMax() const;
    };
}

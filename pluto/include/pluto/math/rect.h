#pragma once

#include "pluto/api.h"

namespace pluto
{
    class Vector2F;

    class PLUTO_API Rect
    {
        float x;
        float y;
        float width;
        float height;

    public:
        Rect();
        Rect(float x, float y, float width, float height);
        Rect(const Vector2F& center, const Vector2F& size);

        float GetX() const;
        float GetY() const;
        float GetWidth() const;
        float GetHeight() const;

        Vector2F GetMin() const;
        Vector2F GetMax() const;

        Vector2F GetCenter();
        Vector2F GetSize();
    };
}

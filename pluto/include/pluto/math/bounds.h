#pragma once

#include "pluto/api.h"
#include "pluto/math/vector3f.h"

namespace pluto
{
    class PLUTO_API Bounds
    {
    private:
        Vector3F min;
        Vector3F max;

    public:
        Bounds();
        Bounds(const Vector3F& center, const Vector3F& size);

        Bounds(const Bounds& other);
        Bounds(Bounds&& other) noexcept;

        ~Bounds();

        Bounds& operator=(const Bounds& rhs);
        Bounds& operator=(Bounds&& rhs) noexcept;

        const Vector3F& GetMin() const;
        const Vector3F& GetMax() const;

        Vector3F GetCenter() const;
        Vector3F GetSize() const;

        bool Contains(const Vector3F& point) const;
        void Encapsulate(const Vector3F& point);
        void Encapsulate(const Bounds& other);
        bool Intersects(const Bounds& other) const;
    };
}

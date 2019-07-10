#include "pluto/math/bounds.h"

namespace pluto
{
    Bounds::Bounds() : min(Vector3F::ZERO), max(Vector3F::ZERO)
    {
    }

    Bounds::Bounds(const Vector3F& center, const Vector3F& size) : min(center.x - size.x / 2, center.y - size.y / 2,
                                                                       center.z - size.z / 2),
                                                                   max(center.x + size.x / 2, center.y + size.y / 2,
                                                                       center.z + size.z / 2)
    {
    }

    Bounds::Bounds(const Bounds& other) = default;

    Bounds::Bounds(Bounds&& other) noexcept : min(std::move(other.min)), max(std::move(other.max))
    {
    }

    Bounds::~Bounds() = default;

    Bounds& Bounds::operator=(const Bounds& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        min = rhs.min;
        max = rhs.max;
        return *this;
    }

    Bounds& Bounds::operator=(Bounds&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        min = std::move(rhs.min);
        max = std::move(rhs.max);
        return *this;
    }

    const Vector3F& Bounds::GetMin() const
    {
        return min;
    }

    const Vector3F& Bounds::GetMax() const
    {
        return max;
    }

    Vector3F Bounds::GetCenter() const
    {
        return Vector3F((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
    }

    Vector3F Bounds::GetSize() const
    {
        return Vector3F(max.x - min.x, max.y - min.y, max.z - min.z);
    }

    bool Bounds::Contains(const Vector3F& point) const
    {
        return point.x < min.x || point.x > max.x ||
            point.y < min.y || point.y > max.y ||
            point.z < min.z || point.z > max.z;
    }

    void Bounds::Encapsulate(const Vector3F& point)
    {
        min = Vector3F::Min(min, point);
        max = Vector3F::Max(max, point);
    }

    void Bounds::Encapsulate(const Bounds& other)
    {
        min = Vector3F::Min(min, other.min);
        max = Vector3F::Max(max, other.max);
    }

    bool Bounds::Intersects(const Bounds& other) const
    {
        return max.x < other.min.x || other.max.x < min.x ||
            max.y < other.min.y || other.max.y < min.y ||
            max.z < other.min.z || other.max.z < min.z;
    }
}

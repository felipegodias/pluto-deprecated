#pragma once

#include "pluto/scene/components/component.h"

namespace pluto
{
    class Vector2F;

    class PLUTO_API Collider2D : public Component
    {
    protected:
        class Impl;

    private:
        Impl* impl;

    public:
        virtual ~Collider2D() = 0;

        explicit Collider2D(Impl& impl);

        Collider2D(const Collider2D& other) = delete;
        Collider2D(Collider2D&& other) noexcept;
        Collider2D& operator=(const Collider2D& rhs) = delete;
        Collider2D& operator=(Collider2D&& rhs) noexcept;

        Vector2F GetOffset() const;
        void SetOffset(const Vector2F& value);

        void OnEarlyFixedUpdate() override;
        void OnLateFixedUpdate() override;
    };
}

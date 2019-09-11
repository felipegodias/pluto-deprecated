#pragma once

#include "collider_2d.h"

namespace pluto
{
    class Vector2F;

    class PLUTO_API BoxCollider2D final : public Collider2D
    {
    public:
        class PLUTO_API Factory final : public Component::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Component> Create(const Resource<GameObject>& gameObject) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~BoxCollider2D() override;
        explicit BoxCollider2D(std::unique_ptr<Impl> impl);

        BoxCollider2D(const BoxCollider2D& other) = delete;
        BoxCollider2D(BoxCollider2D&& other) noexcept;
        BoxCollider2D& operator=(const BoxCollider2D& rhs) = delete;
        BoxCollider2D& operator=(BoxCollider2D&& rhs) noexcept;

        Vector2F GetSize() const;
        void SetSize(const Vector2F& value);
    };
}

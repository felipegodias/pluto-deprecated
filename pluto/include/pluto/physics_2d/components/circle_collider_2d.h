#pragma once

#include "collider_2d.h"
#include <memory>

namespace pluto
{
    class PLUTO_API CircleCollider2D final : public Collider2D
    {
    public:
        class PLUTO_API Factory final : public Component::Factory
        {
        public:
            explicit Factory(ServiceCollection & serviceCollection);
            std::unique_ptr<Component> Create(const Resource<GameObject>& gameObject) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~CircleCollider2D() override;
        explicit CircleCollider2D(std::unique_ptr<Impl> impl);

        CircleCollider2D(const CircleCollider2D& other) = delete;
        CircleCollider2D(CircleCollider2D&& other) noexcept;
        CircleCollider2D& operator=(const CircleCollider2D& rhs) = delete;
        CircleCollider2D& operator=(CircleCollider2D&& rhs) noexcept;

        float GetRadius() const;
        void SetRadius(float value);
    };
}

#pragma once

#include "pluto/scene/components/component.h"
#include <memory>

namespace pluto
{
    class Vector2F;

    class PLUTO_API Rigidbody2D final : public Component
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
        ~Rigidbody2D() override;
        explicit Rigidbody2D(std::unique_ptr<Impl> impl);

        Rigidbody2D(const Rigidbody2D& other) = delete;
        Rigidbody2D(Rigidbody2D&& other) noexcept;
        Rigidbody2D& operator=(const Rigidbody2D& rhs) = delete;
        Rigidbody2D& operator=(Rigidbody2D&& rhs) noexcept;

        void AddForce(const Vector2F& force);
        void AddForce(const Vector2F& force, const Vector2F& point);
        void AddTorque(float torque);

        void OnUpdate() override;
    };
}

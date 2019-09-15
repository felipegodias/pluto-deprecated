#pragma once

#include "physics_2d_shape.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Guid;
    class Physics2DBody;

    class PLUTO_API Physics2DCircleShape final : public Physics2DShape
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Physics2DCircleShape> Create(Physics2DBody& body, const Guid& colliderId,
                                                         const Vector2F& offset,
                                                         float radius) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Physics2DCircleShape() override;
        explicit Physics2DCircleShape(std::unique_ptr<Impl> impl);

        Physics2DCircleShape(const Physics2DCircleShape& other) = delete;
        Physics2DCircleShape(Physics2DCircleShape&& other) noexcept;
        Physics2DCircleShape& operator=(const Physics2DCircleShape& other) = delete;
        Physics2DCircleShape& operator=(Physics2DCircleShape&& other) noexcept;

        Vector2F GetOffset() const override;
        void SetOffset(const Vector2F& value) override;

        float GetRadius() const;
        void SetRadius(float value);
    };
}

#pragma once

#include "physics_2d_shape.h"
#include "pluto/service/base_factory.h"
#include <memory>

namespace pluto
{
    class Guid;
    class Physics2DBody;

    class PLUTO_API Physics2DBoxShape final : public Physics2DShape
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Physics2DBoxShape> Create(Physics2DBody& body, const Guid& colliderId,
                                                      const Vector2F& offset,
                                                      const Vector2F& size) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Physics2DBoxShape() override;
        explicit Physics2DBoxShape(std::unique_ptr<Impl> impl);

        Physics2DBoxShape(const Physics2DBoxShape& other) = delete;
        Physics2DBoxShape(Physics2DBoxShape&& other) noexcept;
        Physics2DBoxShape& operator=(const Physics2DBoxShape& rhs) = delete;
        Physics2DBoxShape& operator=(Physics2DBoxShape&& rhs) noexcept;

        Vector2F GetOffset() const override;
        void SetOffset(const Vector2F& value) override;

        Vector2F GetSize() const;
        void SetSize(const Vector2F& value);
    };
}

#pragma once

#include "pluto/service/base_factory.h"

#include <vector>
#include <memory>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class Collider2D;

    class Vector2F;

    class PLUTO_API Collision2D
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Collision2D> Create(const Resource<Collider2D>& collider,
                                                const Resource<Collider2D>& otherCollider,
                                                const std::vector<Vector2F>& contactPoints) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Collision2D();

        explicit Collision2D(std::unique_ptr<Impl> impl);

        Collision2D(const Collision2D& other) = delete;
        Collision2D(Collision2D&& other) noexcept;
        Collision2D& operator=(const Collision2D& rhs) = delete;
        Collision2D& operator=(Collision2D&& rhs) noexcept;

        Resource<Collider2D> GetCollider() const;
        Resource<Collider2D> GetOtherCollider() const;

        const std::vector<Vector2F>& GetContactPoints() const;
    };
}

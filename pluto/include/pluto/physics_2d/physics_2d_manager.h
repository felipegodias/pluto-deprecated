#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"
#include <memory>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class GameObject;

    class Physics2DBody;

    class Vector2F;

    class PLUTO_API Physics2DManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Physics2DManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Physics2DManager();
        explicit Physics2DManager(std::unique_ptr<Impl> impl);

        Physics2DManager(const Physics2DManager& other) = delete;
        Physics2DManager(Physics2DManager&& other) noexcept;
        Physics2DManager& operator=(const Physics2DManager& rhs) = delete;
        Physics2DManager& operator=(Physics2DManager&& rhs) noexcept;

        bool HasBody(const Resource<GameObject>& gameObject) const;
        Physics2DBody& GetBody(const Resource<GameObject>& gameObject) const;
        Physics2DBody& CreateBody(const Resource<GameObject>& gameObject);
        Physics2DBody& GetOrCreateBody(const Resource<GameObject>& gameObject);
        void DestroyBody(const Resource<GameObject>& gameObject);

        void* GetWorld() const;
    };
}

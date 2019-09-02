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
        Physics2DManager& operator=(const Physics2DManager& other) = delete;
        Physics2DManager& operator=(Physics2DManager&& other) noexcept;

        Physics2DBody& AddToWorld(const Resource<GameObject>& gameObject);
    };
}

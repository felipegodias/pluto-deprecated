#pragma once

#include "pluto/service/base_factory.h"

#include <memory>
#include <string>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class Guid;
    class GameObject;
    class Transform;

    class PLUTO_API Scene
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Scene> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Scene();
        explicit Scene(std::unique_ptr<Impl> impl);

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) noexcept;
        Scene& operator=(const Scene& rhs) = delete;
        Scene& operator=(Scene&& rhs) noexcept;

        const Guid& GetId() const;

        Resource<GameObject> GetRootGameObject() const;

        Resource<GameObject> CreateGameObject();
        Resource<GameObject> CreateGameObject(const std::string& name);
        Resource<GameObject> CreateGameObject(const Resource<Transform>& parent);
        Resource<GameObject> CreateGameObject(const Resource<Transform>& parent, const std::string& name);

        void Destroy();
        void OnUpdate(uint32_t currentFrame);
        void OnCleanup();
    };
}

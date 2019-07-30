#pragma once

#include "pluto/service/base_factory.h"

#include <memory>
#include <string>

namespace pluto
{
    class Guid;
    class GameObject;
    class Transform;

    class PLUTO_API Scene
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
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

        GameObject& GetRootGameObject() const;

        GameObject& CreateGameObject();
        GameObject& CreateGameObject(const std::string& name);
        GameObject& CreateGameObject(Transform& parent);
        GameObject& CreateGameObject(Transform& parent, const std::string& name);

        void Destroy();
        void OnUpdate(uint32_t currentFrame);
        void OnCleanup();
    };
}

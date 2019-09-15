#pragma once

#include "pluto/service/base_factory.h"
#include "pluto/memory/object.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class Guid;
    class Transform;
    class Component;

    class PLUTO_API GameObject final : public Object
    {
    public:
        enum class Flags
        {
            None = 0,
            Static = 1,
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<GameObject> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~GameObject() override;
        explicit GameObject(std::unique_ptr<Impl> impl);

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) noexcept;
        GameObject& operator=(const GameObject& rhs) = delete;
        GameObject& operator=(GameObject&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        Flags GetFlags() const;
        bool IsDestroyed() const;

        Resource<Transform> GetTransform() const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        Resource<T> AddComponent();

        Resource<Component> AddComponent(const std::type_info& type);

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        Resource<T> GetComponent() const;

        Resource<Component> GetComponent(const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        std::vector<Resource<T>> GetComponents() const;

        std::vector<Resource<Component>> GetComponents(
            const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        Resource<T> GetComponentInChildren() const;

        Resource<Component> GetComponentInChildren(
            const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        std::vector<Resource<T>> GetComponentsInChildren() const;

        std::vector<Resource<Component>> GetComponentsInChildren(
            const std::function<bool(const Component& component)>& predicate) const;

        void Destroy();

        void OnEarlyFixedUpdate();
        void OnFixedUpdate();
        void OnLateFixedUpdate();

        void OnEarlyUpdate();
        void OnUpdate();
        void OnLateUpdate();

        void OnPreRender();
        void OnRender();
        void OnPostRender();
    };
}

#include "game_object.inl"

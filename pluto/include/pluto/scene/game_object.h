#pragma once

#include "pluto/service/base_factory.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace pluto
{
    class Guid;
    class Transform;
    class Component;

    template <typename T>
    using IsComponent = std::enable_if_t<std::is_base_of_v<Component, T>, int>;

    class PLUTO_API GameObject
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
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<GameObject> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit GameObject(std::unique_ptr<Impl> impl);

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) noexcept;
        ~GameObject();

        GameObject& operator=(const GameObject& rhs) = delete;
        GameObject& operator=(GameObject&& rhs) noexcept;

        bool operator==(const GameObject& rhs) const;
        bool operator!=(const GameObject& rhs) const;

        const Guid& GetId() const;
        const std::string& GetName() const;
        void SetName(const std::string& value);

        Flags GetFlags() const;
        bool IsDestroyed() const;

        Transform& GetTransform() const;

        template <typename T, IsComponent<T>  = 0>
        T& AddComponent();

        Component& AddComponent(const std::type_info& type);

        template <typename T, IsComponent<T>  = 0>
        T* GetComponent() const;

        Component* GetComponent(const std::function<bool(const Component& component)>& filter) const;

        template <typename T, IsComponent<T>  = 0>
        std::vector<std::reference_wrapper<T>> GetComponents() const;

        std::vector<std::reference_wrapper<Component>> GetComponents(
            const std::function<bool(const Component& component)>& filter) const;

        template <typename T, IsComponent<T>  = 0>
        T* GetComponentInChildren() const;

        Component* GetComponentInChildren(const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, IsComponent<T>  = 0>
        std::vector<std::reference_wrapper<T>> GetComponentsInChildren() const;

        std::vector<std::reference_wrapper<Component>> GetComponentsInChildren(
            const std::function<bool(const Component& component)>& predicate) const;

        void Destroy();

        void OnUpdate(uint32_t currentFrame);
    };

    template <typename T, IsComponent<T>>
    T& GameObject::AddComponent()
    {
        return static_cast<T&>(AddComponent(typeid(T)));
    }

    template <typename T, IsComponent<T>>
    T* GameObject::GetComponent() const
    {
        return dynamic_cast<T*>(GetComponent([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        }));
    }

    template <typename T, IsComponent<T>>
    std::vector<std::reference_wrapper<T>> GameObject::GetComponents() const
    {
        std::vector<std::reference_wrapper<Component>> components = GetComponents([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        });
        std::vector<std::reference_wrapper<T>> result;
        for (auto& it : components)
        {
            result.emplace_back(static_cast<T&>(it.get()));
        }
        return result;
    }

    template <typename T, IsComponent<T>>
    T* GameObject::GetComponentInChildren() const
    {
        return dynamic_cast<T*>(GetComponentInChildren([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        }));
    }

    template <typename T, IsComponent<T>>
    std::vector<std::reference_wrapper<T>> GameObject::GetComponentsInChildren() const
    {
        std::vector<std::reference_wrapper<Component>> components = GetComponentsInChildren(
            [](const Component& component)
            {
                return dynamic_cast<const T*>(&component) != nullptr;
            });
        std::vector<std::reference_wrapper<T>> result;
        for (auto& it : components)
        {
            result.emplace_back(static_cast<T&>(it.get()));
        }
        return result;
    }
}

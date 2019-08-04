#pragma once

namespace pluto
{
    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    T& GameObject::AddComponent()
    {
        return static_cast<T&>(AddComponent(typeid(T)));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    T* GameObject::GetComponent() const
    {
        return dynamic_cast<T*>(GetComponent([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        }));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
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

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    T* GameObject::GetComponentInChildren() const
    {
        return dynamic_cast<T*>(GetComponentInChildren([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        }));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
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

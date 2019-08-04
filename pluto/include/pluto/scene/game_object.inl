#pragma once

#include "pluto/memory/resource.h"

namespace pluto
{
    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    Resource<T> GameObject::AddComponent()
    {
        return ResourceUtils::Cast<T>(AddComponent(typeid(T)));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    Resource<T> GameObject::GetComponent() const
    {
        return ResourceUtils::Cast<T>(GetComponent([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        }));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    std::vector<Resource<T>> GameObject::GetComponents() const
    {
        std::vector<Resource<Component>> components = GetComponents([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        });
        std::vector<Resource<T>> result;
        for (auto& it : components)
        {
            result.emplace_back(ResourceUtils::Cast<T>(it));
        }
        return result;
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    Resource<T> GameObject::GetComponentInChildren() const
    {
        return ResourceUtils::Cast<T>(GetComponentInChildren([](const Component& component)
        {
            return dynamic_cast<const T*>(&component) != nullptr;
        }));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
    std::vector<Resource<T>> GameObject::GetComponentsInChildren() const
    {
        std::vector<Resource<Component>> components = GetComponentsInChildren(
            [](const Component& component)
            {
                return dynamic_cast<const T*>(&component) != nullptr;
            });
        std::vector<Resource<T>> result;
        for (auto& it : components)
        {
            result.emplace_back(ResourceUtils::Cast<T>(it));
        }
        return result;
    }
}

#pragma once

#include "pluto/service/base_factory.h"
#include "pluto/memory/object.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace pluto
{
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
            explicit Factory(ServiceCollection& diContainer);
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

        bool operator==(const GameObject& rhs) const;
        bool operator!=(const GameObject& rhs) const;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        Flags GetFlags() const;
        bool IsDestroyed() const;

        Transform& GetTransform() const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        T& AddComponent();

        Component& AddComponent(const std::type_info& type);

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        T* GetComponent() const;

        Component* GetComponent(const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        std::vector<std::reference_wrapper<T>> GetComponents() const;

        std::vector<std::reference_wrapper<Component>> GetComponents(
            const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        T* GetComponentInChildren() const;

        Component* GetComponentInChildren(const std::function<bool(const Component& component)>& predicate) const;

        template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>  = false>
        std::vector<std::reference_wrapper<T>> GetComponentsInChildren() const;

        std::vector<std::reference_wrapper<Component>> GetComponentsInChildren(
            const std::function<bool(const Component& component)>& predicate) const;

        void Destroy();

        void OnUpdate(uint32_t currentFrame);
    };
}

#include "game_object.inl"

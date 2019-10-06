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

    class Collision2D;
    class Collider2D;

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

        bool IsGloballyActive() const;
        bool IsActive() const;
        void SetActive(bool value);

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

        void OnCollision2DBegin(const Collision2D& collision);
        void OnCollision2DEnd(const Collision2D& collision);

        void OnTrigger2DEnter(const Resource<Collider2D>& collider);
        void OnTrigger2DExit(const Resource<Collider2D>& collider);

        void OnEarlyUpdate();
        void OnUpdate();
        void OnLateUpdate();

        void OnPreRender();
        void OnRender();
        void OnPostRender();
    };
}

#include "game_object.inl"

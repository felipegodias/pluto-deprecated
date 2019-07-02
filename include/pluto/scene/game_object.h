#pragma once

#include "../di/base_factory.h"

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
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
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

        const Guid& GetId() const;
        std::string GetName() const;
        void SetName(std::string name);

        Transform& GetTransform() const;

        GameObject& GetParent() const;
        void SetParent(GameObject& value);

        GameObject& GetChild(int index);
        const std::vector<GameObject>& GetChildren() const;

        template <typename T, IsComponent<T>  = 0>
        T& AddComponent();

        template <typename T, IsComponent<T>  = 0>
        T* GetComponent() const;

        template <typename T, IsComponent<T>  = 0>
        std::vector<T&> GetComponents() const;

        template <typename T, IsComponent<T>  = 0>
        T* GetComponentInChildren() const;

        template <typename T, IsComponent<T>  = 0>
        std::vector<T&> GetComponentsInChildren() const;

        void Destroy();

        void OnUpdate();
        void OnRender();
    };
}

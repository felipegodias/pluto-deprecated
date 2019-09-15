#pragma once

#include "pluto/service/base_factory.h"
#include "pluto/memory/object.h"

#include <memory>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class Guid;
    class GameObject;

    class PLUTO_API Component : public Object
    {
    public:
        class PLUTO_API Factory : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            virtual std::unique_ptr<Component> Create(const Resource<GameObject>& gameObject) const = 0;
        };

    protected:
        class Impl;

    private:
        Impl* impl;

    public:
        virtual ~Component() = 0;
        explicit Component(Impl& impl);

        Component(const Component& other) = delete;
        Component(Component&& other) noexcept;
        Component& operator=(const Component& rhs) = delete;
        Component& operator=(Component&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        Resource<GameObject> GetGameObject() const;

        virtual void OnEarlyFixedUpdate();
        virtual void OnFixedUpdate();
        virtual void OnLateFixedUpdate();

        virtual void OnEarlyUpdate();
        virtual void OnUpdate();
        virtual void OnLateUpdate();

        virtual void OnPreRender();
        virtual void OnRender();
        virtual void OnPostRender();

        virtual void OnDestroy();
    };
}

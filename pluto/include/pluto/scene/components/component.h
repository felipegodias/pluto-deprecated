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

        virtual ~Component() = 0;
        Component();

        Component(const Component& other) = delete;
        Component(Component&& other) noexcept;
        Component& operator=(const Component& rhs) = delete;
        Component& operator=(Component&& rhs) noexcept;

        virtual Resource<GameObject> GetGameObject() const = 0;

        virtual void OnUpdate();
        virtual void OnDestroy();
    };
}

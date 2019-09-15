#include "pluto/scene/components/component.h"
#include "pluto/scene/components/component.impl.hpp"

namespace pluto
{
    Component::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    Component::~Component() = default;

    Component::Component(Impl& impl)
        : impl(&impl)
    {
    }

    Component::Component(Component&& other) noexcept
    {
        impl = other.impl;
        other.impl = nullptr;
    }

    Component& Component::operator=(Component&& rhs) noexcept = default;

    const Guid& Component::GetId() const
    {
        return impl->GetId();
    }

    const std::string& Component::GetName() const
    {
        return impl->GetName();
    }

    void Component::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    Resource<GameObject> Component::GetGameObject() const
    {
        return impl->GetGameObject();
    }

    void Component::OnEarlyFixedUpdate()
    {
    }

    void Component::OnFixedUpdate()
    {
    }

    void Component::OnLateFixedUpdate()
    {
    }

    void Component::OnEarlyUpdate()
    {
    }

    void Component::OnUpdate()
    {
    }

    void Component::OnLateUpdate()
    {
    }

    void Component::OnPreRender()
    {
    }

    void Component::OnRender()
    {
    }

    void Component::OnPostRender()
    {
    }

    void Component::OnDestroy()
    {
    }
}

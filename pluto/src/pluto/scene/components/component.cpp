#include <pluto/scene/components/component.h>
#include <pluto/guid.h>

namespace pluto
{
    Component::Factory::~Factory() = default;

    Component::Factory::Factory(ServiceCollection& diContainer)
        : BaseFactory(diContainer)
    {
    }

    Component::Factory::Factory(Factory&& other) noexcept = default;

    Component::Factory& Component::Factory::operator=(Factory&& rhs) noexcept = default;

    Component::~Component() = default;
    Component::Component() = default;

    Component::Component(Component&& other) noexcept = default;

    Component& Component::operator=(Component&& rhs) noexcept = default;

    bool Component::operator==(const Component& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    bool Component::operator!=(const Component& rhs) const
    {
        return !(*this != rhs);
    }

    void Component::OnUpdate()
    {
    }

    void Component::OnDestroy()
    {
    }
}

#include <pluto/scene/components/component.h>
#include <pluto/guid.h>

namespace pluto
{
    Component::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    Component::~Component() = default;
    Component::Component() = default;

    Component::Component(Component&& other) noexcept = default;

    Component& Component::operator=(Component&& rhs) noexcept = default;

    void Component::OnUpdate()
    {
    }

    void Component::OnDestroy()
    {
    }
}

#include <pluto/scene/component.h>
#include <pluto/guid.h>

namespace pluto
{
    Component::Component() = default;
    Component::~Component() = default;

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

    void Component::OnRender()
    {
    }

    void Component::OnDestroy()
    {
    }
}

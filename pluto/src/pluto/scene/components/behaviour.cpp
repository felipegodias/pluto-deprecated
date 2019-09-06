#include "pluto/scene/components/behaviour.h"
#include "pluto/scene/components/component.impl.hpp"

#include "pluto/guid.h"

namespace pluto
{
    class Behaviour::Impl : public Component::Impl
    {
    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject)
            : Component::Impl(guid, gameObject)
        {
        }
    };

    Behaviour::~Behaviour() = default;

    Behaviour::Behaviour(std::unique_ptr<Impl> impl)
        : Component(*impl),
          impl(std::move(impl))
    {
    }

    Behaviour::Behaviour(const Resource<GameObject>& gameObject)
        : Behaviour(std::make_unique<Impl>(Guid::New(), gameObject))
    {
    }

    Behaviour::Behaviour(Behaviour&& other) noexcept = default;

    Behaviour& Behaviour::operator=(Behaviour&& rhs) noexcept = default;
}

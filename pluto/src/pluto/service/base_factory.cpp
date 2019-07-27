#include <pluto/service/base_factory.h>

namespace pluto
{
    BaseFactory::~BaseFactory() = default;

    BaseFactory::BaseFactory(ServiceCollection& diContainer)
        : diContainer(&diContainer)
    {
    }

    ServiceCollection& BaseFactory::GetServiceCollection() const
    {
        return *diContainer;
    }
}

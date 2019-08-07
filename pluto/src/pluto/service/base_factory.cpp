#include <pluto/service/base_factory.h>

namespace pluto
{
    BaseFactory::~BaseFactory() = default;

    BaseFactory::BaseFactory(ServiceCollection& serviceCollection)
        : diContainer(&serviceCollection)
    {
    }

    BaseFactory::BaseFactory(BaseFactory&& other) noexcept = default;

    BaseFactory& BaseFactory::operator=(BaseFactory&& rhs) noexcept = default;

    ServiceCollection& BaseFactory::GetServiceCollection() const
    {
        return *diContainer;
    }
}

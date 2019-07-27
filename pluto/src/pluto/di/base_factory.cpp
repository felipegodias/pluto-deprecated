#include <pluto/di/base_factory.h>

namespace pluto
{
    BaseFactory::~BaseFactory() = default;

    BaseFactory::BaseFactory(DiContainer& diContainer)
        : diContainer(&diContainer)
    {
    }

    DiContainer& BaseFactory::GetServiceCollection() const
    {
        return *diContainer;
    }
}

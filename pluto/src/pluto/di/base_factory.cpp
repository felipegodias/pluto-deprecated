#include <pluto/di/base_factory.h>

namespace pluto
{
    BaseFactory::BaseFactory(DiContainer& diContainer) : diContainer(diContainer)
    {
    }

    BaseFactory::~BaseFactory() = default;
}

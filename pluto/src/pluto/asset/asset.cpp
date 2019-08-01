#include <pluto/asset/asset.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    Asset::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    Asset::~Asset() = default;

    Asset::Asset() = default;

    Asset::Asset(Asset&& other) noexcept = default;

    Asset& Asset::operator=(Asset&& rhs) noexcept = default;
}

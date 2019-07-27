#include <pluto/asset/asset.h>
#include <pluto/guid.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    Asset::Factory::~Factory() = default;

    Asset::Factory::Factory(DiContainer& diContainer)
        : BaseFactory(diContainer)
    {
    }

    Asset::Factory::Factory(Factory&& other) noexcept = default;

    Asset::Factory& Asset::Factory::operator=(Factory&& rhs) noexcept = default;

    Asset::~Asset() = default;

    Asset::Asset() = default;

    Asset::Asset(Asset&& other) noexcept = default;

    Asset& Asset::operator=(Asset&& rhs) noexcept = default;

    bool Asset::operator==(const Asset& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    bool Asset::operator!=(const Asset& rhs) const
    {
        return !(*this == rhs);
    }
}

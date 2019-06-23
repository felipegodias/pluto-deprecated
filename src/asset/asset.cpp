#include <pluto/asset/asset.h>
#include <pluto/guid.h>

namespace pluto
{
    Asset::~Asset() = default;

    bool Asset::operator==(const Asset& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    bool Asset::operator!=(const Asset& rhs) const
    {
        return !(*this == rhs);
    }
}

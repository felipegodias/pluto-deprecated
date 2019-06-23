#include <pluto/asset/asset.h>
#include <pluto/guid.h>

namespace pluto
{
    bool Asset::operator==(const Asset& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    bool Asset::operator!=(const Asset& rhs) const
    {
        return !(*this == rhs);
    }
}

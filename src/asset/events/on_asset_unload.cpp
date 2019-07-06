#include <pluto/asset/events/on_asset_unload.h>

namespace pluto
{
    OnAssetUnload::OnAssetUnload(const Asset& asset) : asset(asset)
    {
    }

    const Asset& OnAssetUnload::GetAsset() const
    {
        return asset;
    }
}

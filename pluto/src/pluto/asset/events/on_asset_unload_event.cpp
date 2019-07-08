#include <pluto/asset/events/on_asset_unload_event.h>

namespace pluto
{
    OnAssetUnloadEvent::OnAssetUnloadEvent(const Asset& asset) : asset(asset)
    {
    }

    const Asset& OnAssetUnloadEvent::GetAsset() const
    {
        return asset;
    }
}

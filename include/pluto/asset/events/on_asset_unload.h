#pragma once

#include "pluto/event/base_event.h"

namespace pluto
{
    class Asset;

    class PLUTO_API OnAssetUnload final : public BaseEvent
    {
    private:
        const Asset& asset;

    public:
        explicit OnAssetUnload(const Asset& asset);

        const Asset& GetAsset() const;
    };
}

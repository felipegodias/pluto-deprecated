#pragma once

#include "pluto/event/base_event.h"

namespace pluto
{
    class Asset;

    class PLUTO_API OnAssetUnloadEvent final : public BaseEvent
    {
    private:
        const Asset& asset;

    public:
        explicit OnAssetUnloadEvent(const Asset& asset);

        const Asset& GetAsset() const;
    };
}

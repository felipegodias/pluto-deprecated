#pragma once

#include "pluto/event/base_event.h"

namespace pluto
{
    class PLUTO_API OnRenderCameraEvent final : public BaseEvent
    {
        // TODO: Pass which camera is currently rendering.
    };
}

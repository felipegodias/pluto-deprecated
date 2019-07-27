#pragma once

#include "pluto/service/base_service.h"

namespace pluto
{
    class Transform;
    class MeshAsset;
    class MaterialAsset;

    class PLUTO_API RenderManager : public BaseService
    {
    public:
        virtual ~RenderManager() = 0;
    };
}

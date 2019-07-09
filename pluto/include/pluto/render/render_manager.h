#pragma once

#include "pluto/di/singleton.h"

namespace pluto
{
    class Transform;
    class MeshAsset;
    class MaterialAsset;

    class PLUTO_API RenderManager : public Singleton
    {
    public:
        virtual ~RenderManager() = 0;
        virtual void PushToRender(Transform& transform, MeshAsset& mesh, MaterialAsset& material) = 0;
    };
}

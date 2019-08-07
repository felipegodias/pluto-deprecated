#pragma once

#include "pluto/service/base_factory.h"
#include <memory>

namespace pluto
{
    class MeshAsset;

    class PLUTO_API MeshBuffer
    {
    public:
        class PLUTO_API Factory : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            virtual std::unique_ptr<MeshBuffer> Create(const MeshAsset& mesh) const = 0;
        };

        MeshBuffer();

        MeshBuffer(const MeshBuffer& other) = delete;
        MeshBuffer(MeshBuffer&& other) noexcept;

        virtual ~MeshBuffer() = 0;

        MeshBuffer& operator=(const MeshBuffer& rhs) = delete;
        MeshBuffer& operator=(MeshBuffer&& rhs) noexcept;
    };
}

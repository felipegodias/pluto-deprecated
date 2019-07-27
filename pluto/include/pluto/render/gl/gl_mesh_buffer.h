#pragma once

#include "pluto/render/mesh_buffer.h"
#include <memory>

namespace pluto
{
    class MeshAsset;

    class PLUTO_API GlMeshBuffer final : public MeshBuffer
    {
    public:
        class PLUTO_API Factory final : public MeshBuffer::Factory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<MeshBuffer> Create(const MeshAsset& mesh) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit GlMeshBuffer(std::unique_ptr<Impl> impl);
        GlMeshBuffer(const GlMeshBuffer& other) = delete;
        GlMeshBuffer(GlMeshBuffer&& other) noexcept;
        ~GlMeshBuffer() override;
        GlMeshBuffer& operator=(const GlMeshBuffer& rhs) = delete;
        GlMeshBuffer& operator=(GlMeshBuffer&& rhs) noexcept;

        void Bind();
        void Unbind();
        void Draw();
    };
}

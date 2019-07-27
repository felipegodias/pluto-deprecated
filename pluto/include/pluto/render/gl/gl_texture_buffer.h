#pragma once

#include "pluto/render/texture_buffer.h"
#include <memory>

namespace pluto
{
    class PLUTO_API GlTextureBuffer final : public TextureBuffer
    {
    public:
        class PLUTO_API Factory final : public TextureBuffer::Factory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<TextureBuffer> Create() const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit GlTextureBuffer(std::unique_ptr<Impl> impl);
        ~GlTextureBuffer() override;

        GlTextureBuffer(const GlTextureBuffer& other) = delete;
        GlTextureBuffer(GlTextureBuffer&& other) noexcept;
        GlTextureBuffer& operator=(const GlTextureBuffer& rhs) = delete;
        GlTextureBuffer& operator=(GlTextureBuffer&& rhs) noexcept;

        void Update(TextureAsset& textureAsset) override;
        void Bind(uint8_t location);
        void Unbind();
    };
}

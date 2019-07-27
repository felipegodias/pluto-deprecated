#pragma once

#include "pluto/render/texture_buffer.h"
#include <memory>

namespace pluto
{
    class DummyTextureBuffer final : public TextureBuffer
    {
    public:
        class Factory final : public TextureBuffer::Factory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<TextureBuffer> Create() const override;
        };

        DummyTextureBuffer();
        void Update(TextureAsset& textureAsset) override;
    };
}
#include "dummy_texture_buffer.h"
#include <pluto/asset/texture_asset.h>

namespace pluto
{
    DummyTextureBuffer::Factory::Factory(DiContainer& diContainer)
        : TextureBuffer::Factory(diContainer)
    {
    }

    std::unique_ptr<TextureBuffer> DummyTextureBuffer::Factory::Create() const
    {
        return std::make_unique<DummyTextureBuffer>();
    }

    DummyTextureBuffer::DummyTextureBuffer() = default;

    void DummyTextureBuffer::Update(TextureAsset& textureAsset)
    {
    }
}

#include "pluto/render/texture_buffer.h"

namespace pluto
{
    TextureBuffer::Factory::Factory(ServiceCollection& serviceCollection) : BaseFactory(serviceCollection)
    {
    }

    TextureBuffer::TextureBuffer() = default;

    TextureBuffer::TextureBuffer(TextureBuffer&& other) noexcept = default;

    TextureBuffer::~TextureBuffer() = default;

    TextureBuffer& TextureBuffer::operator=(TextureBuffer&& rhs) noexcept = default;
}

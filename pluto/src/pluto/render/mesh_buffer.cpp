#include "pluto/render/mesh_buffer.h"

namespace pluto
{
    MeshBuffer::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    MeshBuffer::MeshBuffer() = default;

    MeshBuffer::MeshBuffer(MeshBuffer&& other) noexcept = default;

    MeshBuffer::~MeshBuffer() = default;

    MeshBuffer& MeshBuffer::operator=(MeshBuffer&& rhs) noexcept = default;
}

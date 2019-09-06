#include <pluto/scene/components/renderer.h>

namespace pluto
{
    Renderer::~Renderer() = default;

    Renderer::Renderer(Impl& impl)
        : Component(impl)
    {
    }

    Renderer::Renderer(Renderer&& other) noexcept = default;

    Renderer& Renderer::operator=(Renderer&& rhs) noexcept = default;
}

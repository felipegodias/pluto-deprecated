#include <pluto/render/render_installer.h>
#include <pluto/render/render_manager.h>

#include <pluto/render/gl/gl_render_manager.h>
#include <pluto/render/gl/gl_mesh_buffer.h>

#include <pluto/di/di_container.h>

namespace pluto
{
    void InstallOpenGl(DiContainer& diContainer)
    {
        diContainer.AddSingleton<MeshBuffer::Factory>(std::make_unique<GlMeshBuffer::Factory>(diContainer));

        const GlRenderManager::Factory factory(diContainer);
        diContainer.AddSingleton<RenderManager>(factory.Create());
    }

    void RenderInstaller::Install(DiContainer& diContainer)
    {
        InstallOpenGl(diContainer);
    }

    void RenderInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<RenderManager>();
        diContainer.RemoveSingleton<MeshBuffer::Factory>();
    }
}

#include <pluto/render/render_installer.h>
#include <pluto/render/render_manager.h>
#include <pluto/render/gl/gl_render_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void RenderInstaller::Install(DiContainer& diContainer)
    {
        const GlRenderManager::Factory factory(diContainer);
        diContainer.AddSingleton<RenderManager>(factory.Create());
    }

    void RenderInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<RenderManager>();
    }
}

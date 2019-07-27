#include <pluto/render/render_installer.h>
#include <pluto/render/render_manager.h>

#include <pluto/render/gl/gl_render_manager.h>
#include <pluto/render/gl/gl_mesh_buffer.h>
#include <pluto/render/gl/gl_shader_program.h>
#include <pluto/render/gl/gl_texture_buffer.h>

#include <pluto/service/service_collection.h>

namespace pluto
{
    void InstallOpenGl(ServiceCollection& diContainer)
    {
        diContainer.AddSingleton<MeshBuffer::Factory>(std::make_unique<GlMeshBuffer::Factory>(diContainer));
        diContainer.AddSingleton<ShaderProgram::Factory>(std::make_unique<GlShaderProgram::Factory>(diContainer));
        diContainer.AddSingleton<TextureBuffer::Factory>(std::make_unique<GlTextureBuffer::Factory>(diContainer));

        const GlRenderManager::Factory factory(diContainer);
        diContainer.AddSingleton<RenderManager>(factory.Create());
    }

    void RenderInstaller::Install(ServiceCollection& diContainer)
    {
        InstallOpenGl(diContainer);
    }

    void RenderInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveSingleton<RenderManager>();
        diContainer.RemoveSingleton<TextureBuffer::Factory>();
        diContainer.RemoveSingleton<ShaderProgram::Factory>();
        diContainer.RemoveSingleton<MeshBuffer::Factory>();
    }
}

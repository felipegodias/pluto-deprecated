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
        diContainer.AddFactory<MeshBuffer>(std::make_unique<GlMeshBuffer::Factory>(diContainer));
        diContainer.AddFactory<ShaderProgram>(std::make_unique<GlShaderProgram::Factory>(diContainer));
        diContainer.AddFactory<TextureBuffer>(std::make_unique<GlTextureBuffer::Factory>(diContainer));

        diContainer.AddService<RenderManager>(GlRenderManager::Factory(diContainer).Create());
    }

    void RenderInstaller::Install(ServiceCollection& diContainer)
    {
        InstallOpenGl(diContainer);
    }

    void RenderInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<RenderManager>();
        diContainer.RemoveFactory<TextureBuffer>();
        diContainer.RemoveFactory<ShaderProgram>();
        diContainer.RemoveFactory<MeshBuffer>();
    }
}

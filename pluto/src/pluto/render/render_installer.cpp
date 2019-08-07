#include <pluto/render/render_installer.h>
#include <pluto/render/render_manager.h>

#include <pluto/render/gl/gl_render_manager.h>
#include <pluto/render/gl/gl_mesh_buffer.h>
#include <pluto/render/gl/gl_shader_program.h>
#include <pluto/render/gl/gl_texture_buffer.h>

#include <pluto/service/service_collection.h>

namespace pluto
{
    void InstallOpenGl(ServiceCollection& serviceCollection)
    {
        serviceCollection.AddFactory<MeshBuffer>(std::make_unique<GlMeshBuffer::Factory>(serviceCollection));
        serviceCollection.AddFactory<ShaderProgram>(std::make_unique<GlShaderProgram::Factory>(serviceCollection));
        serviceCollection.AddFactory<TextureBuffer>(std::make_unique<GlTextureBuffer::Factory>(serviceCollection));

        serviceCollection.AddService<RenderManager>(GlRenderManager::Factory(serviceCollection).Create());
    }

    void RenderInstaller::Install(ServiceCollection& serviceCollection)
    {
        InstallOpenGl(serviceCollection);
    }

    void RenderInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<RenderManager>();
        serviceCollection.RemoveFactory<TextureBuffer>();
        serviceCollection.RemoveFactory<ShaderProgram>();
        serviceCollection.RemoveFactory<MeshBuffer>();
    }
}

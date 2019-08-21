#include "compilers/font_compiler.h"
#include "compilers/material_compiler.h"
#include "compilers/mesh_compiler.h"
#include "compilers/package_compiler.h"
#include "compilers/shader_compiler.h"
#include "compilers/text_compiler.h"
#include "compilers/texture_compiler.h"

#include "dummy/dummy_shader_program.h"
#include "dummy/dummy_texture_buffer.h"

#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/texture_asset.h>
#include <pluto/asset/shader_asset.h>

#include "pluto/log/log_installer.h"

#include "pluto/memory/resource_control.h"
#include "pluto/memory/memory_manager.h"

#include <pluto/render/gl/gl_mesh_buffer.h>

#include <pluto/service/service_collection.h>

#include <pluto/file/path.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include <iostream>
#include <memory>

void InitGl()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize glfw.");
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create glfw window.");
    }
    glfwMakeContextCurrent(window);

    const GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        throw std::runtime_error(fmt::format("Failed to initialize glew. Error: {0}", glewGetErrorString(err)));
    }
}

namespace pluto::compiler
{
    void Compile(const std::string& input)
    {
        std::unique_ptr<ServiceCollection> serviceCollection = std::make_unique<ServiceCollection>();

        auto& fontAssetFactory = serviceCollection->EmplaceFactory<FontAsset, FontAsset::Factory>(*serviceCollection);

        auto& materialAssetFactory = serviceCollection->EmplaceFactory<MaterialAsset, MaterialAsset::Factory>(
            *serviceCollection);

        auto& meshAssetFactory = serviceCollection->EmplaceFactory<MeshAsset, MeshAsset::Factory>(*serviceCollection);

        auto& packageManifestAssetFactory = serviceCollection->EmplaceFactory<
            PackageManifestAsset, PackageManifestAsset::Factory>(*serviceCollection);

        auto& shaderAssetFactory = serviceCollection->EmplaceFactory<ShaderAsset, ShaderAsset::Factory>(
            *serviceCollection);

        auto& textAssetFactory = serviceCollection->EmplaceFactory<TextAsset, TextAsset::Factory>(*serviceCollection);

        auto& textureAssetFactory = serviceCollection->EmplaceFactory<TextureAsset, TextureAsset::Factory>(
            *serviceCollection);

        auto& resourceControlFactory = serviceCollection->EmplaceFactory<ResourceControl, ResourceControl::Factory>(
            *serviceCollection);

        serviceCollection->EmplaceFactory<MeshBuffer, GlMeshBuffer::Factory>(*serviceCollection);

        serviceCollection->EmplaceFactory<ShaderProgram, DummyShaderProgram::Factory>(*serviceCollection);

        serviceCollection->EmplaceFactory<TextureBuffer, DummyTextureBuffer::Factory>(*serviceCollection);

        LogInstaller::Install(nullptr, *serviceCollection);

        serviceCollection->AddService(MemoryManager::Factory(*serviceCollection).Create());

        serviceCollection->EmplaceService<FontCompiler>(fontAssetFactory, materialAssetFactory, textureAssetFactory,
                                                        resourceControlFactory);

        serviceCollection->EmplaceService<MaterialCompiler>(materialAssetFactory, resourceControlFactory);

        serviceCollection->EmplaceService<MeshCompiler>(meshAssetFactory);

        serviceCollection->EmplaceService<ShaderCompiler>(shaderAssetFactory);

        serviceCollection->EmplaceService<TextCompiler>(textAssetFactory);

        serviceCollection->EmplaceService<TextureCompiler>(textureAssetFactory);

        std::vector<std::reference_wrapper<BaseService>> services = serviceCollection->FindServices(
            [](const BaseService& service)
            {
                return dynamic_cast<const BaseCompiler*>(&service) != nullptr;
            });

        std::vector<std::reference_wrapper<BaseCompiler>> compilers;
        for (auto& it : services)
        {
            BaseService& service = it;
            compilers.emplace_back(dynamic_cast<BaseCompiler&>(service));
        }

        const auto& packageCompiler = serviceCollection->EmplaceService<PackageCompiler>(
            packageManifestAssetFactory, compilers);

        std::string outputDir = Path::Combine({input, Path::GetFileName(input)});
        packageCompiler.Compile(input, outputDir);

        serviceCollection->RemoveService<MemoryManager>();
    }
}

int main(int argc, char* argv[])
{
    InitGl();

    try
    {
        std::cout << "Input: ";
        std::string str;
        std::cin >> str;
        pluto::compiler::Compile(str);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
}

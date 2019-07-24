#include "text/text_asset_manager.h"
#include "text/text_asset_menu.h"
#include "mesh/mesh_asset_manager.h"
#include "mesh/mesh_asset_menu.h"
#include "shader/shader_asset_manager.h"
#include "shader/shader_asset_menu.h"
#include "package/package_manager.h"
#include "package/package_menu.h"

#include "base_menu.h"
#include "menu_options.h"

#include <pluto/asset/asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>

#include <pluto/render/gl/gl_mesh_buffer.h>
#include <pluto/render/gl/gl_shader_program.h>

#include <pluto/di/di_container.h>

#include <pluto/file/path.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

#include <iostream>
#include <memory>

namespace pluto
{
    class MainMenu final : public BaseMenu
    {
        std::unique_ptr<DiContainer> diContainer;

        std::unique_ptr<TextAssetManager> textAssetManager;
        std::unique_ptr<MeshAssetManager> meshAssetManager;
        std::unique_ptr<ShaderAssetManager> shaderAssetManager;
        std::unique_ptr<PackageManager> packageManager;

        std::unique_ptr<TextAssetMenu> textAssetMenu;
        std::unique_ptr<MeshAssetMenu> meshAssetMenu;
        std::unique_ptr<ShaderAssetMenu> shaderAssetMenu;
        std::unique_ptr<PackageMenu> packageMenu;

        BaseMenu* currentMenu;
        MenuOptions mainMenu;

    public:
        MainMenu() : mainMenu(MenuOptions("Main Menu"))
        {
            diContainer = std::make_unique<DiContainer>();
            diContainer->AddSingleton(std::make_unique<FileReader::Factory>(*diContainer));
            diContainer->AddSingleton(std::make_unique<FileWriter::Factory>(*diContainer));
            const FileManager::Factory fileManagerFactory(*diContainer);
            FileManager& fileManager = diContainer->AddSingleton(fileManagerFactory.Create(Path("C:/")));

            TextAsset::Factory& textAssetFactory = diContainer->AddSingleton(
                std::make_unique<TextAsset::Factory>(*diContainer));

            diContainer->AddSingleton<MeshBuffer::Factory>(std::make_unique<GlMeshBuffer::Factory>(*diContainer));
            MeshAsset::Factory& meshAssetFactory = diContainer->AddSingleton(
                std::make_unique<MeshAsset::Factory>(*diContainer));

            diContainer->AddSingleton<ShaderProgram::Factory>(std::make_unique<GlShaderProgram::Factory>(*diContainer));
            ShaderAsset::Factory& shaderAssetFactory = diContainer->AddSingleton(
                std::make_unique<ShaderAsset::Factory>(*diContainer));

            PackageManifestAsset::Factory& packageManifestAssetFactory = diContainer->AddSingleton(
                std::make_unique<PackageManifestAsset::Factory>(*diContainer));

            textAssetManager = std::make_unique<TextAssetManager>(fileManager, textAssetFactory);
            meshAssetManager = std::make_unique<MeshAssetManager>(fileManager, meshAssetFactory);
            shaderAssetManager = std::make_unique<ShaderAssetManager>(fileManager, shaderAssetFactory);

            packageManager = std::make_unique<PackageManager>(fileManager, packageManifestAssetFactory,
                                                              *textAssetManager, *meshAssetManager,
                                                              *shaderAssetManager);

            packageMenu = std::make_unique<PackageMenu>(*packageManager, std::bind(&MainMenu::SetMainAsCurrent, this));

            textAssetMenu = std::make_unique<TextAssetMenu>(*textAssetManager,
                                                            std::bind(&MainMenu::SetMainAsCurrent, this));
            meshAssetMenu = std::make_unique<MeshAssetMenu>(*meshAssetManager,
                                                            std::bind(&MainMenu::SetMainAsCurrent, this));
            shaderAssetMenu = std::make_unique<ShaderAssetMenu>(*shaderAssetManager,
                                                                std::bind(&MainMenu::SetMainAsCurrent, this));

            mainMenu.AddOption(0, "Exit", []()
            {
                exit(0);
            });

            mainMenu.AddOption(static_cast<int>(Asset::Type::PackageManifest), "Package",
                               std::bind(&MainMenu::SetPackageAsCurrentContext, this));

            mainMenu.AddOption(static_cast<int>(Asset::Type::Text), "Texts",
                               std::bind(&MainMenu::SetTextAsCurrentContext, this));

            mainMenu.AddOption(static_cast<int>(Asset::Type::Mesh), "Meshes",
                               std::bind(&MainMenu::SetMeshAsCurrentContext, this));

            mainMenu.AddOption(static_cast<int>(Asset::Type::Shader), "Shaders",
                               std::bind(&MainMenu::SetShaderAsCurrentContext, this));

            //mainMenu.AddOption(5, "Textures", std::bind(&MainMenu::SetTextureAsCurrent, this));

            currentMenu = this;
        }

        void SetMainAsCurrent()
        {
            currentMenu = this;
        }

        void SetPackageAsCurrentContext()
        {
            currentMenu = packageMenu.get();
        }

        void SetTextAsCurrentContext()
        {
            currentMenu = textAssetMenu.get();
        }

        void SetMeshAsCurrentContext()
        {
            currentMenu = meshAssetMenu.get();
        }

        void SetShaderAsCurrentContext()
        {
            currentMenu = shaderAssetMenu.get();
        }

        void SetTextureAsCurrentContext()
        {
            //currentMenu = &textureAssetMenu.GetCurrentMenuOptions();
        }

        const MenuOptions& GetCurrentMenuOptions() const override
        {
            if (currentMenu == this)
            {
                return mainMenu;
            }

            return currentMenu->GetCurrentMenuOptions();
        }
    };
}

int main(int argc, char* argv[])
{
    const pluto::MainMenu mainMenu;

    try
    {
        while (true)
        {
            const pluto::MenuOptions& menuOptions = mainMenu.GetCurrentMenuOptions();
            std::cout << menuOptions << std::endl;
            std::cout << "Option: ";
            int option = 0;
            std::cin >> option;
            if (!menuOptions.UseOption(option))
            {
                std::cout << "Invalid Option!" << std::endl;
            }
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

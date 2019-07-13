#include <pluto/root.h>
#include <pluto/di/di_container.h>

#include <pluto/file/file_installer.h>
#include <pluto/log/log_installer.h>
#include <pluto/config/config_installer.h>
#include <pluto/event/event_installer.h>
#include <pluto/window/window_installer.h>
#include <pluto/input/input_installer.h>
#include <pluto/simulation/simulation_installer.h>
#include <pluto/asset/asset_installer.h>
#include <pluto/scene/scene_installer.h>
#include <pluto/render/render_installer.h>

#include <pluto/log/log_manager.h>
#include <pluto/input/input_manager.h>
#include <pluto/input/key_code.h>
#include <pluto/event/event_manager.h>
#include <pluto/event/on_startup_event.h>
#include <pluto/window/window_manager.h>
#include <pluto/simulation/simulation_manager.h>
#include <pluto/scene/scene_manager.h>
#include <pluto/render/render_manager.h>

#include <pluto/asset/asset_manager.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/asset/package_manifest_asset.h>

#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>
#include <pluto/math/matrix4x4.h>
#include <glm/glm.hpp>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>
#include <pluto/scene/components/camera.h>
#include <pluto/scene/components/mesh_renderer.h>

#include <pluto/guid.h>

#include <fmt/format.h>
#include <chrono>
#include <iostream>
#include <regex>
#include <pluto/stop_watch.h>
#include <pluto/exception.h>

namespace pluto
{
    class Root::Impl
    {
    private:
        std::unique_ptr<DiContainer> diContainer;

    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& dataDirectoryName)
        {
            diContainer = std::make_unique<DiContainer>();

            FileInstaller::Install(dataDirectoryName, *diContainer);
            auto& fileManager = diContainer->GetSingleton<FileManager>();

            std::unique_ptr<FileWriter> logFile = fileManager.OpenWrite(logFileName);
            LogInstaller::Install(std::move(logFile), *diContainer);

            std::unique_ptr<FileReader> configFile;
            if (fileManager.Exists(configFileName))
            {
                configFile = fileManager.OpenRead(configFileName);
            }
            ConfigInstaller::Install(configFile.get(), *diContainer);

            EventInstaller::Install(*diContainer);
            WindowInstaller::Install(*diContainer);
            InputInstaller::Install(*diContainer);
            SimulationInstaller::Install(*diContainer);
            AssetInstaller::Install(*diContainer);
            SceneInstaller::Install(*diContainer);
            RenderInstaller::Install(*diContainer);

            auto& logManager = diContainer->GetSingleton<LogManager>();
            logManager.LogInfo("Pluto Engine Initialized!");
        }

        ~Impl()
        {
            RenderInstaller::Uninstall(*diContainer);
            SceneInstaller::Uninstall(*diContainer);
            AssetInstaller::Uninstall(*diContainer);
            SimulationInstaller::Uninstall(*diContainer);
            InputInstaller::Uninstall(*diContainer);
            WindowInstaller::Uninstall(*diContainer);
            EventInstaller::Uninstall(*diContainer);
            ConfigInstaller::Uninstall(*diContainer);
            LogInstaller::Uninstall(*diContainer);
            FileInstaller::Uninstall(*diContainer);
        }

        int Run() const
        {
            auto& windowManager = diContainer->GetSingleton<WindowManager>();
            auto& simulationManager = diContainer->GetSingleton<SimulationManager>();
            auto& logManager = diContainer->GetSingleton<LogManager>();

            {
                // TODO: Move to simulation manager.
                auto& eventManager = diContainer->GetSingleton<EventManager>();
                eventManager.Dispatch(OnStartupEvent());
                auto& assetManager = diContainer->GetSingleton<AssetManager>();

                auto& packageFactory = diContainer->GetSingleton<PackageManifestAsset::Factory>();
                auto package = packageFactory.Create();
                package->SetName("main");
                package->AddAsset("quad", Guid("022d0eeb-165f-4381-bc50-159910308bf2"));
                package->AddAsset("pink", Guid("39d39db2-2968-41cc-9e29-dfa9203cce44"));
                assetManager.Register(std::move(package));

                auto& sceneManager = diContainer->GetSingleton<SceneManager>();
                sceneManager.LoadEmptyScene();

                GameObject& cameraGo = sceneManager.GetActiveScene().CreateGameObject("Camera");
                cameraGo.GetTransform().SetLocalPosition({ 0, 0, -1 });

                auto& camera = cameraGo.AddComponent<Camera>();

                auto& meshAsset = assetManager.Load<MeshAsset>("quad");
                auto& shaderAsset = assetManager.Load<ShaderAsset>("pink");

                auto& materialFactory = diContainer->GetSingleton<MaterialAsset::Factory>();
                auto materialAssetPtr = materialFactory.Create();
                materialAssetPtr->SetName("default");
                materialAssetPtr->SetShader(shaderAsset); 
                auto& materialAsset = assetManager.Register(std::move(materialAssetPtr));

                GameObject& quadGo = sceneManager.GetActiveScene().CreateGameObject("Quad");
                auto& meshRenderer = quadGo.AddComponent<MeshRenderer>();
                meshRenderer.SetMesh(meshAsset);
                meshRenderer.SetMaterial(materialAsset);
            }

            while (windowManager.IsOpen())
            {
                try
                {
                    simulationManager.Run();
                }
                catch (const Exception& e)
                {
                    logManager.LogException(e);
                }
            }
            return 0;
        }
    };

    Root::Root(const std::string& configFileName, const std::string& logFileName,
               const std::string& dataDirectoryName) : impl(
        std::make_unique<Impl>(configFileName, logFileName, dataDirectoryName))
    {
    }

    Root::~Root() = default;

    int Root::Run() const
    {
        return impl->Run();
    }
}

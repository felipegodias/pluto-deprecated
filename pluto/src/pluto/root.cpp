#include <pluto/root.h>
#include <pluto/service/service_collection.h>

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
#include <pluto/asset/texture_asset.h>
#include <pluto/asset/package_manifest_asset.h>

#include <pluto/math/color.h>
#include <pluto/math/vector2f.h>
#include <pluto/math/vector2i.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>
#include <pluto/math/matrix4x4.h>
#include <pluto/math/vector4f.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>

#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/components/transform.h>
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
        std::unique_ptr<ServiceCollection> diContainer;

    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& dataDirectoryName)
        {
            diContainer = std::make_unique<ServiceCollection>();

            FileInstaller::Install(dataDirectoryName, *diContainer);
            auto& fileManager = diContainer->GetService<FileManager>();

            std::unique_ptr<FileWriter> logFile = fileManager.OpenWrite(Path(logFileName));
            LogInstaller::Install(std::move(logFile), *diContainer);

            std::unique_ptr<FileReader> configFile;
            if (fileManager.Exists(Path(configFileName)))
            {
                configFile = fileManager.OpenRead(Path(configFileName));
            }
            ConfigInstaller::Install(configFile.get(), *diContainer);

            EventInstaller::Install(*diContainer);
            WindowInstaller::Install(*diContainer);
            InputInstaller::Install(*diContainer);
            SimulationInstaller::Install(*diContainer);
            AssetInstaller::Install(*diContainer);
            SceneInstaller::Install(*diContainer);
            RenderInstaller::Install(*diContainer);

            auto& logManager = diContainer->GetService<LogManager>();
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
            auto& windowManager = diContainer->GetService<WindowManager>();
            auto& simulationManager = diContainer->GetService<SimulationManager>();
            auto& logManager = diContainer->GetService<LogManager>();
            MaterialAsset* transparentMaterial = nullptr;
            MaterialAsset* unlitMaterial = nullptr;

            {
                // TODO: Move to simulation manager.
                auto& eventManager = diContainer->GetService<EventManager>();
                eventManager.Dispatch(OnStartupEvent());
                auto& assetManager = diContainer->GetService<AssetManager>();

                assetManager.LoadPackage("built-in");

                auto& sceneManager = diContainer->GetService<SceneManager>();
                sceneManager.LoadEmptyScene();

                GameObject& cameraGo = sceneManager.GetActiveScene().CreateGameObject("Camera");
                cameraGo.GetTransform().SetLocalPosition({0, 0, -1});

                auto& camera = cameraGo.AddComponent<Camera>();

                auto meshAsset = assetManager.Load<MeshAsset>(Path("meshes/quad.obj"));
                auto unlitShaderAsset = assetManager.Load<ShaderAsset>(Path("shaders/unlit.glsl"));
                auto transparentShaderAsset = assetManager.Load<ShaderAsset>(Path("shaders/transparent.glsl"));

                auto& materialFactory = diContainer->GetService<MaterialAsset::Factory>();

                auto materialAssetPtr = materialFactory.Create();
                materialAssetPtr->SetName("unlit");
                materialAssetPtr->SetShader(*transparentShaderAsset);
                unlitMaterial = &assetManager.Register(std::move(materialAssetPtr));

                materialAssetPtr = materialFactory.Create();
                materialAssetPtr->SetName("transparent");
                materialAssetPtr->SetShader(*unlitShaderAsset);
                transparentMaterial = &assetManager.Register(std::move(materialAssetPtr));

                GameObject& tquadGo = sceneManager.GetActiveScene().CreateGameObject("Transparent");
                tquadGo.GetTransform().SetPosition({-5, 0, 0});
                auto& tmeshRenderer = tquadGo.AddComponent<MeshRenderer>();
                tmeshRenderer.SetMesh(*meshAsset);
                tmeshRenderer.SetMaterial(*transparentMaterial);

                GameObject& uquadGo = sceneManager.GetActiveScene().CreateGameObject("Unlit");
                uquadGo.GetTransform().SetPosition({5, 0, 0});
                auto& umeshRenderer = uquadGo.AddComponent<MeshRenderer>();
                umeshRenderer.SetMesh(*meshAsset);
                umeshRenderer.SetMaterial(*unlitMaterial);

                auto textureAsset = assetManager.Load<TextureAsset>(Path("textures/pluto-logo.png"));
                transparentMaterial->SetTexture("u_mat.mainTex", *textureAsset);
                unlitMaterial->SetTexture("u_mat.mainTex", *textureAsset);
            }

            int i = 0;
            while (windowManager.IsOpen())
            {
                try
                {
                    simulationManager.Run();
                    ++i;
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
               const std::string& dataDirectoryName)
        : impl(
            std::make_unique<Impl>(configFileName, logFileName, dataDirectoryName))
    {
    }

    Root::~Root() = default;

    int Root::Run() const
    {
        return impl->Run();
    }
}

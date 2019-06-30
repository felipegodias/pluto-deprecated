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

#include <pluto/log/log_manager.h>
#include <pluto/input/input_manager.h>
#include <pluto/input/key_code.h>
#include <pluto/event/event_manager.h>
#include <pluto/event/on_startup_event.h>
#include <pluto/window/window_manager.h>
#include <pluto/simulation/simulation_manager.h>

#include <pluto/asset/asset_manager.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/package_manifest_asset.h>

#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>
#include <pluto/math/vector3int.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

#include <sstream>

#include <fmt/format.h>

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
            LogInstaller::Install(logFileName, *diContainer);
            ConfigInstaller::Install(configFileName, *diContainer);
            EventInstaller::Install(*diContainer);
            WindowInstaller::Install(*diContainer);
            InputInstaller::Install(*diContainer);
            SimulationInstaller::Install(*diContainer);
            AssetInstaller::Install(*diContainer);

            auto& logManager = diContainer->GetSingleton<LogManager>();
            logManager.LogInfo("Pluto Engine Initialized!");

            auto& eventManager = diContainer->GetSingleton<EventManager>();
            eventManager.Dispatch(OnStartupEvent());

            auto& assetManager = diContainer->GetSingleton<AssetManager>();
            assetManager.LoadPackage("main");

            auto& factory = diContainer->GetSingleton<MeshAsset::Factory>();
            std::unique_ptr<MeshAsset> mesh = factory.Create();
            std::vector<Vector3> positions{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}};
            mesh->SetPositions(std::move(positions));

            const std::vector<Vector2> uvs{{12, 13}, {14, 15}, {16, 17}};
            mesh->SetUVs(uvs);

            const std::vector<Vector3Int> triangles{{18, 19, 20}, {21, 22, 23}};
            mesh->SetTriangles(triangles);

            auto& fileManager = diContainer->GetSingleton<FileManager>();
            const auto fileWriter = fileManager.OpenWrite("quad.out");
            mesh->Dump(*fileWriter);

            const auto fileReader = fileManager.OpenRead("quad.out");
            std::unique_ptr<MeshAsset> meshB = factory.Create(*fileReader);
        }

        ~Impl()
        {
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
            while (windowManager.IsOpen())
            {
                simulationManager.Run();
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

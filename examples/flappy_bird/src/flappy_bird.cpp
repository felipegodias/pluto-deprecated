#include <pluto/pluto.h>

#include "components/ground.h"
#include "components/pipe.h"
#include "components/flappy_animation.h"
#include "components/flappy_controller.h"
#include "components/fps_counter.h"
#include "components/point_counter.h"
#include "components/intro.h"
#include "components/game_over.h"
#include "managers/game_manager.h"

#if defined(WIN32) && defined(NDEBUG)
#include <Windows.h>
#endif

using namespace pluto;

void OnSetup(ServiceCollection& serviceCollection)
{
    auto& assetManager = serviceCollection.GetService<AssetManager>();
    assetManager.LoadPackage("flappy_bird");

    serviceCollection.EmplaceFactory<Ground>();
    serviceCollection.EmplaceFactory<Pipe>();
    serviceCollection.EmplaceFactory<FlappyAnimation>();
    serviceCollection.EmplaceFactory<FlappyController>();
    serviceCollection.EmplaceFactory<FPSCounter>();
    serviceCollection.EmplaceFactory<PointCounter>();
    serviceCollection.EmplaceFactory<Intro>();
    serviceCollection.EmplaceFactory<GameOver>();
    serviceCollection.AddService<GameManager>(GameManager::Factory(serviceCollection).Create());
}

void OnTeardown(ServiceCollection& serviceCollection)
{
    serviceCollection.RemoveService<GameManager>();
    serviceCollection.RemoveFactory<GameOver>();
    serviceCollection.RemoveFactory<Intro>();
    serviceCollection.RemoveFactory<PointCounter>();
    serviceCollection.RemoveFactory<FPSCounter>();
    serviceCollection.RemoveFactory<FlappyController>();
    serviceCollection.RemoveFactory<FlappyAnimation>();
    serviceCollection.RemoveFactory<Pipe>();
    serviceCollection.RemoveFactory<Ground>();
}

int main(int argc, char* argv[])
{
#if defined(WIN32) && defined(NDEBUG)
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/flappy_bird");
    const int exitCode = pluto.Run(&OnSetup, &OnTeardown);
    return exitCode;
}

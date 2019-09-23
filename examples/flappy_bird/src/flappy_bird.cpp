#include <pluto/pluto.h>

#include "components/ground.h"
#include "components/pipe.h"
#include "components/flappy_animation.h"
#include "components/flappy_controller.h"
#include "managers/game_manager.h"

using namespace pluto;

void OnSetup(ServiceCollection& serviceCollection)
{
    auto& assetManager = serviceCollection.GetService<AssetManager>();
    assetManager.LoadPackage("flappy_bird");

    serviceCollection.EmplaceFactory<Ground>();
    serviceCollection.EmplaceFactory<Pipe>();
    serviceCollection.EmplaceFactory<FlappyAnimation>();
    serviceCollection.EmplaceFactory<FlappyController>();
    serviceCollection.AddService<GameManager>(GameManager::Factory(serviceCollection).Create());
}

void OnTeardown(ServiceCollection& serviceCollection)
{
    serviceCollection.RemoveService<GameManager>();
    serviceCollection.RemoveFactory<FlappyController>();
    serviceCollection.RemoveFactory<FlappyAnimation>();
    serviceCollection.RemoveFactory<Pipe>();
    serviceCollection.RemoveFactory<Ground>();
}

int main(int argc, char* argv[])
{
    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/flappy_bird");
    const int exitCode = pluto.Run(&OnSetup, &OnTeardown);
    return exitCode;
}

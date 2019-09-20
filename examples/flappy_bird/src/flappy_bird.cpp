#include <pluto/pluto.h>

#include "components/ground.h"
#include "components/flappy_animation.h"
#include "components/flappy_controller.h"
#include "managers/game_manager.h"

using namespace pluto;

void Bind(ServiceCollection& serviceCollection)
{
    serviceCollection.EmplaceFactory<Ground>();
    serviceCollection.EmplaceFactory<FlappyAnimation>();
    serviceCollection.EmplaceFactory<FlappyController>();
    serviceCollection.AddService<GameManager>(GameManager::Factory(serviceCollection).Create());
}

void OnInit(ServiceCollection& serviceCollection)
{
    auto& assetManager = serviceCollection.GetService<AssetManager>();
    assetManager.LoadPackage("flappy_bird");

    Bind(serviceCollection);
}

int main(int argc, char* argv[])
{
    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/flappy_bird");
    const int exitCode = pluto.Run(&OnInit);
    return exitCode;
}

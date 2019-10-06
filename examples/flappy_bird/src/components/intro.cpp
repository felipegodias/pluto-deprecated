#include "intro.h"
#include "../managers/game_manager.h"

using namespace pluto;

Intro::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> Intro::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& gameManager = GetServiceCollection().GetService<GameManager>();
    return std::make_unique<Intro>(gameObject, gameManager);
}

Intro::Intro(const Resource<GameObject>& gameObject, GameManager& gameManager)
    : Behaviour(gameObject),
      gameManager(&gameManager)
{
}

void Intro::OnUpdate()
{
    if (gameManager->IsGameStarted())
    {
        GetGameObject()->SetActive(false);
    }
}

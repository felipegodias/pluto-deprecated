#include "pipe.h"
#include "../managers/game_manager.h"

using namespace pluto;

float GenRandomHeight()
{
    float rand = static_cast<float>(std::rand()) / RAND_MAX;
    return -0.15f + rand * 0.7f;
}

Pipe::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> Pipe::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& simulationManager = GetServiceCollection().GetService<SimulationManager>();
    auto& gameManager = GetServiceCollection().GetService<GameManager>();
    return std::make_unique<Pipe>(gameObject, simulationManager, gameManager);
}

Pipe::Pipe(const Resource<GameObject>& gameObject, SimulationManager& simulationManager, GameManager& gameManager)
    : Behaviour(gameObject),
      simulationManager(&simulationManager),
      gameManager(&gameManager)
{
    Vector3F pos = gameObject->GetTransform()->GetLocalPosition();
    pos.y = GenRandomHeight();
    gameObject->GetTransform()->SetLocalPosition(pos);
}

void Pipe::OnUpdate()
{
    if (gameManager->IsGameOver() || !gameManager->IsGameStarted())
    {
        return;
    }

    Resource<Transform> transform = GetGameObject()->GetTransform();
    Vector3F pos = transform->GetPosition();
    pos.x += -moveSpeed * simulationManager->GetDeltaTime();

    if (pos.x < -0.75f)
    {
        pos.x = 0.75f;
        pos.y = GenRandomHeight();
    }

    transform->SetPosition(pos);
}

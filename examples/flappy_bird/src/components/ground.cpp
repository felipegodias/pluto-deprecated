#include "ground.h"
#include "../managers/game_manager.h"

using namespace pluto;

Ground::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> Ground::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& simulationManager = GetServiceCollection().GetService<SimulationManager>();
    auto& gameManager = GetServiceCollection().GetService<GameManager>();
    return std::make_unique<Ground>(gameObject, simulationManager, gameManager);
}

Ground::Ground(const Resource<GameObject>& gameObject, SimulationManager& simulationManager, GameManager& gameManager)
    : Behaviour(gameObject),
      simulationManager(&simulationManager),
      gameManager(&gameManager)
{
}

void Ground::OnUpdate()
{
    if (!gameManager->IsPlaying())
    {
        return;
    }

    Resource<Transform> transform = GetGameObject()->GetTransform();
    Vector3F pos = transform->GetPosition();
    pos.x += -moveSpeed * simulationManager->GetDeltaTime();

    if (abs(pos.x) > 0.083f)
    {
        pos.x = 0.083f;
    }

    transform->SetPosition(pos);
}

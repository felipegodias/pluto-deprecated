#include "ground.h"

using namespace pluto;

Ground::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> Ground::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& simulationManager = GetServiceCollection().GetService<SimulationManager>();
    return std::make_unique<Ground>(gameObject, simulationManager);
}

Ground::Ground(const Resource<GameObject>& gameObject, SimulationManager& simulationManager)
    : Behaviour(gameObject),
      simulationManager(&simulationManager)
{
}

void Ground::OnUpdate()
{
    Resource<Transform> transform = GetGameObject()->GetTransform();
    Vector3F pos = transform->GetPosition();
    pos.x += -moveSpeed * simulationManager->GetDeltaTime();

    if (abs(pos.x) > 0.083f)
    {
        pos.x = 0.083f;
    }

    transform->SetPosition(pos);
}

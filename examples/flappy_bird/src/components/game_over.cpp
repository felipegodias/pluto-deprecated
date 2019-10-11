#include "game_over.h"
#include "../managers/game_manager.h"

using namespace pluto;

GameOver::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> GameOver::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& simulationManager = GetServiceCollection().GetService<SimulationManager>();
    return std::make_unique<GameOver>(gameObject, simulationManager);
}

GameOver::GameOver(const Resource<GameObject>& gameObject, SimulationManager& simulationManager)
    : Behaviour(gameObject),
      currentPosition(gameObject->GetTransform()->GetPosition()),
      currentColor({1, 1, 1, 0}),
      simulationManager(&simulationManager)
{
    Resource<MeshRenderer> meshRenderer = gameObject->GetComponent<MeshRenderer>();
    Resource<MaterialAsset> material = meshRenderer->GetMaterial();
    material->SetVector4F("u_mat.mainColor", {1, 1, 1, 0.0f});
}

void GameOver::OnUpdate()
{
    Resource<MeshRenderer> meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
    Resource<MaterialAsset> material = meshRenderer->GetMaterial();

    currentColor = Vector4F::Lerp(currentColor, {1, 1, 1, 1}, simulationManager->GetDeltaTime() * 2);
    material->SetVector4F("u_mat.mainColor", currentColor);

    currentPosition = Vector3F::Lerp(currentPosition, {0, 0.5f, 4}, simulationManager->GetDeltaTime() * 2);
    GetGameObject()->GetTransform()->SetPosition(currentPosition);
}

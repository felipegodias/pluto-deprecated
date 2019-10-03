#include "point_counter.h"
#include "../managers/game_manager.h"

#include <sstream>

using namespace pluto;

PointCounter::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> PointCounter::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& gameManager = GetServiceCollection().GetService<GameManager>();
    return std::make_unique<PointCounter>(gameObject, gameManager);
}

PointCounter::PointCounter(const Resource<GameObject>& gameObject, GameManager& gameManager)
    : Behaviour(gameObject),
      gameManager(&gameManager)
{
}

void PointCounter::OnUpdate()
{
    std::vector<Resource<TextRenderer>> textRenderers = GetGameObject()->GetComponentsInChildren<TextRenderer>();

    std::stringstream ss;
    ss << gameManager->GetPoints();
    for (auto& textRenderer : textRenderers)
    {
        textRenderer->SetText(ss.str());
    }
}

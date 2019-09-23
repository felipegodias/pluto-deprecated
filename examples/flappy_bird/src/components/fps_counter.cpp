#include "fps_counter.h"
#include <sstream>

using namespace pluto;

FPSCounter::Factory::Factory(ServiceCollection& serviceCollection)
    : Behaviour::Factory(serviceCollection)
{
}

std::unique_ptr<Component> FPSCounter::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& simulationManager = GetServiceCollection().GetService<SimulationManager>();
    return std::make_unique<FPSCounter>(gameObject, simulationManager);
}

FPSCounter::FPSCounter(const Resource<GameObject>& gameObject, SimulationManager& simulationManager)
    : Behaviour(gameObject),
      fps(0),
      time(0),
      textRenderer(gameObject->GetComponent<TextRenderer>()),
      simulationManager(&simulationManager)
{
}

void FPSCounter::OnUpdate()
{
    ++fps;
    time += simulationManager->GetDeltaTime();
    if (time >= 1)
    {
        std::stringstream ss;
        ss << fps;
        textRenderer->SetText(ss.str());
        fps = 0;
        time = 0;
    }
}

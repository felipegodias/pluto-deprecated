#include "rotator.h"

using namespace pluto;

Rotator::Factory::Factory(ServiceCollection& serviceCollection)
    : Component::Factory(serviceCollection)
{
}

std::unique_ptr<Component> Rotator::Factory::Create(const Resource<GameObject>& gameObject) const
{
    return std::make_unique<Rotator>(gameObject);
}

Rotator::Rotator(const Resource<GameObject>& gameObject)
    : Behaviour(gameObject),
      rotationSpeed(1)
{
}

void Rotator::OnUpdate()
{
    Vector3F euler = GetGameObject()->GetTransform()->GetRotation().GetEulerAngles();
    euler.z += rotationSpeed;
    GetGameObject()->GetTransform()->SetRotation(Quaternion::Euler(euler));
}

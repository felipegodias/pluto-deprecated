#include "rotator.h"
#include <utility>

using namespace pluto;

Rotator::Factory::Factory(ServiceCollection& serviceCollection)
    : Component::Factory(serviceCollection)
{
}

std::unique_ptr<Component> Rotator::Factory::Create(const Resource<GameObject>& gameObject) const
{
    return std::make_unique<Rotator>(gameObject);
}

Rotator::Rotator(Resource<GameObject> gameObject)
    : guid(Guid::New()),
      rotationSpeed(1),
      gameObject(std::move(gameObject))
{
}

const Guid& Rotator::GetId() const
{
    return guid;
}

const std::string& Rotator::GetName() const
{
    return gameObject->GetName();
}

void Rotator::SetName(const std::string& value)
{
    gameObject->SetName(value);
}

Resource<GameObject> Rotator::GetGameObject() const
{
    return gameObject;
}

void Rotator::OnUpdate()
{
    Vector3F euler = gameObject->GetTransform()->GetRotation().GetEulerAngles();
    euler.z += rotationSpeed;
    gameObject->GetTransform()->SetRotation(Quaternion::Euler(euler));
}

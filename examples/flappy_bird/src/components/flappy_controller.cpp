#include "flappy_controller.h"
#include "../managers/game_manager.h"

using namespace pluto;

FlappyController::Factory::Factory(ServiceCollection& serviceCollection)
    : Component::Factory(serviceCollection)
{
}

std::unique_ptr<Component> FlappyController::Factory::Create(const Resource<GameObject>& gameObject) const
{
    auto& inputManager = GetServiceCollection().GetService<InputManager>();
    auto& gameManager = GetServiceCollection().GetService<GameManager>();
    return std::make_unique<FlappyController>(gameObject, inputManager, gameManager);
}

FlappyController::FlappyController(const Resource<GameObject>& gameObject, InputManager& inputManager,
                                   GameManager& gameManager)
    : Behaviour(gameObject),
      currentAngle(0),
      rigidbody(gameObject->GetComponent<Rigidbody2D>()),
      inputManager(&inputManager),
      gameManager(&gameManager)
{
}

void FlappyController::OnUpdate()
{
    if (inputManager->GetKeyDown(KeyCode::MouseButton0))
    {
        rigidbody->SetVelocity(Vector2F::UP * 2);
    }

    Vector2F velocity = Vector2F::ZERO;
    if (Vector2F::Distance(rigidbody->GetVelocity(), Vector2F::ZERO) > Math::EPSILON)
    {
        velocity = rigidbody->GetVelocity().GetNormalized();
    }

    currentAngle = currentAngle + 0.05f * (velocity.y - currentAngle);
    GetGameObject()->GetTransform()->SetRotation(Quaternion::Euler({0, 0, currentAngle * 60}));

    if (inputManager->GetKeyDown(KeyCode::MouseButton1))
    {
        gameManager->Reload();
    }
}
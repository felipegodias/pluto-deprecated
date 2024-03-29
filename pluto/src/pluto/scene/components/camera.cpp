#include <utility>
#include "pluto/scene/components/camera.h"
#include "pluto/scene/components/component.impl.hpp"

#include "pluto/scene/game_object.h"
#include "pluto/scene/components/transform.h"

#include "pluto/memory/resource.h"

#include "pluto/service/service_collection.h"
#include "pluto/window/window_manager.h"
#include "pluto/math/matrix4x4.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/quaternion.h"
#include "pluto/guid.h"

namespace pluto
{
    class Camera::Impl : public Component::Impl
    {
        Type type;
        float orthographicSize;
        float nearPlane;
        float farPlane;

        Matrix4X4 viewMatrix;
        bool isViewMatrixDirty;
        Vector3F lastPosition;
        Quaternion lastRotation;

        Matrix4X4 projectionMatrix;
        bool isProjectionMatrixDirty;

        const WindowManager* windowManager;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject, const WindowManager& windowManager)
            : Component::Impl(guid, gameObject),
              type(Type::Orthographic),
              orthographicSize(5),
              nearPlane(0.1f),
              farPlane(100),
              viewMatrix(Matrix4X4::IDENTITY),
              isViewMatrixDirty(true),
              projectionMatrix(Matrix4X4::IDENTITY),
              isProjectionMatrixDirty(true),
              windowManager(&windowManager)
        {
        }

        Type GetType() const
        {
            return type;
        }

        void SetType(const Type value)
        {
            type = value;
            isProjectionMatrixDirty = true;
        }

        float GetOrthographicSize() const
        {
            return orthographicSize;
        }

        void SetOrthographicSize(const float value)
        {
            orthographicSize = value;
            isProjectionMatrixDirty = true;
        }

        float GetNearPlane() const
        {
            return nearPlane;
        }

        void SetNearPlane(const float value)
        {
            nearPlane = value;
            isProjectionMatrixDirty = true;
        }

        float GetFarPlane() const
        {
            return farPlane;
        }

        void SetFarPlane(const float value)
        {
            farPlane = value;
            isProjectionMatrixDirty = true;
        }

        bool IsVisible(Renderer& renderer)
        {
            return true;
        }

        const Matrix4X4& GetViewMatrix()
        {
            if (isViewMatrixDirty)
            {
                Resource<Transform> transform = GetGameObject()->GetTransform();
                const Vector3F position = transform->GetPosition();
                viewMatrix = Matrix4X4::LookAt(position, position - transform->GetForward(), transform->GetUp());
            }

            return viewMatrix;
        }

        const Matrix4X4& GetProjectionMatrix()
        {
            if (isProjectionMatrixDirty)
            {
                projectionMatrix = type == Type::Orthographic ? CreateOrthographicProjection() : Matrix4X4::IDENTITY;
            }

            return projectionMatrix;
        }

        void OnUpdate()
        {
            Resource<Transform> transform = GetGameObject()->GetTransform();
            if (lastPosition != transform->GetPosition())
            {
                isViewMatrixDirty = true;
                lastPosition = transform->GetPosition();
            }

            if (lastRotation != transform->GetRotation())
            {
                isViewMatrixDirty = true;
                lastRotation = transform->GetRotation();
            }
        }

    private:
        Matrix4X4 CreateOrthographicProjection()
        {
            const float aspectRatio = windowManager->GetWindowAspectRatio();
            const float right = orthographicSize * aspectRatio;
            const float top = orthographicSize;
            return Matrix4X4::Ortho(-right, right, -top, top, nearPlane, farPlane);
        }
    };

    Camera::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> Camera::Factory::Create(const Resource<GameObject>& gameObject) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        const auto& windowManager = serviceCollection.GetService<WindowManager>();
        return std::make_unique<Camera>(std::make_unique<Impl>(Guid::New(), gameObject, windowManager));
    }

    Camera::~Camera() = default;

    Camera::Camera(std::unique_ptr<Impl> impl)
        : Component(*impl),
          impl(std::move(impl))
    {
    }

    Camera::Camera(Camera&& other) noexcept = default;

    Camera& Camera::operator=(Camera&& rhs) noexcept = default;

    Camera::Type Camera::GetType() const
    {
        return impl->GetType();
    }

    void Camera::SetType(const Type value)
    {
        impl->SetType(value);
    }

    float Camera::GetOrthographicSize() const
    {
        return impl->GetNearPlane();
    }

    void Camera::SetOrthographicSize(const float value)
    {
        impl->SetOrthographicSize(value);
    }

    float Camera::GetNearPlane() const
    {
        return impl->GetNearPlane();
    }

    void Camera::SetNearPlane(const float value)
    {
        impl->SetNearPlane(value);
    }

    float Camera::GetFarPlane() const
    {
        return impl->GetFarPlane();
    }

    void Camera::SetFarPlane(const float value)
    {
        impl->SetFarPlane(value);
    }

    bool Camera::IsVisible(Renderer& renderer)
    {
        return impl->IsVisible(renderer);
    }

    const Matrix4X4& Camera::GetViewMatrix()
    {
        return impl->GetViewMatrix();
    }

    const Matrix4X4& Camera::GetProjectionMatrix()
    {
        return impl->GetProjectionMatrix();
    }

    void Camera::OnUpdate()
    {
        impl->OnUpdate();
    }
}

#pragma once

#include "pluto/scene/component.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Renderer;
    class Matrix4X4;

    class PLUTO_API Camera final : public Component
    {
    public:
        enum class Type
        {
            Orthographic = 0
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<Camera> Create(GameObject& gameObject) const;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit Camera(std::unique_ptr<Impl> impl);
        Camera(const Camera& other) = delete;
        Camera(Camera&& other) noexcept;
        ~Camera();

        Camera& operator=(const Camera& rhs) = delete;
        Camera& operator=(Camera&& rhs) noexcept;

        const Guid& GetId() const override;

        GameObject& GetGameObject() const override;

        Type GetType() const;
        void SetType(Type value);

        float GetOrthographicSize() const;
        void SetOrthographicSize(float value);

        float GetNearPlane() const;
        void SetNearPlane(float value);

        float GetFarPlane() const;
        void SetFarPlane(float value);

        bool IsVisible(Renderer& renderer);

        const Matrix4X4& GetViewMatrix();
        const Matrix4X4& GetProjectionMatrix();

        void OnUpdate() override;
    };
}

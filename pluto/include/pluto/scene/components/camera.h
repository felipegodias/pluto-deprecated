#pragma once

#include "component.h"

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

        class PLUTO_API Factory final : public Component::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Component> Create(const Resource<GameObject>& gameObject) const override;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Camera() override;
        explicit Camera(std::unique_ptr<Impl> impl);

        Camera(const Camera& other) = delete;
        Camera(Camera&& other) noexcept;
        Camera& operator=(const Camera& rhs) = delete;
        Camera& operator=(Camera&& rhs) noexcept;

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

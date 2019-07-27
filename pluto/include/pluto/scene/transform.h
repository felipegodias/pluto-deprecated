#pragma once

#include "component.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <vector>

namespace pluto
{
    class Guid;
    class GameObject;
    class Vector3F;
    class Quaternion;
    class Matrix4X4;

    class PLUTO_API Transform final : public Component
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<Transform> Create(GameObject& gameObject) const;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit Transform(std::unique_ptr<Impl> impl);
        Transform(const Transform& other) = delete;
        Transform(Transform&& other) noexcept;
        ~Transform();

        Transform& operator=(const Transform& rhs) = delete;
        Transform& operator=(Transform&& rhs) noexcept;

        const Guid& GetId() const override;

        GameObject& GetGameObject() const override;

        bool IsRoot();
        Transform& GetParent() const;
        void SetParent(Transform& value) const;

        const std::vector<std::reference_wrapper<Transform>>& GetChildren() const;

        const Vector3F& GetLocalPosition() const;
        void SetLocalPosition(Vector3F value);

        const Quaternion& GetLocalRotation() const;
        void SetLocalRotation(Quaternion value);

        const Vector3F& GetLocalScale() const;
        void SetLocalScale(Vector3F value);

        Vector3F GetPosition();
        void SetPosition(const Vector3F& value);

        Quaternion GetRotation();
        void SetRotation(const Quaternion& value);

        Vector3F GetScale();
        void SetScale(const Vector3F& value);

        Vector3F GetUp();
        Vector3F GetDown();
        Vector3F GetRight();
        Vector3F GetLeft();
        Vector3F GetForward();
        Vector3F GetBack();

        const Matrix4X4& GetLocalMatrix();
        const Matrix4X4& GetWorldMatrix();
    };
}

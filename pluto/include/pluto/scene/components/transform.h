#pragma once

#include "component.h"

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
        ~Transform() override;
        explicit Transform(std::unique_ptr<Impl> impl);

        Transform(const Transform& other) = delete;
        Transform(Transform&& other) noexcept;
        Transform& operator=(const Transform& rhs) = delete;
        Transform& operator=(Transform&& rhs) noexcept;

        bool IsRoot();
        Resource<Transform> GetParent() const;
        void SetParent(const Resource<Transform>& value) const;

        const std::vector<Resource<Transform>>& GetChildren() const;

        Resource<Transform> FindChild(const std::string& name) const;

        const Vector3F& GetLocalPosition() const;
        void SetLocalPosition(const Vector3F& value);

        const Quaternion& GetLocalRotation() const;
        void SetLocalRotation(const Quaternion& value);

        const Vector3F& GetLocalScale() const;
        void SetLocalScale(const Vector3F& value);

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

#pragma once

#include "../di/base_factory.h"
#include "../guid.h"

#include <memory>

namespace pluto
{
    class GameObject;
    class Vector3F;
    class Quaternion;
    class Matrix4;

    class PLUTO_API Transform
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<Transform> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        const Guid& GetId() const;

        GameObject& GetGameObject() const;

        const Vector3F& GetLocalPosition() const;
        void SetLocalPosition(Vector3F value);

        const Quaternion& GetLocalRotation() const;
        void SetLocalRotation(Quaternion value);

        const Vector3F& GetLocalScale() const;
        void SetLocalScale(Vector3F value);

        const Vector3F& GetPosition();
        void SetPosition(Vector3F value);

        const Quaternion& GetRotation();
        void SetRotation(Quaternion value);

        const Vector3F& GetScale();
        void SetScale(Vector3F value);

        const Vector3F& GetUp();
        const Vector3F& GetDown();
        const Vector3F& GetRight();
        const Vector3F& GetLeft();
        const Vector3F& GetForward();
        const Vector3F& GetBack();

        const Matrix4& GetLocalMatrix();
        const Matrix4& GetWorldMatrix();
    };
}

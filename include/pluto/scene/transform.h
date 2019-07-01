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

        const Vector3F& GetPosition() const;
        void SetPosition(Vector3F value);

        const Quaternion& GetRotation() const;
        void SetRotation(Quaternion value);

        const Vector3F& GetScale() const;
        void SetScale(Vector3F value);

        const Vector3F& GetUp() const;
        const Vector3F& GetDown() const;
        const Vector3F& GetRight() const;
        const Vector3F& GetLeft() const;
        const Vector3F& GetForward() const;
        const Vector3F& GetBack() const;

        const Matrix4 GetLocalToWorld() const;

        void Update(bool isDirty);
    };
}

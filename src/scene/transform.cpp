#include <pluto/scene/transform.h>
#include <pluto/scene/game_object.h>

#include <pluto/guid.h>

#include <pluto/math/vector3f.h>
#include <pluto/math/vector4f.h>
#include <pluto/math/quaternion.h>
#include <pluto/math/matrix4x4.h>

namespace pluto
{
    class Transform::Impl
    {
    private:
        Guid guid;
        GameObject& gameObject;

        Vector3F localPosition;
        Quaternion localRotation;
        Vector3F localScale;

        bool isLocalMatrixDirty;
        Matrix4X4 localMatrix;

        Matrix4X4 worldMatrix;

    public:
        Impl(Guid guid, GameObject& gameObject) : guid(std::move(guid)), gameObject(gameObject),
                                                  localPosition(Vector3F::ZERO), localRotation(Quaternion::IDENTITY),
                                                  localScale(Vector3F::ONE), isLocalMatrixDirty(false),
                                                  localMatrix(Matrix4X4::IDENTITY)
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        GameObject& GetGameObject() const
        {
            return gameObject;
        }

        const Vector3F& GetLocalPosition() const
        {
            return localPosition;
        }

        void SetLocalPosition(Vector3F value)
        {
            localPosition = std::move(value);
            SetDirty();
        }

        const Quaternion& GetLocalRotation() const
        {
            return localRotation;
        }

        void SetLocalRotation(Quaternion value)
        {
            localRotation = std::move(value);
            SetDirty();
        }

        const Vector3F& GetLocalScale() const
        {
            return localScale;
        }

        void SetLocalScale(Vector3F value)
        {
            localScale = std::move(value);
            SetDirty();
        }

        Vector3F GetPosition()
        {
            if (gameObject.IsRoot())
            {
                return localPosition;
            }

            return GetWorldMatrix() * Vector4F(localPosition);
        }

        void SetPosition(const Vector3F& value)
        {
            localPosition = GetInverseWorldMatrix() * Vector4F(value);
            SetDirty();
        }

        Quaternion GetRotation()
        {
            if (gameObject.IsRoot())
            {
                return localRotation;
            }

            return gameObject.GetParent().GetTransform().GetRotation() * localRotation;
        }

        void SetRotation(const Quaternion& value)
        {
            localRotation = gameObject.GetParent().GetTransform().GetRotation().GetInverse() * value;
            SetDirty();
        }

        Vector3F GetScale()
        {
            if (gameObject.IsRoot())
            {
                return localScale;
            }

            return Vector3F::Scale(gameObject.GetParent().GetTransform().GetScale(), localScale);
        }

        void SetScale(const Vector3F& value)
        {
            Vector3F parentScale = gameObject.GetParent().GetTransform().GetScale();
            parentScale.x = 1 / parentScale.x;
            parentScale.y = 1 / parentScale.y;
            parentScale.z = 1 / parentScale.z;
            localScale = Vector3F::Scale(parentScale, value);
            SetDirty();
        }

        Vector3F GetUp()
        {
            return GetRotation() * Vector3F::UP;
        }

        Vector3F GetDown()
        {
            return GetRotation() * Vector3F::DOWN;
        }

        Vector3F GetRight()
        {
            return GetRotation() * Vector3F::RIGHT;
        }

        Vector3F GetLeft()
        {
            return GetRotation() * Vector3F::LEFT;
        }

        Vector3F GetForward()
        {
            return GetRotation() * Vector3F::FORWARD;
        }

        Vector3F GetBack()
        {
            return GetRotation() * Vector3F::BACK;
        }

        const Matrix4X4& GetLocalMatrix()
        {
            if (isLocalMatrixDirty)
            {
                localMatrix = Matrix4X4::TSR(localPosition, localRotation, localScale);
                isLocalMatrixDirty = false;
            }

            return localMatrix;
        }

        const Matrix4X4& GetWorldMatrix()
        {
            if (IsWorldMatrixDirty())
            {
                worldMatrix = gameObject.GetParent().GetTransform().GetWorldMatrix() * GetLocalMatrix();
            }

            return worldMatrix;
        }

    private:
        void SetDirty()
        {
            isLocalMatrixDirty = true;
        }

        bool IsWorldMatrixDirty() const
        {
            if (isLocalMatrixDirty)
            {
                return true;
            }

            if (gameObject.IsRoot())
            {
                return false;
            }

            return gameObject.GetParent().GetTransform().impl->IsWorldMatrixDirty();
        }

        Matrix4X4 GetInverseWorldMatrix()
        {
            return GetWorldMatrix().GetInverse();
        }
    };

    Transform::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<Transform> Transform::Factory::Create(GameObject& gameObject) const
    {
        return std::make_unique<Transform>(std::make_unique<Impl>(Guid::New(), gameObject));
    }

    Transform::Transform(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    Transform::Transform(Transform&& other) noexcept : impl(std::move(other.impl))
    {
    }

    Transform::~Transform() = default;

    Transform& Transform::operator=(Transform&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    const Guid& Transform::GetId() const
    {
        return impl->GetId();
    }

    GameObject& Transform::GetGameObject() const
    {
        return impl->GetGameObject();
    }

    const Vector3F& Transform::GetLocalPosition() const
    {
        return impl->GetLocalPosition();
    }

    void Transform::SetLocalPosition(Vector3F value)
    {
        impl->SetLocalPosition(std::move(value));
    }

    const Quaternion& Transform::GetLocalRotation() const
    {
        return impl->GetLocalRotation();
    }

    void Transform::SetLocalRotation(Quaternion value)
    {
        impl->SetLocalRotation(std::move(value));
    }

    const Vector3F& Transform::GetLocalScale() const
    {
        return impl->GetLocalScale();
    }

    void Transform::SetLocalScale(Vector3F value)
    {
        impl->SetLocalScale(std::move(value));
    }

    Vector3F Transform::GetPosition()
    {
        return impl->GetPosition();
    }

    void Transform::SetPosition(const Vector3F& value)
    {
        impl->SetPosition(value);
    }

    Quaternion Transform::GetRotation()
    {
        return impl->GetRotation();
    }

    void Transform::SetRotation(const Quaternion& value)
    {
        impl->SetRotation(value);
    }

    Vector3F Transform::GetScale()
    {
        return impl->GetScale();
    }

    void Transform::SetScale(const Vector3F& value)
    {
        impl->SetScale(value);
    }

    Vector3F Transform::GetUp()
    {
        return impl->GetUp();
    }

    Vector3F Transform::GetDown()
    {
        return impl->GetDown();
    }

    Vector3F Transform::GetRight()
    {
        return impl->GetRight();
    }

    Vector3F Transform::GetLeft()
    {
        return impl->GetLeft();
    }

    Vector3F Transform::GetForward()
    {
        return impl->GetForward();
    }

    Vector3F Transform::GetBack()
    {
        return impl->GetBack();
    }

    const Matrix4X4& Transform::GetLocalMatrix()
    {
        return impl->GetWorldMatrix();
    }

    const Matrix4X4& Transform::GetWorldMatrix()
    {
        return impl->GetWorldMatrix();
    }
}

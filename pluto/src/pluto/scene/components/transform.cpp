#include "pluto/scene/components/transform.h"
#include "pluto/scene/components/component.impl.hpp"

#include "pluto/scene/game_object.h"

#include "pluto/memory/resource.h"

#include "pluto/guid.h"
#include "pluto/exception.h"

#include "pluto/math/vector3f.h"
#include "pluto/math/vector4f.h"
#include "pluto/math/quaternion.h"
#include "pluto/math/matrix4x4.h"

namespace pluto
{
    class Transform::Impl : public Component::Impl
    {
        Resource<Transform> parent;
        std::vector<Resource<Transform>> children;

        Vector3F localPosition;
        Quaternion localRotation;
        Vector3F localScale;

        bool isLocalMatrixDirty;
        Matrix4X4 localMatrix;

        bool isWorldMatrixDirty;
        Matrix4X4 worldMatrix;

    public:
        Impl(const Guid& guid, const Resource<GameObject>& gameObject)
            : Component::Impl(guid, gameObject),
              parent(nullptr),
              localPosition(Vector3F::ZERO),
              localRotation(Quaternion::IDENTITY),
              localScale(Vector3F::ONE),
              isLocalMatrixDirty(true),
              localMatrix(Matrix4X4::IDENTITY),
              isWorldMatrixDirty(true),
              worldMatrix(Matrix4X4::IDENTITY)
        {
        }

        bool IsRoot() const
        {
            return parent == nullptr;
        }

        Resource<Transform> GetParent() const
        {
            return parent;
        }

        void SetParent(const Resource<Transform>& value)
        {
            Resource<Transform> me = GetGameObject()->GetTransform();
            if (value->impl->IsMyParent(me))
            {
                Exception::Throw(std::runtime_error("Can not set a transform parent if the same is it's child."));
            }

            if (parent != nullptr)
            {
                parent->impl->RemoveChild(me);
            }
            value->impl->AddChild(me);
            parent = value;
        }

        const std::vector<Resource<Transform>>& GetChildren() const
        {
            return children;
        }

        const Vector3F& GetLocalPosition() const
        {
            return localPosition;
        }

        void SetLocalPosition(const Vector3F& value)
        {
            localPosition = value;
            SetDirty();
        }

        const Quaternion& GetLocalRotation() const
        {
            return localRotation;
        }

        void SetLocalRotation(const Quaternion value)
        {
            localRotation = value;
            SetDirty();
        }

        const Vector3F& GetLocalScale() const
        {
            return localScale;
        }

        void SetLocalScale(const Vector3F value)
        {
            localScale = value;
            SetDirty();
        }

        Vector3F GetPosition()
        {
            if (IsRoot())
            {
                return localPosition;
            }

            return Vector3F(GetParent()->GetWorldMatrix() * Vector4F(localPosition));
        }

        void SetPosition(const Vector3F& value)
        {
            if (IsRoot())
            {
                localPosition = value;
            }
            else
            {
                localPosition = GetParent()->GetWorldMatrix().GetInverse() * Vector4F(value);
            }
            SetDirty();
        }

        Quaternion GetRotation()
        {
            if (IsRoot())
            {
                return localRotation;
            }

            return GetParent()->GetRotation() * localRotation;
        }

        void SetRotation(const Quaternion& value)
        {
            if (IsRoot())
            {
                localRotation = value;
            }
            else
            {
                localRotation = GetParent()->GetRotation().GetInverse() * value;
            }
            SetDirty();
        }

        Vector3F GetScale()
        {
            if (IsRoot())
            {
                return localScale;
            }

            return Vector3F::Scale(GetParent()->GetScale(), localScale);
        }

        void SetScale(const Vector3F& value)
        {
            if (IsRoot())
            {
                localScale = value;
            }
            else
            {
                Vector3F parentScale = parent->GetScale();
                parentScale.x = 1 / parentScale.x;
                parentScale.y = 1 / parentScale.y;
                parentScale.z = 1 / parentScale.z;
                localScale = Vector3F::Scale(parentScale, value);
            }
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
            if (isWorldMatrixDirty)
            {
                worldMatrix = IsRoot() ? GetLocalMatrix() : GetParent()->GetWorldMatrix() * GetLocalMatrix();
                isWorldMatrixDirty = false;
            }
            return worldMatrix;
        }

        bool IsMyParent(const Resource<Transform>& transform)
        {
            if (IsRoot())
            {
                return false;
            }

            return parent == transform || parent->impl->IsMyParent(transform);
        }

        void AddChild(const Resource<Transform>& child)
        {
            children.emplace_back(child);
        }

        void RemoveChild(const Resource<Transform>& child)
        {
            auto it = children.begin();
            while (it != children.end())
            {
                if (*it == child)
                {
                    children.erase(it);
                    return;
                }
                ++it;
            }
        }

        void SetDirty()
        {
            isLocalMatrixDirty = true;
            SetWorldMatrixAsDirty();
        }

        void SetWorldMatrixAsDirty()
        {
            isWorldMatrixDirty = true;
            for (auto& it : children)
            {
                it->impl->SetWorldMatrixAsDirty();
            }
        }
    };

    Transform::Factory::Factory(ServiceCollection& serviceCollection)
        : Component::Factory(serviceCollection)
    {
    }

    std::unique_ptr<Component> Transform::Factory::Create(const Resource<GameObject>& gameObject) const
    {
        return std::make_unique<Transform>(std::make_unique<Impl>(Guid::New(), gameObject));
    }

    Transform::~Transform() = default;

    Transform::Transform(std::unique_ptr<Impl> impl)
        : Component(*impl),
          impl(std::move(impl))
    {
    }

    Transform::Transform(Transform&& other) noexcept = default;

    Transform& Transform::operator=(Transform&& rhs) noexcept = default;

    bool Transform::IsRoot()
    {
        return impl->IsRoot();
    }

    Resource<Transform> Transform::GetParent() const
    {
        return impl->GetParent();
    }

    void Transform::SetParent(const Resource<Transform>& value) const
    {
        return impl->SetParent(value);
    }

    const std::vector<Resource<Transform>>& Transform::GetChildren() const
    {
        return impl->GetChildren();
    }

    const Vector3F& Transform::GetLocalPosition() const
    {
        return impl->GetLocalPosition();
    }

    void Transform::SetLocalPosition(const Vector3F& value)
    {
        impl->SetLocalPosition(value);
    }

    const Quaternion& Transform::GetLocalRotation() const
    {
        return impl->GetLocalRotation();
    }

    void Transform::SetLocalRotation(const Quaternion& value)
    {
        impl->SetLocalRotation(value);
    }

    const Vector3F& Transform::GetLocalScale() const
    {
        return impl->GetLocalScale();
    }

    void Transform::SetLocalScale(const Vector3F& value)
    {
        impl->SetLocalScale(value);
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

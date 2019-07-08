#include <pluto/scene/transform.h>
#include <pluto/scene/game_object.h>

#include <pluto/guid.h>
#include <pluto/exception.h>

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

		Transform* parent;
		std::vector<std::reference_wrapper<Transform>> children;

		Vector3F localPosition;
		Quaternion localRotation;
		Vector3F localScale;

		bool isLocalMatrixDirty;
		Matrix4X4 localMatrix;

		bool isWorldMatrixDirty;
		Matrix4X4 worldMatrix;

	public:
		Impl(Guid guid, GameObject& gameObject) : guid(std::move(guid)), gameObject(gameObject), parent(nullptr),
		                                          localPosition(Vector3F::ZERO), localRotation(Quaternion::IDENTITY),
		                                          localScale(Vector3F::ONE), isLocalMatrixDirty(true),
		                                          localMatrix(Matrix4X4::IDENTITY), isWorldMatrixDirty(true),
		                                          worldMatrix(Matrix4X4::IDENTITY)
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

		bool IsRoot() const
		{
			return parent == nullptr;
		}

		Transform& GetParent() const
		{
			return *parent;
		}

		void SetParent(Transform& value)
		{
			Transform& me = gameObject.GetTransform();
			if (value.impl->IsMyParent(me))
			{
				Exception::Throw(std::runtime_error("Can not set a transform parent if the same is it's child."));
			}

			if (parent != nullptr)
			{
				parent->impl->RemoveChild(me);
			}
			value.impl->AddChild(me);
			parent = &value;
		}

		const std::vector<std::reference_wrapper<Transform>>& GetChildren() const
		{
			return children;
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
			if (IsRoot())
			{
				return localPosition;
			}

			return GetWorldMatrix() * Vector4F(localPosition);
		}

		void SetPosition(const Vector3F& value)
		{
			if (IsRoot())
			{
				localPosition = value;
			}
			else
			{
				localPosition = GetWorldMatrix().GetInverse() * Vector4F(value);
			}
			SetDirty();
		}

		Quaternion GetRotation()
		{
			if (IsRoot())
			{
				return localRotation;
			}

			return GetParent().GetRotation() * localRotation;
		}

		void SetRotation(const Quaternion& value)
		{
			if (IsRoot())
			{
				localRotation = value;
			}
			else
			{
				localRotation = parent->GetRotation().GetInverse() * value;
			}
			SetDirty();
		}

		Vector3F GetScale()
		{
			if (IsRoot())
			{
				return localScale;
			}

			return Vector3F::Scale(GetParent().GetScale(), localScale);
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
				worldMatrix = GetParent().GetWorldMatrix() * GetLocalMatrix();
				isWorldMatrixDirty = false;
			}
			return worldMatrix;
		}

	public:
		bool IsMyParent(Transform& transform)
		{
			if (IsRoot())
			{
				return false;
			}

			return *parent == transform || parent->impl->IsMyParent(transform);
		}

		void AddChild(Transform& child)
		{
			children.emplace_back(child);
		}

		void RemoveChild(Transform& child)
		{
			auto it = children.begin();
			while (it != children.end())
			{
				Transform& current = *it;
				if (current == child)
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
			for (auto it : children)
			{
				Transform& child = it;
				child.impl->SetWorldMatrixAsDirty();
			}
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

	bool Transform::IsRoot()
	{
		return impl->IsRoot();
	}

	Transform& Transform::GetParent() const
	{
		return impl->GetParent();
	}

	void Transform::SetParent(Transform& value) const
	{
		return impl->SetParent(value);
	}

	const std::vector<std::reference_wrapper<Transform>>& Transform::GetChildren() const
	{
		return impl->GetChildren();
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

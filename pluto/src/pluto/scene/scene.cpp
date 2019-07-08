#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>

#include <pluto/guid.h>

#include <pluto/di/di_container.h>

#include <list>

namespace pluto
{
	class Scene::Impl
	{
	private:
		Guid guid;
		std::list<std::unique_ptr<GameObject>> gameObjects;

		const GameObject::Factory& gameObjectFactory;

	public:
		Impl(Guid guid, std::unique_ptr<GameObject> rootGameObject,
		     const GameObject::Factory& gameObjectFactory) : guid(std::move(guid)), gameObjectFactory(gameObjectFactory)
		{
			gameObjects.push_back(std::move(rootGameObject));
		}

		const Guid& GetId() const
		{
			return guid;
		}

		GameObject& GetRootGameObject() const
		{
			return *gameObjects.front();
		}

		GameObject& CreateGameObject(Transform& parent, std::string name)
		{
			std::unique_ptr<GameObject> gameObjectPtr = gameObjectFactory.Create();
			GameObject& gameObject = *gameObjectPtr;
			gameObjects.push_back(std::move(gameObjectPtr));

			gameObject.SetName(std::move(name));
			gameObject.GetTransform().SetParent(parent);
			return gameObject;
		}

		void Destroy()
		{
			gameObjects.front()->Destroy();
		}

		void OnUpdate(const uint32_t currentFrame)
		{
			gameObjects.front()->OnUpdate(currentFrame);
		}

		void OnRender()
		{
			gameObjects.front()->OnRender();
		}

		void OnCleanup()
		{
			auto it = gameObjects.begin();
			while (it != gameObjects.end())
			{
				if ((*it)->IsDestroyed())
				{
					gameObjects.erase(it++);
				}
				else
				{
					++it;
				}
			}
		}
	};

	Scene::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
	{
	}

	std::unique_ptr<Scene> Scene::Factory::Create() const
	{
		const auto& gameObjectFactory = diContainer.GetSingleton<GameObject::Factory>();
		auto rootGameObject = gameObjectFactory.Create();
		rootGameObject->SetName("root");
		return std::make_unique<Scene>(
			std::make_unique<Impl>(Guid::New(), std::move(rootGameObject), gameObjectFactory));
	}

	Scene::Scene(std::unique_ptr<Impl> impl) : impl(std::move(impl))
	{
	}

	Scene::Scene(Scene&& other) noexcept : impl(std::move(other.impl))
	{
	}

	Scene::~Scene() = default;

	Scene& Scene::operator=(Scene&& rhs) noexcept
	{
		if (this == &rhs)
		{
			return *this;
		}

		impl = std::move(rhs.impl);
		return *this;
	}

	const Guid& Scene::GetId() const
	{
		return impl->GetId();
	}

	GameObject& Scene::GetRootGameObject() const
	{
		return impl->GetRootGameObject();
	}

	GameObject& Scene::CreateGameObject()
	{
		return impl->CreateGameObject(GetRootGameObject().GetTransform(), "New Game Object");
	}

	GameObject& Scene::CreateGameObject(std::string name)
	{
		return impl->CreateGameObject(GetRootGameObject().GetTransform(), std::move(name));
	}

	GameObject& Scene::CreateGameObject(Transform& parent)
	{
		return impl->CreateGameObject(parent, "New Game Object");
	}

	GameObject& Scene::CreateGameObject(Transform& parent, std::string name)
	{
		return impl->CreateGameObject(parent, std::move(name));
	}

	void Scene::Destroy()
	{
		impl->Destroy();
	}

	void Scene::OnUpdate(const uint32_t currentFrame)
	{
		impl->OnUpdate(currentFrame);
	}

	void Scene::OnRender()
	{
		impl->OnRender();
	}

	void Scene::OnCleanup()
	{
		impl->OnCleanup();
	}
}
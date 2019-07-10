#include <pluto/scene/scene_installer.h>

#include <pluto/di/di_container.h>

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>
#include <pluto/scene/components/camera.h>

namespace pluto
{
    void SceneInstaller::Install(DiContainer& diContainer)
    {
        diContainer.AddSingleton(std::make_unique<GameObject::Factory>(diContainer));
        diContainer.AddSingleton(std::make_unique<Transform::Factory>(diContainer));
        diContainer.AddSingleton(std::make_unique<Camera::Factory>(diContainer));
        diContainer.AddSingleton(std::make_unique<Scene::Factory>(diContainer));

        const SceneManager::Factory sceneManagerFactory(diContainer);
        diContainer.AddSingleton(sceneManagerFactory.Create());
    }

    void SceneInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<SceneManager>();
        diContainer.RemoveSingleton<Scene::Factory>();
        diContainer.RemoveSingleton<Camera::Factory>();
        diContainer.RemoveSingleton<Transform::Factory>();
        diContainer.RemoveSingleton<GameObject::Factory>();
    }
}

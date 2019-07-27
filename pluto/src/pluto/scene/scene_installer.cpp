#include <pluto/scene/scene_installer.h>

#include <pluto/service/service_collection.h>

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>
#include <pluto/scene/components/camera.h>
#include <pluto/scene/components/mesh_renderer.h>

namespace pluto
{
    void SceneInstaller::Install(ServiceCollection& diContainer)
    {
        diContainer.AddService(std::make_unique<GameObject::Factory>(diContainer));
        diContainer.AddService(std::make_unique<Transform::Factory>(diContainer));
        diContainer.AddService(std::make_unique<Camera::Factory>(diContainer));
        diContainer.AddService(std::make_unique<Scene::Factory>(diContainer));
        diContainer.AddService(std::make_unique<MeshRenderer::Factory>(diContainer));

        const SceneManager::Factory sceneManagerFactory(diContainer);
        diContainer.AddService(sceneManagerFactory.Create());
    }

    void SceneInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<SceneManager>();
        diContainer.RemoveService<MeshRenderer::Factory>();
        diContainer.RemoveService<Scene::Factory>();
        diContainer.RemoveService<Camera::Factory>();
        diContainer.RemoveService<Transform::Factory>();
        diContainer.RemoveService<GameObject::Factory>();
    }
}

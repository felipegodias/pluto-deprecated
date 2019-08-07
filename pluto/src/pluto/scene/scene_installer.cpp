#include <pluto/scene/scene_installer.h>

#include <pluto/service/service_collection.h>

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/components/transform.h>
#include <pluto/scene/components/camera.h>
#include <pluto/scene/components/mesh_renderer.h>

namespace pluto
{
    void SceneInstaller::Install(ServiceCollection& diContainer)
    {
        diContainer.AddFactory<GameObject>(std::make_unique<GameObject::Factory>(diContainer));
        diContainer.AddFactory<Transform>(std::make_unique<Transform::Factory>(diContainer));
        diContainer.AddFactory<Camera>(std::make_unique<Camera::Factory>(diContainer));
        diContainer.AddFactory<Scene>(std::make_unique<Scene::Factory>(diContainer));
        diContainer.AddFactory<MeshRenderer>(std::make_unique<MeshRenderer::Factory>(diContainer));
        diContainer.AddService(SceneManager::Factory(diContainer).Create());
    }

    void SceneInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<SceneManager>();
        diContainer.RemoveFactory<MeshRenderer>();
        diContainer.RemoveFactory<Scene>();
        diContainer.RemoveFactory<Camera>();
        diContainer.RemoveFactory<Transform>();
        diContainer.RemoveFactory<GameObject>();
    }
}

#include <pluto/scene/scene_installer.h>

#include <pluto/service/service_collection.h>

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/components/transform.h>
#include <pluto/scene/components/camera.h>
#include <pluto/scene/components/mesh_renderer.h>
#include <pluto/scene/components/text_renderer.h>

namespace pluto
{
    void SceneInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.AddFactory<GameObject>(std::make_unique<GameObject::Factory>(serviceCollection));
        serviceCollection.AddFactory<Transform>(std::make_unique<Transform::Factory>(serviceCollection));
        serviceCollection.AddFactory<Camera>(std::make_unique<Camera::Factory>(serviceCollection));
        serviceCollection.AddFactory<Scene>(std::make_unique<Scene::Factory>(serviceCollection));
        serviceCollection.AddFactory<MeshRenderer>(std::make_unique<MeshRenderer::Factory>(serviceCollection));
        serviceCollection.EmplaceFactory<TextRenderer, TextRenderer::Factory>(serviceCollection);
        serviceCollection.AddService(SceneManager::Factory(serviceCollection).Create());
    }

    void SceneInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<SceneManager>();
        serviceCollection.RemoveFactory<TextRenderer>();
        serviceCollection.RemoveFactory<MeshRenderer>();
        serviceCollection.RemoveFactory<Scene>();
        serviceCollection.RemoveFactory<Camera>();
        serviceCollection.RemoveFactory<Transform>();
        serviceCollection.RemoveFactory<GameObject>();
    }
}

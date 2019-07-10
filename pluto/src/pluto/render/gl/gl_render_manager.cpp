#include "pluto/render/gl/gl_render_manager.h"
#include "pluto/render/events/on_render_event.h"

#include "pluto/log/log_manager.h"
#include "pluto/event/event_manager.h"

#include "pluto/scene/scene_manager.h"
#include "pluto/scene/scene.h"
#include "pluto/scene/game_object.h"
#include "pluto/scene/components/renderer.h"

#include "pluto/di/di_container.h"
#include "pluto/guid.h"

namespace pluto
{
    class GlRenderManager::Impl
    {
    private:
        LogManager& logManager;
        EventManager& eventManager;
        const SceneManager& sceneManager;

        Guid onRenderListenerId;

    public:
        Impl(LogManager& logManager, EventManager& eventManager, const SceneManager& sceneManager) :
            logManager(logManager), eventManager(eventManager), sceneManager(sceneManager)
        {
            onRenderListenerId = eventManager.Subscribe<OnRenderEvent>(
                std::bind(&Impl::OnRender, this, std::placeholders::_1));

            logManager.LogInfo("OpenGL RenderManager initialized!");
        }

        ~Impl()
        {
            eventManager.Unsubscribe<OnRenderEvent>(onRenderListenerId);

            logManager.LogInfo("OpenGL RenderManager terminated!");
        }

        void OnRender(const OnRenderEvent& evt)
        {
            const Scene& activeScene = sceneManager.GetActiveScene();
            const GameObject& rootGameObject = activeScene.GetRootGameObject();
            
            std::vector<std::reference_wrapper<Renderer>> renderers = rootGameObject.GetComponents<Renderer>();
            for (auto& it : renderers)
            {
                Renderer& renderer = it;
                // Check renderer bounds against camera frustum.
                Draw(renderer.GetTransform(), renderer.GetMesh(), renderer.GetMaterial());
            }
        }

        void Draw(Transform& transform, MeshAsset& mesh, MaterialAsset& material)
        {
        }
    };

    GlRenderManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<GlRenderManager> GlRenderManager::Factory::Create() const
    {
        auto& logManager = diContainer.GetSingleton<LogManager>();
        auto& eventManager = diContainer.GetSingleton<EventManager>();
        auto& sceneManager = diContainer.GetSingleton<SceneManager>();
        return std::make_unique<GlRenderManager>(std::make_unique<Impl>(logManager, eventManager, sceneManager));
    }

    GlRenderManager::GlRenderManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    GlRenderManager::GlRenderManager(GlRenderManager&& other) noexcept : impl(std::move(other.impl))
    {
    }

    GlRenderManager::~GlRenderManager() = default;

    GlRenderManager& GlRenderManager::operator=(GlRenderManager&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }
}

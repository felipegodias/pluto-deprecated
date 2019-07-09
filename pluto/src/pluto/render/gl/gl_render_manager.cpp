#include "pluto/render/gl/gl_render_manager.h"
#include "pluto/render/events/on_render_event.h"
#include "pluto/render/events/on_render_camera_event.h"
#include "pluto/log/log_manager.h"
#include "pluto/event/event_manager.h"
#include "pluto/di/di_container.h"
#include "pluto/guid.h"

#include <queue>

namespace pluto
{
    class GlRenderManager::Impl
    {
    private:
        struct DrawData
        {
            Transform& transform;
            MeshAsset& mesh;
            MaterialAsset& material;
        };

        std::queue<DrawData> dataToDraw;

        LogManager& logManager;
        EventManager& eventManager;

        Guid onRenderListenerId;

    public:
        explicit Impl(LogManager& logManager, EventManager& eventManager) : logManager(logManager),
                                                                            eventManager(eventManager)
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

        void PushToRender(Transform& transform, MeshAsset& mesh, MaterialAsset& material)
        {
            dataToDraw.push({transform, mesh, material});
        }

        void OnRender(const OnRenderEvent& evt)
        {
            // TODO: Evaluate for each camera in scene.
            eventManager.Dispatch(OnRenderCameraEvent());
            while (!dataToDraw.empty())
            {
                DrawData current = dataToDraw.front();
                // TODO: Draw
                dataToDraw.pop();
            }
        }
    };

    GlRenderManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<GlRenderManager> GlRenderManager::Factory::Create() const
    {
        auto& logManager = diContainer.GetSingleton<LogManager>();
        auto& eventManager = diContainer.GetSingleton<EventManager>();
        return std::make_unique<GlRenderManager>(std::make_unique<Impl>(logManager, eventManager));
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

    void GlRenderManager::PushToRender(Transform& transform, MeshAsset& mesh, MaterialAsset& material)
    {
        impl->PushToRender(transform, mesh, material);
    }
}

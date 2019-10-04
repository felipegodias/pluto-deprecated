#include "pluto/render/gl/gl_render_manager.h"
#include "pluto/render/gl/gl_mesh_buffer.h"
#include "pluto/render/gl/gl_shader_program.h"
#include "pluto/render/gl/gl_call.h"
#include "pluto/render/events/on_render_event.h"

#include "pluto/log/log_manager.h"
#include "pluto/event/event_manager.h"
#include "pluto/window/window_manager.h"

#include "pluto/memory/resource.h"

#include "pluto/asset/mesh_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/asset/shader_asset.h"

#include "pluto/scene/scene_manager.h"
#include "pluto/scene/scene.h"
#include "pluto/scene/game_object.h"
#include "pluto/scene/components/transform.h"
#include "pluto/scene/components/renderer.h"
#include "pluto/scene/components/camera.h"

#include "pluto/math/math.h"
#include "pluto/math/color.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/matrix4x4.h"

#include "pluto/service/service_collection.h"
#include "pluto/guid.h"

#include <GL/glew.h>
#include <Box2D/Box2D.h>
#include <utility>

namespace pluto
{
    class Gizmo
    {
    public:
        virtual ~Gizmo() = 0
        {
        }

        virtual void Draw(const Matrix4X4& projection, const Matrix4X4& view) = 0
        {
        }
    };

    class CircleGizmo final : public Gizmo
    {
        Vector2F position;
        float radius;
        Color color;

    public:
        ~CircleGizmo() override = default;

        CircleGizmo(const Vector2F& position, const float radius, const Color& color)
            : position(position),
              radius(radius),
              color(color)
        {
        }

        void Draw(const Matrix4X4& projection, const Matrix4X4& view) override
        {
            const Matrix4X4 mvp = projection * view * Matrix4X4::Translate({position.x, position.y, 0});

            glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

            glBegin(GL_LINE_LOOP);
            for (float a = 0; a < Math::Radians(360); a += Math::Radians(15))
            {
                Vector2F p = mvp.MultiplyPoint(Vector2F(sinf(a) * radius, cosf(a) * radius));
                glVertex2f(p.x, p.y);
            }
            glEnd();
        }
    };

    class PolygonGizmo final : public Gizmo
    {
        std::vector<Vector2F> points;
        Color color;

    public:
        ~PolygonGizmo() override = default;

        PolygonGizmo(std::vector<Vector2F> vector2Fs, const Color& color)
            : points(std::move(vector2Fs)),
              color(color)
        {
        }

        void Draw(const Matrix4X4& projection, const Matrix4X4& view) override
        {
            glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
            glBegin(GL_LINE_LOOP);
            for (size_t i = 0; i < points.size(); ++i)
            {
                const Matrix4X4 from = projection * view * Matrix4X4::Translate({points[i].x, points[i].y, 0});
                const Vector2F a = from.MultiplyPoint(Vector2F::ZERO);
                glVertex2f(a.x, a.y);
            }
            glEnd();
        }
    };

    class LineGizmo : public Gizmo
    {
        Vector2F from;
        Vector2F to;
        Color color;

    public:
        ~LineGizmo() override = default;

        LineGizmo(const Vector2F& from, const Vector2F& to, const Color& color)
            : from(from),
              to(to),
              color(color)
        {
        }

        void Draw(const Matrix4X4& projection, const Matrix4X4& view) override
        {
        }
    };

    class GlRenderManager::Impl
    {
        Guid onRenderEventListenerId;
        std::vector<std::unique_ptr<Gizmo>> gizmosToDraw;

        LogManager* logManager;
        EventManager* eventManager;
        SceneManager* sceneManager;
        WindowManager* windowManager;

    public:
        ~Impl()
        {
            eventManager->Unsubscribe<OnRenderEvent>(onRenderEventListenerId);
            logManager->LogInfo("OpenGL RenderManager terminated!");
        }

        Impl(LogManager& logManager, EventManager& eventManager, SceneManager& sceneManager,
             WindowManager& windowManager)
            : logManager(&logManager),
              eventManager(&eventManager),
              sceneManager(&sceneManager),
              windowManager(&windowManager)
        {
            glewInit();
            glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
            glEnable(GL_MULTISAMPLE);
            onRenderEventListenerId = eventManager.Subscribe<OnRenderEvent>(
                std::bind(&Impl::OnRender, this, std::placeholders::_1));
            logManager.LogInfo("OpenGL RenderManager initialized!");
        }

        Impl(const Impl& other) = delete;
        Impl(Impl&& other) noexcept = default;
        Impl& operator=(const Impl& rhs) = delete;
        Impl& operator=(Impl&& rhs) noexcept = default;

        void DrawCircleGizmo(const Vector2F& position, float radius, const Color& color)
        {
            gizmosToDraw.push_back(std::make_unique<CircleGizmo>(position, radius, color));
        }

        void DrawPolygonGizmo(const std::vector<Vector2F>& points, const Color& color)
        {
            gizmosToDraw.push_back(std::make_unique<PolygonGizmo>(points, color));
        }

        void DrawLineGizmo(const Vector2F& from, const Vector2F& to, const Color& color)
        {
        }

        void OnRender(const OnRenderEvent& evt)
        {
            GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            const Scene& activeScene = sceneManager->GetActiveScene();
            const Resource<GameObject> rootGameObject = activeScene.GetRootGameObject();

            auto camera = rootGameObject->GetComponentInChildren<Camera>();
            if (camera == nullptr)
            {
                return;
            }

            std::vector<Resource<Renderer>> renderers = rootGameObject->GetComponentsInChildren<Renderer>();

            // TODO: Optimize this... Use a heap or other approach.
            auto compare = [&](Resource<Renderer>& lhs, Resource<Renderer>& rhs) -> bool
            {
                float d1 = (lhs->GetGameObject()->GetTransform()->GetPosition() - camera->GetGameObject()->GetTransform()->GetPosition()).z;
                float d2 = (rhs->GetGameObject()->GetTransform()->GetPosition() - camera->GetGameObject()->GetTransform()->GetPosition()).z;
                return d1 < d2;
            };

            std::sort(renderers.begin(), renderers.end(), compare);

            for (auto& it : renderers)
            {
                Renderer& renderer = *it.Get();
                if (!camera->IsVisible(renderer))
                {
                    continue;
                }

                Draw(*camera.Get(), *renderer.GetGameObject()->GetTransform().Get(), *renderer.GetMesh().Get(),
                     *renderer.GetMaterial().Get());
            }

            gizmosToDraw.clear();

            /*
            GL_CALL(glUseProgram(0));
            GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

            for (auto it = gizmosToDraw.begin(); it != gizmosToDraw.end(); ++it)
            {
                it->get()->Draw(camera->GetProjectionMatrix(), camera->GetViewMatrix());
            }
            gizmosToDraw.clear();

            glColor3f(0.2f, 0.2f, 0.2f);
            glBegin(GL_LINES);
            glVertex2f(-1, 0);
            glVertex2f(1, 0);
            glVertex2f(0, -1);
            glVertex2f(0, 1);
            glEnd();
            */

            windowManager->SwapBuffers();
        }

        void DrawQuadGizmo(const Matrix4X4& mvp, const Vector2F& offset, const Vector2F& size, const Color& color)
        {
            glColor3f(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
            glBegin(GL_LINE_LOOP);
            const Vector2F a = mvp.MultiplyPoint(offset - size / 2);
            const Vector2F b = mvp.MultiplyPoint({offset.x + size.x / 2, offset.y - size.y / 2});
            const Vector2F c = mvp.MultiplyPoint({offset.x - size.x / 2, offset.y + size.y / 2});
            const Vector2F d = mvp.MultiplyPoint(offset + size / 2);
            glVertex2f(a.x, a.y);
            glVertex2f(b.x, b.y);
            glVertex2f(d.x, d.y);
            glVertex2f(c.x, c.y);
            glEnd();
        }

        static void Draw(Camera& camera, Transform& transform, MeshAsset& meshAsset, MaterialAsset& materialAsset)
        {
            const Matrix4X4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * transform.GetWorldMatrix();

            auto& meshBuffer = dynamic_cast<GlMeshBuffer&>(meshAsset.GetMeshBuffer());

            Resource<ShaderAsset> shaderAsset = materialAsset.GetShader();
            auto& shaderProgram = dynamic_cast<GlShaderProgram&>(shaderAsset->GetShaderProgram());
            shaderProgram.Bind(mvp, materialAsset);

            meshBuffer.Bind();
            meshBuffer.Draw();
            meshBuffer.Unbind();

            shaderProgram.Unbind();
        }
    };

    GlRenderManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<GlRenderManager> GlRenderManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetService<LogManager>();
        auto& eventManager = serviceCollection.GetService<EventManager>();
        auto& sceneManager = serviceCollection.GetService<SceneManager>();
        auto& windowManager = serviceCollection.GetService<WindowManager>();
        return std::make_unique<GlRenderManager>(
            std::make_unique<Impl>(logManager, eventManager, sceneManager, windowManager));
    }

    GlRenderManager::GlRenderManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    GlRenderManager::GlRenderManager(GlRenderManager&& other) noexcept
        : impl(std::move(other.impl))
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

    void GlRenderManager::DrawCircleGizmo(const Vector2F& position, const float radius, const Color& color)
    {
        impl->DrawCircleGizmo(position, radius, color);
    }

    void GlRenderManager::DrawPolygonGizmo(const std::vector<Vector2F>& points, const Color& color)
    {
        impl->DrawPolygonGizmo(points, color);
    }

    void GlRenderManager::DrawLineGizmo(const Vector2F& from, const Vector2F& to, const Color& color)
    {
        impl->DrawLineGizmo(from, to, color);
    }
}

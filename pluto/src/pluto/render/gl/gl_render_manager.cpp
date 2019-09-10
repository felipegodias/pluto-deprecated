#include "pluto/render/gl/gl_render_manager.h"
#include "pluto/render/gl/gl_mesh_buffer.h"
#include "pluto/render/gl/gl_shader_program.h"
#include "pluto/render/gl/gl_call.h"

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
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/vector3i.h"
#include "pluto/math/matrix4x4.h"

#include "pluto/physics_2d/components/circle_collider_2d.h"

#include "pluto/service/service_collection.h"

#include <GL/glew.h>

namespace pluto
{
    class GlRenderManager::Impl
    {
        LogManager* logManager;
        SceneManager* sceneManager;
        WindowManager* windowManager;

    public:
        Impl(LogManager& logManager, SceneManager& sceneManager, WindowManager& windowManager)
            : logManager(&logManager),
              sceneManager(&sceneManager),
              windowManager(&windowManager)
        {
            glewInit();
            glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
            logManager.LogInfo("OpenGL RenderManager initialized!");
        }

        ~Impl()
        {
            logManager->LogInfo("OpenGL RenderManager terminated!");
        }

        void MainLoop()
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
                float d1 = Vector3F::Distance(lhs->GetGameObject()->GetTransform()->GetPosition(),
                                              camera->GetGameObject()->GetTransform()->GetPosition());
                float d2 = Vector3F::Distance(rhs->GetGameObject()->GetTransform()->GetPosition(),
                                              camera->GetGameObject()->GetTransform()->GetPosition());
                return d1 > d2;
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

            GL_CALL(glUseProgram(0));
            GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

            glColor3f(0, 1, 0);
            std::vector<Resource<Collider2D>> colliders = rootGameObject->GetComponentsInChildren<Collider2D>();
            for (auto& it : colliders)
            {
                DrawCollider(*camera.Get(), *it.Get());
            }

            glColor3f(1, 1, 1);
            glBegin(GL_LINES);
            glVertex2f(-1, 0);
            glVertex2f(1, 0);
            glVertex2f(0, -1);
            glVertex2f(0, 1);
            glEnd();

            windowManager->SwapBuffers();
        }

        void DrawCollider(Camera& camera, Collider2D& collider)
        {
            Transform* t = collider.GetGameObject()->GetTransform().Get();
            const Matrix4X4 mvp = camera.GetProjectionMatrix() * camera.GetViewMatrix() * t->GetWorldMatrix();

            auto* circle = dynamic_cast<CircleCollider2D*>(&collider);

            glBegin(GL_LINE_LOOP);
            for (float a = 0; a < Math::Radians(360); a += Math::Radians(15))
            {
                Vector2F p = mvp.MultiplyPoint({ sinf(a) * circle->GetRadius(), cosf(a) * circle->GetRadius() });
                glVertex2f(p.x, p.y);
            }
            glEnd();

            glBegin(GL_LINES);
            Vector2F center = mvp.MultiplyPoint(Vector2F::ZERO);
            Vector2F right = mvp.MultiplyPoint(Vector2F::RIGHT * circle->GetRadius());
            glVertex2f(center.x, center.y);
            glVertex2f(right.x, right.y);
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
        auto& sceneManager = serviceCollection.GetService<SceneManager>();
        auto& windowManager = serviceCollection.GetService<WindowManager>();
        return std::make_unique<GlRenderManager>(std::make_unique<Impl>(logManager, sceneManager, windowManager));
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

    void GlRenderManager::MainLoop()
    {
        impl->MainLoop();
    }
}

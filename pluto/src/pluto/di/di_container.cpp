#include <pluto/di/di_container.h>
#include <pluto/di/singleton.h>

#include <pluto/log/log_manager.h>

#include <pluto/config/config_manager.h>

#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

#include <pluto/event/event_manager.h>

#include <pluto/window/window_manager.h>

#include <pluto/input/input_manager.h>

#include <pluto/simulation/simulation_manager.h>

#include <pluto/asset/asset_manager.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/asset/texture_asset.h>

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>
#include <pluto/scene/components/camera.h>
#include <pluto/scene/components/mesh_renderer.h>

#include <pluto/render/render_manager.h>
#include <pluto/render/mesh_buffer.h>
#include <pluto/render/shader_program.h>
#include <pluto/render/texture_buffer.h>

#include "pluto/exception.h"

#include <fmt/format.h>

#include <unordered_map>
#include <typeindex>

namespace pluto
{
    class DiContainer::Impl
    {
        std::unordered_map<std::type_index, std::unique_ptr<Singleton>> singletons;

    public:
        template <typename T, IsSingleton<T>  = 0>
        T& AddSingleton(std::unique_ptr<T> instance)
        {
            const auto it = singletons.find(typeid(T));
            if (it != singletons.end())
            {
                Exception::Throw(
                    std::runtime_error(fmt::format("Singleton instance of type {0} already exists in the container.",
                                                   typeid(T).name())));
            }

            T* ptr = instance.get();
            singletons.emplace(typeid(T), std::move(instance));
            return *ptr;
        }

        template <typename T, IsSingleton<T>  = 0>
        void RemoveSingleton()
        {
            singletons.erase(typeid(T));
        }

        template <typename T, IsSingleton<T>  = 0>
        T& GetSingleton() const
        {
            const auto it = singletons.find(typeid(T));
            if (it == singletons.end())
            {
                Exception::Throw(
                    std::runtime_error(fmt::format("Could not resolve singleton for type {0}", typeid(T).name())));
            }

            return static_cast<T&>(*it->second);
        }
    };

    DiContainer::DiContainer() : impl(std::make_unique<Impl>())
    {
    }

    DiContainer::~DiContainer() = default;

    template <typename T, IsSingleton<T>>
    T& DiContainer::AddSingleton(std::unique_ptr<T> instance)
    {
        return impl->AddSingleton(std::move(instance));
    }

    template <typename T, IsSingleton<T>>
    void DiContainer::RemoveSingleton()
    {
        impl->RemoveSingleton<T>();
    }

    template <typename T, IsSingleton<T>>
    T& DiContainer::GetSingleton() const
    {
        return impl->GetSingleton<T>();
    }

    template PLUTO_API LogManager& DiContainer::AddSingleton(std::unique_ptr<LogManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<LogManager>();
    template PLUTO_API LogManager& DiContainer::GetSingleton() const;

    template PLUTO_API ConfigManager& DiContainer::AddSingleton(std::unique_ptr<ConfigManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<ConfigManager>();
    template PLUTO_API ConfigManager& DiContainer::GetSingleton() const;

    template PLUTO_API FileManager& DiContainer::AddSingleton(std::unique_ptr<FileManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<FileManager>();
    template PLUTO_API FileManager& DiContainer::GetSingleton() const;

    template PLUTO_API FileReader::Factory& DiContainer::AddSingleton(std::unique_ptr<FileReader::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<FileReader::Factory>();
    template PLUTO_API FileReader::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API FileWriter::Factory& DiContainer::AddSingleton(std::unique_ptr<FileWriter::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<FileWriter::Factory>();
    template PLUTO_API FileWriter::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API EventManager& DiContainer::AddSingleton(std::unique_ptr<EventManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<EventManager>();
    template PLUTO_API EventManager& DiContainer::GetSingleton() const;

    template PLUTO_API WindowManager& DiContainer::AddSingleton(std::unique_ptr<WindowManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<WindowManager>();
    template PLUTO_API WindowManager& DiContainer::GetSingleton() const;

    template PLUTO_API InputManager& DiContainer::AddSingleton(std::unique_ptr<InputManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<InputManager>();
    template PLUTO_API InputManager& DiContainer::GetSingleton() const;

    template PLUTO_API SimulationManager& DiContainer::AddSingleton(std::unique_ptr<SimulationManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<SimulationManager>();
    template PLUTO_API SimulationManager& DiContainer::GetSingleton() const;

    template PLUTO_API AssetManager& DiContainer::AddSingleton(std::unique_ptr<AssetManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<AssetManager>();
    template PLUTO_API AssetManager& DiContainer::GetSingleton() const;

    template PLUTO_API PackageManifestAsset::Factory& DiContainer::AddSingleton(
        std::unique_ptr<PackageManifestAsset::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<PackageManifestAsset::Factory>();
    template PLUTO_API PackageManifestAsset::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API TextAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<TextAsset::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<TextAsset::Factory>();
    template PLUTO_API TextAsset::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API MeshAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<MeshAsset::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<MeshAsset::Factory>();
    template PLUTO_API MeshAsset::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API ShaderAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<ShaderAsset::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<ShaderAsset::Factory>();
    template PLUTO_API ShaderAsset::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API MaterialAsset::Factory& DiContainer::AddSingleton(
        std::unique_ptr<MaterialAsset::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<MaterialAsset::Factory>();
    template PLUTO_API MaterialAsset::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API TextureAsset::Factory& DiContainer::AddSingleton(
        std::unique_ptr<TextureAsset::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<TextureAsset::Factory>();
    template PLUTO_API TextureAsset::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API SceneManager& DiContainer::AddSingleton(std::unique_ptr<SceneManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<SceneManager>();
    template PLUTO_API SceneManager& DiContainer::GetSingleton() const;

    template PLUTO_API Scene::Factory& DiContainer::AddSingleton(std::unique_ptr<Scene::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<Scene::Factory>();
    template PLUTO_API Scene::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API GameObject::Factory& DiContainer::AddSingleton(std::unique_ptr<GameObject::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<GameObject::Factory>();
    template PLUTO_API GameObject::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API Transform::Factory& DiContainer::AddSingleton(std::unique_ptr<Transform::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<Transform::Factory>();
    template PLUTO_API Transform::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API Camera::Factory& DiContainer::AddSingleton(std::unique_ptr<Camera::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<Camera::Factory>();
    template PLUTO_API Camera::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API MeshRenderer::Factory& DiContainer::
    AddSingleton(std::unique_ptr<MeshRenderer::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<MeshRenderer::Factory>();
    template PLUTO_API MeshRenderer::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API RenderManager& DiContainer::AddSingleton(std::unique_ptr<RenderManager> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<RenderManager>();
    template PLUTO_API RenderManager& DiContainer::GetSingleton() const;

    template PLUTO_API MeshBuffer::Factory& DiContainer::AddSingleton(std::unique_ptr<MeshBuffer::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<MeshBuffer::Factory>();
    template PLUTO_API MeshBuffer::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API ShaderProgram::Factory& DiContainer::AddSingleton(
        std::unique_ptr<ShaderProgram::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<ShaderProgram::Factory>();
    template PLUTO_API ShaderProgram::Factory& DiContainer::GetSingleton() const;

    template PLUTO_API TextureBuffer::Factory& DiContainer::AddSingleton(
        std::unique_ptr<TextureBuffer::Factory> instance);
    template PLUTO_API void DiContainer::RemoveSingleton<TextureBuffer::Factory>();
    template PLUTO_API TextureBuffer::Factory& DiContainer::GetSingleton() const;
}

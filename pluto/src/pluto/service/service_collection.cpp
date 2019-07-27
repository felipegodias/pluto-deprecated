#include <pluto/service/service_collection.h>
#include <pluto/service/base_service.h>

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
    class ServiceCollection::Impl
    {
        std::unordered_map<std::type_index, std::unique_ptr<BaseService>> singletons;

    public:
        template <typename T, IsService<T>  = 0>
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

        template <typename T, IsService<T>  = 0>
        void RemoveSingleton()
        {
            singletons.erase(typeid(T));
        }

        template <typename T, IsService<T>  = 0>
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

    ServiceCollection::ServiceCollection() : impl(std::make_unique<Impl>())
    {
    }

    ServiceCollection::~ServiceCollection() = default;

    template <typename T, IsService<T>>
    T& ServiceCollection::AddSingleton(std::unique_ptr<T> instance)
    {
        return impl->AddSingleton(std::move(instance));
    }

    template <typename T, IsService<T>>
    void ServiceCollection::RemoveSingleton()
    {
        impl->RemoveSingleton<T>();
    }

    template <typename T, IsService<T>>
    T& ServiceCollection::GetSingleton() const
    {
        return impl->GetSingleton<T>();
    }

    template PLUTO_API LogManager& ServiceCollection::AddSingleton(std::unique_ptr<LogManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<LogManager>();
    template PLUTO_API LogManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API ConfigManager& ServiceCollection::AddSingleton(std::unique_ptr<ConfigManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<ConfigManager>();
    template PLUTO_API ConfigManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API FileManager& ServiceCollection::AddSingleton(std::unique_ptr<FileManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<FileManager>();
    template PLUTO_API FileManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API FileReader::Factory& ServiceCollection::AddSingleton(std::unique_ptr<FileReader::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<FileReader::Factory>();
    template PLUTO_API FileReader::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API FileWriter::Factory& ServiceCollection::AddSingleton(std::unique_ptr<FileWriter::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<FileWriter::Factory>();
    template PLUTO_API FileWriter::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API EventManager& ServiceCollection::AddSingleton(std::unique_ptr<EventManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<EventManager>();
    template PLUTO_API EventManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API WindowManager& ServiceCollection::AddSingleton(std::unique_ptr<WindowManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<WindowManager>();
    template PLUTO_API WindowManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API InputManager& ServiceCollection::AddSingleton(std::unique_ptr<InputManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<InputManager>();
    template PLUTO_API InputManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API SimulationManager& ServiceCollection::AddSingleton(std::unique_ptr<SimulationManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<SimulationManager>();
    template PLUTO_API SimulationManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API AssetManager& ServiceCollection::AddSingleton(std::unique_ptr<AssetManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<AssetManager>();
    template PLUTO_API AssetManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API PackageManifestAsset::Factory& ServiceCollection::AddSingleton(
        std::unique_ptr<PackageManifestAsset::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<PackageManifestAsset::Factory>();
    template PLUTO_API PackageManifestAsset::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API TextAsset::Factory& ServiceCollection::AddSingleton(std::unique_ptr<TextAsset::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<TextAsset::Factory>();
    template PLUTO_API TextAsset::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API MeshAsset::Factory& ServiceCollection::AddSingleton(std::unique_ptr<MeshAsset::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<MeshAsset::Factory>();
    template PLUTO_API MeshAsset::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API ShaderAsset::Factory& ServiceCollection::AddSingleton(std::unique_ptr<ShaderAsset::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<ShaderAsset::Factory>();
    template PLUTO_API ShaderAsset::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API MaterialAsset::Factory& ServiceCollection::AddSingleton(
        std::unique_ptr<MaterialAsset::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<MaterialAsset::Factory>();
    template PLUTO_API MaterialAsset::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API TextureAsset::Factory& ServiceCollection::AddSingleton(
        std::unique_ptr<TextureAsset::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<TextureAsset::Factory>();
    template PLUTO_API TextureAsset::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API SceneManager& ServiceCollection::AddSingleton(std::unique_ptr<SceneManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<SceneManager>();
    template PLUTO_API SceneManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API Scene::Factory& ServiceCollection::AddSingleton(std::unique_ptr<Scene::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<Scene::Factory>();
    template PLUTO_API Scene::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API GameObject::Factory& ServiceCollection::AddSingleton(std::unique_ptr<GameObject::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<GameObject::Factory>();
    template PLUTO_API GameObject::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API Transform::Factory& ServiceCollection::AddSingleton(std::unique_ptr<Transform::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<Transform::Factory>();
    template PLUTO_API Transform::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API Camera::Factory& ServiceCollection::AddSingleton(std::unique_ptr<Camera::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<Camera::Factory>();
    template PLUTO_API Camera::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API MeshRenderer::Factory& ServiceCollection::
    AddSingleton(std::unique_ptr<MeshRenderer::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<MeshRenderer::Factory>();
    template PLUTO_API MeshRenderer::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API RenderManager& ServiceCollection::AddSingleton(std::unique_ptr<RenderManager> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<RenderManager>();
    template PLUTO_API RenderManager& ServiceCollection::GetSingleton() const;

    template PLUTO_API MeshBuffer::Factory& ServiceCollection::AddSingleton(std::unique_ptr<MeshBuffer::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<MeshBuffer::Factory>();
    template PLUTO_API MeshBuffer::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API ShaderProgram::Factory& ServiceCollection::AddSingleton(
        std::unique_ptr<ShaderProgram::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<ShaderProgram::Factory>();
    template PLUTO_API ShaderProgram::Factory& ServiceCollection::GetSingleton() const;

    template PLUTO_API TextureBuffer::Factory& ServiceCollection::AddSingleton(
        std::unique_ptr<TextureBuffer::Factory> instance);
    template PLUTO_API void ServiceCollection::RemoveSingleton<TextureBuffer::Factory>();
    template PLUTO_API TextureBuffer::Factory& ServiceCollection::GetSingleton() const;
}

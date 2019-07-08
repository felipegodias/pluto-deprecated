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

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/transform.h>

#include <unordered_map>
#include <typeindex>

namespace pluto
{
	class DiContainer::Impl
	{
	private:
		std::unordered_map<std::type_index, std::unique_ptr<Singleton>> singletons;

	public:
		template <typename T, IsSingleton<T>  = 0>
		T& AddSingleton(std::unique_ptr<T> instance)
		{
			T* ptr = instance.get();
			singletons[typeid(T)] = std::move(instance);
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
			Singleton& singleton = *singletons.at(typeid(T));
			return static_cast<T&>(singleton);
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

	template LogManager& DiContainer::AddSingleton(std::unique_ptr<LogManager> instance);
	template void DiContainer::RemoveSingleton<LogManager>();
	template LogManager& DiContainer::GetSingleton() const;

	template ConfigManager& DiContainer::AddSingleton(std::unique_ptr<ConfigManager> instance);
	template void DiContainer::RemoveSingleton<ConfigManager>();
	template ConfigManager& DiContainer::GetSingleton() const;

	template FileManager& DiContainer::AddSingleton(std::unique_ptr<FileManager> instance);
	template void DiContainer::RemoveSingleton<FileManager>();
	template FileManager& DiContainer::GetSingleton() const;

	template FileReader::Factory& DiContainer::AddSingleton(std::unique_ptr<FileReader::Factory> instance);
	template void DiContainer::RemoveSingleton<FileReader::Factory>();
	template FileReader::Factory& DiContainer::GetSingleton() const;

	template FileWriter::Factory& DiContainer::AddSingleton(std::unique_ptr<FileWriter::Factory> instance);
	template void DiContainer::RemoveSingleton<FileWriter::Factory>();
	template FileWriter::Factory& DiContainer::GetSingleton() const;

	template EventManager& DiContainer::AddSingleton(std::unique_ptr<EventManager> instance);
	template void DiContainer::RemoveSingleton<EventManager>();
	template EventManager& DiContainer::GetSingleton() const;

	template WindowManager& DiContainer::AddSingleton(std::unique_ptr<WindowManager> instance);
	template void DiContainer::RemoveSingleton<WindowManager>();
	template WindowManager& DiContainer::GetSingleton() const;

	template InputManager& DiContainer::AddSingleton(std::unique_ptr<InputManager> instance);
	template void DiContainer::RemoveSingleton<InputManager>();
	template InputManager& DiContainer::GetSingleton() const;

	template SimulationManager& DiContainer::AddSingleton(std::unique_ptr<SimulationManager> instance);
	template void DiContainer::RemoveSingleton<SimulationManager>();
	template SimulationManager& DiContainer::GetSingleton() const;

	template AssetManager& DiContainer::AddSingleton(std::unique_ptr<AssetManager> instance);
	template void DiContainer::RemoveSingleton<AssetManager>();
	template AssetManager& DiContainer::GetSingleton() const;

	template PackageManifestAsset::Factory& DiContainer::AddSingleton(
		std::unique_ptr<PackageManifestAsset::Factory> instance);
	template void DiContainer::RemoveSingleton<PackageManifestAsset::Factory>();
	template PackageManifestAsset::Factory& DiContainer::GetSingleton() const;

	template TextAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<TextAsset::Factory> instance);
	template void DiContainer::RemoveSingleton<TextAsset::Factory>();
	template TextAsset::Factory& DiContainer::GetSingleton() const;

	template MeshAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<MeshAsset::Factory> instance);
	template void DiContainer::RemoveSingleton<MeshAsset::Factory>();
	template MeshAsset::Factory& DiContainer::GetSingleton() const;

	template ShaderAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<ShaderAsset::Factory> instance);
	template void DiContainer::RemoveSingleton<ShaderAsset::Factory>();
	template ShaderAsset::Factory& DiContainer::GetSingleton() const;

	template MaterialAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<MaterialAsset::Factory> instance);
	template void DiContainer::RemoveSingleton<MaterialAsset::Factory>();
	template MaterialAsset::Factory& DiContainer::GetSingleton() const;

	template SceneManager& DiContainer::AddSingleton(std::unique_ptr<SceneManager> instance);
	template void DiContainer::RemoveSingleton<SceneManager>();
	template SceneManager& DiContainer::GetSingleton() const;

	template Scene::Factory& DiContainer::AddSingleton(std::unique_ptr<Scene::Factory> instance);
	template void DiContainer::RemoveSingleton<Scene::Factory>();
	template Scene::Factory& DiContainer::GetSingleton() const;

	template GameObject::Factory& DiContainer::AddSingleton(std::unique_ptr<GameObject::Factory> instance);
	template void DiContainer::RemoveSingleton<GameObject::Factory>();
	template GameObject::Factory& DiContainer::GetSingleton() const;

	template Transform::Factory& DiContainer::AddSingleton(std::unique_ptr<Transform::Factory> instance);
	template void DiContainer::RemoveSingleton<Transform::Factory>();
	template Transform::Factory& DiContainer::GetSingleton() const;
}

#include <pluto/di/di_container.h>
#include <pluto/di/singleton.h>
#include <pluto/log/log_manager.h>
#include <pluto/config/config_manager.h>
#include <pluto/file/file_manager.h>
#include <pluto/event/event_manager.h>
#include <pluto/window/window_manager.h>
#include <pluto/input/input_manager.h>
#include <pluto/simulation/simulation_manager.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>

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

    template TextAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<TextAsset::Factory> instance);
    template void DiContainer::RemoveSingleton<TextAsset::Factory>();
    template TextAsset::Factory& DiContainer::GetSingleton() const;

    template MeshAsset::Factory& DiContainer::AddSingleton(std::unique_ptr<MeshAsset::Factory> instance);
    template void DiContainer::RemoveSingleton<MeshAsset::Factory>();
    template MeshAsset::Factory& DiContainer::GetSingleton() const;
}

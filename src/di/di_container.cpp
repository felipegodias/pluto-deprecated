#include <pluto/di/di_container.h>
#include <pluto/di/singleton.h>
#include <pluto/log/log_manager.h>
#include <pluto/config/config_manager.h>
#include <pluto/file/file_manager.h>

#include <unordered_map>
#include <typeindex>

namespace pluto
{
    class DiContainer::Impl
    {
    private:
        std::unordered_map<std::type_index, std::unique_ptr<Singleton>> singletons;

    public:
        template <typename T>
        T& AddSingleton(std::unique_ptr<T> instance)
        {
            T* ptr = instance.get();
            singletons.emplace(typeid(T), std::move(instance));
            return *ptr;
        }

        template <typename T>
        void RemoveSingleton()
        {
            singletons.erase(typeid(T));
        }

        template <typename T>
        T& Resolve() const
        {
            Singleton& singleton = *singletons.at(typeid(T));
            return static_cast<T&>(singleton);
        }
    };

    DiContainer::DiContainer() : impl(std::make_unique<Impl>())
    {
    }

    DiContainer::~DiContainer() = default;

    template <typename T>
    T& DiContainer::AddSingleton(std::unique_ptr<T> instance)
    {
        return impl->AddSingleton(std::move(instance));
    }

    template <typename T>
    void DiContainer::RemoveSingleton()
    {
        impl->RemoveSingleton<T>();
    }

    template <typename T>
    T& DiContainer::Resolve() const
    {
        return impl->Resolve<T>();
    }

    template LogManager& DiContainer::AddSingleton(std::unique_ptr<LogManager> instance);
    template void DiContainer::RemoveSingleton<LogManager>();
    template LogManager& DiContainer::Resolve() const;

    template ConfigManager& DiContainer::AddSingleton(std::unique_ptr<ConfigManager> instance);
    template void DiContainer::RemoveSingleton<ConfigManager>();
    template ConfigManager& DiContainer::Resolve() const;

    template FileManager& DiContainer::AddSingleton(std::unique_ptr<FileManager> instance);
    template void DiContainer::RemoveSingleton<FileManager>();
    template FileManager& DiContainer::Resolve() const;
}

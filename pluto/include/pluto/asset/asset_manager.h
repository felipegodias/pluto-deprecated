#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"
#include "pluto/memory/resource.h"

#include <typeindex>
#include <memory>
#include <string>
#include <vector>

namespace pluto
{
    class Asset;
    class Guid;
    class Path;

    template <typename T>
    using IsAsset = std::enable_if_t<std::is_base_of_v<Asset, T>, int>;

    class PLUTO_API AssetManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<AssetManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~AssetManager();

        explicit AssetManager(std::unique_ptr<Impl> impl);

        AssetManager(const AssetManager& other) = delete;
        AssetManager(AssetManager&& other) noexcept;
        AssetManager& operator=(const AssetManager& rhs) = delete;
        AssetManager& operator=(AssetManager&& rhs) noexcept;

        void LoadPackage(const std::string& name);

        template <typename T, IsAsset<T>  = 0>
        Resource<T> Load(const Path& path);

        Resource<Asset, 0> Load(const std::type_index& type, const Path& path);

        template <typename T, IsAsset<T>  = 0>
        Resource<T> Load(const Guid& guid);

        Resource<Asset, 0> Load(const std::type_index& type, const Guid& guid);

        void Unload(const Asset& asset);

        template <typename T, IsAsset<T>  = 0>
        Resource<T> Register(std::unique_ptr<T> asset);

        Resource<Asset, 0> RegisterAsset(std::unique_ptr<Asset> asset);
    };

    template <typename T, IsAsset<T>>
    Resource<T> AssetManager::Load(const Path& path)
    {
        return ResourceUtils::Cast<T>(Load(typeid(T), path));
    }

    template <typename T, IsAsset<T>>
    Resource<T> AssetManager::Load(const Guid& guid)
    {
        return ResourceUtils::Cast<T>(Load(typeid(T), guid));
    }

    template <typename T, IsAsset<T>>
    Resource<T> AssetManager::Register(std::unique_ptr<T> asset)
    {
        return ResourceUtils::Cast<T>(RegisterAsset(std::move(asset)));
    }
}

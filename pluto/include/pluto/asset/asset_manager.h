#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <string>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class Asset;
    class Guid;
    class Path;

    class PLUTO_API AssetManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
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

        template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>  = false>
        Resource<T> Load(const std::string& path);
        template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>  = false>
        Resource<T> Load(const Guid& guid);
        template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>  = false>
        Resource<T> Register(std::unique_ptr<T> asset);

        void LoadPackage(const std::string& name);
        Resource<Asset> Load(const std::type_info& type, const std::string& path);
        Resource<Asset> Load(const std::type_info& type, const Guid& guid);
        void Unload(const Asset& asset);
        Resource<Asset> RegisterAsset(std::unique_ptr<Asset> asset);
    };
}

#include "asset_manager.inl"

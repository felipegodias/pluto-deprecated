#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
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

    class PLUTO_API AssetManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<AssetManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit AssetManager(std::unique_ptr<Impl> impl);
        ~AssetManager();

        void LoadPackage(const std::string& name);

        template <typename T, IsAsset<T>  = 0>
        T& Load(const Path& path);

        template <typename T, IsAsset<T>  = 0>
        T& Load(const Guid& guid);

        template <typename T, IsAsset<T>  = 0>
        void Unload(const T& asset);

        template <typename T, IsAsset<T>  = 0>
        T& Register(std::unique_ptr<T> asset);

        std::vector<Asset*> GetLoadedAssets() const;
    };
}

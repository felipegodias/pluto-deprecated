#pragma once

#include "pluto/memory/resource.h"

namespace pluto
{
    template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>>
    Resource<T> AssetManager::Load(const Path& path)
    {
        return ResourceUtils::Cast<T>(Load(typeid(T), path));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>>
    Resource<T> AssetManager::Load(const Guid& guid)
    {
        return ResourceUtils::Cast<T>(Load(typeid(T), guid));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>>
    Resource<T> AssetManager::Register(std::unique_ptr<T> asset)
    {
        return ResourceUtils::Cast<T>(RegisterAsset(std::move(asset)));
    }
}

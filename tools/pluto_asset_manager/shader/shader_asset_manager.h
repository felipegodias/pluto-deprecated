#pragma once

#include <memory>
#include <pluto/di/di_container.h>

namespace pluto
{
    class ShaderAsset;
    class Path;

    class ShaderAssetManager
    {
        std::unique_ptr<DiContainer> diContainer;

    public:
        ~ShaderAssetManager();
        ShaderAssetManager();

        ShaderAssetManager(const ShaderAssetManager& other) = delete;
        ShaderAssetManager(ShaderAssetManager&& other) noexcept = delete;
        ShaderAssetManager& operator=(const ShaderAssetManager& rhs) = delete;
        ShaderAssetManager& operator=(ShaderAssetManager&& rhs) noexcept = delete;

        std::unique_ptr<ShaderAsset> Create(const Path& path);
        std::unique_ptr<ShaderAsset> Load(const Path& path);
    };
}

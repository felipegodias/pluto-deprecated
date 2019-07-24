#pragma once

#include <pluto/asset/shader_asset.h>

#include <memory>

namespace pluto
{
    class FileManager;
    class Path;

    class ShaderAssetManager
    {
        FileManager* fileManager;
        ShaderAsset::Factory* shaderAssetFactory;

    public:
        ~ShaderAssetManager();
        ShaderAssetManager(FileManager& fileManager, ShaderAsset::Factory& shaderAssetFactory);

        ShaderAssetManager(const ShaderAssetManager& other) = delete;
        ShaderAssetManager(ShaderAssetManager&& other) noexcept = delete;
        ShaderAssetManager& operator=(const ShaderAssetManager& rhs) = delete;
        ShaderAssetManager& operator=(ShaderAssetManager&& rhs) noexcept = delete;

        std::unique_ptr<ShaderAsset> Create(const Path& path);
        std::unique_ptr<ShaderAsset> Load(const Path& path);
    };
}

#pragma once

#include <pluto/asset/texture_asset.h>

#include <memory>

namespace pluto
{
    class FileManager;
    class Path;

    class TextureAssetManager
    {
        FileManager* fileManager;
        TextureAsset::Factory* textureAssetFactory;

    public:
        TextureAssetManager(FileManager& fileManager, TextureAsset::Factory& textureAssetFactory);
        std::unique_ptr<TextureAsset> Create(const Path& inputPath, const Path& outputDir);
    };
}

#pragma once

#include <pluto/asset/text_asset.h>

#include <memory>

namespace pluto
{
    class FileManager;
    class Path;

    class TextAssetManager
    {
        FileManager* fileManager;
        TextAsset::Factory* textAssetFactory;

    public:
        ~TextAssetManager();
        TextAssetManager(FileManager& fileManager, TextAsset::Factory& textAssetFactory);

        TextAssetManager(const TextAssetManager& other) = delete;
        TextAssetManager(TextAssetManager&& other) noexcept = delete;
        TextAssetManager& operator=(const TextAssetManager& rhs) = delete;
        TextAssetManager& operator=(TextAssetManager&& rhs) noexcept = delete;

        std::unique_ptr<TextAsset> Create(const Path& path);
        std::unique_ptr<TextAsset> Load(const Path& path);
    };
}

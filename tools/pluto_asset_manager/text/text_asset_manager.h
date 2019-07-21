#pragma once

#include <memory>
#include <pluto/di/di_container.h>

namespace pluto
{
    class TextAsset;
    class Path;

    class TextAssetManager
    {
        std::unique_ptr<DiContainer> diContainer;

    public:
        ~TextAssetManager();
        TextAssetManager();

        TextAssetManager(const TextAssetManager& other) = delete;
        TextAssetManager(TextAssetManager&& other) noexcept = delete;
        TextAssetManager& operator=(const TextAssetManager& rhs) = delete;
        TextAssetManager& operator=(TextAssetManager&& rhs) noexcept = delete;

        std::unique_ptr<TextAsset> Create(const Path& path);
        std::unique_ptr<TextAsset> Load(const Path& path);
    };
}

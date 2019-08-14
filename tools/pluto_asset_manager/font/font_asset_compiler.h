#pragma once

#include "pluto/asset/font_asset.h"
#include <memory>

namespace pluto
{
    class FileManager;
    class Path;

    class FontAssetCompiler
    {
        FileManager* fileManager;
        FontAsset::Factory* fontAssetFactory;

    public:
        ~FontAssetCompiler();
        explicit FontAssetCompiler(FileManager& fileManager);

        FontAssetCompiler(const FontAssetCompiler& other) = delete;
        FontAssetCompiler(FontAssetCompiler&& other) noexcept;
        FontAssetCompiler& operator=(const FontAssetCompiler& rhs) = delete;
        FontAssetCompiler& operator=(FontAssetCompiler&& rhs) noexcept;

        std::unique_ptr<FontAsset> Compile(const Path& path);
    };
}

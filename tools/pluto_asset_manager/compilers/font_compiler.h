#pragma once

#include "../base_compiler.h"
#include "pluto/asset/font_asset.h"

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class FontCompiler final : public BaseCompiler
    {
        FileManager* fileManager;
        FontAsset::Factory* fontAssetFactory;

    public:
        FontCompiler(FileManager& fileManager, FontAsset::Factory& fontAssetFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

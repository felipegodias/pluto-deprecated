#pragma once

#include "../base_compiler.h"
#include <pluto/asset/texture_asset.h>

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class TextureCompiler final : public BaseCompiler
    {
        FileManager* fileManager;
        TextureAsset::Factory* textureAssetFactory;

    public:
        TextureCompiler(FileManager& fileManager, TextureAsset::Factory& textureAssetFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

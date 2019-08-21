#pragma once

#include "../base_compiler.h"
#include <pluto/asset/font_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/asset/texture_asset.h>
#include <pluto/memory/resource_control.h>

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class FontCompiler final : public BaseCompiler
    {
        FontAsset::Factory* fontAssetFactory;
        MaterialAsset::Factory* materialAssetFactory;
        TextureAsset::Factory* textureAssetFactory;
        ResourceControl::Factory* resourceControlFactory;

    public:
        FontCompiler(FontAsset::Factory& fontAssetFactory, MaterialAsset::Factory& materialAssetFactory,
                     TextureAsset::Factory& textureAssetFactory, ResourceControl::Factory& resourceControlFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

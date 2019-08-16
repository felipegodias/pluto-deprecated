#pragma once

#include "../base_compiler.h"
#include "pluto/asset/material_asset.h"
#include "pluto/memory/resource_control.h"

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class MaterialCompiler final : public BaseCompiler
    {
        FileManager* fileManager;
        MaterialAsset::Factory* materialAssetFactory;
        ResourceControl::Factory* resourceControlFactory;

    public:
        MaterialCompiler(FileManager& fileManager, MaterialAsset::Factory& materialAssetFactory,
                         ResourceControl::Factory& resourceControlFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

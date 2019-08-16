#pragma once

#include "../base_compiler.h"
#include <pluto/asset/shader_asset.h>

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class ShaderCompiler final : public BaseCompiler
    {
        FileManager* fileManager;
        ShaderAsset::Factory* shaderAssetFactory;

    public:
        ShaderCompiler(FileManager& fileManager, ShaderAsset::Factory& shaderAssetFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

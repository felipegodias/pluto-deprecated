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
        ShaderAsset::Factory* shaderAssetFactory;

    public:
        explicit ShaderCompiler(ShaderAsset::Factory& shaderAssetFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

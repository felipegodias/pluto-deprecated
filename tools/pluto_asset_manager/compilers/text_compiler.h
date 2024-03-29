#pragma once

#include "../base_compiler.h"
#include <pluto/asset/text_asset.h>

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class TextCompiler final : public BaseCompiler
    {
        TextAsset::Factory* textAssetFactory;

    public:
        explicit TextCompiler(TextAsset::Factory& textAssetFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

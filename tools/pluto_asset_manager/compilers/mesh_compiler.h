#pragma once

#include "../base_compiler.h"
#include <pluto/asset/mesh_asset.h>

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class MeshCompiler final : public BaseCompiler
    {
        FileManager* fileManager;
        MeshAsset::Factory* meshAssetFactory;

    public:
        MeshCompiler(FileManager& fileManager, MeshAsset::Factory& meshAssetFactory);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

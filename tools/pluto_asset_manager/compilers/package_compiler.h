#pragma once

#include "../base_compiler.h"
#include <pluto/asset/package_manifest_asset.h>
#include <unordered_map>

namespace pluto
{
    class FileManager;
}

namespace pluto::compiler
{
    class PackageCompiler final : public BaseCompiler
    {
        FileManager* fileManager;
        PackageManifestAsset::Factory* packageManifestAssetFactory;
        std::unordered_map<std::string, BaseCompiler*> compilers;

    public:

        PackageCompiler(FileManager& fileManager, PackageManifestAsset::Factory& packageManifestAssetFactory,
                        const std::vector<std::reference_wrapper<BaseCompiler>>& compilers);

        std::vector<std::string> GetExtensions() const override;
        std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const override;
    };
}

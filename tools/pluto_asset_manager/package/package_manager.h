#pragma once

#include <pluto/asset/package_manifest_asset.h>

namespace pluto
{
    class Path;
    class FileManager;
    class TextAssetManager;
    class MeshAssetManager;
    class ShaderAssetManager;
    class TextureAssetManager;

    class PackageManager
    {
        FileManager* fileManager;
        PackageManifestAsset::Factory* packageManifestAssetFactory;
        TextAssetManager* textAssetManager;
        MeshAssetManager* meshAssetManager;
        ShaderAssetManager* shaderAssetManager;
        TextureAssetManager* textureAssetManager;

    public:
        PackageManager(FileManager& fileManager, PackageManifestAsset::Factory& packageManifestAssetFactory,
                       TextAssetManager& textAssetManager, MeshAssetManager& meshAssetManager,
                       ShaderAssetManager& shaderAssetManager, TextureAssetManager& textureAssetManager);

        void Create(const Path& path);
    };
}

#pragma once

#include <pluto/asset/mesh_asset.h>

#include <memory>

namespace pluto
{
    class FileManager;

    class Path;

    class MeshAssetManager
    {
        FileManager* fileManager;
        MeshAsset::Factory* meshAssetFactory;

    public:
        ~MeshAssetManager();
        MeshAssetManager(FileManager& fileManager, MeshAsset::Factory& meshAssetFactory);

        MeshAssetManager(const MeshAssetManager& other) = delete;
        MeshAssetManager(MeshAssetManager&& other) noexcept = delete;
        MeshAssetManager& operator=(const MeshAssetManager& rhs) = delete;
        MeshAssetManager& operator=(MeshAssetManager&& rhs) noexcept = delete;

        std::unique_ptr<MeshAsset> Create(const Path& path);
        std::unique_ptr<MeshAsset> Load(const Path& path);
    };
}

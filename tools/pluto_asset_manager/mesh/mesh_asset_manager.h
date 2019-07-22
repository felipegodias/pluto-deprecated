#pragma once

#include <memory>
#include <pluto/di/di_container.h>

namespace pluto
{
    class MeshAsset;
    class Path;

    class MeshAssetManager
    {
        std::unique_ptr<DiContainer> diContainer;

    public:
        ~MeshAssetManager();
        MeshAssetManager();

        MeshAssetManager(const MeshAssetManager& other) = delete;
        MeshAssetManager(MeshAssetManager&& other) noexcept = delete;
        MeshAssetManager& operator=(const MeshAssetManager& rhs) = delete;
        MeshAssetManager& operator=(MeshAssetManager&& rhs) noexcept = delete;

        std::unique_ptr<MeshAsset> Create(const Path& path);
        std::unique_ptr<MeshAsset> Load(const Path& path);
    };
}

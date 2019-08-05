#pragma once

#include "pluto/asset/material_asset.h"
#include "pluto/memory/resource_control.h"

#include <memory>

namespace pluto
{
    class FileManager;

    class Path;

    class MaterialAssetManager
    {
        FileManager* fileManager;
        MaterialAsset::Factory* materialAssetFactory;
        ResourceControl::Factory* resourceControlFactory;

    public:
        ~MaterialAssetManager();
        MaterialAssetManager(FileManager& fileManager, MaterialAsset::Factory& materialAssetFactory,
                             ResourceControl::Factory& resourceControlFactory);

        MaterialAssetManager(const MaterialAssetManager& other) = delete;
        MaterialAssetManager(MaterialAssetManager&& other) noexcept;
        MaterialAssetManager& operator=(const MaterialAssetManager& rhs) = delete;
        MaterialAssetManager& operator=(MaterialAssetManager&& rhs) noexcept;

        std::unique_ptr<MaterialAsset> Create(const Path& inputPath, const Path& outputDir);
    };
}

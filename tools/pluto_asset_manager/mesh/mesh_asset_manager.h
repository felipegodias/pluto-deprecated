#pragma once

#include <string>
#include <memory>

namespace pluto
{
    class MeshAsset;
    class Guid;

    std::unique_ptr<MeshAsset> CreateMeshAsset(const MeshAsset& other);
    std::unique_ptr<MeshAsset> CreateMeshAsset(const std::string& path);
    std::unique_ptr<MeshAsset> LoadMeshAsset(const Guid& guid);
}

#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <vector>

namespace pluto
{
    class Vector2;
    class Vector3;
    class Vector3Int;

    /*
     * File layout in disk. (Version 1)
     * +--------------+------+------------------------------+
     * | Type         | Size | Description                  |
     * +--------------+------+------------------------------+
     * | GUID         | 16   | File signature.              |
     * | uint8_t      | 1    | Serializer version.          |
     * | uint8_t      | 1    | Asset type.                  |
     * | GUID         | 16   | Asset unique identifier.     |
     * | uint8_t      | 1    | Asset name length.           |
     * | string       | *    | Asset name.                  |
     * +--------------+------+------------------------------+
     * | uint16_t     | 2    | Positions count.             |
     * | Vector3[]    | 12   | Position.                    |
     * | uint16_t     | 2    | UVs count.                   |
     * | Vector2[]    | 8    | UV.                          |
     * | uint16_t     | 2    | Triangles count.             |
     * | Vector3Int[] | 12   | Triangle.                    |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API MeshAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<MeshAsset> Create() const;
            std::unique_ptr<MeshAsset> Create(const MeshAsset& original) const;
            std::unique_ptr<MeshAsset> Create(std::istream& is) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit MeshAsset(std::unique_ptr<Impl> impl);
        MeshAsset(const MeshAsset& other) = delete;
        MeshAsset(MeshAsset&& other) noexcept;
        ~MeshAsset() override;

        MeshAsset& operator=(const MeshAsset& rhs);
        MeshAsset& operator=(MeshAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string name) override;
        void Dump(std::ostream& os) const override;

        const std::vector<Vector3>& GetPositions() const;
        void SetPositions(std::vector<Vector3> positions);
        const std::vector<Vector2>& GetUVs() const;
        void SetUVs(std::vector<Vector2> uvs);
        const std::vector<Vector3Int>& GetTriangles() const;
        void SetTriangles(std::vector<Vector3Int> triangles);
    };
}

#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <vector>

namespace pluto
{
    class Vector2F;
    class Vector3F;
    class Vector3I;
    class FileReader;

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
     * | Vector3F[]   | 12   | Position.                    |
     * | uint16_t     | 2    | UVs count.                   |
     * | Vector2F[]   | 8    | UV.                          |
     * | uint16_t     | 2    | Triangles count.             |
     * | Vector3I[]   | 12   | Triangle.                    |
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
            std::unique_ptr<MeshAsset> Create(FileReader& fileReader) const;
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
        void SetName(std::string value) override;
        void Dump(FileWriter& fileWriter) const override;

        const std::vector<Vector3F>& GetPositions() const;
        void SetPositions(std::vector<Vector3F> value);
        const std::vector<Vector2F>& GetUVs() const;
        void SetUVs(std::vector<Vector2F> value);
        const std::vector<Vector3I>& GetTriangles() const;
        void SetTriangles(std::vector<Vector3I> value);
    };
}
#include <pluto/asset/mesh_asset.h>
#include <pluto/guid.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>
#include <pluto/math/vector3int.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

namespace pluto
{
    class MeshAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::vector<Vector3> positions;
        std::vector<Vector2> uvs;
        std::vector<Vector3Int> triangles;

    public:
        explicit Impl(Guid guid) : guid(std::move(guid))
        {
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(std::string name)
        {
            this->name = std::move(name);
        }

        void Dump(FileWriter& fileWriter) const
        {
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 2;
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            uint16_t positionsCount = positions.size();
            fileWriter.Write(&positionsCount, sizeof(uint16_t));
            fileWriter.Write(positions.data(), sizeof(Vector3) * positionsCount);

            uint16_t uvsCount = uvs.size();
            fileWriter.Write(&uvsCount, sizeof(uint16_t));
            fileWriter.Write(uvs.data(), sizeof(Vector2) * uvsCount);

            uint16_t trianglesCount = triangles.size();
            fileWriter.Write(&trianglesCount, sizeof(uint16_t));
            fileWriter.Write(triangles.data(), sizeof(Vector3Int) * trianglesCount);

            fileWriter.Flush();
        }

        const std::vector<Vector3>& GetPositions() const
        {
            return this->positions;
        }

        void SetPositions(std::vector<Vector3> positions)
        {
            this->positions = std::move(positions);
        }

        const std::vector<Vector2>& GetUVs() const
        {
            return this->uvs;
        }

        void SetUVs(std::vector<Vector2> uvs)
        {
            this->uvs = std::move(uvs);
        }

        const std::vector<Vector3Int>& GetTriangles() const
        {
            return triangles;
        }

        void SetTriangles(std::vector<Vector3Int> triangles)
        {
            this->triangles = std::move(triangles);
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            positions = other.positions;
            uvs = other.uvs;
            triangles = other.triangles;
        }
    };

    MeshAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<MeshAsset> MeshAsset::Factory::Create() const
    {
        return std::make_unique<MeshAsset>(std::make_unique<Impl>(Guid::New()));
    }

    std::unique_ptr<MeshAsset> MeshAsset::Factory::Create(const MeshAsset& original) const
    {
        std::unique_ptr<MeshAsset> instance = Create();
        instance->SetName(original.GetName());
        instance->SetPositions(original.GetPositions());
        instance->SetUVs(original.GetUVs());
        instance->SetTriangles(original.GetTriangles());
        return instance;
    }

    std::unique_ptr<MeshAsset> MeshAsset::Factory::Create(FileReader& fileReader) const
    {
        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));
        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));

        auto meshAsset = std::make_unique<MeshAsset>(std::make_unique<Impl>(assetId));

        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);
        meshAsset->SetName(assetName);

        uint16_t positionsCount;
        fileReader.Read(&positionsCount, sizeof(uint16_t));

        std::vector<Vector3> positions(positionsCount);
        fileReader.Read(positions.data(), sizeof(Vector3) * positionsCount);
        meshAsset->SetPositions(std::move(positions));

        uint16_t uvsCount;
        fileReader.Read(&uvsCount, sizeof(uint16_t));
        std::vector<Vector2> uvs(uvsCount);
        fileReader.Read(uvs.data(), sizeof(Vector2) * uvsCount);
        meshAsset->SetUVs(std::move(uvs));

        uint16_t trianglesCount;
        fileReader.Read(&trianglesCount, sizeof(uint16_t));
        std::vector<Vector3Int> triangles(trianglesCount);
        fileReader.Read(triangles.data(), sizeof(Vector3Int) * trianglesCount);
        meshAsset->SetTriangles(std::move(triangles));

        return meshAsset;
    }

    MeshAsset::MeshAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    MeshAsset::MeshAsset(MeshAsset&& other) noexcept : MeshAsset(std::move(other.impl))
    {
    }

    MeshAsset::~MeshAsset() = default;

    MeshAsset& MeshAsset::operator=(const MeshAsset& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl->Clone(*rhs.impl);
        return *this;
    }

    MeshAsset& MeshAsset::operator=(MeshAsset&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    const Guid& MeshAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& MeshAsset::GetName() const
    {
        return impl->GetName();
    }

    void MeshAsset::SetName(std::string name)
    {
        impl->SetName(std::move(name));
    }

    void MeshAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    const std::vector<Vector3>& MeshAsset::GetPositions() const
    {
        return impl->GetPositions();
    }

    void MeshAsset::SetPositions(std::vector<Vector3> positions)
    {
        impl->SetPositions(std::move(positions));
    }

    const std::vector<Vector2>& MeshAsset::GetUVs() const
    {
        return impl->GetUVs();
    }

    void MeshAsset::SetUVs(std::vector<Vector2> uvs)
    {
        impl->SetUVs(std::move(uvs));
    }

    const std::vector<Vector3Int>& MeshAsset::GetTriangles() const
    {
        return impl->GetTriangles();
    }

    void MeshAsset::SetTriangles(std::vector<Vector3Int> triangles)
    {
        impl->SetTriangles(std::move(triangles));
    }
}

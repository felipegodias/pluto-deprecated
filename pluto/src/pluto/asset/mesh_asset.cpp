#include <pluto/asset/mesh_asset.h>

#include <pluto/service/service_collection.h>
#include <pluto/render/mesh_buffer.h>

#include <pluto/guid.h>
#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>

namespace pluto
{
    class MeshAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::vector<Vector3F> positions;
        std::vector<Vector2F> uvs;
        std::vector<Vector3I> triangles;

        MeshAsset* instance;

        std::unique_ptr<MeshBuffer> meshBuffer;
        bool isBufferDirty;

        const MeshBuffer::Factory& meshBufferFactory;

    public:
        Impl(Guid guid, const MeshBuffer::Factory& meshBufferFactory)
            : guid(std::move(guid)),
              instance(nullptr),
              meshBuffer(nullptr),
              isBufferDirty(true),
              meshBufferFactory(meshBufferFactory)
        {
        }

        // TODO: Look for a better way to pass MeshAsset instance to it.
        void SetInstance(MeshAsset& value)
        {
            instance = &value;
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(const std::string& value)
        {
            name = value;
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
            fileWriter.Write(positions.data(), sizeof(Vector3F) * positionsCount);

            uint16_t uvsCount = uvs.size();
            fileWriter.Write(&uvsCount, sizeof(uint16_t));
            fileWriter.Write(uvs.data(), sizeof(Vector2F) * uvsCount);

            uint16_t trianglesCount = triangles.size();
            fileWriter.Write(&trianglesCount, sizeof(uint16_t));
            fileWriter.Write(triangles.data(), sizeof(Vector3I) * trianglesCount);

            fileWriter.Flush();
        }

        const std::vector<Vector3F>& GetPositions() const
        {
            return positions;
        }

        void SetPositions(std::vector<Vector3F> value)
        {
            positions = std::move(value);
            isBufferDirty = true;
        }

        const std::vector<Vector2F>& GetUVs() const
        {
            return uvs;
        }

        void SetUVs(std::vector<Vector2F> value)
        {
            uvs = std::move(value);
            isBufferDirty = true;
        }

        const std::vector<Vector3I>& GetTriangles() const
        {
            return triangles;
        }

        void SetTriangles(std::vector<Vector3I> value)
        {
            triangles = std::move(value);
            isBufferDirty = true;
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            positions = other.positions;
            uvs = other.uvs;
            triangles = other.triangles;
            isBufferDirty = true;
        }

        MeshBuffer& GetMeshBuffer()
        {
            if (isBufferDirty)
            {
                meshBuffer = meshBufferFactory.Create(*instance);
                isBufferDirty = false;
            }
            return *meshBuffer;
        }
    };

    MeshAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<MeshAsset> MeshAsset::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& meshBufferFactory = serviceCollection.GetService<MeshBuffer::Factory>();
        auto meshAsset = std::make_unique<MeshAsset>(std::make_unique<Impl>(Guid::New(), meshBufferFactory));
        meshAsset->impl->SetInstance(*meshAsset);
        return meshAsset;
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

    std::unique_ptr<Asset> MeshAsset::Factory::Create(FileReader& fileReader) const
    {
        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));
        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));

        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& meshBufferFactory = serviceCollection.GetService<MeshBuffer::Factory>();
        auto meshAsset = std::make_unique<MeshAsset>(std::make_unique<Impl>(assetId, meshBufferFactory));
        meshAsset->impl->SetInstance(*meshAsset);

        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);
        meshAsset->SetName(assetName);

        uint16_t positionsCount;
        fileReader.Read(&positionsCount, sizeof(uint16_t));

        std::vector<Vector3F> positions(positionsCount);
        fileReader.Read(positions.data(), sizeof(Vector3F) * positionsCount);
        meshAsset->SetPositions(std::move(positions));

        uint16_t uvsCount;
        fileReader.Read(&uvsCount, sizeof(uint16_t));
        std::vector<Vector2F> uvs(uvsCount);
        fileReader.Read(uvs.data(), sizeof(Vector2F) * uvsCount);
        meshAsset->SetUVs(std::move(uvs));

        uint16_t trianglesCount;
        fileReader.Read(&trianglesCount, sizeof(uint16_t));
        std::vector<Vector3I> triangles(trianglesCount);
        fileReader.Read(triangles.data(), sizeof(Vector3I) * trianglesCount);
        meshAsset->SetTriangles(std::move(triangles));

        return meshAsset;
    }

    MeshAsset::~MeshAsset() = default;

    MeshAsset::MeshAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    MeshAsset::MeshAsset(MeshAsset&& other) noexcept = default;

    MeshAsset& MeshAsset::operator=(MeshAsset&& rhs) noexcept = default;

    const Guid& MeshAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& MeshAsset::GetName() const
    {
        return impl->GetName();
    }

    void MeshAsset::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    std::shared_ptr<LazyPtr> MeshAsset::GetPtr() const
    {
        return nullptr;
    }

    void MeshAsset::SetPtr(std::shared_ptr<LazyPtr> value)
    {
    }

    void MeshAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    const std::vector<Vector3F>& MeshAsset::GetPositions() const
    {
        return impl->GetPositions();
    }

    void MeshAsset::SetPositions(std::vector<Vector3F> value)
    {
        impl->SetPositions(std::move(value));
    }

    const std::vector<Vector2F>& MeshAsset::GetUVs() const
    {
        return impl->GetUVs();
    }

    void MeshAsset::SetUVs(std::vector<Vector2F> value)
    {
        impl->SetUVs(std::move(value));
    }

    const std::vector<Vector3I>& MeshAsset::GetTriangles() const
    {
        return impl->GetTriangles();
    }

    void MeshAsset::SetTriangles(std::vector<Vector3I> value)
    {
        impl->SetTriangles(std::move(value));
    }

    MeshBuffer& MeshAsset::GetMeshBuffer()
    {
        return impl->GetMeshBuffer();
    }
}

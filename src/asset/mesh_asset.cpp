#include <pluto/asset/mesh_asset.h>
#include <pluto/guid.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>

namespace pluto
{
    class MeshAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        std::vector<Vector3> positions;
        std::vector<Vector2> uvs;
        std::vector<int> triangles;

    public:
        Impl(Guid guid, std::string name) : guid(std::move(guid)), name(std::move(name))
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

        const std::vector<int>& GetTriangles() const
        {
            return triangles;
        }

        void SetTriangles(std::vector<int> triangles)
        {
            this->triangles = std::move(triangles);
        }
    };

    MeshAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<MeshAsset> MeshAsset::Factory::Create() const
    {
        return std::make_unique<MeshAsset>(std::make_unique<Impl>(Guid(), ""));
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

    std::unique_ptr<MeshAsset> MeshAsset::Factory::Create(std::istream& is) const
    {
        // TODO: Remove test code.
        return std::make_unique<MeshAsset>(std::make_unique<Impl>(Guid("b798360e-7add-4a10-9045-301ee55dd228"), ""));
    }

    MeshAsset::MeshAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    MeshAsset::~MeshAsset() = default;

    MeshAsset& MeshAsset::operator=(const MeshAsset& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

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

    void MeshAsset::Dump(std::ostream& os)
    {
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

    const std::vector<int>& MeshAsset::GetTriangles() const
    {
        return impl->GetTriangles();
    }

    void MeshAsset::SetTriangles(std::vector<int> triangles)
    {
        impl->SetTriangles(std::move(triangles));
    }
}

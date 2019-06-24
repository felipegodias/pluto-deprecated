#include <pluto/asset/mesh.h>
#include <pluto/guid.h>
#include <pluto/math/vector2.h>
#include <pluto/math/vector3.h>

namespace pluto
{
    class Mesh::Impl
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

    Mesh::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<Mesh> Mesh::Factory::Create() const
    {
        return std::make_unique<Mesh>(std::make_unique<Impl>(Guid(), ""));
    }

    std::unique_ptr<Mesh> Mesh::Factory::Create(const Mesh& original) const
    {
        std::unique_ptr<Mesh> instance = Create();
        instance->SetName(original.GetName());
        instance->SetPositions(original.GetPositions());
        instance->SetUVs(original.GetUVs());
        instance->SetTriangles(original.GetTriangles());
        return instance;
    }

    std::unique_ptr<Mesh> Mesh::Factory::Create(std::ifstream& ifs) const
    {
        // TODO: Remove test code.
        return std::make_unique<Mesh>(std::make_unique<Impl>(Guid("b798360e-7add-4a10-9045-301ee55dd228"), ""));
    }

    Mesh::Mesh(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    Mesh::~Mesh() = default;

    Mesh& Mesh::operator=(const Mesh& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        return *this;
    }

    const Guid& Mesh::GetId() const
    {
        return impl->GetId();
    }

    const std::string& Mesh::GetName() const
    {
        return impl->GetName();
    }

    void Mesh::SetName(std::string name)
    {
        impl->SetName(std::move(name));
    }

    const std::vector<Vector3>& Mesh::GetPositions() const
    {
        return impl->GetPositions();
    }

    void Mesh::SetPositions(std::vector<Vector3> positions)
    {
        impl->SetPositions(std::move(positions));
    }

    const std::vector<Vector2>& Mesh::GetUVs() const
    {
        return impl->GetUVs();
    }

    void Mesh::SetUVs(std::vector<Vector2> uvs)
    {
        impl->SetUVs(std::move(uvs));
    }

    const std::vector<int>& Mesh::GetTriangles() const
    {
        return impl->GetTriangles();
    }

    void Mesh::SetTriangles(std::vector<int> triangles)
    {
        impl->SetTriangles(std::move(triangles));
    }
}

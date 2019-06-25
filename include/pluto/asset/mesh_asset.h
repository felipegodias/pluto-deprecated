#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <vector>

namespace pluto
{
    class Vector2;
    class Vector3;

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
        MeshAsset(MeshAsset&& other) noexcept = delete;
        ~MeshAsset() override;

        MeshAsset& operator=(const MeshAsset& rhs);
        MeshAsset& operator=(MeshAsset&& rhs) noexcept = delete;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string name) override;
        void Dump(std::ostream& os) const override;

        const std::vector<Vector3>& GetPositions() const;
        void SetPositions(std::vector<Vector3> positions);
        const std::vector<Vector2>& GetUVs() const;
        void SetUVs(std::vector<Vector2> uvs);
        const std::vector<int>& GetTriangles() const;
        void SetTriangles(std::vector<int> triangles);
    };
}

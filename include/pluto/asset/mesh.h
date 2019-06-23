#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <vector>

namespace pluto
{
    class Vector2;
    class Vector3;

    class PLUTO_API Mesh final : public Asset
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<Mesh> Create() const;
            std::unique_ptr<Mesh> Create(const Mesh& original) const;
            std::unique_ptr<Mesh> Create(std::ifstream& ifs) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit Mesh(std::unique_ptr<Impl> impl);
        Mesh(const Mesh& other) = delete;
        Mesh(Mesh&& other) noexcept = delete;
        ~Mesh() override;

        Mesh& operator=(const Mesh& rhs);
        Mesh& operator=(Mesh&& rhs) noexcept = delete;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string name) override;

        const std::vector<Vector3>& GetPositions() const;
        void SetPositions(std::vector<Vector3> positions);
        const std::vector<Vector2>& GetUVs() const;
        void SetUVs(std::vector<Vector2> uvs);
        const std::vector<int>& GetTriangles() const;
        void SetTriangles(std::vector<int> triangles);
    };
}

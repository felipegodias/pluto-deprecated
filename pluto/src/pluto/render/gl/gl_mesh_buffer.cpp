#include "pluto/render/gl/gl_mesh_buffer.h"
#include "pluto/asset/mesh_asset.h"

#include "pluto/render/gl/gl_call.h"

#include "pluto/math/vector2f.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/vector3i.h"

#include <GL/glew.h>

#include <vector>

namespace pluto
{
    class GlMeshBuffer::Impl
    {
        const int verticesCount;
        const uint32_t vertexArrayObject;
        const uint32_t indexBufferObject;
        const std::vector<uint32_t> vertexBufferObjects;
        bool isBound;

    public:
        Impl(const int verticesCount, const uint32_t vertexArrayObject, const uint32_t indexBufferObject,
             std::vector<uint32_t> vertexBufferObjects) : verticesCount(verticesCount),
                                                          vertexArrayObject(vertexArrayObject),
                                                          indexBufferObject(indexBufferObject),
                                                          vertexBufferObjects(std::move(vertexBufferObjects)),
                                                          isBound(false)
        {
        }

        ~Impl()
        {
            GL_CALL(glDeleteVertexArrays(1, &vertexArrayObject));
            GL_CALL(glDeleteBuffers(1, &indexBufferObject));
            GL_CALL(glDeleteBuffers(vertexBufferObjects.size(), &vertexBufferObjects[0]));
        }

        void Bind()
        {
            if (isBound)
            {
                return;
            }
            isBound = true;
            GL_CALL(glBindVertexArray(vertexArrayObject));
            GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));
        }

        void Unbind()
        {
            if (!isBound)
            {
                return;
            }
            isBound = false;
            GL_CALL(glBindVertexArray(0));
        }

        void Draw()
        {
            Bind();
            GL_CALL(glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, nullptr));
        }
    };

    GlMeshBuffer::Factory::Factory(ServiceCollection& serviceCollection) : MeshBuffer::Factory(serviceCollection)
    {
    }

    template <typename T>
    void CreateVertexBufferObject(const uint32_t stride, const std::vector<T>& values,
                                  std::vector<uint32_t>& vertexBufferObjects)
    {
        uint32_t vertexArrayObject = 0;
        if (!values.empty())
        {
            GL_CALL(glGenBuffers(1, &vertexArrayObject));
            GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexArrayObject));
            GL_CALL(glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(T), values.data(), GL_STATIC_DRAW));
            GL_CALL(glEnableVertexAttribArray(vertexBufferObjects.size()));
            GL_CALL(glVertexAttribPointer(vertexBufferObjects.size(), stride, GL_FLOAT, GL_FALSE, 0, nullptr));
        }
        vertexBufferObjects.push_back(vertexArrayObject);
    }

    std::unique_ptr<MeshBuffer> GlMeshBuffer::Factory::Create(const MeshAsset& mesh) const
    {
        uint32_t vertexArrayObject;
        GL_CALL(glGenVertexArrays(1, &vertexArrayObject));

        uint32_t indexBufferObject;
        GL_CALL(glGenBuffers(1, &indexBufferObject));

        GL_CALL(glBindVertexArray(vertexArrayObject));
        std::vector<uint32_t> vertexBufferObjects;
        CreateVertexBufferObject<Vector3F>(3, mesh.GetPositions(), vertexBufferObjects);
        CreateVertexBufferObject<Vector2F>(2, mesh.GetUVs(), vertexBufferObjects);

        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));

        const std::vector<Vector3I>& triangles = mesh.GetTriangles();
        if (!triangles.empty())
        {
            const size_t bufferSize = triangles.size() * sizeof(Vector3I);
            GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, triangles.data(), GL_STATIC_DRAW));
        }

        int verticesCount = static_cast<int>(triangles.size()) * 3;

        auto impl = std::make_unique<Impl>(verticesCount, vertexArrayObject, indexBufferObject,
                                           std::move(vertexBufferObjects));
        return std::make_unique<GlMeshBuffer>(std::move(impl));
    }

    GlMeshBuffer::GlMeshBuffer(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    GlMeshBuffer::GlMeshBuffer(GlMeshBuffer&& other) noexcept : impl(std::move(other.impl))
    {
    }

    GlMeshBuffer::~GlMeshBuffer() = default;

    GlMeshBuffer& GlMeshBuffer::operator=(GlMeshBuffer&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    void GlMeshBuffer::Bind()
    {
        impl->Bind();
    }

    void GlMeshBuffer::Unbind()
    {
        impl->Unbind();
    }

    void GlMeshBuffer::Draw()
    {
        impl->Draw();
    }
}

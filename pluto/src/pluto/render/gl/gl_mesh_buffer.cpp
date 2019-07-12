#include "pluto/render/gl/gl_mesh_buffer.h"
#include "pluto/asset/mesh_asset.h"

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
            glDeleteVertexArrays(1, &vertexArrayObject);
            glDeleteBuffers(1, &indexBufferObject);
            glDeleteBuffers(vertexBufferObjects.size(), &vertexBufferObjects[0]);
        }

        void Bind()
        {
            if (isBound)
            {
                return;
            }
            isBound = true;
            glBindVertexArray(vertexArrayObject);
        }

        void Unbind()
        {
            if (!isBound)
            {
                return;
            }
            isBound = false;
            glBindVertexArray(0);
        }

        void Draw()
        {
            Bind();
            glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, nullptr);
        }
    };

    GlMeshBuffer::Factory::Factory(DiContainer& diContainer) : MeshBuffer::Factory(diContainer)
    {
    }

    template <typename T>
    void CreateVertexBufferObject(const uint32_t stride, const std::vector<T>& values,
                                  std::vector<uint32_t>& vertexBufferObjects)
    {
        uint32_t vertexArrayObject = 0;
        if (!values.empty())
        {
            glGenBuffers(1, &vertexArrayObject);
            glBindBuffer(GL_ARRAY_BUFFER, vertexArrayObject);
            glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(T), &values[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(vertexBufferObjects.size());
            glVertexAttribPointer(vertexBufferObjects.size(), stride, GL_FLOAT, GL_FALSE, 0, nullptr);
        }
        vertexBufferObjects.push_back(vertexArrayObject);
    }

    std::unique_ptr<MeshBuffer> GlMeshBuffer::Factory::Create(const MeshAsset& mesh) const
    {
        uint32_t vertexArrayObject;
        glGenVertexArrays(1, &vertexArrayObject);

        GLuint indexBufferObject;
        glGenBuffers(1, &indexBufferObject);

        std::vector<uint32_t> vertexBufferObjects;
        CreateVertexBufferObject<Vector3F>(3, mesh.GetPositions(), vertexBufferObjects);
        CreateVertexBufferObject<Vector2F>(2, mesh.GetUVs(), vertexBufferObjects);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

        const std::vector<Vector3I>& triangles = mesh.GetTriangles();
        const size_t bufferSize = triangles.size() * sizeof(Vector3I);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, &triangles[0], GL_STATIC_DRAW);

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

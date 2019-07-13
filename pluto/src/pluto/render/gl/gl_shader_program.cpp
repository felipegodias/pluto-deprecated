#include "pluto/render/gl/gl_shader_program.h"
#include "pluto/asset/shader_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/math/matrix4x4.h"

#include <GL/glew.h>

#include <array>
#include <unordered_map>

namespace pluto
{
    static const std::array<GLenum, static_cast<int>(ShaderAsset::BlendFunction::Count)> BLEND_FUNCTIONS{
        GL_NONE,
        GL_FUNC_ADD,
        GL_FUNC_SUBTRACT,
        GL_FUNC_REVERSE_SUBTRACT,
        GL_MIN,
        GL_MAX
    };

    static const std::array<GLenum, static_cast<int>(ShaderAsset::BlendFactor::Count)> BLEND_FACTORS{
        GL_ZERO,
        GL_ONE,
        GL_SRC_COLOR,
        GL_DST_COLOR,
        GL_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_SRC_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
    };

    static const std::array<GLenum, static_cast<int>(ShaderAsset::ZTest::Count)> DEPTH_TESTS{
        GL_NEVER,
        GL_NEVER,
        GL_LESS,
        GL_LEQUAL,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_GEQUAL,
        GL_GREATER,
        GL_ALWAYS,
    };

    static const std::array<GLenum, static_cast<int>(ShaderAsset::CullMode::Count)> FACE_CULLING{
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK,
    };

    class GlShaderProgram::Impl
    {
    private:
        const GLuint programId;
        const ShaderAsset& shaderAsset;

        uint8_t mvpUniformLocation;

        bool isBound;
        const MaterialAsset* lastMaterialAsset;

    public:
        Impl(const GLuint programId, const ShaderAsset& shaderAsset) : programId(programId), shaderAsset(shaderAsset),
                                                                       mvpUniformLocation(255), isBound(false),
                                                                       lastMaterialAsset(nullptr)
        {
            for (const auto& property : shaderAsset.GetProperties())
            {
                if (property.name != "u_mvp")
                {
                    continue;
                }

                mvpUniformLocation = property.id;
                break;
            }
        }

        ~Impl()
        {
            glDeleteProgram(programId);
            glUseProgram(0);
        }

        void Bind(const Matrix4X4& mvp, const MaterialAsset& materialAsset)
        {
            if (!isBound)
            {
                glUseProgram(programId);
                UpdateBlendFunction();
                UpdateDepthTest();
                UpdateFaceCulling();
                isBound = true;
            }

            if (lastMaterialAsset == nullptr || *lastMaterialAsset != materialAsset)
            {
                lastMaterialAsset = &materialAsset;
                UpdateMaterial();
            }

            UpdateModelViewProjection(mvp);
        }

        void Unbind()
        {
            glUseProgram(0);
            lastMaterialAsset = nullptr;
            isBound = false;
        }

    private:
        void UpdateBlendFunction()
        {
            const bool isBlendOn = glIsEnabled(GL_BLEND);
            if (shaderAsset.GetBlendFunction() != ShaderAsset::BlendFunction::None)
            {
                if (!isBlendOn)
                {
                    glEnable(GL_BLEND);
                }

                const GLenum srcBlendFactor = BLEND_FACTORS[static_cast<int>(shaderAsset.GetSrcBlendFactor())];
                const GLenum dstBlendFactor = BLEND_FACTORS[static_cast<int>(shaderAsset.GetDstBlendFactor())];
                glBlendFunc(srcBlendFactor, dstBlendFactor);

                glBlendEquation(BLEND_FUNCTIONS[static_cast<int>(shaderAsset.GetBlendFunction())]);
            }
            else if (isBlendOn)
            {
                glDisable(GL_BLEND);
            }
        }

        void UpdateDepthTest()
        {
            const bool isZTestOn = glIsEnabled(GL_DEPTH_TEST);
            if (shaderAsset.GetZTest() != ShaderAsset::ZTest::None)
            {
                if (!isZTestOn)
                {
                    glEnable(GL_DEPTH_TEST);
                }
                glDepthFunc(DEPTH_TESTS[static_cast<int>(shaderAsset.GetZTest())]);
            }
            else if (isZTestOn)
            {
                glDisable(GL_DEPTH_TEST);
            }
        }

        void UpdateFaceCulling()
        {
            glCullFace(FACE_CULLING[static_cast<int>(shaderAsset.GetCullMode())]);
        }

        void UpdateMaterial()
        {
            // TODO: Implement.
        }

        void UpdateModelViewProjection(const Matrix4X4& mvp)
        {
            glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, mvp.data.data());
        }
    };

    GlShaderProgram::Factory::Factory(DiContainer& diContainer) : ShaderProgram::Factory(diContainer)
    {
    }

    std::unique_ptr<ShaderProgram> GlShaderProgram::Factory::Create(const ShaderAsset& shaderAsset) const
    {
        const GLuint programId = glCreateProgram();
        const std::vector<uint8_t>& binary = shaderAsset.GetBinary();
        glProgramBinary(programId, shaderAsset.GetBinaryFormat(), binary.data(), binary.size());

        return std::make_unique<GlShaderProgram>(std::make_unique<Impl>(programId, shaderAsset));
    }

    GlShaderProgram::GlShaderProgram(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    GlShaderProgram::GlShaderProgram(GlShaderProgram&& other) noexcept : impl(std::move(other.impl))
    {
    }

    GlShaderProgram::~GlShaderProgram() = default;

    GlShaderProgram& GlShaderProgram::operator=(GlShaderProgram&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    void GlShaderProgram::Bind(const Matrix4X4& mvp, const MaterialAsset& materialAsset)
    {
        impl->Bind(mvp, materialAsset);
    }

    void GlShaderProgram::Unbind()
    {
        impl->Unbind();
    }
}

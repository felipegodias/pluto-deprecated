#include "pluto/render/gl/gl_shader_program.h"
#include "pluto/render/gl/gl_texture_buffer.h"
#include "pluto/render/gl/gl_call.h"
#include "pluto/asset/shader_asset.h"
#include "pluto/asset/material_asset.h"
#include "pluto/asset/texture_asset.h"

#include "pluto/math/vector2i.h"
#include "pluto/math/vector2f.h"
#include "pluto/math/vector3i.h"
#include "pluto/math/vector3f.h"
#include "pluto/math/vector4i.h"
#include "pluto/math/vector4f.h"
#include "pluto/math/matrix4x4.h"

#include "pluto/exception.h"

#include <GL/glew.h>

#include <array>
#include <unordered_map>

namespace pluto
{
    static const std::array<GLenum, static_cast<int>(ShaderAsset::BlendEquation::Count)> BLEND_EQUATIONS{
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
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
    };

    static const std::array<GLenum, static_cast<int>(ShaderAsset::DepthTest::Count)> DEPTH_TESTS{
        GL_NONE,
        GL_ALWAYS,
        GL_NEVER,
        GL_LEQUAL,
        GL_EQUAL,
        GL_LEQUAL,
        GL_GREATER,
        GL_NOTEQUAL,
        GL_GEQUAL,
    };

    static const std::array<GLenum, static_cast<int>(ShaderAsset::CullFace::Count)> FACE_CULLING{
        GL_NONE,
        GL_FRONT,
        GL_BACK,
    };

    class GlShaderProgram::Impl
    {
        const GLuint programId;
        const ShaderAsset* shaderAsset;

        uint8_t mvpUniformLocation;

        bool isBound;
        const MaterialAsset* lastMaterialAsset;

    public:
        Impl(const GLuint programId, const ShaderAsset& shaderAsset)
            : programId(programId),
              shaderAsset(&shaderAsset),
              mvpUniformLocation(255),
              isBound(false),
              lastMaterialAsset(nullptr)
        {
            for (const auto& property : shaderAsset.GetUniforms())
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
            GL_CALL(glDeleteProgram(programId));
            GL_CALL(glUseProgram(0));
        }

        void Bind(const Matrix4X4& mvp, const MaterialAsset& materialAsset)
        {
            if (!isBound)
            {
                GL_CALL(glUseProgram(programId));
                UpdateBlendFunction();
                UpdateDepthTest();
                UpdateFaceCull();
                UpdateAttributes();
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
            GL_CALL(glUseProgram(0));
            lastMaterialAsset = nullptr;
            isBound = false;
        }

    private:
        void UpdateBlendFunction()
        {
            GL_CALL(const bool isBlendOn = glIsEnabled(GL_BLEND));
            if (shaderAsset->GetBlendEquation() != ShaderAsset::BlendEquation::Off)
            {
                if (!isBlendOn)
                {
                    GL_CALL(glEnable(GL_BLEND));
                }

                const GLenum blendSrcFactor = BLEND_FACTORS[static_cast<int>(shaderAsset->GetBlendSrcFactor())];
                const GLenum blendDstFactor = BLEND_FACTORS[static_cast<int>(shaderAsset->GetBlendDstFactor())];

                const GLenum blendSrcAlphaFactor = BLEND_FACTORS[static_cast<int>(shaderAsset->GetBlendSrcAlphaFactor())
                ];

                const GLenum blendDstAlphaFactor = BLEND_FACTORS[static_cast<int>(shaderAsset->GetBlendDstAlphaFactor())
                ];

                if (blendSrcFactor == blendSrcAlphaFactor && blendDstFactor == blendDstAlphaFactor)
                {
                    GL_CALL(glBlendFunc(blendSrcFactor, blendDstFactor));
                }
                else
                {
                    GL_CALL(
                        glBlendFuncSeparate(blendSrcFactor, blendDstFactor, blendSrcAlphaFactor, blendDstAlphaFactor));
                }

                const GLenum blendEquation = BLEND_EQUATIONS[static_cast<int>(shaderAsset->GetBlendEquation())];
                const GLenum blendAlphaEquation = BLEND_EQUATIONS[static_cast<int>(shaderAsset->GetBlendAlphaEquation())
                ];
                if (blendEquation == blendAlphaEquation)
                {
                    GL_CALL(glBlendEquation(blendEquation));
                }
                else
                {
                    GL_CALL(glBlendEquationSeparate(blendEquation, blendAlphaEquation));
                }
            }
            else if (isBlendOn)
            {
                GL_CALL(glDisable(GL_BLEND));
            }
        }

        void UpdateDepthTest()
        {
            GL_CALL(const bool isDepthTestOn = glIsEnabled(GL_DEPTH_TEST));
            if (shaderAsset->GetDepthTest() != ShaderAsset::DepthTest::Off)
            {
                if (!isDepthTestOn)
                {
                    GL_CALL(glEnable(GL_DEPTH_TEST));
                }

                GL_CALL(glDepthFunc(DEPTH_TESTS[static_cast<int>(shaderAsset->GetDepthTest())]));
            }
            else if (isDepthTestOn)
            {
                GL_CALL(glDisable(GL_DEPTH_TEST));
            }
        }

        void UpdateFaceCull()
        {
            GL_CALL(const bool isFaceCullOn = glIsEnabled(GL_CULL_FACE));
            if (shaderAsset->GetCullFace() != ShaderAsset::CullFace::Off)
            {
                if (!isFaceCullOn)
                {
                    GL_CALL(glEnable(GL_CULL_FACE));
                }

                GL_CALL(glCullFace(FACE_CULLING[static_cast<int>(shaderAsset->GetCullFace())]));
            }
            else if (isFaceCullOn)
            {
                GL_CALL(glDisable(GL_DEPTH_TEST));
            }
        }

        void UpdateAttributes()
        {
            for (const auto& uniform : shaderAsset->GetUniforms())
            {
                if (uniform.name == "vertex.pos")
                {
                    glBindAttribLocation(programId, 0, "vertex.pos");
                }
                else if (uniform.name == "vertex.uv")
                {
                    glBindAttribLocation(programId, 1, "vertex.uv");
                }
            }
        }

        void UpdateMaterial()
        {
            for (const auto& uniform : shaderAsset->GetUniforms())
            {
                if (uniform.id == mvpUniformLocation)
                {
                    continue;
                }

                UpdateUniform(uniform);
            }
        }

        void UpdateModelViewProjection(const Matrix4X4& mvp)
        {
            GL_CALL(glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, mvp.Data()));
        }

        void UpdateUniform(const ShaderAsset::Property& uniform)
        {
            switch (uniform.type)
            {
            case ShaderAsset::Property::Type::Bool:
                GL_CALL(glUniform1i(uniform.id, lastMaterialAsset->GetBool(uniform.name)));
                break;
            case ShaderAsset::Property::Type::Int:
                GL_CALL(glUniform1i(uniform.id, lastMaterialAsset->GetInt(uniform.name)));
                break;
            case ShaderAsset::Property::Type::Float:
                GL_CALL(glUniform1f(uniform.id, lastMaterialAsset->GetFloat(uniform.name)));
                break;
            case ShaderAsset::Property::Type::Vector2I:
                GL_CALL(glUniform2iv(uniform.id, 1, lastMaterialAsset->GetVector2I(uniform.name).Data()));
                break;
            case ShaderAsset::Property::Type::Vector2F:
                GL_CALL(glUniform2fv(uniform.id, 1, lastMaterialAsset->GetVector2F(uniform.name).Data()));
                break;
            case ShaderAsset::Property::Type::Vector3I:
                GL_CALL(glUniform3iv(uniform.id, 1, lastMaterialAsset->GetVector3I(uniform.name).Data()));
                break;
            case ShaderAsset::Property::Type::Vector3F:
                GL_CALL(glUniform3fv(uniform.id, 1, lastMaterialAsset->GetVector3F(uniform.name).Data()));
                break;
            case ShaderAsset::Property::Type::Vector4I:
                GL_CALL(glUniform4iv(uniform.id, 1, lastMaterialAsset->GetVector4I(uniform.name).Data()));
                break;
            case ShaderAsset::Property::Type::Vector4F:
                GL_CALL(glUniform4fv(uniform.id, 1, lastMaterialAsset->GetVector4F(uniform.name).Data()));
                break;
            case ShaderAsset::Property::Type::Matrix4X4:
                GL_CALL(
                    glUniformMatrix3fv(uniform.id, 1, GL_FALSE, lastMaterialAsset->GetMatrix4X4(uniform.name).Data()
                    ));
                break;
            case ShaderAsset::Property::Type::Sampler2D:
                BindTexture(uniform);
                break;
            default: ;
            }
        }

        void BindTexture(const ShaderAsset::Property& uniform)
        {
            TextureAsset& textureAsset = lastMaterialAsset->GetTexture(uniform.name);
            auto& textureBuffer = dynamic_cast<GlTextureBuffer&>(textureAsset.GetTextureBuffer());
            textureBuffer.Bind(0);
            GL_CALL(glUniform1i(uniform.id, 0));
        }
    };

    GlShaderProgram::Factory::Factory(DiContainer& diContainer)
        : ShaderProgram::Factory(diContainer)
    {
    }

    std::unique_ptr<ShaderProgram> GlShaderProgram::Factory::Create(const ShaderAsset& shaderAsset) const
    {
        GL_CALL(const GLuint programId = glCreateProgram());
        const std::vector<uint8_t>& binaryData = shaderAsset.GetBinaryData();
        GL_CALL(glProgramBinary(programId, shaderAsset.GetBinaryFormat(), binaryData.data(), binaryData.size()));

        return std::make_unique<GlShaderProgram>(std::make_unique<Impl>(programId, shaderAsset));
    }

    GlShaderProgram::GlShaderProgram(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    GlShaderProgram::GlShaderProgram(GlShaderProgram&& other) noexcept
        : impl(std::move(other.impl))
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

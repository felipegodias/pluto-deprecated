#pragma once

#include "pluto/render/shader_program.h"
#include <memory>

namespace pluto
{
    class Matrix4X4;
    class MaterialAsset;

    class PLUTO_API GlShaderProgram final : public ShaderProgram
    {
    public:
        class PLUTO_API Factory final : public ShaderProgram::Factory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<ShaderProgram> Create(const ShaderAsset& shaderAsset) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit GlShaderProgram(std::unique_ptr<Impl> impl);
        GlShaderProgram(const GlShaderProgram& other) = delete;
        GlShaderProgram(GlShaderProgram&& other) noexcept;
        ~GlShaderProgram() override;
        GlShaderProgram& operator=(const GlShaderProgram& rhs) = delete;
        GlShaderProgram& operator=(GlShaderProgram&& rhs) noexcept;

        void Bind(const Matrix4X4& mvp, const MaterialAsset& materialAsset);
        void Unbind();
    };
}

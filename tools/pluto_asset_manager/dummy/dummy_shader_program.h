#pragma once

#include "pluto/render/shader_program.h"
#include <memory>

namespace pluto
{
    class DummyShaderProgram final : public ShaderProgram
    {
    public:
        class Factory final : public ShaderProgram::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<ShaderProgram> Create(const ShaderAsset& shaderAsset) const override;
        };

        DummyShaderProgram();
    };
}

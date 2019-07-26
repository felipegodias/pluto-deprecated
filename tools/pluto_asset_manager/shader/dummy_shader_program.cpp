#include "dummy_shader_program.h"

namespace pluto
{
    DummyShaderProgram::Factory::Factory(DiContainer& diContainer)
        : ShaderProgram::Factory(diContainer)
    {
    }

    std::unique_ptr<ShaderProgram> DummyShaderProgram::Factory::Create(const ShaderAsset& shaderAsset) const
    {
        return std::make_unique<DummyShaderProgram>();
    }

    DummyShaderProgram::DummyShaderProgram() = default;
}

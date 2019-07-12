#include "pluto/render/shader_program.h"

namespace pluto
{
    ShaderProgram::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    ShaderProgram::ShaderProgram() = default;

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept = default;

    ShaderProgram::~ShaderProgram() = default;

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs) noexcept = default;
}

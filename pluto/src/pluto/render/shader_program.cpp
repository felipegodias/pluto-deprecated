#include "pluto/render/shader_program.h"

namespace pluto
{
    ShaderProgram::Factory::Factory(ServiceCollection& serviceCollection) : BaseFactory(serviceCollection)
    {
    }

    ShaderProgram::ShaderProgram() = default;

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept = default;

    ShaderProgram::~ShaderProgram() = default;

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs) noexcept = default;
}

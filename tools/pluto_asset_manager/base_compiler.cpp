#include "base_compiler.h"

namespace pluto::compiler
{
    BaseCompiler::~BaseCompiler() = default;

    BaseCompiler::BaseCompiler() = default;

    BaseCompiler::BaseCompiler(BaseCompiler&& other) noexcept = default;

    BaseCompiler& BaseCompiler::operator=(BaseCompiler&& rhs) noexcept = default;
}

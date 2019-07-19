#pragma once

#include <string>

namespace pluto
{
    void CheckOpenGlError(const std::string& stmt);

#ifdef _DEBUG
#define GL_CALL(stmt) stmt; CheckOpenGlError(#stmt)
#else
#define GL_CALL(stmt) stmt
#endif
}

#include "pluto/render/gl/gl_call.h"

#include "pluto/exception.h"

#include <fmt/format.h>
#include <GL/glew.h>
#include <stdexcept>

namespace pluto
{
    void CheckOpenGlError(const std::string& stmt)
    {
        const GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            Exception::Throw(std::runtime_error(fmt::format("OpenGL Error {0}: {1}", err, stmt)));
        }
    }
}

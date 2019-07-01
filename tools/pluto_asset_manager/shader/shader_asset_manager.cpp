#include "shader_asset_manager.h"

#include <pluto/guid.h>
#include <pluto/di/di_container.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/file/file_reader.h>

#include <boost/algorithm/string.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace pluto
{
    std::unordered_map<std::string, ShaderAsset::BlendFunction> blendFunctions = {
        {"none", ShaderAsset::BlendFunction::None},
        {"add", ShaderAsset::BlendFunction::Add},
        {"sub", ShaderAsset::BlendFunction::Subtract},
        {"rev_sub", ShaderAsset::BlendFunction::ReverseSubtract},
        {"min", ShaderAsset::BlendFunction::Min},
        {"max", ShaderAsset::BlendFunction::Max}
    };

    std::unordered_map<std::string, ShaderAsset::BlendFactor> blendFactors = {
        {"zero", ShaderAsset::BlendFactor::Zero},
        {"one", ShaderAsset::BlendFactor::One},
        {"src_color", ShaderAsset::BlendFactor::SrcAlpha},
        {"dst_color", ShaderAsset::BlendFactor::DstColor},
        {"src_alpha", ShaderAsset::BlendFactor::SrcAlpha},
        {"dst_alpha", ShaderAsset::BlendFactor::DstAlpha},
        {"one_minus_src_color", ShaderAsset::BlendFactor::OneMinusSrcColor},
        {"one_minus_src_alpha", ShaderAsset::BlendFactor::OneMinusSrcAlpha},
        {"one_minus_dst_color", ShaderAsset::BlendFactor::OneMinusDstColor},
        {"one_minus_dst_alpha", ShaderAsset::BlendFactor::OneMinusDstAlpha},
    };

    std::unordered_map<std::string, ShaderAsset::ZTest> zTests = {
        {"none", ShaderAsset::ZTest::None},
        {"never", ShaderAsset::ZTest::Never},
        {"less", ShaderAsset::ZTest::Less},
        {"less_equal", ShaderAsset::ZTest::LessEqual},
        {"equal", ShaderAsset::ZTest::Equal},
        {"not_equal", ShaderAsset::ZTest::NotEqual},
        {"greater", ShaderAsset::ZTest::Greater},
        {"greater_equal", ShaderAsset::ZTest::GreaterEqual},
        {"always", ShaderAsset::ZTest::Always},
    };

    std::unordered_map<std::string, ShaderAsset::CullMode> cullModes = {
        {"front", ShaderAsset::CullMode::Front},
        {"back", ShaderAsset::CullMode::Back},
        {"front_and_back", ShaderAsset::CullMode::FrontAndBack},
    };

    std::unordered_map<GLuint, ShaderAsset::Property::Type> propertyTypes = {
        {GL_BOOL, ShaderAsset::Property::Type::Bool},
        {GL_INT, ShaderAsset::Property::Type::Int},
        {GL_FLOAT, ShaderAsset::Property::Type::Float},
        {GL_INT_VEC2, ShaderAsset::Property::Type::Vector2I},
        {GL_FLOAT_VEC2, ShaderAsset::Property::Type::Vector2F},
        {GL_INT_VEC3, ShaderAsset::Property::Type::Vector3I},
        {GL_FLOAT_VEC3, ShaderAsset::Property::Type::Vector3F},
        {GL_INT_VEC4, ShaderAsset::Property::Type::Vector4I},
        {GL_FLOAT_VEC4, ShaderAsset::Property::Type::Vector4F},
        {GL_FLOAT_MAT2, ShaderAsset::Property::Type::Matrix2X2},
        {GL_FLOAT_MAT3, ShaderAsset::Property::Type::Matrix3X3},
        {GL_FLOAT_MAT4, ShaderAsset::Property::Type::Matrix4X4},
    };

    struct ShaderFileData
    {
        std::string vertex;
        std::string frag;
        std::string blend;
        std::string blendSrc;
        std::string blendDst;
        std::string zTest;
        std::string cullMode;
    };

    ShaderFileData ParseShader(std::istream& is)
    {
        std::string line;
        int state = 0;
        ShaderFileData shaderData;
        std::stringstream vertex;
        std::stringstream frag;
        while (std::getline(is, line))
        {
            if (line.empty())
            {
                continue;
            }
            if (line[0] == '#')
            {
                std::vector<std::string> split;
                boost::split(split, line, boost::is_any_of(" "));
                if (split[0] == "#set")
                {
                    if (split[1] == "state")
                    {
                        if (split[2] == "vertex")
                        {
                            state = 1;
                        }
                        else if (split[2] == "fragment")
                        {
                            state = 2;
                        }
                        else if (split[2] == "shared")
                        {
                            state = 3;
                        }
                    }
                    else if (split[1] == "blend")
                    {
                        shaderData.blend = split[2];
                        shaderData.blendSrc = split[3];
                        shaderData.blendDst = split[4];
                    }
                    else if (split[1] == "z_test")
                    {
                        shaderData.zTest = split[2];
                    }
                    else if (split[1] == "cull")
                    {
                        shaderData.cullMode = split[2];
                    }
                    continue;
                }
            }
            if (state == 1)
            {
                vertex << line << '\n';
            }
            else if (state == 2)
            {
                frag << line << '\n';
            }
            else if (state == 3)
            {
                vertex << line << '\n';
                frag << line << '\n';
            }
        }
        shaderData.vertex = vertex.str();
        shaderData.frag = frag.str();
        return shaderData;
    }

    void CompileShader(const GLuint shaderId, const char* src)
    {
        glShaderSource(shaderId, 1, &src, nullptr);
        glCompileShader(shaderId);
        int result;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int msgLength;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msgLength);
            const auto message = static_cast<char*>(alloca(msgLength * sizeof(char)));
            glGetShaderInfoLog(shaderId, msgLength, &msgLength, message);
            std::stringstream ss;
            ss << "Fail to compile shader. OpenGL message: " << message;
            throw std::runtime_error(ss.str());
        }
    }

    GLuint CompileVertexShader(const std::string& vertex)
    {
        const GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(shaderId, vertex.c_str());
        return shaderId;
    }

    GLuint CompileFragmentShader(const std::string& fragment)
    {
        const GLuint shaderId = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(shaderId, fragment.c_str());
        return shaderId;
    }

    GLuint CreateShader(const std::string& vertex, const std::string& fragment)
    {
        const GLuint programId = glCreateProgram();
        const GLuint vertexShaderId = CompileVertexShader(vertex);
        const GLuint fragmentShaderId = CompileFragmentShader(fragment);
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        glBindFragDataLocation(programId, 0, "outColor");
        int linkResult;
        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &linkResult);
        if (linkResult == GL_FALSE)
        {
            int msgLength;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &msgLength);
            const auto message = static_cast<char*>(alloca(msgLength * sizeof(char)));
            glGetProgramInfoLog(programId, msgLength, &msgLength, message);
            std::stringstream ss;
            ss << "Failed create shader program. (Linking Step)\nOpenGL message: " << message;
            throw std::runtime_error(ss.str());
        }
        int validationResult;
        glValidateProgram(programId);
        glGetProgramiv(programId, GL_VALIDATE_STATUS, &validationResult);
        if (validationResult == GL_FALSE)
        {
            int msgLength;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &msgLength);
            const auto message = static_cast<char*>(alloca(msgLength * sizeof(char)));
            glGetProgramInfoLog(programId, msgLength, &msgLength, message);
            std::stringstream ss;
            ss << "Failed create shader program. (Validation Step)\nOpenGL message: " << message;
            throw std::runtime_error(ss.str());
        }
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        return programId;
    }

    std::vector<ShaderAsset::Property> FetchUniforms(const GLuint programId)
    {
        std::vector<ShaderAsset::Property> uniforms;
        int bufferSize;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufferSize);
        int uniformsCount;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformsCount);
        auto* name = static_cast<char*>(alloca(sizeof(char) * bufferSize));
        GLuint type;
        int size;
        int nameLength;
        for (int i = 0; i < uniformsCount; ++i)
        {
            const unsigned index = i;
            glGetActiveUniform(programId, index, bufferSize, &nameLength, &size, &type, name);
            uniforms.push_back({static_cast<uint8_t>(index), name, propertyTypes.at(type)});
        }
        return uniforms;
    }

    ShaderAsset::BlendFunction ParseBlendFunc(const std::string& str)
    {
        const auto it = blendFunctions.find(str);
        if (it != blendFunctions.end())
        {
            return it->second;
        }
        return ShaderAsset::BlendFunction::Default;
    }

    ShaderAsset::BlendFactor ParseBlendFactor(const std::string& str)
    {
        const auto it = blendFactors.find(str);
        if (it != blendFactors.end())
        {
            return it->second;
        }
        return ShaderAsset::BlendFactor::Default;
    }

    ShaderAsset::ZTest ParseZTest(const std::string& str)
    {
        const auto it = zTests.find(str);
        if (it != zTests.end())
        {
            return it->second;
        }
        return ShaderAsset::ZTest::Default;
    }

    ShaderAsset::CullMode ParseCull(const std::string& str)
    {
        const auto it = cullModes.find(str);
        if (it != cullModes.end())
        {
            return it->second;
        }
        return ShaderAsset::CullMode::Default;
    }

    void InitGl()
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize glfw.");
        }
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create glfw window.");
        }
        glfwMakeContextCurrent(window);

        const GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            throw std::runtime_error(fmt::format("Failed to initialize glew. Error: {0}", glewGetErrorString(err)));
        }
    }

    std::unique_ptr<ShaderAsset> CreateShaderAsset(const ShaderAsset& other)
    {
        DiContainer diContainer;
        const ShaderAsset::Factory factory(diContainer);
        auto shaderAsset = factory.Create(other);
        return shaderAsset;
    }

    std::unique_ptr<ShaderAsset> CreateShaderAsset(const std::string& path)
    {
        std::ifstream file(path);
        const ShaderFileData shaderData = ParseShader(file);

        InitGl();
        const GLuint programId = CreateShader(shaderData.vertex, shaderData.frag);
        GLint binLength = -1;
        GLenum binFormat = -1;
        glGetProgramiv(programId, GL_PROGRAM_BINARY_LENGTH, &binLength);
        std::vector<uint8_t> programBinary(binLength);
        GLsizei bytesWritten;
        glGetProgramBinary(programId, binLength, &bytesWritten, &binFormat, programBinary.data());

        ShaderAsset::BlendFunction blend = ParseBlendFunc(shaderData.blend);
        ShaderAsset::BlendFactor blendSrc = ParseBlendFactor(shaderData.blendSrc);
        ShaderAsset::BlendFactor blendDst = ParseBlendFactor(shaderData.blendDst);
        ShaderAsset::ZTest zTest = ParseZTest(shaderData.zTest);
        ShaderAsset::CullMode cullMode = ParseCull(shaderData.cullMode);
        std::vector<ShaderAsset::Property> uniforms = FetchUniforms(programId);
        glDeleteProgram(programId);
        glfwTerminate();

        DiContainer diContainer;
        const ShaderAsset::Factory factory(diContainer);
        auto shaderAsset = factory.Create();
        const std::filesystem::path filePath(path);
        shaderAsset->SetName(filePath.filename().replace_extension("").string());
        shaderAsset->SetBlendFunction(blend);
        shaderAsset->SetSrcBlendFactor(blendSrc);
        shaderAsset->SetDstBlendFactor(blendDst);
        shaderAsset->SetZTest(zTest);
        shaderAsset->SetCullMode(cullMode);
        shaderAsset->SetProperties(std::move(uniforms));
        shaderAsset->SetBinaryFormat(binFormat);
        shaderAsset->SetBinary(std::move(programBinary));

        return shaderAsset;
    }

    std::unique_ptr<ShaderAsset> LoadShaderAsset(const Guid& guid)
    {
        DiContainer diContainer;
        const ShaderAsset::Factory shaderAssetFactory(diContainer);
        const FileReader::Factory fileReaderFactory(diContainer);

        std::ifstream ifs(guid.Str(), std::ios::binary);
        const auto fileReader = fileReaderFactory.Create(std::move(ifs));
        auto textAsset = shaderAssetFactory.Create(*fileReader);
        return textAsset;
    }
}

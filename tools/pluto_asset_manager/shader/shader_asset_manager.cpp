#include "shader_asset_manager.h"

#include <pluto/guid.h>
#include <pluto/di/di_container.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>

#include <boost/algorithm/string.hpp>
#include <GL/glew.h>
#include <yaml-cpp/yaml.h>
#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace pluto
{
    std::unordered_map<std::string, ShaderAsset::BlendEquation> blendEquations = {
        {"OFF", ShaderAsset::BlendEquation::Off},
        {"ADD", ShaderAsset::BlendEquation::Add},
        {"SUB", ShaderAsset::BlendEquation::Subtract},
        {"REV_SUB", ShaderAsset::BlendEquation::ReverseSubtract},
        {"MIN", ShaderAsset::BlendEquation::Min},
        {"MAX", ShaderAsset::BlendEquation::Max}
    };

    std::unordered_map<std::string, ShaderAsset::BlendFactor> blendFactors = {
        {"ZERO", ShaderAsset::BlendFactor::Zero},
        {"ONE", ShaderAsset::BlendFactor::One},
        {"SRC_COLOR", ShaderAsset::BlendFactor::SrcColor},
        {"ONE_MINUS_SRC_COLOR", ShaderAsset::BlendFactor::OneMinusSrcColor},
        {"DST_COLOR", ShaderAsset::BlendFactor::DstColor},
        {"ONE_MINUS_DST_COLOR", ShaderAsset::BlendFactor::OneMinusDstColor},
        {"SRC_ALPHA", ShaderAsset::BlendFactor::SrcAlpha},
        {"ONE_MINUS_SRC_ALPHA", ShaderAsset::BlendFactor::OneMinusSrcAlpha},
        {"DST_ALPHA", ShaderAsset::BlendFactor::DstAlpha},
        {"ONE_MINUS_DST_ALPHA", ShaderAsset::BlendFactor::OneMinusDstAlpha},
    };

    std::unordered_map<std::string, ShaderAsset::DepthTest> depthTests = {
        {"OFF", ShaderAsset::DepthTest::Off},
        {"ALWAYS", ShaderAsset::DepthTest::Always},
        {"NEVER", ShaderAsset::DepthTest::Never},
        {"LESS", ShaderAsset::DepthTest::Less},
        {"EQUAL", ShaderAsset::DepthTest::Equal},
        {"LESS_EQUAL", ShaderAsset::DepthTest::LessEqual},
        {"GREATER", ShaderAsset::DepthTest::Greater},
        {"NOT_EQUAL", ShaderAsset::DepthTest::NotEqual},
        {"GREATER_EQUAL", ShaderAsset::DepthTest::GreaterEqual},
    };

    std::unordered_map<std::string, ShaderAsset::CullFace> cullModes = {
        {"OFF", ShaderAsset::CullFace::Off},
        {"FRONT", ShaderAsset::CullFace::Front},
        {"BACK", ShaderAsset::CullFace::Back},
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
        {GL_SAMPLER_2D, ShaderAsset::Property::Type::Sampler2D},
    };

    struct ShaderFileData
    {
        std::string vertexSrc;
        std::string fragSrc;
        std::string blendSrcFactor;
        std::string blendDstFactor;
        std::string blendSrcAlphaFactor;
        std::string blendDstAlphaFactor;
        std::string blendEquation;
        std::string blendAlphaEquation;
        std::string depthTest;
        std::string cullFace;
    };

    ShaderFileData ParseShader(std::istream& is)
    {
        ShaderFileData shaderData{};
        std::stringstream vertexSrc;
        std::stringstream fragSrc;

        std::string line;
        while (std::getline(is, line))
        {
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            boost::trim(line);
            if (line.empty())
            {
                continue;
            }
            if (line[0] == '#')
            {
                std::vector<std::string> split;
                boost::split(split, line, boost::is_any_of(" "));
                if (split[0] == "#version")
                {
                    vertexSrc << line << '\n';
                    vertexSrc << "#define PLUTO_VERTEX_SHADER\n";
                    fragSrc << line << '\n';
                    fragSrc << "#define PLUTO_FRAGMENT_SHADER\n";
                }
                else
                {
                    if (split[0] == "#define")
                    {
                        if (split[1] == "PLUTO_BLEND")
                        {
                            if (split.size() == 4)
                            {
                                // Parse blend factors for RGBA
                                shaderData.blendSrcFactor = split[2];
                                shaderData.blendDstFactor = split[3];
                                shaderData.blendSrcAlphaFactor = split[2];
                                shaderData.blendDstAlphaFactor = split[3];
                            }
                            else if (split.size() == 6)
                            {
                                // Parse blend factors for RGB and A separately.
                                shaderData.blendSrcFactor = split[2];
                                shaderData.blendDstFactor = split[3];
                                shaderData.blendSrcAlphaFactor = split[4];
                                shaderData.blendDstAlphaFactor = split[5];
                            }
                            else
                            {
                                throw std::runtime_error("");
                            }
                        }
                        else if (split[1] == "PLUTO_BLEND_EQUATION")
                        {
                            if (split.size() == 3)
                            {
                                shaderData.blendEquation = split[2];
                                shaderData.blendAlphaEquation = split[2];
                            }
                            else if (split.size() == 4)
                            {
                                shaderData.blendEquation = split[2];
                                shaderData.blendAlphaEquation = split[3];
                            }
                            else
                            {
                                throw std::runtime_error("");
                            }
                        }
                        else if (split[1] == "PLUTO_CULL_FACE")
                        {
                            if (split.size() == 3)
                            {
                                shaderData.cullFace = split[2];
                            }
                            else
                            {
                                throw std::runtime_error("");
                            }
                        }
                        else if (split[1] == "PLUTO_DEPTH_TEST")
                        {
                            if (split.size() == 3)
                            {
                                shaderData.depthTest = split[2];
                            }
                            else
                            {
                                throw std::runtime_error("");
                            }
                        }
                    }
                    vertexSrc << line << '\n';
                    fragSrc << line << '\n';
                }
            }
            else
            {
                vertexSrc << line << '\n';
                fragSrc << line << '\n';
            }
        }

        shaderData.vertexSrc = vertexSrc.str();
        shaderData.fragSrc = fragSrc.str();
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

    std::vector<ShaderAsset::Property> FetchAttributes(const GLuint programId)
    {
        std::vector<ShaderAsset::Property> attributes;
        int bufferSize;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufferSize);
        int attributesCount;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &attributesCount);
        auto* name = static_cast<char*>(alloca(sizeof(char) * bufferSize));
        GLuint type;
        int size;
        int nameLength;
        for (int i = 0; i < attributesCount; ++i)
        {
            const unsigned index = i;
            glGetActiveAttrib(programId, index, bufferSize, &nameLength, &size, &type, name);
            attributes.push_back({static_cast<uint8_t>(index), name, propertyTypes.at(type)});
        }
        return attributes;
    }

    ShaderAsset::BlendEquation ParseBlendFunc(const std::string& str)
    {
        const auto it = blendEquations.find(str);
        if (it != blendEquations.end())
        {
            return it->second;
        }
        return ShaderAsset::BlendEquation::Default;
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

    ShaderAsset::DepthTest ParseZTest(const std::string& str)
    {
        const auto it = depthTests.find(str);
        if (it != depthTests.end())
        {
            return it->second;
        }
        return ShaderAsset::DepthTest::Default;
    }

    ShaderAsset::CullFace ParseCull(const std::string& str)
    {
        const auto it = cullModes.find(str);
        if (it != cullModes.end())
        {
            return it->second;
        }
        return ShaderAsset::CullFace::Default;
    }

    ShaderAssetManager::~ShaderAssetManager() = default;

    ShaderAssetManager::
    ShaderAssetManager(FileManager& fileManager, ShaderAsset::Factory& shaderAssetFactory)
        : fileManager(&fileManager),
          shaderAssetFactory(
              &shaderAssetFactory)
    {
    }

    std::unique_ptr<ShaderAsset> ShaderAssetManager::Create(const Path& inputPath, const Path& outputDir)
    {
        Path plutoFilePath = inputPath;
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!fileManager->Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        Guid guid(plutoFile["guid"].as<std::string>());

        auto fr = fileManager->OpenRead(inputPath);
        const ShaderFileData shaderData = ParseShader(fr->GetStream());

        const GLuint programId = CreateShader(shaderData.vertexSrc, shaderData.fragSrc);
        GLint binLength = -1;
        GLenum binFormat = -1;
        glGetProgramiv(programId, GL_PROGRAM_BINARY_LENGTH, &binLength);
        std::vector<uint8_t> programBinary(binLength);
        GLsizei bytesWritten;
        glGetProgramBinary(programId, binLength, &bytesWritten, &binFormat, programBinary.data());

        ShaderAsset::BlendEquation blendEquation = ParseBlendFunc(shaderData.blendEquation);
        ShaderAsset::BlendEquation blendAlphaEquation = ParseBlendFunc(shaderData.blendAlphaEquation);

        ShaderAsset::BlendFactor blendSrcFactor = ParseBlendFactor(shaderData.blendSrcFactor);
        ShaderAsset::BlendFactor blendDstFactor = ParseBlendFactor(shaderData.blendDstFactor);
        ShaderAsset::BlendFactor blendAlphaSrcFactor = ParseBlendFactor(shaderData.blendSrcAlphaFactor);
        ShaderAsset::BlendFactor blendAlphaDstFactor = ParseBlendFactor(shaderData.blendDstFactor);

        ShaderAsset::DepthTest depthTest = ParseZTest(shaderData.depthTest);
        ShaderAsset::CullFace cullFace = ParseCull(shaderData.cullFace);

        std::vector<ShaderAsset::Property> uniforms = FetchUniforms(programId);

        std::vector<ShaderAsset::Property> attributes = FetchAttributes(programId);

        glDeleteProgram(programId);

        auto shaderAsset = shaderAssetFactory->Create();

        // Evil, I know. But it's better than expose the guid to changes directly.
        const_cast<Guid&>(shaderAsset->GetId()) = guid;

        std::string ss = shaderAsset->GetId().Str();

        shaderAsset->SetName(inputPath.GetNameWithoutExtension());
        shaderAsset->SetBlendAlphaEquation(blend);
        shaderAsset->SetSrcBlendFactor(blendSrc);
        shaderAsset->SetDstBlendFactor(blendDst);
        shaderAsset->SetZTest(depthTest);
        shaderAsset->SetCullMode(cullFace);
        shaderAsset->SetProperties(std::move(uniforms));
        shaderAsset->SetBinaryFormat(binFormat);
        shaderAsset->SetBinary(std::move(programBinary));

        const auto fileWriter = fileManager->OpenWrite(Path(outputDir.Str() + "/" + shaderAsset->GetId().Str()));
        shaderAsset->Dump(*fileWriter);
        std::cout << "Asset \"" << shaderAsset->GetName() << "\" saved with id " << shaderAsset->GetId() << "."
            << std::endl;

        return shaderAsset;
    }

    std::unique_ptr<ShaderAsset> ShaderAssetManager::Load(const Path& path)
    {
        const auto fileReader = fileManager->OpenRead(path);
        auto shaderAsset = shaderAssetFactory->Create(*fileReader);
        return shaderAsset;
    }
}

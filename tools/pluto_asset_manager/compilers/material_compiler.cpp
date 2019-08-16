#include "material_compiler.h"

#include "pluto/guid.h"
#include "pluto/asset/material_asset.h"
#include "pluto/asset/shader_asset.h"
#include "pluto/asset/texture_asset.h"
#include "pluto/file/file_reader.h"
#include "pluto/file/file_writer.h"
#include "pluto/file/file_manager.h"
#include "pluto/file/path.h"

#include "pluto/memory/resource.h"

#include "pluto/math/vector4f.h"
#include "pluto/math/matrix4x4.h"

#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <iostream>

namespace pluto::compiler
{
    MaterialCompiler::MaterialCompiler(FileManager& fileManager, MaterialAsset::Factory& materialAssetFactory,
                                       ResourceControl::Factory& resourceControlFactory)
        : fileManager(&fileManager),
          materialAssetFactory(&materialAssetFactory),
          resourceControlFactory(&resourceControlFactory)
    {
    }

    std::vector<std::string> MaterialCompiler::GetExtensions() const
    {
        return {".mat"};
    }

    std::vector<BaseCompiler::CompiledAsset> MaterialCompiler::Compile(const std::string& input,
                                                                       const std::string& outputDir) const
    {
        std::vector<CompiledAsset> assets;

        auto inputPath = Path(input);
        auto plutoFilePath = Path(input);
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!fileManager->Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        Guid guid(plutoFile["guid"].as<std::string>());

        YAML::Node materialFile = YAML::LoadFile(inputPath.Str());
        YAML::Node materialNode = materialFile["material"];
        const auto shaderGuidStr = materialNode["shader"].as<std::string>();
        Guid shaderGuid(shaderGuidStr);
        Resource<ShaderAsset> shader(resourceControlFactory->Create(shaderGuid));
        auto materialAsset = materialAssetFactory->Create(shader);

        const_cast<Guid&>(materialAsset->GetId()) = guid;
        materialAsset->SetName(inputPath.GetNameWithoutExtension());

        YAML::Node floatsNode = materialNode["floats"];
        for (YAML::const_iterator it = floatsNode.begin(); it != floatsNode.end(); ++it)
        {
            materialAsset->SetFloat(it->first.as<std::string>(), it->second.as<float>());
        }

        YAML::Node vectorsNode = materialNode["vectors"];
        for (YAML::const_iterator it = vectorsNode.begin(); it != vectorsNode.end(); ++it)
        {
            Vector4F vector = {
                it->second["x"].as<float>(),
                it->second["y"].as<float>(),
                it->second["z"].as<float>(),
                it->second["w"].as<float>()
            };
            materialAsset->SetVector4F(it->first.as<std::string>(), vector);
        }

        YAML::Node matricesNode = materialNode["matrices"];
        for (YAML::const_iterator it = matricesNode.begin(); it != matricesNode.end(); ++it)
        {
            Matrix4X4 matrix = {
                it->second["x0"].as<float>(),
                it->second["y0"].as<float>(),
                it->second["z0"].as<float>(),
                it->second["w0"].as<float>(),
                it->second["x1"].as<float>(),
                it->second["y1"].as<float>(),
                it->second["z1"].as<float>(),
                it->second["w1"].as<float>(),
                it->second["x2"].as<float>(),
                it->second["y2"].as<float>(),
                it->second["z2"].as<float>(),
                it->second["w2"].as<float>(),
                it->second["x3"].as<float>(),
                it->second["y3"].as<float>(),
                it->second["z3"].as<float>(),
                it->second["w3"].as<float>(),
            };

            materialAsset->SetMatrix4X4(it->first.as<std::string>(), matrix);
        }

        YAML::Node textureNode = materialNode["textures"];
        for (YAML::const_iterator it = textureNode.begin(); it != textureNode.end(); ++it)
        {
            Guid textureGuid(it->second.as<std::string>());
            Resource<TextureAsset> texture(resourceControlFactory->Create(textureGuid));
            materialAsset->SetTexture(it->first.as<std::string>(), texture);
        }

        const auto fileWriter = fileManager->OpenWrite(Path(outputDir + "/" + materialAsset->GetId().Str()));
        materialAsset->Dump(*fileWriter);

        assets.push_back({materialAsset->GetId(), input});
        return assets;
    }
}

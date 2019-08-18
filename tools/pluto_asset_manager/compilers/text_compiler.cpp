#include "text_compiler.h"

#include <pluto/asset/text_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#include <yaml-cpp/yaml.h>

#include <filesystem>

namespace pluto::compiler
{
    TextCompiler::TextCompiler(TextAsset::Factory& textAssetFactory)
        : textAssetFactory(&textAssetFactory)
    {
    }

    std::vector<std::string> TextCompiler::GetExtensions() const
    {
        return {".txt"};
    }

    std::vector<BaseCompiler::CompiledAsset> TextCompiler::Compile(const std::string& input,
                                                                   const std::string& outputDir) const
    {
        const std::string plutoFilePath = Path::ChangeExtension(input, Path::GetExtension(input) + ".pluto");
        if (!FileManager::Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath);
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath);
        const Guid guid(plutoFile["guid"].as<std::string>());

        FileReader fr = FileManager::OpenRead(input);
        const std::string fileContent{
            std::istreambuf_iterator<char>(fr.GetStream()), std::istreambuf_iterator<char>()
        };

        auto textAsset = textAssetFactory->Create();
        textAsset->SetName(Path::GetFileNameWithoutExtension(input));
        textAsset->SetText(fileContent);

        const_cast<Guid&>(textAsset->GetId()) = guid;

        FileWriter fileWriter = FileManager::OpenWrite(Path::Combine({outputDir, textAsset->GetId().Str()}));
        textAsset->Dump(fileWriter);

        std::vector<CompiledAsset> assets;
        assets.push_back({textAsset->GetId(), input});

        return assets;
    }
}

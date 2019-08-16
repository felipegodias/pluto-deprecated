#include "text_compiler.h"

#include <pluto/asset/text_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <iostream>

namespace pluto::compiler
{
    TextCompiler::TextCompiler(FileManager& fileManager, TextAsset::Factory& textAssetFactory)
        : fileManager(&fileManager),
          textAssetFactory(&textAssetFactory)
    {
    }

    std::vector<std::string> TextCompiler::GetExtensions() const
    {
        return {".txt"};
    }

    std::vector<BaseCompiler::CompiledAsset> TextCompiler::Compile(const std::string& input,
                                                                   const std::string& outputDir) const
    {
        auto inputPath = Path(input);
        Path plutoFilePath = inputPath;
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!fileManager->Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        Guid guid(plutoFile["guid"].as<std::string>());

        const auto fr = fileManager->OpenRead(inputPath);
        const std::string fileContent{
            std::istreambuf_iterator<char>(fr->GetStream()), std::istreambuf_iterator<char>()
        };

        auto textAsset = textAssetFactory->Create();
        textAsset->SetName(inputPath.GetNameWithoutExtension());
        textAsset->SetText(fileContent);

        // Evil, I know. But it's better than expose the guid to changes directly.
        const_cast<Guid&>(textAsset->GetId()) = guid;

        const auto fileWriter = fileManager->OpenWrite(Path(outputDir + "/" + textAsset->GetId().Str()));
        textAsset->Dump(*fileWriter);

        std::vector<CompiledAsset> assets;
        assets.push_back({textAsset->GetId(), input});

        return assets;
    }
}

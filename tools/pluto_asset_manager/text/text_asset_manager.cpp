#include "text_asset_manager.h"

#include <pluto/asset/text_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <iostream>

namespace pluto
{
    TextAssetManager::~TextAssetManager() = default;

    TextAssetManager::
    TextAssetManager(FileManager& fileManager, TextAsset::Factory& textAssetFactory)
        : fileManager(&fileManager),
          textAssetFactory(
              &textAssetFactory)
    {
    }

    std::unique_ptr<TextAsset> TextAssetManager::Create(const Path& inputPath, const Path& outputDir)
    {
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

        const auto fileWriter = fileManager->OpenWrite(Path(outputDir.Str() + "/" + textAsset->GetId().Str()));
        textAsset->Dump(*fileWriter);
        std::cout << "Asset \"" << textAsset->GetName() << "\" saved with id " << textAsset->GetId() << "."
            << std::endl;

        return textAsset;
    }

    std::unique_ptr<TextAsset> TextAssetManager::Load(const Path& path)
    {
        const auto fileReader = fileManager->OpenRead(path);
        auto asset = textAssetFactory->Create(*fileReader);
        return std::unique_ptr<TextAsset>(dynamic_cast<TextAsset*>(asset.release()));
    }
}

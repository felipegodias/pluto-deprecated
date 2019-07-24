#include "text_asset_manager.h"

#include <pluto/asset/text_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace pluto
{
    TextAssetManager::~TextAssetManager() = default;

    TextAssetManager::
    TextAssetManager(FileManager& fileManager, TextAsset::Factory& textAssetFactory) : fileManager(&fileManager),
                                                                                       textAssetFactory(
                                                                                           &textAssetFactory)
    {
    }

    std::unique_ptr<TextAsset> TextAssetManager::Create(const Path& path)
    {
        Path plutoFilePath = path;
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!fileManager->Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        Guid guid(plutoFile["guid"].as<std::string>());

        const auto fr = fileManager->OpenRead(path);
        const std::string fileContent{
            std::istreambuf_iterator<char>(fr->GetStream()), std::istreambuf_iterator<char>()
        };

        auto textAsset = textAssetFactory->Create();
        textAsset->SetName(path.GetNameWithoutExtension());
        textAsset->SetText(fileContent);

        // Evil, I know. But it's better than expose the guid to changes directly.
        Guid& shaderGuid = const_cast<Guid&>(textAsset->GetId());
        shaderGuid = guid;

        return textAsset;
    }

    std::unique_ptr<TextAsset> TextAssetManager::Load(const Path& path)
    {
        const auto fileReader = fileManager->OpenRead(path);
        auto textAsset = textAssetFactory->Create(*fileReader);
        return textAsset;
    }
}

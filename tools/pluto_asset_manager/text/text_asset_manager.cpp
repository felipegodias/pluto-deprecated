#include "text_asset_manager.h"

#include <pluto/asset/text_asset.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace pluto
{
    TextAssetManager::~TextAssetManager() = default;

    TextAssetManager::TextAssetManager() : diContainer(std::make_unique<DiContainer>())
    {
        diContainer->AddSingleton(std::make_unique<FileReader::Factory>(*diContainer));
        diContainer->AddSingleton(std::make_unique<FileWriter::Factory>(*diContainer));

        const FileManager::Factory fileManagerFactory(*diContainer);
        diContainer->AddSingleton(fileManagerFactory.Create(Path(std::filesystem::current_path().string())));
        diContainer->AddSingleton(std::make_unique<TextAsset::Factory>(*diContainer));
    }

    std::unique_ptr<TextAsset> TextAssetManager::Create(const Path& path)
    {
        Path plutoFilePath = path;
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!diContainer->GetSingleton<FileManager>().Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        Guid guid(plutoFile["guid"].as<std::string>());

        const auto fr = diContainer->GetSingleton<FileManager>().OpenRead(path);
        const std::string fileContent{
            std::istreambuf_iterator<char>(fr->GetStream()), std::istreambuf_iterator<char>()
        };

        auto textAsset = diContainer->GetSingleton<TextAsset::Factory>().Create();
        textAsset->SetName(path.GetNameWithoutExtension());
        textAsset->SetText(fileContent);

        // Evil, I know. But it's better than expose the guid to changes directly.
        Guid& shaderGuid = const_cast<Guid&>(textAsset->GetId());
        shaderGuid = guid;

        return textAsset;
    }

    std::unique_ptr<TextAsset> TextAssetManager::Load(const Path& path)
    {
        const auto fileReader = diContainer->GetSingleton<FileManager>().OpenRead(path);
        auto textAsset = diContainer->GetSingleton<TextAsset::Factory>().Create(*fileReader);
        return textAsset;
    }
}

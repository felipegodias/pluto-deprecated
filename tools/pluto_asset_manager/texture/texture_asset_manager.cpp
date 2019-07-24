#include "texture_asset_manager.h"

#include <pluto/math/color.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>
#include <pluto/guid.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <yaml-cpp/yaml.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include <iostream>

namespace pluto
{
    TextureAssetManager::
    TextureAssetManager(FileManager& fileManager, TextureAsset::Factory& textureAssetFactory) :
        fileManager(&fileManager), textureAssetFactory(&textureAssetFactory)
    {
    }

    TextureAsset::Format GetTrueColorTextureFormat(const uint8_t channelCount)
    {
        switch (channelCount)
        {
        case 1:
            return TextureAsset::Format::Alpha8;
        case 3:
            return TextureAsset::Format::RGB24;
        case 4:
            return TextureAsset::Format::RGBA32;
        default: ;
        }
        throw std::runtime_error("");
    }

    std::unique_ptr<TextureAsset> TextureAssetManager::Create(const Path& inputPath, const Path& outputDir)
    {
        Path plutoFilePath = inputPath;
        plutoFilePath.ChangeExtension(plutoFilePath.GetExtension() + ".pluto");
        if (!fileManager->Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath.Str());
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath.Str());
        const Guid guid(plutoFile["guid"].as<std::string>());

        int width, height, channels;
        uint8_t* bytes = stbi_load(inputPath.Str().c_str(), &width, &height, &channels, 0);

        auto textureAsset = textureAssetFactory->Create(width, height, GetTrueColorTextureFormat(channels));
        textureAsset->SetName(inputPath.GetName());

        std::vector<Color> colors;
        for (int i = 0; i < width * height * channels; i += channels)
        {
            colors.emplace_back(bytes[i], bytes[i + 1], bytes[i + 2], bytes[i + 3]);
        }
        textureAsset->SetPixels(colors);

        // Evil, I know. But it's better than expose the guid to changes directly.
        const_cast<Guid&>(textureAsset->GetId()) = guid;

        const auto fileWriter = fileManager->OpenWrite(Path(outputDir.Str() + "/" + textureAsset->GetId().Str()));
        textureAsset->Dump(*fileWriter);
        std::cout << "Asset \"" << textureAsset->GetName() << "\" saved with id " << textureAsset->GetId() << "."
            << std::endl;

        return textureAsset;
    }
}

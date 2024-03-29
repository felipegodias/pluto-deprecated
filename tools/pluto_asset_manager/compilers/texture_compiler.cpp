#include "texture_compiler.h"

#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/file/file_stream_writer.h>
#include <pluto/guid.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <yaml-cpp/yaml.h>

#include <fmt/format.h>

namespace pluto::compiler
{
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

    TextureCompiler::TextureCompiler(TextureAsset::Factory& textureAssetFactory)
        : textureAssetFactory(&textureAssetFactory)
    {
    }

    std::vector<std::string> TextureCompiler::GetExtensions() const
    {
        return {".png"};
    }

    std::vector<BaseCompiler::CompiledAsset> TextureCompiler::Compile(const std::string& input,
                                                                      const std::string& outputDir) const
    {
        const std::string plutoFilePath = Path::ChangeExtension(input, Path::GetExtension(input) + ".pluto");
        if (!FileManager::Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath);
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath);
        const Guid guid(plutoFile["guid"].as<std::string>());

        int width, height, channels;
        uint8_t* bytes = stbi_load(input.c_str(), &width, &height, &channels, 0);

        std::vector<uint8_t> data(bytes, bytes + static_cast<size_t>(width) * height * channels);
        auto textureAsset = textureAssetFactory->Create(width, height, GetTrueColorTextureFormat(channels),
                                                        std::move(data));
        textureAsset->SetName(Path::GetFileNameWithoutExtension(input));

        const_cast<Guid&>(textureAsset->GetId()) = guid;

        FileStreamWriter fileWriter = FileManager::OpenWrite(Path::Combine({outputDir, textureAsset->GetId().Str()}));
        textureAsset->Dump(fileWriter);

        std::vector<CompiledAsset> assets;
        assets.push_back({textureAsset->GetId(), input});
        return assets;
    }
}

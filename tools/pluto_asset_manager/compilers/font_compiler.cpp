#include "font_compiler.h"

#include <pluto/file/file_reader.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>

#include <pluto/asset/shader_asset.h>
#include <pluto/memory/resource.h>

#include <yaml-cpp/yaml.h>

#include <array>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace pluto::compiler
{
    FontCompiler::FontCompiler(FontAsset::Factory& fontAssetFactory)
        : fontAssetFactory(&fontAssetFactory)
    {
    }

    std::vector<std::string> FontCompiler::GetExtensions() const
    {
        return {".ttf"};
    }

    std::vector<BaseCompiler::CompiledAsset> FontCompiler::Compile(const std::string& input,
                                                                   const std::string& outputDir) const
    {
        const std::string plutoFilePath = Path::ChangeExtension(input, Path::GetExtension(input) + ".pluto");
        if (!FileManager::Exists(plutoFilePath))
        {
            throw std::runtime_error("Pluto file not found at " + plutoFilePath);
        }

        YAML::Node plutoFile = YAML::LoadFile(plutoFilePath);
        const Guid guid(plutoFile["guid"].as<std::string>());

        YAML::Node subAssetsNode = plutoFile["subAssets"];
        YAML::Node textureAssetNode = subAssetsNode["texture"];
        YAML::Node materialAssetNode = subAssetsNode["material"];

        const Guid textureGuid(textureAssetNode["guid"].as<std::string>());
        const Guid materialGuid(materialAssetNode["guid"].as<std::string>());
        const Guid shaderGuid(materialAssetNode["shader"].as<std::string>());

        std::vector<uint8_t> bytes = FileManager::OpenRead(input).ReadAllBytes();

        const char first = 32; // Space;
        const char last = 127; // Del
        const float fontSize = 64;
        const uint16_t bitmapWidth = 512;
        const uint16_t bitmapHeight = 512;

        std::array<stbtt_bakedchar, last - first + 1> bakedChars{};
        std::vector<uint8_t> bitmap(bitmapWidth * bitmapHeight);
        stbtt_BakeFontBitmap(bytes.data(), 0, fontSize, bitmap.data(), bitmapWidth,
                             bitmapHeight, first,
                             bakedChars.size(),
                             bakedChars.data());

        std::vector<FontAsset::Glyph> glyphs(bakedChars.size());
        for (size_t i = 0; i < glyphs.size(); ++ i)
        {
            glyphs[i].character = static_cast<char>(first + i);
            glyphs[i].xMin = bakedChars[i].x0;
            glyphs[i].yMin = bakedChars[i].y0;
            glyphs[i].xMax = bakedChars[i].x1;
            glyphs[i].yMax = bakedChars[i].y1;
            glyphs[i].xBearing = bakedChars[i].xoff;
            glyphs[i].yBearing = bakedChars[i].yoff;
            glyphs[i].advance = bakedChars[i].xadvance;
        }

        std::unique_ptr<TextureAsset> textureAsset = textureAssetFactory->Create(
            bitmapWidth, bitmapHeight, TextureAsset::Format::Alpha8, bitmap);
        const_cast<Guid&>(textureAsset->GetId()) = textureGuid;
        textureAsset->SetName(Path::GetFileNameWithoutExtension(input) + "-texture");
        Resource<TextureAsset> textureAssetResource(resourceControlFactory->Create(textureAsset->GetId()));

        Resource<ShaderAsset> shaderAsset(resourceControlFactory->Create(shaderGuid));

        std::unique_ptr<MaterialAsset> materialAsset = materialAssetFactory->Create(shaderAsset);
        const_cast<Guid&>(textureAsset->GetId()) = materialGuid;
        materialAsset->SetName(Path::GetFileNameWithoutExtension(input) + "-material");
        materialAsset->SetTexture("mainTex", textureAssetResource);

        Resource<ShaderAsset> materialAssetResource(resourceControlFactory->Create(shaderAsset->GetId()));

        std::unique_ptr<FontAsset> fontAsset = fontAssetFactory->Create(fontSize, glyphs,
                                                                        bitmapWidth, bitmapHeight,
                                                                        bitmap);
        
        fontAsset->SetName(Path::GetFileNameWithoutExtension(input));
        const_cast<Guid&>(fontAsset->GetId()) = guid;

        std::vector<CompiledAsset> assets;
        assets.push_back({fontAsset->GetId(), input});
        assets.push_back({textureAsset->GetId(), Path::Combine({input, "texture.png"})});
        assets.push_back({materialAsset->GetId(), Path::Combine({input, "material.mat"})});
        return assets;
    }
}

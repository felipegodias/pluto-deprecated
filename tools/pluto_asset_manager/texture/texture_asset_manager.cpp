#include "texture_asset_manager.h"

#include <pluto/di/di_container.h>
#include <pluto/math/color.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

namespace pluto
{
    TextureAssetManager::TextureAssetManager(DiContainer& diContainer)
    {
    }

    std::unique_ptr<TextureAsset> TextureAssetManager::Create(const TextureAsset& other)
    {
        return nullptr;
    }

    std::unique_ptr<TextureAsset> TextureAssetManager::Create(const std::string& path)
    {
        int width, height, channels;
        uint8_t* bytes = stbi_load(path.c_str(), &width, &height, &channels, 0);

        std::vector<Color> colors;
        for (int i = 0; i < width * height * channels; i += channels)
        {
            colors.emplace_back(bytes[i], bytes[i + 1], bytes[i + 2], bytes[i + 3]);
            std::cout << colors[colors.size() - 1] << std::endl;
        }
        return nullptr;
    }

    std::unique_ptr<TextureAsset> TextureAssetManager::Load(const Guid& guid)
    {
        return nullptr;
    }
}

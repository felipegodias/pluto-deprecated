#pragma once

#include <pluto/asset/texture_asset.h>

#include <string>
#include <memory>

namespace pluto
{
    class TextureAssetManager
    {
    public:
        explicit TextureAssetManager(DiContainer& diContainer);
        std::unique_ptr<TextureAsset> Create(const TextureAsset& other);
        std::unique_ptr<TextureAsset> Create(const std::string& path);
        std::unique_ptr<TextureAsset> Load(const Guid& guid);
    };
}

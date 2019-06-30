#pragma once

#include <string>
#include <pluto/asset/text_asset.h>

namespace pluto
{
    std::unique_ptr<TextAsset> CreateTextAsset(const TextAsset& other);
    std::unique_ptr<TextAsset> CreateTextAsset(const std::string& path);
    std::unique_ptr<TextAsset> LoadTextAsset(const Guid& guid);
}

#pragma once

#include <string>
#include <pluto/asset/text_asset.h>

namespace pluto
{
    std::unique_ptr<TextAsset> BuildTextAsset(const std::string& path);
}

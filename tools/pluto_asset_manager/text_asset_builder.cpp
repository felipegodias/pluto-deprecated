#include "text_asset_builder.h"
#include <pluto/di/di_container.h>
#include <pluto/asset/text_asset.h>
#include <filesystem>
#include <fstream>

namespace pluto
{
    std::unique_ptr<TextAsset> BuildTextAsset(const std::string& path)
    {
        DiContainer diContainer;
        const TextAsset::Factory factory(diContainer);

        std::ifstream ifs(path, std::ios::binary);
        const std::string fileContent{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

        auto textAsset = factory.Create();
        const std::filesystem::path filePath(path);
        textAsset->SetName(filePath.filename().replace_extension("").string());
        textAsset->SetText(fileContent);
        return textAsset;
    }
}

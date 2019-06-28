#include "text_asset_manager.h"
#include <pluto/di/di_container.h>
#include <pluto/asset/text_asset.h>
#include <pluto/guid.h>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace pluto
{
    std::unique_ptr<TextAsset> CreateTextAsset(const TextAsset& other)
    {
        DiContainer diContainer;
        const TextAsset::Factory factory(diContainer);
        auto textAsset = factory.Create(other);
        return textAsset;
    }

    std::unique_ptr<TextAsset> CreateTextAsset(const std::string& path)
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

    std::unique_ptr<TextAsset> LoadTextAsset(const Guid& guid)
    {
        DiContainer diContainer;
        const TextAsset::Factory factory(diContainer);

        std::stringstream ss;
        ss << guid;
        std::ifstream ifs(ss.str(), std::ios::binary);
        auto textAsset = factory.Create(ifs);
        return textAsset;
    }
}

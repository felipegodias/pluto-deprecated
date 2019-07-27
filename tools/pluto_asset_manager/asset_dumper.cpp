#include "asset_dumper.h"

#include <pluto/service/service_collection.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/path.h>
#include <pluto/asset/asset.h>
#include <pluto/guid.h>
#include <iostream>

namespace pluto
{
    void DumpAsset(const Path& path, const Asset& asset)
    {
        ServiceCollection diContainer;
        diContainer.AddSingleton(std::make_unique<FileWriter::Factory>(diContainer));
        diContainer.AddSingleton(std::make_unique<FileReader::Factory>(diContainer));

        const FileManager::Factory fileManagerFactory(diContainer);
        const std::unique_ptr<FileManager> fileManager = fileManagerFactory.Create(path);

        const std::string guidStr = asset.GetId().Str();

        const auto fileWriter = fileManager->OpenWrite(Path(path.Str() + "/" + guidStr));
        asset.Dump(*fileWriter);

        std::cout << "Asset \"" << asset.GetName() << "\" saved with id " << guidStr << "." << std::endl;
    }
}

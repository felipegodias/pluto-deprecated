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
        ServiceCollection serviceCollection;
        serviceCollection.AddService(std::make_unique<FileWriter::Factory>(serviceCollection));
        serviceCollection.AddService(std::make_unique<FileReader::Factory>(serviceCollection));

        const FileManager::Factory fileManagerFactory(serviceCollection);
        const std::unique_ptr<FileManager> fileManager = fileManagerFactory.Create(path);

        const std::string guidStr = asset.GetId().Str();

        const auto fileWriter = fileManager->OpenWrite(Path(path.Str() + "/" + guidStr));
        asset.Dump(*fileWriter);

        std::cout << "Asset \"" << asset.GetName() << "\" saved with id " << guidStr << "." << std::endl;
    }
}

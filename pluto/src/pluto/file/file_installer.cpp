#include <pluto/file/file_installer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void FileInstaller::Install(const std::string& dataDirectoryName, ServiceCollection& serviceCollection)
    {
        serviceCollection.AddFactory<FileReader>(std::make_unique<FileReader::Factory>(serviceCollection));
        serviceCollection.AddFactory<FileWriter>(std::make_unique<FileWriter::Factory>(serviceCollection));
        serviceCollection.AddService(FileManager::Factory(serviceCollection).Create(Path(dataDirectoryName)));
    }

    void FileInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<FileManager>();
        serviceCollection.RemoveFactory<FileWriter>();
        serviceCollection.RemoveFactory<FileReader>();
    }
}

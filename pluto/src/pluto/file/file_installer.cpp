#include <pluto/file/file_installer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/path.h>
#include <pluto/service/service_collection.h>

namespace pluto
{
    void FileInstaller::Install(const std::string& dataDirectoryName, ServiceCollection& diContainer)
    {
        diContainer.AddService(std::make_unique<FileReader::Factory>(diContainer));
        diContainer.AddService(std::make_unique<FileWriter::Factory>(diContainer));

        const FileManager::Factory factory(diContainer);
        diContainer.AddService(factory.Create(Path(dataDirectoryName)));
    }

    void FileInstaller::Uninstall(ServiceCollection& diContainer)
    {
        diContainer.RemoveService<FileManager>();
        diContainer.RemoveService<FileWriter::Factory>();
        diContainer.RemoveService<FileReader::Factory>();
    }
}

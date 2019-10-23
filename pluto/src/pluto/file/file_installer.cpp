#include "pluto/file/file_installer.h"
#include "pluto/file/file_manager.h"
#include "pluto/file/file_stream_reader.h"
#include "pluto/file/file_stream_writer.h"
#include "pluto/service/service_collection.h"

namespace pluto
{
    void FileInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.EmplaceFactory<FileStreamReader>();
        serviceCollection.EmplaceFactory<FileStreamWriter>();
        serviceCollection.AddService(FileManager::Factory(serviceCollection).Create());
    }

    void FileInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<FileManager>();
        serviceCollection.RemoveFactory<FileStreamWriter>();
        serviceCollection.RemoveFactory<FileStreamReader>();
    }
}

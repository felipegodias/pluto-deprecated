#include <pluto/file/file_installer.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void FileInstaller::Install(const std::string& dataDirectoryName, DiContainer& diContainer)
    {
        diContainer.AddSingleton(std::make_unique<FileReader::Factory>(diContainer));
        diContainer.AddSingleton(std::make_unique<FileWriter::Factory>(diContainer));

        const FileManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create(dataDirectoryName));
    }

    void FileInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<FileManager>();
        diContainer.RemoveSingleton<FileWriter::Factory>();
        diContainer.RemoveSingleton<FileReader::Factory>();
    }
}

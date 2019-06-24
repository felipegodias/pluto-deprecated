#include <pluto/file/file_installer.h>
#include <pluto/file/file_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void FileInstaller::Install(const std::string& dataDirectoryName, DiContainer& diContainer)
    {
        const FileManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create(dataDirectoryName));
    }

    void FileInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<FileManager>();
    }
}

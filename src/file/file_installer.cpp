#include <pluto/file/file_installer.h>
#include <pluto/file/file_manager.h>
#include <pluto/di/di_container.h>

namespace pluto
{
    void FileInstaller::Install(DiContainer& diContainer)
    {
        const FileManager::Factory factory(diContainer);
        diContainer.AddSingleton(factory.Create());
    }

    void FileInstaller::Uninstall(DiContainer& diContainer)
    {
        diContainer.RemoveSingleton<FileManager>();
    }
}

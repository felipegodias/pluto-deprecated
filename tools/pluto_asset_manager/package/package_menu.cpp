#include "package_menu.h"

#include "package_manager.h"

#include <pluto/file/path.h>

#include <iostream>

namespace pluto
{
    PackageMenu::PackageMenu(PackageManager& packageManager, const std::function<void()>& backCallback) :
        packageManager(
            &packageManager),
        mainMenu(MenuOptions(
            "Package"))
    {
        mainMenu.AddOption(0, "Cancel", backCallback);
        mainMenu.AddOption(1, "Create", std::bind(&PackageMenu::OnCreatePackageOptionSelected, this));

        currentMenu = &mainMenu;
    }

    const MenuOptions& PackageMenu::GetCurrentMenuOptions() const
    {
        return *currentMenu;
    }

    void PackageMenu::OnCreatePackageOptionSelected()
    {
        std::cout << "Enter the package directory path: ";
        std::string input;
        std::cin >> input;

        Path path(input);
        packageManager->Create(path);
        std::cout << "" << std::endl;
    }
}

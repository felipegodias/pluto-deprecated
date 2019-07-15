#include "package_menu.h"

#include "package_manager.h"

#include <iostream>

namespace pluto
{
    void PrintPackageMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Package ***" << std::endl;
        std::cout << "1: Create    2: Load    0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void CreatePackageMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Create Package ***" << std::endl;
        std::cout << "Enter the package folder path: ";
        std::string filePath;
        std::cin >> filePath;

        CreatePackage(filePath);
    }

    void PackageMenu()
    {
        int option = 0;
        do
        {
            PrintPackageMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                CreatePackageMenu();
                break;
            case 2:
                //LoadMeshMenu();
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }
}

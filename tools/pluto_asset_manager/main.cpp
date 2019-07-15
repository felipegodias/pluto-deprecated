#include "mesh/mesh_asset_menu.h"
#include "shader/shader_asset_menu.h"
#include "text/text_asset_menu.h"
#include "package/package_menu.h"

#include <iostream>

void PrintMenu()
{
    std::cout << std::endl;
    std::cout << "*** Menu ***" << std::endl;
    std::cout << "1: Text    2: Mesh    3: Shader    4: Package" << std::endl;
    std::cout << "0: Exit" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    int option = 0;
    do
    {
        try
        {
            PrintMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                pluto::TextAssetMenu();
                break;
            case 2:
                pluto::MeshAssetMenu();
                break;
            case 3:
                pluto::ShaderAssetMenu();
                break;
            case 4:
                pluto::PackageMenu();
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

    }
    while (option != 0);
    return 0;
}

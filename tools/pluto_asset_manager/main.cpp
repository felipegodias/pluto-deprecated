#include <iostream>
#include <sstream>
#include <pluto/guid.h>

#include "text_asset_builder.h"

void PrintMenu()
{
    std::cout << std::endl;
    std::cout << "*** Menu ***" << std::endl;
    std::cout << "1: Build Text    2: Build Mesh    3: Build Shader    0: Exit" << std::endl;
    std::cout << std::endl;
}

void BuildTextMenu()
{
    std::cout << std::endl;
    std::cout << "*** Build Text ***" << std::endl;
    std::cout << "Enter the text file path: ";
    std::string filePath;
    std::cin >> filePath;

    const auto textAsset = pluto::BuildTextAsset(filePath);
    std::stringstream ss;
    ss << textAsset->GetId();
    std::ofstream ofs(ss.str());
    textAsset->Dump(ofs);
    std::cout << "Text Asset \"" << textAsset->GetName() << "\" saved with id " << ss.str() << std::endl;
}

int main(int argc, char* argv[])
{
    int option = 0;
    do
    {
        PrintMenu();
        std::cout << "Option: ";
        std::cin >> option;
        switch (option)
        {
        case 0:
            break;
        case 1:
            BuildTextMenu();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            std::cout << "Invalid option." << std::endl;
            break;
        }
    }
    while (option != 0);
    return 0;
}

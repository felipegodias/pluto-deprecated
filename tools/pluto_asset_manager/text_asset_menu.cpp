#include "text_asset_menu.h"
#include "text_asset_manager.h"
#include <pluto/guid.h>
#include <pluto/di/di_container.h>
#include <pluto/file/file_writer.h>
#include <iostream>
#include <sstream>

namespace pluto
{
    void PrintTextAssetMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Text ***" << std::endl;
        std::cout << "1: Create    2: Load    0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void PrintTextAssetLoadMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Text Actions ***" << std::endl;
        std::cout << "1: Print guid    2: Print name    3: Print Text    4: Print All" << std::endl;
        std::cout << "5: Clone         0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void DumpTextAsset(const TextAsset& textAsset)
    {
        DiContainer diContainer;
        FileWriter::Factory fileWriterFactory(diContainer);
        const std::string guidStr = textAsset.GetId().Str();
        std::ofstream ofs(guidStr);
        const auto fileWriter = fileWriterFactory.Create(std::move(ofs));
        textAsset.Dump(*fileWriter);
        std::cout << "Text Asset \"" << textAsset.GetName() << "\" saved with id " << guidStr << std::endl;
    }

    void CreateTextMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Create Text ***" << std::endl;
        std::cout << "Enter the text file path: ";
        std::string filePath;
        std::cin >> filePath;

        const auto textAsset = CreateTextAsset(filePath);
        DumpTextAsset(*textAsset);
    }

    void PrintGuid(const TextAsset& textAsset)
    {
        std::stringstream ss;
        ss << textAsset.GetId();
        std::cout << "Guid: " << ss.str() << std::endl;
    }

    void PrintName(const TextAsset& textAsset)
    {
        std::cout << "Name: " << textAsset.GetName() << std::endl;
    }

    void PrintText(const TextAsset& textAsset)
    {
        std::cout << "Text: " << textAsset.GetText() << std::endl;
    }

    void PrintAll(const TextAsset& textAsset)
    {
        PrintGuid(textAsset);
        PrintName(textAsset);
        PrintText(textAsset);
    }

    void Clone(const TextAsset& textAsset)
    {
        const auto newTextAsset = CreateTextAsset(textAsset);
        DumpTextAsset(*newTextAsset);
    }

    void LoadTextMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Load Text ***" << std::endl;
        std::unique_ptr<TextAsset> textAsset = nullptr;
        std::cout << "Enter the text file guid: ";
        std::string guidStr;
        std::cin >> guidStr;
        try
        {
            const Guid guid(guidStr);
            textAsset = LoadTextAsset(guid);
        }
        catch (std::exception& e)
        {
            std::cout << "Invalid guid or asset does not exist!" << std::endl;
            return;
        }

        int option = 0;
        do
        {
            PrintTextAssetLoadMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                PrintGuid(*textAsset);
                break;
            case 2:
                PrintName(*textAsset);
                break;
            case 3:
                PrintText(*textAsset);
                break;
            case 4:
                PrintAll(*textAsset);
                break;
            case 5:
                Clone(*textAsset);
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }

    void TextAssetMenu()
    {
        int option = 0;
        do
        {
            PrintTextAssetMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                CreateTextMenu();
                break;
            case 2:
                LoadTextMenu();
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }
}

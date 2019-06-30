#include "mesh_asset_menu.h"
#include "mesh_asset_manager.h"

#include <pluto/guid.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/di/di_container.h>
#include <pluto/file/file_writer.h>

#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>

#include <iostream>
#include <sstream>

namespace pluto
{
    void PrintMeshAssetMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Mesh ***" << std::endl;
        std::cout << "1: Create    2: Load    0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void PrintMeshAssetLoadMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Mesh Actions ***" << std::endl;
        std::cout << "1: Print guid         2: Print name    3: Print Positions    4: Print UVs" << std::endl;
        std::cout << "5: Print Triangles    6: Print all     7: Clone              0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void DumpMeshAsset(const MeshAsset& meshAsset)
    {
        DiContainer diContainer;
        const FileWriter::Factory fileWriterFactory(diContainer);
        const std::string guidStr = meshAsset.GetId().Str();
        std::ofstream ofs(guidStr);
        const auto fileWriter = fileWriterFactory.Create(std::move(ofs));
        meshAsset.Dump(*fileWriter);
        std::cout << "Mesh Asset \"" << meshAsset.GetName() << "\" saved with id " << guidStr << std::endl;
    }

    void CreateMeshMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Create Mesh ***" << std::endl;
        std::cout << "Enter the mesh file path: ";
        std::string filePath;
        std::cin >> filePath;

        const auto textAsset = CreateMeshAsset(filePath);
        DumpMeshAsset(*textAsset);
    }

    void PrintGuid(const MeshAsset& meshAsset)
    {
        std::stringstream ss;
        ss << meshAsset.GetId();
        std::cout << "Guid: " << ss.str() << std::endl;
    }

    void PrintName(const MeshAsset& meshAsset)
    {
        std::cout << "Name: " << meshAsset.GetName() << std::endl;
    }

    void PrintPositions(const MeshAsset& meshAsset)
    {
        const std::vector<Vector3F>& positions = meshAsset.GetPositions();
        std::cout << "Positions: " << std::endl;
        for (const auto& position : positions)
        {
            std::cout << position << std::endl;
        }
    }

    void PrintUVs(const MeshAsset& meshAsset)
    {
        const std::vector<Vector2F>& uvs = meshAsset.GetUVs();
        std::cout << "UVs: " << std::endl;
        for (const auto& uv : uvs)
        {
            std::cout << uv << std::endl;
        }
    }

    void PrintTriangles(const MeshAsset& meshAsset)
    {
        const std::vector<Vector3I>& triangles = meshAsset.GetTriangles();
        std::cout << "Triangles: " << std::endl;
        for (const auto& triangle : triangles)
        {
            std::cout << triangle << std::endl;
        }
    }

    void PrintAll(const MeshAsset& meshAsset)
    {
        PrintGuid(meshAsset);
        PrintName(meshAsset);
        PrintPositions(meshAsset);
        PrintUVs(meshAsset);
        PrintTriangles(meshAsset);
    }

    void CloneMeshAsset(const MeshAsset& meshAsset)
    {
        const auto newTextAsset = CreateMeshAsset(meshAsset);
        DumpMeshAsset(*newTextAsset);
    }

    void LoadMeshMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Load Mesh ***" << std::endl;
        std::unique_ptr<MeshAsset> meshAsset = nullptr;
        std::cout << "Enter the mesh asset guid: ";
        std::string guidStr;
        std::cin >> guidStr;
        try
        {
            const Guid guid(guidStr);
            meshAsset = LoadMeshAsset(guid);
        }
        catch (std::exception& e)
        {
            std::cout << "Invalid guid or asset does not exist!" << std::endl;
            return;
        }

        int option = 0;
        do
        {
            PrintMeshAssetLoadMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                PrintGuid(*meshAsset);
                break;
            case 2:
                PrintName(*meshAsset);
                break;
            case 3:
                PrintPositions(*meshAsset);
                break;
            case 4:
                PrintUVs(*meshAsset);
                break;
            case 5:
                PrintTriangles(*meshAsset);
                break;
            case 6:
                PrintAll(*meshAsset);
                break;
            case 7:
                CloneMeshAsset(*meshAsset);
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }

    void MeshAssetMenu()
    {
        int option = 0;
        do
        {
            PrintMeshAssetMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                CreateMeshMenu();
                break;
            case 2:
                LoadMeshMenu();
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }
}

#include "mesh_asset_menu.h"
#include "mesh_asset_manager.h"
#include "../asset_dumper.h"

#include <pluto/guid.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/file/path.h>

#include <pluto/math/vector2f.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>

#include <iostream>

namespace pluto
{
    MeshAssetMenu::~MeshAssetMenu() = default;

    MeshAssetMenu::MeshAssetMenu(MeshAssetManager& meshAssetManager, const std::function<void()>& backCallback) :
        meshAssetManager(&meshAssetManager), mainMenu("Mesh Asset"), manageMenu("Manage Mesh Asset")
    {
        mainMenu.AddOption(0, "Cancel", backCallback);
        mainMenu.AddOption(1, "Create", std::bind(&MeshAssetMenu::OnCreateMeshOptionSelected, this));
        mainMenu.AddOption(2, "Load", std::bind(&MeshAssetMenu::OnManageMeshOptionSelected, this));

        manageMenu.AddOption(0, "Cancel", std::bind(&MeshAssetMenu::OnManageMeshCancelOptionSelected, this));
        manageMenu.AddOption(1, "Show Id", std::bind(&MeshAssetMenu::OnManageMeshShowIdOptionSelected, this));
        manageMenu.AddOption(2, "Show Name", std::bind(&MeshAssetMenu::OnManageMeshShowNameOptionSelected, this));
        manageMenu.AddOption(3, "Show Positions",
                             std::bind(&MeshAssetMenu::OnManageMeshShowPositionsOptionSelected, this));
        manageMenu.AddOption(4, "Show UVs", std::bind(&MeshAssetMenu::OnManageMeshShowUVsOptionSelected, this));
        manageMenu.AddOption(5, "Show Triangles",
                             std::bind(&MeshAssetMenu::OnManageMeshShowTrianglesOptionSelected, this));
        manageMenu.AddOption(6, "Show All", std::bind(&MeshAssetMenu::OnManageMeshShowAllOptionSelected, this));

        currentMenu = &mainMenu;
    }

    const MenuOptions& MeshAssetMenu::GetCurrentMenuOptions() const
    {
        return *currentMenu;
    }

    void MeshAssetMenu::OnCreateMeshOptionSelected()
    {
        std::cout << "Enter the mesh file path: ";
        std::string input;
        std::cin >> input;

        const Path path(input);
        const auto shaderAsset = meshAssetManager->Create(path);
        DumpAsset(path.GetDirectory(), *shaderAsset);
    }

    void MeshAssetMenu::OnManageMeshOptionSelected()
    {
        std::cout << "Enter the mesh asset path: ";
        std::string input;
        std::cin >> input;

        currentManagedAsset = meshAssetManager->Load(Path(input));
        currentMenu = &manageMenu;
    }

    void MeshAssetMenu::OnManageMeshShowIdOptionSelected()
    {
        std::cout << "Id: " << currentManagedAsset->GetId() << std::endl;
    }

    void MeshAssetMenu::OnManageMeshShowNameOptionSelected()
    {
        std::cout << "Name: " << currentManagedAsset->GetName() << std::endl;
    }

    void MeshAssetMenu::OnManageMeshShowPositionsOptionSelected()
    {
        const std::vector<Vector3F>& positions = currentManagedAsset->GetPositions();
        std::cout << "Positions: " << std::endl;
        for (const auto& position : positions)
        {
            std::cout << position << std::endl;
        }
    }

    void MeshAssetMenu::OnManageMeshShowUVsOptionSelected()
    {
        const std::vector<Vector2F>& uvs = currentManagedAsset->GetUVs();
        std::cout << "UVs: " << std::endl;
        for (const auto& uv : uvs)
        {
            std::cout << uv << std::endl;
        }
    }

    void MeshAssetMenu::OnManageMeshShowTrianglesOptionSelected()
    {
        const std::vector<Vector3I>& triangles = currentManagedAsset->GetTriangles();
        std::cout << "Triangles: " << std::endl;
        for (const auto& triangle : triangles)
        {
            std::cout << triangle << std::endl;
        }
    }

    void MeshAssetMenu::OnManageMeshShowAllOptionSelected()
    {
        OnManageMeshShowIdOptionSelected();
        OnManageMeshShowNameOptionSelected();
        OnManageMeshShowPositionsOptionSelected();
        OnManageMeshShowUVsOptionSelected();
        OnManageMeshShowTrianglesOptionSelected();
    }

    void MeshAssetMenu::OnManageMeshCancelOptionSelected()
    {
        currentMenu = &mainMenu;
        currentManagedAsset.reset();
    }
}

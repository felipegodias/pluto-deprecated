#include "material_asset_menu.h"
#include "material_asset_manager.h"

#include "pluto/file/path.h"

#include <iostream>

namespace pluto
{
    MaterialAssetMenu::MaterialAssetMenu(MaterialAssetManager& materialAssetManager,
                                         const std::function<void()>& backCallback)
        : materialAssetManager(&materialAssetManager),
          mainMenu("Material Asset")
    {
        mainMenu.AddOption(0, "Cancel", backCallback);
        mainMenu.AddOption(1, "Create", std::bind(&MaterialAssetMenu::OnCreateMaterialOptionSelected, this));
        currentMenu = &mainMenu;
    }

    const MenuOptions& MaterialAssetMenu::GetCurrentMenuOptions() const
    {
        return *currentMenu;
    }

    void MaterialAssetMenu::OnCreateMaterialOptionSelected()
    {
        std::cout << "Enter the mesh file path: ";
        std::string input;
        std::cin >> input;

        const Path inputPath(input);
        const auto materialAsset = materialAssetManager->Create(inputPath, inputPath.GetDirectory());
    }
}

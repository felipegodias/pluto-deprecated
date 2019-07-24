#include "texture_asset_menu.h"
#include "texture_asset_manager.h"

#include <pluto/file/path.h>

#include <iostream>

namespace pluto
{
    TextureAssetMenu::TextureAssetMenu(TextureAssetManager& textureAssetManager,
                                       const std::function<void()>& backCallback) :
        textureAssetManager(&textureAssetManager),
        mainMenu(MenuOptions("Texture Asset"))
    {
        mainMenu.AddOption(0, "Cancel", backCallback);
        mainMenu.AddOption(1, "Create", std::bind(&TextureAssetMenu::OnCreateTextureOptionSelected, this));

        currentMenu = &mainMenu;
    }

    const MenuOptions& TextureAssetMenu::GetCurrentMenuOptions() const
    {
        return *currentMenu;
    }

    void TextureAssetMenu::OnCreateTextureOptionSelected()
    {
        std::cout << "Enter the texture file path: ";
        std::string input;
        std::cin >> input;

        const Path inputPath(input);
        auto asset = textureAssetManager->Create(inputPath, inputPath.GetDirectory());
    }
}

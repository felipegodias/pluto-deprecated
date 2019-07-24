#include "texture_asset_menu.h"
#include "texture_asset_manager.h"

#include <pluto/di/di_container.h>
#include <iostream>

namespace pluto
{
    TextureAssetMenu::~TextureAssetMenu() = default;

    TextureAssetMenu::TextureAssetMenu(const std::function<void()>& backCallback) : mainMenu(
        MenuOptions("Texture Asset"))
    {
        diContainer = std::make_unique<DiContainer>();
        textureAssetManager = std::make_unique<TextureAssetManager>(*diContainer);

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
        //std::cout << "File Path: ";
        //std::string input;
        //std::cin >> input;
        //textureAssetManager->Create(input);
    }
}

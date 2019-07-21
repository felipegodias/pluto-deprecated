#include "mesh/mesh_asset_menu.h"
#include "shader/shader_asset_menu.h"
#include "text/text_asset_menu.h"
#include "package/package_menu.h"
#include "texture/texture_asset_menu.h"

#include "base_menu.h"
#include "menu_options.h"

#include <iostream>

class MainMenu final : public pluto::BaseMenu
{
    BaseMenu* currentMenu;
    pluto::MenuOptions mainMenu;
    pluto::TextAssetMenu textAssetMenu;
    pluto::TextureAssetMenu textureAssetMenu;

public:
    MainMenu() : mainMenu(pluto::MenuOptions("Main Menu")),
                 textAssetMenu(pluto::TextAssetMenu(std::bind(&MainMenu::SetMainAsCurrent, this))),
                 textureAssetMenu(pluto::TextureAssetMenu(std::bind(&MainMenu::SetMainAsCurrent, this)))
    {
        mainMenu.AddOption(0, "Exit", []()
        {
            exit(0);
        });

        mainMenu.AddOption(1, "Texts", std::bind(&MainMenu::SetTextAsCurrentContext, this));

        //mainMenu.AddOption(5, "Textures", std::bind(&MainMenu::SetTextureAsCurrent, this));

        currentMenu = this;
    }

    void SetMainAsCurrent()
    {
        currentMenu = this;
    }

    void SetTextAsCurrentContext()
    {
        currentMenu = &textAssetMenu;
    }

    void SetTextureAsCurrent()
    {
        //currentMenu = &textureAssetMenu.GetCurrentMenuOptions();
    }

    const pluto::MenuOptions& GetCurrentMenuOptions() const override
    {
        if (currentMenu == this)
        {
            return mainMenu;
        }

        return currentMenu->GetCurrentMenuOptions();
    }
};

int main(int argc, char* argv[])
{
    const MainMenu mainMenu;

    try
    {
        while (true)
        {
            const pluto::MenuOptions& menuOptions = mainMenu.GetCurrentMenuOptions();
            std::cout << menuOptions << std::endl;
            std::cout << "Option: ";
            int option = 0;
            std::cin >> option;
            if (!menuOptions.UseOption(option))
            {
                std::cout << "Invalid Option!" << std::endl;
            }
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

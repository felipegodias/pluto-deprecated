#include "text_asset_menu.h"
#include "text_asset_manager.h"
#include "../asset_dumper.h"

#include <pluto/guid.h>
#include <pluto/file/path.h>
#include <pluto/asset/text_asset.h>

#include <iostream>

namespace pluto
{
    TextAssetMenu::~TextAssetMenu() = default;

    TextAssetMenu::TextAssetMenu(TextAssetManager& textAssetManager, const std::function<void()>& backCallback) :
        textAssetManager(&textAssetManager),
        mainMenu("Text Asset"), manageMenu("Manage Text Asset")
    {
        mainMenu.AddOption(0, "Cancel", backCallback);
        mainMenu.AddOption(1, "Create", std::bind(&TextAssetMenu::OnCreateTextOptionSelected, this));
        mainMenu.AddOption(2, "Load", std::bind(&TextAssetMenu::OnManageTextOptionSelected, this));

        manageMenu.AddOption(0, "Cancel", std::bind(&TextAssetMenu::OnManageTextCancelOptionSelected, this));
        manageMenu.AddOption(1, "Show Id", std::bind(&TextAssetMenu::OnManageTextShowIdOptionSelected, this));
        manageMenu.AddOption(2, "Show Name", std::bind(&TextAssetMenu::OnManageTextShowNameOptionSelected, this));
        manageMenu.AddOption(3, "Show Text", std::bind(&TextAssetMenu::OnManageTextShowTextOptionSelected, this));
        manageMenu.AddOption(4, "Show All", std::bind(&TextAssetMenu::OnManageTextShowAllOptionSelected, this));

        currentMenu = &mainMenu;
    }

    const MenuOptions& TextAssetMenu::GetCurrentMenuOptions() const
    {
        return *currentMenu;
    }

    void TextAssetMenu::OnCreateTextOptionSelected()
    {
        std::cout << "Enter the text file path: ";
        std::string input;
        std::cin >> input;

        const Path inputPath(input);
        const auto textAsset = textAssetManager->Create(inputPath, inputPath.GetDirectory());
    }

    void TextAssetMenu::OnManageTextOptionSelected()
    {
        std::cout << "Enter the text asset path: ";
        std::string input;
        std::cin >> input;

        currentManagedAsset = textAssetManager->Load(Path(input));
        currentMenu = &manageMenu;
    }

    void TextAssetMenu::OnManageTextShowIdOptionSelected()
    {
        std::cout << "Id: " << currentManagedAsset->GetId() << std::endl;
    }

    void TextAssetMenu::OnManageTextShowNameOptionSelected()
    {
        std::cout << "Name: " << currentManagedAsset->GetName() << std::endl;
    }

    void TextAssetMenu::OnManageTextShowTextOptionSelected()
    {
        std::cout << "Text: " << currentManagedAsset->GetText() << std::endl;
    }

    void TextAssetMenu::OnManageTextShowAllOptionSelected()
    {
        OnManageTextShowIdOptionSelected();
        OnManageTextShowNameOptionSelected();
        OnManageTextShowTextOptionSelected();
    }

    void TextAssetMenu::OnManageTextCancelOptionSelected()
    {
        currentMenu = &mainMenu;
        currentManagedAsset.reset();
    }
}

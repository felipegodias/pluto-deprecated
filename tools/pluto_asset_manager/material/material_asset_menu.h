#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>
#include <memory>

namespace pluto
{
    class MaterialAssetManager;
    class MaterialAsset;

    class MaterialAssetMenu final : public BaseMenu
    {
        MaterialAssetManager* materialAssetManager;
        MenuOptions* currentMenu;
        MenuOptions mainMenu;

    public:
        MaterialAssetMenu(MaterialAssetManager& materialAssetManager, const std::function<void()>& backCallback);
        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreateMaterialOptionSelected();
    };
}

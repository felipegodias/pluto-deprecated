#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>

namespace pluto
{
    class TextureAssetManager;

    class TextureAssetMenu final : public BaseMenu
    {
        TextureAssetManager* textureAssetManager;

        MenuOptions* currentMenu;
        MenuOptions mainMenu;

    public:
        TextureAssetMenu(TextureAssetManager& textureAssetManager, const std::function<void()>& backCallback);

        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreateTextureOptionSelected();
    };
}

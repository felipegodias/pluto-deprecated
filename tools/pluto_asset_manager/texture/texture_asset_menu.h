#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>
#include <memory>

namespace pluto
{
    class DiContainer;
    class TextureAssetManager;

    class TextureAssetMenu final : BaseMenu
    {
        std::unique_ptr<DiContainer> diContainer;
        std::unique_ptr<TextureAssetManager> textureAssetManager;

        MenuOptions* currentMenu;
        MenuOptions mainMenu;

    public:
        ~TextureAssetMenu() override;
        explicit TextureAssetMenu(const std::function<void()>& backCallback);

        TextureAssetMenu(const TextureAssetMenu& other) = delete;
        TextureAssetMenu(TextureAssetMenu&& other) noexcept = delete;
        TextureAssetMenu& operator=(const TextureAssetMenu& other) = delete;
        TextureAssetMenu& operator=(TextureAssetMenu&& other) noexcept = delete;

        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreateTextureOptionSelected();
    };
}

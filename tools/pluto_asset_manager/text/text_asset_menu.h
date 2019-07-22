#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>
#include <memory>

namespace pluto
{
    class TextAssetManager;
    class TextAsset;

    class TextAssetMenu final : public BaseMenu
    {
        std::unique_ptr<TextAssetManager> textAssetManager;
        std::unique_ptr<TextAsset> currentManagedAsset;

        MenuOptions* currentMenu;
        MenuOptions mainMenu;
        MenuOptions manageMenu;

    public:
        ~TextAssetMenu() override;
        explicit TextAssetMenu(const std::function<void()>& backCallback);

        TextAssetMenu(const TextAssetMenu& other) = delete;
        TextAssetMenu(TextAssetMenu&& other) noexcept = delete;
        TextAssetMenu& operator=(const TextAssetMenu& rhs) = delete;
        TextAssetMenu& operator=(TextAssetMenu&& rhs) noexcept = delete;

        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreateTextOptionSelected();
        void OnManageTextOptionSelected();

        void OnManageTextShowIdOptionSelected();
        void OnManageTextShowNameOptionSelected();
        void OnManageTextShowTextOptionSelected();
        void OnManageTextShowAllOptionSelected();
        void OnManageTextCancelOptionSelected();
    };
}

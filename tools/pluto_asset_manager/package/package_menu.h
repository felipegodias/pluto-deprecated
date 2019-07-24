#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>

namespace pluto
{
    class PackageManager;
    class TextAssetManager;
    class MeshAssetManager;
    class ShaderAssetManager;

    class PackageMenu final : public BaseMenu
    {
        PackageManager* packageManager;
        TextAssetManager* textAssetManager;
        MeshAssetManager* meshAssetManager;
        ShaderAssetManager* shaderAssetManager;

        MenuOptions* currentMenu;
        MenuOptions mainMenu;

    public:
        ~PackageMenu() override;
        PackageMenu(PackageManager& packageManager, TextAssetManager& textAssetManager,
                    MeshAssetManager& meshAssetManager, ShaderAssetManager& shaderAssetManager,
                    const std::function<void()>& backCallback);

        PackageMenu(const PackageMenu& other) = delete;
        PackageMenu(PackageMenu&& other) noexcept = delete;
        PackageMenu& operator=(const PackageMenu& rhs) = delete;
        PackageMenu& operator=(PackageMenu&& rhs) noexcept = delete;

        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreatePackageOptionSelected();
    };
}

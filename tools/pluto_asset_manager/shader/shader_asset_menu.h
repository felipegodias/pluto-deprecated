#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>
#include <memory>

namespace pluto
{
    class ShaderAssetManager;
    class ShaderAsset;

    class ShaderAssetMenu final : public BaseMenu
    {
        ShaderAssetManager* shaderAssetManager;
        std::unique_ptr<ShaderAsset> currentManagedAsset;

        MenuOptions* currentMenu;
        MenuOptions mainMenu;
        MenuOptions manageMenu;

    public:
        ~ShaderAssetMenu() override;
        ShaderAssetMenu(ShaderAssetManager& shaderAssetManager, const std::function<void()>& backCallback);

        ShaderAssetMenu(const ShaderAssetMenu& other) = delete;
        ShaderAssetMenu(ShaderAssetMenu&& other) noexcept = delete;
        ShaderAssetMenu& operator=(const ShaderAssetMenu& rhs) = delete;
        ShaderAssetMenu& operator=(ShaderAssetMenu&& rhs) noexcept = delete;

        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreateShaderOptionSelected();
        void OnManageShaderOptionSelected();

        void OnManageShaderShowIdOptionSelected();
        void OnManageShaderShowNameOptionSelected();
        void OnManageShaderShowBlendFunctionOptionSelected();
        void OnManageShaderShowSrcFactorOptionSelected();
        void OnManageShaderShowDstFactorOptionSelected();
        void OnManageShaderShowDepthTestOptionSelected();
        void OnManageShaderShowCullModeOptionSelected();
        void OnManageShaderShowPropertiesOptionSelected();
        void OnManageShaderShowAllOptionSelected();
        void OnManageShaderCancelOptionSelected();
    };
}

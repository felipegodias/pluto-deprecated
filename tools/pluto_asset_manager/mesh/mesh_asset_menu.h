#pragma once

#include "../base_menu.h"
#include "../menu_options.h"
#include <functional>
#include <memory>

namespace pluto
{
    class MeshAssetManager;
    class MeshAsset;

    class MeshAssetMenu final : public BaseMenu
    {
        MeshAssetManager* meshAssetManager;
        std::unique_ptr<MeshAsset> currentManagedAsset;

        MenuOptions* currentMenu;
        MenuOptions mainMenu;
        MenuOptions manageMenu;

    public:
        ~MeshAssetMenu() override;
        MeshAssetMenu(MeshAssetManager& meshAssetManager, const std::function<void()>& backCallback);

        MeshAssetMenu(const MeshAssetMenu& other) = delete;
        MeshAssetMenu(MeshAssetMenu&& other) noexcept = delete;
        MeshAssetMenu& operator=(const MeshAssetMenu& rhs) = delete;
        MeshAssetMenu& operator=(MeshAssetMenu&& rhs) noexcept = delete;

        const MenuOptions& GetCurrentMenuOptions() const override;

    private:
        void OnCreateMeshOptionSelected();
        void OnManageMeshOptionSelected();

        void OnManageMeshShowIdOptionSelected();
        void OnManageMeshShowNameOptionSelected();
        void OnManageMeshShowPositionsOptionSelected();
        void OnManageMeshShowUVsOptionSelected();
        void OnManageMeshShowTrianglesOptionSelected();
        void OnManageMeshShowAllOptionSelected();
        void OnManageMeshCancelOptionSelected();
    };
}

#include <pluto/asset/asset_installer.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/package_manifest_asset.h>
#include <pluto/asset/text_asset.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/material_asset.h>
#include <pluto/di/di_container.h>

namespace pluto
{
	void AssetInstaller::Install(DiContainer& diContainer)
	{
		diContainer.AddSingleton(std::make_unique<PackageManifestAsset::Factory>(diContainer));
		diContainer.AddSingleton(std::make_unique<MeshAsset::Factory>(diContainer));
		diContainer.AddSingleton(std::make_unique<TextAsset::Factory>(diContainer));
		diContainer.AddSingleton(std::make_unique<ShaderAsset::Factory>(diContainer));
		diContainer.AddSingleton(std::make_unique<MaterialAsset::Factory>(diContainer));

		const AssetManager::Factory factory(diContainer);
		diContainer.AddSingleton(factory.Create());
	}

	void AssetInstaller::Uninstall(DiContainer& diContainer)
	{
		diContainer.RemoveSingleton<AssetManager>();
		diContainer.RemoveSingleton<MaterialAsset::Factory>();
		diContainer.RemoveSingleton<ShaderAsset::Factory>();
		diContainer.RemoveSingleton<TextAsset::Factory>();
		diContainer.RemoveSingleton<MeshAsset::Factory>();
		diContainer.RemoveSingleton<PackageManifestAsset::Factory>();
	}
}

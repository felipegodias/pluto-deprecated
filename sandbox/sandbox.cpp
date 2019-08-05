#include <pluto/root.h>
#include <pluto/service/service_collection.h>
#include <pluto/asset/material_asset.h>

#include <pluto/scene/scene_manager.h>
#include <pluto/scene/components/camera.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/asset/mesh_asset.h>
#include <pluto/file/path.h>
#include <pluto/scene/components/renderer.h>
#include <pluto/scene/components/mesh_renderer.h>
#include <pluto/scene/scene_manager.h>
#include <pluto/scene/scene.h>
#include <pluto/scene/game_object.h>
#include <pluto/scene/components/transform.h>
#include <pluto/math/vector3f.h>
#include <pluto/memory/resource.h>

#include <iostream>

using namespace pluto;

void OnInit(ServiceCollection& serviceCollection)
{
    auto& assetManager = serviceCollection.GetService<AssetManager>();

    auto& sceneManager = serviceCollection.GetService<SceneManager>();
    sceneManager.LoadEmptyScene();

    Resource<GameObject> cameraGo = sceneManager.GetActiveScene().CreateGameObject("Camera");

    cameraGo->GetTransform()->SetLocalPosition({0, 0, -1});
    Resource<Camera> camera = cameraGo->AddComponent<Camera>();

    const Resource<MeshAsset> meshAsset = assetManager.Load<MeshAsset>(Path("meshes/quad.obj"));

    const Resource<MaterialAsset> material = assetManager.Load<MaterialAsset>(Path("materials/pluto-logo.mat"));

    Resource<GameObject> quadGo = sceneManager.GetActiveScene().CreateGameObject("Transparent");
    quadGo->GetTransform()->SetPosition({0, 0, 0});
    Resource<MeshRenderer> meshRenderer = quadGo->AddComponent<MeshRenderer>();

    meshRenderer->SetMesh(meshAsset);
    meshRenderer->SetMaterial(material);
}

int main(int argc, char* argv[])
{
    const Root pluto("config.yml", "sandbox.log", "C:/Users/felipe.dias/Repos/sandbox");
    int exitCode;
    try
    {
        exitCode = pluto.Run(&OnInit);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        exitCode = 1;
    }
    return exitCode;
}

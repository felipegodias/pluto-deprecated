#include <pluto/physics_2d/physics_2d_installer.h>
#include <pluto/service/service_collection.h>
#include <pluto/physics_2d/physics_2d_manager.h>
#include <pluto/physics_2d/physics_2d_body.h>
#include <pluto/physics_2d/shapes/physics_2d_circle_shape.h>

namespace pluto
{
    void Physics2DInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.EmplaceFactory<Physics2DBody, Physics2DBody::Factory>(serviceCollection);
        serviceCollection.EmplaceFactory<Physics2DCircleShape, Physics2DCircleShape::Factory>(serviceCollection);
        serviceCollection.AddService<Physics2DManager>(Physics2DManager::Factory(serviceCollection).Create());
    }

    void Physics2DInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveFactory<Physics2DBody>();
        serviceCollection.RemoveFactory<Physics2DCircleShape>();
        serviceCollection.RemoveService<Physics2DManager>();
    }
}

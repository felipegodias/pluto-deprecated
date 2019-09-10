#include <pluto/physics_2d/physics_2d_installer.h>
#include <pluto/service/service_collection.h>
#include <pluto/physics_2d/physics_2d_manager.h>
#include <pluto/physics_2d/physics_2d_body.h>
#include <pluto/physics_2d/components/circle_collider_2d.h>
#include <pluto/physics_2d/components/rigidbody_2d.h>
#include <pluto/physics_2d/shapes/physics_2d_circle_shape.h>

namespace pluto
{
    void Physics2DInstaller::Install(ServiceCollection& serviceCollection)
    {
        serviceCollection.EmplaceFactory<Physics2DBody>();
        serviceCollection.EmplaceFactory<Physics2DCircleShape>();
        serviceCollection.EmplaceFactory<Rigidbody2D>();
        serviceCollection.EmplaceFactory<CircleCollider2D>();
        serviceCollection.AddService<Physics2DManager>(Physics2DManager::Factory(serviceCollection).Create());
    }

    void Physics2DInstaller::Uninstall(ServiceCollection& serviceCollection)
    {
        serviceCollection.RemoveService<Physics2DManager>();
        serviceCollection.RemoveFactory<CircleCollider2D>();
        serviceCollection.RemoveFactory<Rigidbody2D>();
        serviceCollection.RemoveFactory<Physics2DCircleShape>();
        serviceCollection.RemoveFactory<Physics2DBody>();
    }
}

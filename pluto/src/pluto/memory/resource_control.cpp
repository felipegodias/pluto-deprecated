#include "pluto/memory/resource_control.h"
#include "pluto/memory/memory_manager.h"
#include "pluto/memory/object.h"
#include "pluto/service/service_collection.h"
#include "pluto/guid.h"

namespace pluto
{
    class ResourceControl::Impl
    {
        Guid objectId;

        MemoryManager* memoryManager;

    public:
        Impl(const Guid& objectId, MemoryManager& memoryManager)
            : objectId(objectId),
              memoryManager(&memoryManager)
        {
        }

        const Guid& GetObjectId() const
        {
            return objectId;
        }

        Object* Get() const
        {
            return memoryManager->GetPtr(objectId);
        }
    };

    ResourceControl::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<ResourceControl> ResourceControl::Factory::Create(const Object& object) const
    {
        return Create(object.GetId());
    }

    std::unique_ptr<ResourceControl> ResourceControl::Factory::Create(const Guid& objectId) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        return std::make_unique<ResourceControl>(std::make_unique<Impl>(objectId, memoryManager));
    }

    ResourceControl::~ResourceControl() = default;

    ResourceControl::ResourceControl(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    ResourceControl::ResourceControl(ResourceControl&& other) noexcept = default;

    ResourceControl& ResourceControl::operator=(ResourceControl&& rhs) noexcept = default;

    const Guid& ResourceControl::GetObjectId() const
    {
        return impl->GetObjectId();
    }

    Object* ResourceControl::Get() const
    {
        return impl->Get();
    }
}

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
        std::weak_ptr<Object> object;

        MemoryManager* memoryManager;

    public:
        Impl(const Guid& objectId, const std::shared_ptr<Object>& object, MemoryManager& memoryManager)
            : objectId(objectId),
              object(object),
              memoryManager(&memoryManager)
        {
        }

        const Guid& GetObjectId() const
        {
            return objectId;
        }

        Object* Get()
        {
            if (object.expired())
            {
                object = memoryManager->GetPtr(objectId);
            }
            return object.lock().get();
        }
    };

    ResourceControl::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<ResourceControl> ResourceControl::Factory::Create(const std::shared_ptr<Object>& object) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        return std::make_unique<ResourceControl>(std::make_unique<Impl>(object->GetId(), object, memoryManager));
    }

    std::unique_ptr<ResourceControl> ResourceControl::Factory::Create(const Guid& objectId) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        return std::make_unique<ResourceControl>(std::make_unique<Impl>(objectId, nullptr, memoryManager));
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

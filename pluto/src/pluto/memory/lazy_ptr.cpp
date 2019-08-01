#include "pluto/memory/lazy_ptr.h"
#include "pluto/memory/memory_manager.h"
#include "pluto/memory/object.h"
#include "pluto/service/service_collection.h"
#include "pluto/guid.h"

namespace pluto
{
    class LazyPtr::Impl
    {
        size_t instanceId;
        Guid objectId;

        MemoryManager* memoryManager;

    public:

        Impl(const size_t instanceId, const Guid& objectId, MemoryManager& memoryManager)
            : instanceId(instanceId),
              objectId(objectId),
              memoryManager(&memoryManager)
        {
        }

        const Guid& GetObjectId() const
        {
            return objectId;
        }

        Object* LazyPtr::Get() const
        {
            return memoryManager->Get(instanceId);
        }
    };

    LazyPtr::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<LazyPtr> LazyPtr::Factory::Create(const size_t instanceId, const Object& object) const
    {
        return Create(instanceId, object.GetId());
    }

    std::unique_ptr<LazyPtr> LazyPtr::Factory::Create(const size_t instanceId, const Guid& objectId) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        return std::make_unique<LazyPtr>(std::make_unique<Impl>(instanceId, objectId, memoryManager));
    }

    LazyPtr::~LazyPtr() = default;

    LazyPtr::LazyPtr(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    LazyPtr::LazyPtr(LazyPtr&& other) noexcept = default;

    LazyPtr& LazyPtr::operator=(LazyPtr&& rhs) noexcept = default;

    const Guid& LazyPtr::GetObjectId() const
    {
        return impl->GetObjectId();
    }

    Object* LazyPtr::Get() const
    {
        return impl->Get();
    }
}

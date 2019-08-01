#include "pluto/memory/lazy_ptr.h"
#include "pluto/memory/memory_manager.h"
#include "pluto/memory/object.h"
#include "pluto/service/service_collection.h"
#include "pluto/guid.h"

namespace pluto
{
    class LazyPtr::Impl
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
            return memoryManager->Get(objectId);
        }
    };

    LazyPtr::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<LazyPtr> LazyPtr::Factory::Create(const Object& object) const
    {
        return Create(object.GetId());
    }

    std::unique_ptr<LazyPtr> LazyPtr::Factory::Create(const Guid& objectId) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& memoryManager = serviceCollection.GetService<MemoryManager>();
        return std::make_unique<LazyPtr>(std::make_unique<Impl>(objectId, memoryManager));
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

#include "pluto/memory/memory_manager.h"
#include "pluto/memory/lazy_ptr.h"
#include "pluto/memory/object.h"

#include "pluto/service/service_collection.h"
#include "pluto/log/log_manager.h"
#include "pluto/guid.h"
#include "pluto/exception.h"

#include <unordered_map>
#include <memory>

namespace pluto
{
    class MemoryManager::Impl
    {
        std::unordered_map<Guid, std::unique_ptr<Object>> objects;

        LogManager* logManager;
        LazyPtr::Factory* lazyPtrFactory;

    public:
        ~Impl()
        {
            logManager->LogInfo("MemoryManager terminated!");
        }

        Impl(LogManager& logManager, LazyPtr::Factory& lazyPtrFactory)
            : logManager(&logManager),
              lazyPtrFactory(&lazyPtrFactory)
        {
            logManager.LogInfo("MemoryManager initialized!");
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& other) = delete;

        Impl& operator=(Impl&& other) noexcept = default;

        Object& Add(std::unique_ptr<Object> object)
        {
            const auto it = objects.find(object->GetId());
            if (it != objects.end())
            {
                Exception::Throw(std::runtime_error("Object with id already exists in memory manager."));
            }

            auto lazyPtr = lazyPtrFactory->Create(*object);
            object->SetPtr(std::shared_ptr<LazyPtr>(lazyPtr.release()));
            Object& out = *object;
            objects.emplace(object->GetId(), std::move(object));
            return out;
        }

        void Remove(const Guid& objectId)
        {
            const auto it = objects.find(objectId);
            if (it != objects.end())
            {
                objects.erase(it);
            }
        }

        Object* Get(const Guid& objectId)
        {
            const auto it = objects.find(objectId);
            if (it == objects.end())
            {
                return it->second.get();
            }
            return nullptr;
        }
    };

    MemoryManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<MemoryManager> MemoryManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetService<LogManager>();
        auto& lazyPtrFactory = serviceCollection.GetService<LazyPtr::Factory>();

        return std::make_unique<MemoryManager>(std::make_unique<Impl>(logManager, lazyPtrFactory));
    }

    MemoryManager::~MemoryManager() = default;

    MemoryManager::MemoryManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    MemoryManager::MemoryManager(MemoryManager&& other) noexcept = default;

    MemoryManager& MemoryManager::operator=(MemoryManager&& rhs) noexcept = default;

    Object& MemoryManager::Add(std::unique_ptr<Object> object)
    {
        return impl->Add(std::move(object));
    }

    void MemoryManager::Remove(const Guid& objectId)
    {
        impl->Remove(objectId);
    }

    Object* MemoryManager::Get(const Guid& objectId)
    {
        return impl->Get(objectId);
    }
}

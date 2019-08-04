#include "pluto/memory/memory_manager.h"
#include "pluto/memory/resource_control.h"
#include "pluto/memory/resource.h"
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
        std::unordered_map<Guid, std::shared_ptr<Object>> objects;

        LogManager* logManager;
        ResourceControl::Factory* resourceControlFactory;

    public:
        ~Impl()
        {
            logManager->LogInfo("MemoryManager terminated!");
        }

        Impl(LogManager& logManager, ResourceControl::Factory& resourceControlFactory)
            : logManager(&logManager),
              resourceControlFactory(&resourceControlFactory)
        {
            logManager.LogInfo("MemoryManager initialized!");
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = default;

        Impl& operator=(const Impl& other) = delete;

        Impl& operator=(Impl&& other) noexcept = default;

        Resource<Object> Add(std::unique_ptr<Object> object)
        {
            const auto it = objects.find(object->GetId());
            if (it != objects.end())
            {
                Exception::Throw(std::runtime_error("Object with id already exists in memory manager."));
            }

            std::shared_ptr<Object> ptr(object.release());
            Resource<Object> resource(resourceControlFactory->Create(ptr));
            objects.emplace(ptr->GetId(), ptr);
            return resource;
        }

        Resource<Object> Get(const Guid& objectId) const
        {
            const std::shared_ptr<Object> object = GetPtr(objectId);
            if (object == nullptr)
            {
                return nullptr;
            }

            return Resource<Object>(resourceControlFactory->Create(object));
        }

        void Remove(const Object& object)
        {
            const auto it = objects.find(object.GetId());
            if (it != objects.end())
            {
                objects.erase(it);
            }
        }

        std::shared_ptr<Object> GetPtr(const Guid& objectId) const
        {
            const auto it = objects.find(objectId);
            if (it == objects.end())
            {
                return nullptr;
            }
            return it->second;
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
        auto& resourceControlFactory = serviceCollection.GetService<ResourceControl::Factory>();

        return std::make_unique<MemoryManager>(std::make_unique<Impl>(logManager, resourceControlFactory));
    }

    MemoryManager::~MemoryManager() = default;

    MemoryManager::MemoryManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    MemoryManager::MemoryManager(MemoryManager&& other) noexcept = default;

    MemoryManager& MemoryManager::operator=(MemoryManager&& rhs) noexcept = default;

    Resource<Object> MemoryManager::Add(std::unique_ptr<Object> object)
    {
        return impl->Add(std::move(object));
    }

    Resource<Object> MemoryManager::Get(const Guid& objectId) const
    {
        return impl->Get(objectId);
    }

    void MemoryManager::Remove(const Object& object)
    {
        impl->Remove(object);
    }

    std::shared_ptr<Object> MemoryManager::GetPtr(const Guid& objectId) const
    {
        return impl->GetPtr(objectId);
    }
}

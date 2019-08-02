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
        std::unordered_map<Guid, std::unique_ptr<Object>> objects;

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

        std::shared_ptr<Resource<Object>> Add(std::unique_ptr<Object> object)
        {
            const auto it = objects.find(object->GetId());
            if (it != objects.end())
            {
                Exception::Throw(std::runtime_error("Object with id already exists in memory manager."));
            }

            auto resource = std::make_shared<Resource<Object>>(resourceControlFactory->Create(*object));
            objects.emplace(object->GetId(), std::move(object));
            return resource;
        }

        std::shared_ptr<Resource<Object>> Get(const Guid& objectId) const
        {
            Object* object = GetPtr(objectId);
            if (object == nullptr)
            {
                return nullptr;
            }

            return std::make_shared<Resource<Object>>(resourceControlFactory->Create(*object));
        }

        void Remove(const Guid& objectId)
        {
            const auto it = objects.find(objectId);
            if (it != objects.end())
            {
                objects.erase(it);
            }
        }

        Object* GetPtr(const Guid& objectId) const
        {
            const auto it = objects.find(objectId);
            if (it == objects.end())
            {
                return nullptr;
            }
            return it->second.get();
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

    std::shared_ptr<Resource<Object>> MemoryManager::Add(std::unique_ptr<Object> object)
    {
        return impl->Add(std::move(object));
    }

    std::shared_ptr<Resource<Object>> MemoryManager::Get(const Guid& objectId) const
    {
        return impl->Get(objectId);
    }

    void MemoryManager::Remove(const Guid& objectId)
    {
        impl->Remove(objectId);
    }

    Object* MemoryManager::GetPtr(const Guid& objectId) const
    {
        return impl->GetPtr(objectId);
    }
}

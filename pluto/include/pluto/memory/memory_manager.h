#pragma once

#include "pluto/api.h"
#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Object;
    class Guid;

    template <typename T>
    using IsObject = std::enable_if_t<std::is_base_of_v<Object, T>, int>;

    template <typename T, IsObject<T>  = 0>
    class Resource;

    class PLUTO_API MemoryManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<MemoryManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~MemoryManager();
        explicit MemoryManager(std::unique_ptr<Impl> impl);

        MemoryManager(const MemoryManager& other) = delete;
        MemoryManager(MemoryManager&& other) noexcept;
        MemoryManager& operator=(const MemoryManager& rhs) = delete;
        MemoryManager& operator=(MemoryManager&& rhs) noexcept;

        std::shared_ptr<Resource<Object>> Add(std::unique_ptr<Object> object);
        std::shared_ptr<Resource<Object>> Get(const Guid& objectId) const;
        void Remove(const Guid& objectId);
        Object* GetPtr(const Guid& objectId) const;
    };
}

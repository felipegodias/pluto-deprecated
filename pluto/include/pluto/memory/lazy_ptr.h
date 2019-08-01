#pragma once

#include "pluto/api.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Object;
    class Guid;

    class PLUTO_API LazyPtr
    {
    public:
        class Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<LazyPtr> Create(size_t instanceId, const Object& object) const;
            std::unique_ptr<LazyPtr> Create(size_t instanceId, const Guid& id) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~LazyPtr();
        explicit LazyPtr(std::unique_ptr<Impl> impl);

        LazyPtr(const LazyPtr& other) = delete;
        LazyPtr(LazyPtr&& other) noexcept;
        LazyPtr& operator=(const LazyPtr& rhs) = delete;
        LazyPtr& operator=(LazyPtr&& rhs) noexcept;

        const Guid& GetObjectId() const;

        Object* Get() const;
    };
}

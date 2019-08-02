#pragma once

#include "pluto/api.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class Object;
    class Guid;

    class PLUTO_API ResourceControl
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<ResourceControl> Create(const Object& object) const;
            std::unique_ptr<ResourceControl> Create(const Guid& objectId) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~ResourceControl();
        explicit ResourceControl(std::unique_ptr<Impl> impl);

        ResourceControl(const ResourceControl& other) = delete;
        ResourceControl(ResourceControl&& other) noexcept;
        ResourceControl& operator=(const ResourceControl& rhs) = delete;
        ResourceControl& operator=(ResourceControl&& rhs) noexcept;

        const Guid& GetObjectId() const;

        Object* Get() const;
    };
}

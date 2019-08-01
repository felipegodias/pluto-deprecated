#pragma once

#include "pluto/api.h"
#include <memory>

namespace pluto
{
    class Guid;
    class LazyPtr;

    class PLUTO_API Object
    {
    public:
        virtual ~Object() = 0;
        Object();

        Object(const Object& other) = delete;
        Object(Object&& other) noexcept;
        Object& operator=(const Object& rhs) = delete;
        Object& operator=(Object&& rhs) noexcept;

        bool operator==(const Object& rhs) const;
        bool operator!=(const Object& rhs) const;

        virtual const Guid& GetId() const = 0;

        virtual std::shared_ptr<LazyPtr> GetPtr() const = 0;
        virtual void SetPtr(std::shared_ptr<LazyPtr> value) = 0;
    };
}

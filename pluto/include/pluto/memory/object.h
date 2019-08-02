#pragma once

#include "pluto/api.h"
#include <string>

namespace pluto
{
    class Guid;
    class ResourceControl;

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
        virtual const std::string& GetName() const = 0;
        virtual void SetName(const std::string& value) = 0;
    };
}

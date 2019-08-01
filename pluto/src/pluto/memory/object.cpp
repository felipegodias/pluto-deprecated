#include "pluto/memory/object.h"
#include "pluto/guid.h"

namespace pluto
{
    Object::Object() = default;

    Object::Object(Object&& other) noexcept = default;

    Object& Object::operator=(Object&& rhs) noexcept = default;

    bool Object::operator==(const Object& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    bool Object::operator!=(const Object& rhs) const
    {
        return !(*this == rhs);
    }
}

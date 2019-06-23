#include <pluto/guid.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_hash.hpp>

namespace pluto
{
    static boost::uuids::random_generator uuidRandomGenerator;

    inline boost::uuids::uuid& ToBoost(Guid& v)
    {
        return reinterpret_cast<boost::uuids::uuid&>(v);
    }

    inline const boost::uuids::uuid& ToBoost(const Guid& v)
    {
        return reinterpret_cast<const boost::uuids::uuid&>(v);
    }

    inline Guid& FromBoost(boost::uuids::uuid& v)
    {
        return reinterpret_cast<Guid&>(v);
    }

    inline const Guid& FromBoost(const boost::uuids::uuid& v)
    {
        return reinterpret_cast<const Guid&>(v);
    }

    Guid::Guid() : data()
    {
        const boost::uuids::uuid uuid = uuidRandomGenerator();
        *this = FromBoost(uuid);
    }

    Guid::Guid(const std::array<uint8_t, 16>& data) : data(data)
    {
    }

    Guid::Guid(const Guid& other) : data(other.data)
    {
    }

    Guid::Guid(Guid&& other) noexcept : data(other.data)
    {
    }

    Guid& Guid::operator=(const Guid& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        data = rhs.data;
        return *this;
    }

    Guid& Guid::operator=(Guid&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        data = rhs.data;
        return *this;
    }

    bool Guid::operator==(const Guid& rhs) const
    {
        return ToBoost(*this) == ToBoost(rhs);
    }

    bool Guid::operator!=(const Guid& rhs) const
    {
        return !(*this == rhs);
    }

    std::ostream& operator<<(std::ostream& os, const Guid& guid)
    {
        return os;
    }

}

size_t std::hash<pluto::Guid>::operator()(const pluto::Guid& guid) const noexcept
{
    return hash_value(ToBoost(guid));
}
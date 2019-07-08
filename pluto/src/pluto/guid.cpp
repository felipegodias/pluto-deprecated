#include <pluto/guid.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_hash.hpp>

#include <sstream>

namespace pluto
{
    static boost::uuids::random_generator uuidRandomGenerator;
    static boost::uuids::string_generator uuidStringGenerator;

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
    }

    Guid::Guid(const std::array<uint8_t, 16>& data) : data(data)
    {
    }

    Guid::Guid(const std::string& guid)
    {
        const boost::uuids::uuid uuid = uuidStringGenerator(guid);
        *this = FromBoost(uuid);
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
        os << to_string(ToBoost(guid));
        return os;
    }

    std::string Guid::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    Guid Guid::New()
    {
        const boost::uuids::uuid uuid = uuidRandomGenerator();
        return FromBoost(uuid);
    }
}

size_t std::hash<pluto::Guid>::operator()(const pluto::Guid& guid) const noexcept
{
    return hash_value(ToBoost(guid));
}

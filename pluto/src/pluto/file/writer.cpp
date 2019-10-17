#include "pluto/file/writer.h"

namespace pluto
{
    Writer::~Writer() = default;

    Writer::Writer(Writer&& other) noexcept = default;

    Writer& Writer::operator=(Writer&& rhs) noexcept = default;

    Writer::Writer() = default;
}

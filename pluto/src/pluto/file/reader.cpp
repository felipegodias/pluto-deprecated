#include "pluto/file/reader.h"

namespace pluto
{
    Reader::~Reader() = default;

    Reader::Reader(Reader&& other) noexcept = default;

    Reader& Reader::operator=(Reader&& rhs) noexcept = default;

    Reader::Reader() = default;
}

#include "pluto/file/stream_reader.h"

namespace pluto
{
    StreamReader::~StreamReader() = default;

    StreamReader::StreamReader(StreamReader&& other) noexcept = default;

    StreamReader& StreamReader::operator=(StreamReader&& rhs) noexcept = default;

    StreamReader::StreamReader() = default;
}

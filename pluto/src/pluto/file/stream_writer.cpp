#include "pluto/file/stream_writer.h"

namespace pluto
{
    StreamWriter::~StreamWriter() = default;

    StreamWriter::StreamWriter(StreamWriter&& other) noexcept = default;

    StreamWriter& StreamWriter::operator=(StreamWriter&& rhs) noexcept = default;

    StreamWriter::StreamWriter() = default;
}

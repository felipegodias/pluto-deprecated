#include "pluto/file/stream.h"

namespace pluto
{
    Stream::~Stream() = default;

    Stream::Stream(Stream&& other) noexcept = default;

    Stream& Stream::operator=(Stream&& rhs) noexcept = default;

    Stream::Stream() = default;

    size_t Stream::GetSize()
    {
        return {};
    }

    size_t Stream::GetReadPosition()
    {
        return {};
    }

    void Stream::SetReadPosition(size_t position)
    {
    }

    void Stream::Read(void* ptr, size_t size)
    {
    }

    std::vector<uint8_t> Stream::ReadAllBytes()
    {
        return {};
    }

    std::string Stream::ReadAllText()
    {
        return {};
    }

    size_t Stream::GetWritePosition()
    {
        return {};
    }

    void Stream::SetWritePosition(size_t position)
    {
    }

    void Stream::Write(const void* ptr, size_t size)
    {
    }

    void Stream::Write(const std::vector<uint8_t>& bytes)
    {
    }

    void Stream::Write(const std::string& str)
    {
    }

    void Stream::Flush()
    {
    }
}

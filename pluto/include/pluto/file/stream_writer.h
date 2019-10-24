#pragma once

#include "pluto/api.h"

#include <vector>
#include <string>

namespace pluto
{
    class PLUTO_API StreamWriter
    {
    public:
        virtual ~StreamWriter() = 0;

        StreamWriter(const StreamWriter& other) = delete;
        StreamWriter(StreamWriter&& other) noexcept;
        StreamWriter& operator=(const StreamWriter& rhs) = delete;
        StreamWriter& operator=(StreamWriter&& rhs) noexcept;

        virtual size_t GetSize() = 0;
        virtual size_t GetWritePosition() = 0;
        virtual void SetWritePosition(size_t position) = 0;
        virtual void Write(const void* ptr, size_t size) = 0;
        virtual void Write(const std::vector<uint8_t>& bytes) = 0;
        virtual void Write(const std::string& str) = 0;
        virtual void Flush() = 0;

    protected:
        StreamWriter();
    };
}

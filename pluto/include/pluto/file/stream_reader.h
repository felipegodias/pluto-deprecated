#pragma once

#include "pluto/api.h"

#include <vector>
#include <string>

namespace pluto
{
    class PLUTO_API StreamReader
    {
    public:
        virtual ~StreamReader() = 0;

        StreamReader(const StreamReader& other) = delete;
        StreamReader(StreamReader&& other) noexcept;
        StreamReader& operator=(const StreamReader& rhs) = delete;
        StreamReader& operator=(StreamReader&& rhs) noexcept;

        virtual size_t GetSize() = 0;
        virtual size_t GetPosition() = 0;
        virtual void SetPosition(size_t position) = 0;
        virtual void Read(void* ptr, size_t size) = 0;
        virtual std::vector<uint8_t> ReadAllBytes() = 0;
        virtual std::string ReadAllText() = 0;

    protected:
        StreamReader();
    };
}

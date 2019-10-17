#pragma once

#include "pluto/api.h"

#include <vector>
#include <string>

namespace pluto
{
    class PLUTO_API Writer
    {
    public:
        virtual ~Writer() = 0;

        Writer(const Writer& other) = delete;
        Writer(Writer&& other) noexcept;
        Writer& operator=(const Writer& rhs) = delete;
        Writer& operator=(Writer&& rhs) noexcept;

        virtual size_t GetSize() = 0;
        virtual size_t GetPosition() = 0;
        virtual void SetPosition(size_t position) = 0;
        virtual void Write(const void* ptr, size_t size) = 0;
        virtual void Write(const std::vector<uint8_t>& bytes) = 0;
        virtual void Write(const std::string& str) = 0;
        virtual void Flush() = 0;

    protected:
        Writer();
    };
}

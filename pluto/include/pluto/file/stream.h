#pragma once

#include "stream_reader.h"
#include "stream_writer.h"

namespace pluto
{
    class PLUTO_API Stream : public StreamReader, public StreamWriter
    {
    public:
        virtual ~Stream() = 0;

        Stream(const Stream& other) = delete;
        Stream(Stream&& other) noexcept;
        Stream& operator=(const Stream& rhs) = delete;
        Stream& operator=(Stream&& rhs) noexcept;

        size_t GetSize() override;
        size_t GetReadPosition() override;
        void SetReadPosition(size_t position) override;
        void Read(void* ptr, size_t size) override;
        std::vector<uint8_t> ReadAllBytes() override;
        std::string ReadAllText() override;

        size_t GetWritePosition() override;
        void SetWritePosition(size_t position) override;
        void Write(const void* ptr, size_t size) override;
        void Write(const std::vector<uint8_t>& bytes) override;
        void Write(const std::string& str) override;
        void Flush() override;
    protected:
        Stream();
    };
}

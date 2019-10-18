#pragma once

#include "stream_writer.h"

#include <fstream>

namespace pluto
{
    class PLUTO_API FileStreamWriter final : public StreamWriter
    {
        std::ofstream ofs;

    public:
        ~FileStreamWriter() override;
        explicit FileStreamWriter(std::ofstream ofs);

        FileStreamWriter(const FileStreamWriter& other) = delete;
        FileStreamWriter(FileStreamWriter&& other) noexcept;
        FileStreamWriter& operator=(const FileStreamWriter& rhs) = delete;
        FileStreamWriter& operator=(FileStreamWriter&& rhs) noexcept;

        uint64_t GetSize() override;
        uint64_t GetPosition() override;
        void SetPosition(size_t position) override;
        void Write(const void* ptr, size_t size) override;
        void Write(const std::vector<uint8_t>& bytes) override;
        void Write(const std::string& str) override;
        void Flush() override;
    };
}

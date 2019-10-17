#pragma once

#include "writer.h"

#include <fstream>

namespace pluto
{
    class PLUTO_API FileWriter final : Writer
    {
        std::ofstream ofs;

    public:
        ~FileWriter() override;
        explicit FileWriter(std::ofstream ofs);

        FileWriter(const FileWriter& other) = delete;
        FileWriter(FileWriter&& other) noexcept;
        FileWriter& operator=(const FileWriter& rhs) = delete;
        FileWriter& operator=(FileWriter&& rhs) noexcept;

        std::ofstream& GetStream();

        uint64_t GetSize() override;
        uint64_t GetPosition() override;
        void SetPosition(size_t position) override;
        void Write(const void* ptr, size_t size) override;
        void Write(const std::vector<uint8_t>& bytes) override;
        void Write(const std::string& str) override;
        void Flush() override;
    };
}

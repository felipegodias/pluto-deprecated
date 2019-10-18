#pragma once

#include "stream_reader.h"

#include <fstream>
#include <vector>

namespace pluto
{
    class PLUTO_API FileReader final : public StreamReader
    {
        std::ifstream ifs;

    public:
        ~FileReader() override;
        explicit FileReader(std::ifstream ifs);

        FileReader(const FileReader& other) = delete;
        FileReader(FileReader&& other) noexcept;
        FileReader& operator=(const FileReader& rhs) = delete;
        FileReader& operator=(FileReader&& rhs) noexcept;

        size_t GetSize() override;
        size_t GetPosition() override;
        void SetPosition(size_t position) override;
        void Read(void* ptr, size_t size) override;
        std::vector<uint8_t> ReadAllBytes() override;
        std::string ReadAllText() override;
    };
}

#pragma once

#include "stream_reader.h"

#include <fstream>
#include <vector>

namespace pluto
{
    class PLUTO_API FileStreamReader final : public StreamReader
    {
        std::ifstream ifs;

    public:
        ~FileStreamReader() override;
        explicit FileStreamReader(std::ifstream ifs);

        FileStreamReader(const FileStreamReader& other) = delete;
        FileStreamReader(FileStreamReader&& other) noexcept;
        FileStreamReader& operator=(const FileStreamReader& rhs) = delete;
        FileStreamReader& operator=(FileStreamReader&& rhs) noexcept;

        size_t GetSize() override;
        size_t GetPosition() override;
        void SetPosition(size_t position) override;
        void Read(void* ptr, size_t size) override;
        std::vector<uint8_t> ReadAllBytes() override;
        std::string ReadAllText() override;
    };
}

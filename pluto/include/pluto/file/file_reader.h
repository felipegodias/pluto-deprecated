#pragma once

#include "reader.h"

#include <fstream>
#include <vector>

namespace pluto
{
    class PLUTO_API FileReader final : Reader
    {
        std::ifstream ifs;

    public:
        ~FileReader() override;
        explicit FileReader(std::ifstream ifs);

        FileReader(const FileReader& other) = delete;
        FileReader(FileReader&& other) noexcept;
        FileReader& operator=(const FileReader& rhs) = delete;
        FileReader& operator=(FileReader&& rhs) noexcept;

        std::ifstream& GetStream();

        size_t GetSize() override;
        size_t GetPosition() override;
        void SetPosition(size_t position) override;
        void Read(void* ptr, size_t size) override;
        std::vector<uint8_t> ReadAllBytes() override;
        std::string ReadAllText() override;
    };
}

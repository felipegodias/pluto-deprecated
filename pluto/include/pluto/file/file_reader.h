#pragma once

#include "pluto/api.h"

#include <fstream>
#include <vector>

namespace pluto
{
    class PLUTO_API FileReader
    {
        std::ifstream ifs;

    public:
        ~FileReader();
        explicit FileReader(std::ifstream ifs);

        FileReader(const FileReader& other) = delete;
        FileReader(FileReader&& other) noexcept;
        FileReader& operator=(const FileReader& rhs) = delete;
        FileReader& operator=(FileReader&& rhs) noexcept;

        std::ifstream& GetStream();
        size_t GetSize();
        size_t GetPosition();
        void SetPosition(size_t position);
        void Read(void* ptr, size_t size);
        std::vector<uint8_t> ReadAllBytes();
    };
}

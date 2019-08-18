#pragma once

#include "pluto/api.h"

#include <fstream>

namespace pluto
{
    class PLUTO_API FileWriter
    {
        std::ofstream ofs;

    public:
        ~FileWriter();
        explicit FileWriter(std::ofstream ofs);

        FileWriter(const FileWriter& other) = delete;
        FileWriter(FileWriter&& other) noexcept;
        FileWriter& operator=(const FileWriter& rhs) = delete;
        FileWriter& operator=(FileWriter&& rhs) noexcept;

        std::ofstream& GetStream();
        uint64_t GetSize();
        uint64_t GetPosition();
        void SetPosition(size_t position);
        void Write(const void* ptr, size_t size);
        void Flush();
    };
}

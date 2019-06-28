#pragma once

#include "../di/base_factory.h"
#include <memory>
#include <fstream>

namespace pluto
{
    class PLUTO_API FileWriter
    {
    public:
        class Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<FileWriter> Create(std::ofstream ofs);
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit FileWriter(std::unique_ptr<Impl> impl);

        FileWriter(const FileWriter& other) = delete;
        FileWriter(FileWriter&& other) noexcept;
        FileWriter& operator=(const FileWriter& rhs) = delete;
        FileWriter& operator=(FileWriter&& rhs) noexcept;
        ~FileWriter();

        std::ofstream& GetStream() const;
        uint64_t GetSize() const;
        uint64_t GetPosition() const;
        void SetPosition(uint64_t value);
        void Write(const void* ptr, uint64_t size);
        void Flush();
    };
}

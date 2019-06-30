#pragma once

#include "../di/base_factory.h"
#include <memory>
#include <fstream>

namespace pluto
{
    class PLUTO_API FileReader
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<FileReader> Create(std::ifstream ifs) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit FileReader(std::unique_ptr<Impl> impl);

        FileReader(const FileReader& other) = delete;
        FileReader(FileReader&& other) noexcept;
        ~FileReader();

        FileReader& operator=(const FileReader& rhs) = delete;
        FileReader& operator=(FileReader&& rhs) noexcept;

        std::ifstream& GetStream() const;
        uint64_t GetSize() const;
        uint64_t GetPosition() const;
        void SetPosition(uint64_t value);
        void Read(void* ptr, uint64_t size) const;
    };
}

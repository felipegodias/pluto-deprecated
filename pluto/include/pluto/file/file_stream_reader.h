#pragma once

#include "stream_reader.h"
#include "pluto/service/base_factory.h"

#include <string>
#include <memory>

namespace pluto
{
    class PLUTO_API FileStreamReader final : public StreamReader
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<FileStreamReader> Create(const std::string& path) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~FileStreamReader() override;
        explicit FileStreamReader(std::unique_ptr<Impl> impl);

        FileStreamReader(const FileStreamReader& other) = delete;
        FileStreamReader(FileStreamReader&& other) noexcept;
        FileStreamReader& operator=(const FileStreamReader& rhs) = delete;
        FileStreamReader& operator=(FileStreamReader&& rhs) noexcept;

        size_t GetSize() override;
        size_t GetReadPosition() override;
        void SetReadPosition(size_t position) override;
        void Read(void* ptr, size_t size) override;
        std::vector<uint8_t> ReadAllBytes() override;
        std::string ReadAllText() override;
    };
}

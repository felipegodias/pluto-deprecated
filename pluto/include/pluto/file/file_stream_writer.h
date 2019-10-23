#pragma once

#include "stream_writer.h"
#include "pluto/service/base_factory.h"

#include <string>
#include <memory>

namespace pluto
{
    class PLUTO_API FileStreamWriter final : public StreamWriter
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<FileStreamWriter> Create(const std::string& path) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~FileStreamWriter() override;
        explicit FileStreamWriter(std::unique_ptr<Impl> impl);

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

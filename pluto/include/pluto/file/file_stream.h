#pragma once

#include "stream.h"
#include "pluto/service/base_factory.h"

#include <string>
#include <memory>

namespace pluto
{
    class PLUTO_API FileStream final : public Stream
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<FileStream> Create(const std::string& path) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~FileStream() override;
        explicit FileStream(std::unique_ptr<Impl> impl);

        FileStream(const FileStream& other) = delete;
        FileStream(FileStream&& other) noexcept;
        FileStream& operator=(const FileStream& rhs) = delete;
        FileStream& operator=(FileStream&& rhs) noexcept;

        size_t GetSize() override;
        size_t GetReadPosition() override;
        void SetReadPosition(size_t position) override;
        void Read(void* ptr, size_t size) override;
        std::vector<uint8_t> ReadAllBytes() override;
        std::string ReadAllText() override;

        size_t GetWritePosition() override;
        void SetWritePosition(size_t position) override;
        void Write(const void* ptr, size_t size) override;
        void Write(const std::vector<uint8_t>& bytes) override;
        void Write(const std::string& str) override;
        void Flush() override;
    };
}

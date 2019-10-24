#include "pluto/file/file_stream.h"
#include <fstream>

namespace pluto
{
    class FileStream::Impl
    {
        std::fstream fs;

    public:
        explicit Impl(std::fstream fs)
            : fs(std::move(fs))
        {
        }

        size_t GetSize()
        {
            const size_t pos = fs.tellp();
            fs.seekp(0, std::ios::end);
            const size_t size = fs.tellp();
            fs.seekp(pos, std::ios::beg);
            return size;
        }

        size_t GetReadPosition()
        {
            return fs.tellg();
        }

        void SetReadPosition(const size_t position)
        {
            fs.seekg(position, std::ios::beg);
        }

        void Read(void* ptr, const size_t size)
        {
            fs.read(reinterpret_cast<char*>(ptr), size);
        }

        std::vector<uint8_t> ReadAllBytes()
        {
            const size_t pos = fs.tellg();
            fs.seekg(0, std::ios::end);
            const size_t size = fs.tellg();
            fs.seekg(0, std::ios::beg);
            std::vector<uint8_t> bytes(size);
            fs.read(reinterpret_cast<char*>(bytes.data()), size);
            fs.seekg(pos, std::ios::beg);
            return bytes;
        }

        std::string ReadAllText()
        {
            const size_t pos = fs.tellg();
            std::string str;
            fs.seekg(0, std::ios::end);
            str.reserve(fs.tellg());
            fs.seekg(0, std::ios::beg);
            str.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
            fs.seekg(pos, std::ios::beg);
            return str;
        }

        size_t GetWritePosition()
        {
            return fs.tellp();
        }

        void SetWritePosition(const size_t position)
        {
            fs.seekp(position, std::ios::beg);
        }

        void Write(const void* ptr, const size_t size)
        {
            fs.write(reinterpret_cast<const char*>(ptr), size);
        }

        void Write(const std::vector<uint8_t>& bytes)
        {
            Write(bytes.data(), bytes.size());
        }

        void Write(const std::string& str)
        {
            Write(str.data(), str.size());
        }

        void Flush()
        {
            fs.flush();
        }
    };

    FileStream::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<FileStream> FileStream::Factory::Create(const std::string& path) const
    {
        return std::make_unique<FileStream>(std::make_unique<Impl>(std::fstream(path, std::ios::binary)));
    }

    FileStream::~FileStream() = default;

    FileStream::FileStream(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    FileStream::FileStream(FileStream&& other) noexcept = default;

    FileStream& FileStream::operator=(FileStream&& rhs) noexcept = default;

    size_t FileStream::GetSize()
    {
        return impl->GetSize();
    }

    size_t FileStream::GetReadPosition()
    {
        return impl->GetReadPosition();
    }

    void FileStream::SetReadPosition(const size_t position)
    {
        impl->SetReadPosition(position);
    }

    void FileStream::Read(void* ptr, const size_t size)
    {
        impl->Read(ptr, size);
    }

    std::vector<uint8_t> FileStream::ReadAllBytes()
    {
        return impl->ReadAllBytes();
    }

    std::string FileStream::ReadAllText()
    {
        return impl->ReadAllText();
    }

    size_t FileStream::GetWritePosition()
    {
        return impl->GetWritePosition();
    }

    void FileStream::SetWritePosition(const size_t position)
    {
        return impl->SetWritePosition(position);
    }

    void FileStream::Write(const void* ptr, const size_t size)
    {
        impl->Write(ptr, size);
    }

    void FileStream::Write(const std::vector<uint8_t>& bytes)
    {
        impl->Write(bytes);
    }

    void FileStream::Write(const std::string& str)
    {
        impl->Write(str);
    }

    void FileStream::Flush()
    {
        impl->Flush();
    }
}

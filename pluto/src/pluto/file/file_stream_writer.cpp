#include "pluto/file/file_stream_writer.h"
#include <fstream>

namespace pluto
{
    class FileStreamWriter::Impl
    {
        std::ofstream ofs;

    public:
        explicit Impl(std::ofstream ofs)
            : ofs(std::move(ofs))
        {
        }

        size_t GetSize()
        {
            const size_t pos = ofs.tellp();
            ofs.seekp(0, std::ios::end);
            const size_t size = ofs.tellp();
            ofs.seekp(pos, std::ios::beg);
            return size;
        }

        size_t GetWritePosition()
        {
            return ofs.tellp();
        }

        void SetWritePosition(const size_t position)
        {
            ofs.seekp(position, std::ios::beg);
        }

        void Write(const void* ptr, const size_t size)
        {
            ofs.write(reinterpret_cast<const char*>(ptr), size);
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
            ofs.flush();
        }
    };

    FileStreamWriter::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<FileStreamWriter> FileStreamWriter::Factory::Create(const std::string& path) const
    {
        return std::make_unique<FileStreamWriter>(std::make_unique<Impl>(std::ofstream(path, std::ios::binary)));
    }

    FileStreamWriter::~FileStreamWriter() = default;

    FileStreamWriter::FileStreamWriter(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    FileStreamWriter::FileStreamWriter(FileStreamWriter&& other) noexcept = default;

    FileStreamWriter& FileStreamWriter::operator=(FileStreamWriter&& rhs) noexcept = default;

    size_t FileStreamWriter::GetSize()
    {
        return impl->GetSize();
    }

    size_t FileStreamWriter::GetWritePosition()
    {
        return impl->GetWritePosition();
    }

    void FileStreamWriter::SetWritePosition(const size_t position)
    {
        impl->SetWritePosition(position);
    }

    void FileStreamWriter::Write(const void* ptr, const size_t size)
    {
        impl->Write(ptr, size);
    }

    void FileStreamWriter::Write(const std::vector<uint8_t>& bytes)
    {
        impl->Write(bytes);
    }

    void FileStreamWriter::Write(const std::string& str)
    {
        impl->Write(str);
    }

    void FileStreamWriter::Flush()
    {
        impl->Flush();
    }
}

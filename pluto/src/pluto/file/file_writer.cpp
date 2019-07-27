#include <pluto/file/file_writer.h>

namespace pluto
{
    class FileWriter::Impl
    {
    private:
        std::ofstream ofs;

    public:
        explicit Impl(std::ofstream ofs) : ofs(std::move(ofs))
        {
        }

        std::ofstream& GetStream()
        {
            return ofs;
        }

        uint64_t GetSize()
        {
            const uint64_t pos = ofs.tellp();
            ofs.seekp(0, std::ios::end);
            const size_t size = ofs.tellp();
            ofs.seekp(pos, std::ios::beg);
            return size;
        }

        uint64_t GetPosition()
        {
            return ofs.tellp();
        }

        void SetPosition(const uint64_t value)
        {
            ofs.seekp(value, std::ios::beg);
        }

        void Write(const void* ptr, const uint64_t size)
        {
            ofs.write(reinterpret_cast<const char*>(ptr), size);
        }

        void Flush()
        {
            ofs.flush();
        }
    };

    FileWriter::Factory::Factory(ServiceCollection& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<FileWriter> FileWriter::Factory::Create(std::ofstream ofs) const
    {
        return std::make_unique<FileWriter>(std::make_unique<Impl>(std::move(ofs)));
    }

    FileWriter::FileWriter(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    FileWriter::FileWriter(FileWriter&& other) noexcept : FileWriter(std::move(other.impl))
    {
    }

    FileWriter::~FileWriter() = default;

    FileWriter& FileWriter::operator=(FileWriter&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    std::ofstream& FileWriter::GetStream() const
    {
        return impl->GetStream();
    }

    uint64_t FileWriter::GetSize() const
    {
        return impl->GetSize();
    }

    uint64_t FileWriter::GetPosition() const
    {
        return impl->GetPosition();
    }

    void FileWriter::SetPosition(const uint64_t value)
    {
        impl->SetPosition(value);
    }

    void FileWriter::Write(const void* ptr, const uint64_t size)
    {
        impl->Write(ptr, size);
    }

    void FileWriter::Flush()
    {
        impl->Flush();
    }
}

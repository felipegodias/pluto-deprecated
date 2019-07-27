#include <pluto/file/file_reader.h>

namespace pluto
{
    class FileReader::Impl
    {
    private:
        std::ifstream ifs;

    public:
        explicit Impl(std::ifstream ifs) : ifs(std::move(ifs))
        {
        }

        std::ifstream& GetStream()
        {
            return ifs;
        }

        uint64_t GetSize()
        {
            const uint64_t pos = ifs.tellg();
            ifs.seekg(0, std::ios::end);
            const size_t size = ifs.tellg();
            ifs.seekg(pos, std::ios::beg);
            return size;
        }

        uint64_t GetPosition()
        {
            return ifs.tellg();
        }

        void SetPosition(const uint64_t value)
        {
            ifs.seekg(value, std::ios::beg);
        }

        void Read(void* ptr, const uint64_t size)
        {
            ifs.read(reinterpret_cast<char*>(ptr), size);
        }
    };

    FileReader::Factory::Factory(ServiceCollection& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<FileReader> FileReader::Factory::Create(std::ifstream ifs) const
    {
        return std::make_unique<FileReader>(std::make_unique<Impl>(std::move(ifs)));
    }

    FileReader::FileReader(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    FileReader::FileReader(FileReader&& other) noexcept : FileReader(std::move(other.impl))
    {
    }

    FileReader::~FileReader() = default;

    FileReader& FileReader::operator=(FileReader&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    std::ifstream& FileReader::GetStream() const
    {
        return impl->GetStream();
    }

    uint64_t FileReader::GetSize() const
    {
        return impl->GetSize();
    }

    uint64_t FileReader::GetPosition() const
    {
        return impl->GetPosition();
    }

    void FileReader::SetPosition(const uint64_t value)
    {
        impl->SetPosition(value);
    }

    void FileReader::Read(void* ptr, const uint64_t size) const
    {
        impl->Read(ptr, size);
    }
}

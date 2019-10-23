#include "pluto/file/file_stream_reader.h"
#include <fstream>

namespace pluto
{
    class FileStreamReader::Impl
    {
        std::ifstream ifs;

    public:
        explicit Impl(std::ifstream ifs)
            : ifs(std::move(ifs))
        {
        }

        size_t GetSize()
        {
            const size_t pos = ifs.tellg();
            ifs.seekg(0, std::ios::end);
            const size_t size = ifs.tellg();
            ifs.seekg(pos, std::ios::beg);
            return size;
        }

        size_t GetPosition()
        {
            return ifs.tellg();
        }

        void SetPosition(const size_t position)
        {
            ifs.seekg(position, std::ios::beg);
        }

        void Read(void* ptr, const size_t size)
        {
            ifs.read(reinterpret_cast<char*>(ptr), size);
        }

        std::vector<uint8_t> ReadAllBytes()
        {
            const size_t pos = ifs.tellg();
            ifs.seekg(0, std::ios::end);
            const size_t size = ifs.tellg();
            ifs.seekg(0, std::ios::beg);
            std::vector<uint8_t> bytes(size);
            ifs.read(reinterpret_cast<char*>(bytes.data()), size);
            ifs.seekg(pos, std::ios::beg);
            return bytes;
        }

        std::string ReadAllText()
        {
            const size_t pos = ifs.tellg();
            std::string str;
            ifs.seekg(0, std::ios::end);
            str.reserve(ifs.tellg());
            ifs.seekg(0, std::ios::beg);
            str.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
            ifs.seekg(pos, std::ios::beg);
            return str;
        }
    };

    FileStreamReader::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<FileStreamReader> FileStreamReader::Factory::Create(const std::string& path) const
    {
        return std::make_unique<FileStreamReader>(std::make_unique<Impl>(std::ifstream(path, std::ios::binary)));
    }

    FileStreamReader::~FileStreamReader() = default;

    FileStreamReader::FileStreamReader(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    FileStreamReader::FileStreamReader(FileStreamReader&& other) noexcept = default;

    FileStreamReader& FileStreamReader::operator=(FileStreamReader&& rhs) noexcept = default;

    size_t FileStreamReader::GetSize()
    {
        return impl->GetSize();
    }

    size_t FileStreamReader::GetPosition()
    {
        return impl->GetPosition();
    }

    void FileStreamReader::SetPosition(const size_t position)
    {
        impl->SetPosition(position);
    }

    void FileStreamReader::Read(void* ptr, const size_t size)
    {
        impl->Read(ptr, size);
    }

    std::vector<uint8_t> FileStreamReader::ReadAllBytes()
    {
        return impl->ReadAllBytes();
    }

    std::string FileStreamReader::ReadAllText()
    {
        return impl->ReadAllText();
    }
}

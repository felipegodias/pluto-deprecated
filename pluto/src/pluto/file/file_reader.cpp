#include <pluto/file/file_reader.h>

namespace pluto
{
    FileReader::~FileReader() = default;

    FileReader::FileReader(std::ifstream ifs)
        : ifs(std::move(ifs))
    {
    }

    FileReader::FileReader(FileReader&& other) noexcept
        : ifs(std::move(other.ifs))
    {
    }

    FileReader& FileReader::operator=(FileReader&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        ifs = std::move(rhs.ifs);
        return *this;
    }

    size_t FileReader::GetSize()
    {
        const size_t pos = ifs.tellg();
        ifs.seekg(0, std::ios::end);
        const size_t size = ifs.tellg();
        ifs.seekg(pos, std::ios::beg);
        return size;
    }

    size_t FileReader::GetPosition()
    {
        return ifs.tellg();
    }

    void FileReader::SetPosition(const size_t position)
    {
        ifs.seekg(position, std::ios::beg);
    }

    void FileReader::Read(void* ptr, const size_t size)
    {
        ifs.read(reinterpret_cast<char*>(ptr), size);
    }

    std::vector<uint8_t> FileReader::ReadAllBytes()
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

    std::string FileReader::ReadAllText()
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
}

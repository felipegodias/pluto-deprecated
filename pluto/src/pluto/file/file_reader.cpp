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

    std::ifstream& FileReader::GetStream()
    {
        return ifs;
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
}

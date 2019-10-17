#include "pluto/file/file_writer.h"

namespace pluto
{
    FileWriter::~FileWriter() = default;

    FileWriter::FileWriter(std::ofstream ofs)
        : ofs(std::move(ofs))
    {
    }

    FileWriter::FileWriter(FileWriter&& other) noexcept
        : ofs(std::move(other.ofs))
    {
    }

    FileWriter& FileWriter::operator=(FileWriter&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        ofs = std::move(rhs.ofs);
        return *this;
    }

    std::ofstream& FileWriter::GetStream()
    {
        return ofs;
    }

    size_t FileWriter::GetSize()
    {
        const size_t pos = ofs.tellp();
        ofs.seekp(0, std::ios::end);
        const size_t size = ofs.tellp();
        ofs.seekp(pos, std::ios::beg);
        return size;
    }

    size_t FileWriter::GetPosition()
    {
        return ofs.tellp();
    }

    void FileWriter::SetPosition(const size_t position)
    {
        ofs.seekp(position, std::ios::beg);
    }

    void FileWriter::Write(const void* ptr, const size_t size)
    {
        ofs.write(reinterpret_cast<const char*>(ptr), size);
    }

    void FileWriter::Write(const std::vector<uint8_t>& bytes)
    {
        Write(bytes.data(), bytes.size());
    }

    void FileWriter::Write(const std::string& str)
    {
        Write(str.data(), str.size());
    }

    void FileWriter::Flush()
    {
        ofs.flush();
    }
}

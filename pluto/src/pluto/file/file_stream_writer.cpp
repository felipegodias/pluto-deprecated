#include "pluto/file/file_stream_writer.h"

namespace pluto
{
    FileStreamWriter::~FileStreamWriter() = default;

    FileStreamWriter::FileStreamWriter(std::ofstream ofs)
        : ofs(std::move(ofs))
    {
    }

    FileStreamWriter::FileStreamWriter(FileStreamWriter&& other) noexcept
        : ofs(std::move(other.ofs))
    {
    }

    FileStreamWriter& FileStreamWriter::operator=(FileStreamWriter&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        ofs = std::move(rhs.ofs);
        return *this;
    }

    size_t FileStreamWriter::GetSize()
    {
        const size_t pos = ofs.tellp();
        ofs.seekp(0, std::ios::end);
        const size_t size = ofs.tellp();
        ofs.seekp(pos, std::ios::beg);
        return size;
    }

    size_t FileStreamWriter::GetPosition()
    {
        return ofs.tellp();
    }

    void FileStreamWriter::SetPosition(const size_t position)
    {
        ofs.seekp(position, std::ios::beg);
    }

    void FileStreamWriter::Write(const void* ptr, const size_t size)
    {
        ofs.write(reinterpret_cast<const char*>(ptr), size);
    }

    void FileStreamWriter::Write(const std::vector<uint8_t>& bytes)
    {
        Write(bytes.data(), bytes.size());
    }

    void FileStreamWriter::Write(const std::string& str)
    {
        Write(str.data(), str.size());
    }

    void FileStreamWriter::Flush()
    {
        ofs.flush();
    }
}

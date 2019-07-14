#include "pluto/file/path.h"

#include <filesystem>
#include <string>

namespace pluto
{
    class Path::Impl
    {
    private:
        std::filesystem::path path;

    public:
        explicit Impl(std::string pathStr)
        {
            std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
            path = std::filesystem::path(std::move(pathStr));
        }

        explicit Impl(std::filesystem::path path) : path(std::move(path))
        {
        }

        std::string GetName() const
        {
            return path.filename().string();
        }

        std::string GetNameWithoutExtension() const
        {
            return path.filename().replace_extension("").string();
        }

        bool HasExtension() const
        {
            return path.has_extension();
        }

        std::string GetExtension() const
        {
            return path.extension().string();
        }

        Path GetDirectory() const
        {
            return Path(path.parent_path().string());
        }

        std::string Str() const
        {
            return path.string();
        }

        void ChangeExtension(const std::string& extension)
        {
            path = path.replace_extension(extension);
        }

        void RemoveExtension()
        {
            path = path.replace_extension("");
        }

        void Clone(const Impl& other)
        {
            path = other.path;
        }
    };

    Path::Path(std::string path) : impl(std::make_unique<Impl>(std::move(path)))
    {
    }

    Path::Path(const Path& other) : impl(std::make_unique<Impl>(std::filesystem::path(other.Str())))
    {
    }

    Path::Path(Path&& other) noexcept : impl(std::move(other.impl))
    {
    }

    Path::~Path() = default;

    Path& Path::operator=(const Path& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl->Clone(*rhs.impl);
        return *this;
    }

    Path& Path::operator=(Path&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const Path& path)
    {
        os << path.Str();
        return os;
    }

    std::string Path::GetName() const
    {
        return impl->GetName();
    }

    std::string Path::GetNameWithoutExtension() const
    {
        return impl->GetNameWithoutExtension();
    }

    bool Path::HasExtension() const
    {
        return impl->HasExtension();
    }

    std::string Path::GetExtension() const
    {
        return impl->GetExtension();
    }

    Path Path::GetDirectory() const
    {
        return impl->GetDirectory();
    }

    std::string Path::Str() const
    {
        return impl->Str();
    }

    void Path::ChangeExtension(const std::string& extension)
    {
        impl->ChangeExtension(extension);
    }

    void Path::RemoveExtension()
    {
        impl->RemoveExtension();
    }
}

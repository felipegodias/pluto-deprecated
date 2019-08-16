#include "pluto/file/path.h"

#include <filesystem>
#include <string>

namespace pluto
{
    std::string Path::Normalize(const std::string& path)
    {
        std::string normalized = path;
        std::replace(normalized.begin(), normalized.end(), '\\', '/');
        return normalized;
    }

    std::string Path::ChangeExtension(const std::string& path, const std::string& extension)
    {
        return std::filesystem::path(path).replace_extension(extension).string();
    }

    bool Path::HasExtension(const std::string& path)
    {
        return std::filesystem::path(path).has_extension();
    }

    std::string Path::RemoveExtension(const std::string& path)
    {
        return ChangeExtension(path, "");
    }

    std::string Path::GetExtension(const std::string& path)
    {
        return std::filesystem::path(path).extension().string();
    }

    std::string Path::Combine(const std::vector<std::string>& values)
    {
        std::filesystem::path path;
        for (auto& value : values)
        {
            path.append(value);
        }
        return path.string();
    }

    std::string Path::GetRelativePath(const std::string& path, const std::string& base)
    {
        return relative(std::filesystem::path(path), std::filesystem::path(base)).string();
    }

    std::string Path::GetDirectoryName(const std::string& path)
    {
        return std::filesystem::path(path).parent_path().string();
    }

    std::string Path::GetFileName(const std::string& path)
    {
        return std::filesystem::path(path).filename().string();
    }

    std::string Path::GetFileNameWithoutExtension(const std::string& path)
    {
        return std::filesystem::path(path).filename().replace_extension("").string();
    }
}

#pragma once

#include "pluto/service/base_factory.h"

#include <string>
#include <vector>

namespace pluto
{
    class PLUTO_API Path
    {
    public:
        ~Path() = delete;
        Path() = delete;

        Path(const std::string& other) = delete;
        Path(Path&& other) noexcept = delete;
        Path& operator=(const std::string& rhs) = delete;
        Path& operator=(Path&& rhs) noexcept = delete;

        static std::string Normalize(const std::string& path);
        static std::string ChangeExtension(const std::string& path, const std::string& extension);
        static bool HasExtension(const std::string& path);
        static std::string RemoveExtension(const std::string& path);
        static std::string GetExtension(const std::string& path);

        static std::string Combine(const std::vector<std::string>& values);

        static std::string GetRelativePath(const std::string& path, const std::string& base);
        static std::string GetDirectoryName(const std::string& path);
        static std::string GetFileName(const std::string& path);
        static std::string GetFileNameWithoutExtension(const std::string& path);
    };
}

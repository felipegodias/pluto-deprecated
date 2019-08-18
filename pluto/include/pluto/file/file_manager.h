#pragma once

#include "pluto/api.h"

#include <vector>
#include <string>

namespace pluto
{
    class FileWriter;
    class FileReader;
    class Regex;
    class Path;

    class PLUTO_API FileManager
    {
    public:
        enum SearchOptions
        {
            TopDirectoryOnly = 0,
            AllDirectories = 1,
        };

        ~FileManager() = delete;
        FileManager() = delete;

        FileManager(const FileManager& other) = delete;
        FileManager(FileManager&& other) noexcept = delete;
        FileManager& operator=(const FileManager& rhs) = delete;
        FileManager& operator=(FileManager&& rhs) noexcept = delete;

        static std::string GetRootPath();

        static void SetRootPath(const std::string& path);

        static bool Exists(const std::string& path);

        static bool IsFile(const std::string& path);

        static bool IsDirectory(const std::string& path);

        static std::vector<std::string> GetDirectories(const std::string& path);

        static std::vector<std::string> GetDirectories(const std::string& path, SearchOptions searchOptions);

        static std::vector<std::string> GetDirectories(const std::string& path, const Regex& regex);

        static std::vector<std::string> GetDirectories(const std::string& path, const Regex& regex,
                                                       SearchOptions searchOptions);

        static std::vector<std::string> GetFiles(const std::string& path);

        static std::vector<std::string> GetFiles(const std::string& path, SearchOptions searchOptions);

        static std::vector<std::string> GetFiles(const std::string& path, const Regex& regex);

        static std::vector<std::string> GetFiles(const std::string& path, const Regex& regex,
                                                 SearchOptions searchOptions);

        static void CreateDirectory(const std::string& path);

        static FileReader OpenRead(const std::string& path);

        static FileWriter OpenWrite(const std::string& path);

        static void Delete(const std::string& path);
    };
}

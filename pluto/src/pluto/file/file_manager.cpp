#include "pluto/file/file_manager.h"
#include "pluto/file/file_reader.h"
#include "pluto/file/file_writer.h"
#include "pluto/regex.h"

#include <filesystem>

namespace pluto
{
    namespace file_manager_cpp
    {
        template <typename TFilter>
        std::vector<std::string> GetEntries(const std::string& path, const FileManager::SearchOptions searchOptions,
                                            TFilter filter)
        {
            if (searchOptions == FileManager::SearchOptions::TopDirectoryOnly)
            {
                return FilterEntries(std::filesystem::directory_iterator(path), filter);
            }
            return FilterEntries(std::filesystem::recursive_directory_iterator(path), filter);
        }

        template <typename TIterator, typename TFilter>
        std::vector<std::string> FilterEntries(TIterator iterator, TFilter filter)
        {
            std::vector<std::string> result;
            for (auto& it : iterator)
            {
                if (filter(it))
                {
                    result.push_back(it.path().string());
                }
            }
            return result;
        }
    }

    std::string FileManager::GetRootPath()
    {
        return std::filesystem::current_path().string();
    }

    void FileManager::SetRootPath(const std::string& path)
    {
        std::filesystem::current_path(path);
    }

    bool FileManager::Exists(const std::string& path)
    {
        return std::filesystem::exists(path);
    }

    bool FileManager::IsFile(const std::string& path)
    {
        return std::filesystem::is_regular_file(path);
    }

    bool FileManager::IsDirectory(const std::string& path)
    {
        return std::filesystem::is_directory(path);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path)
    {
        return GetDirectories(path, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, SearchOptions searchOptions)
    {
        const auto filter = [](const std::filesystem::directory_entry& e)-> bool
        {
            return e.is_directory();
        };

        return file_manager_cpp::GetEntries(path, searchOptions, filter);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const Regex& regex)
    {
        return GetDirectories(path, regex, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const Regex& regex,
                                                         const SearchOptions searchOptions)
    {
        const auto filter = [&regex](const std::filesystem::directory_entry& e)-> bool
        {
            return e.is_directory() && regex.Match(e.path().string());
        };

        return file_manager_cpp::GetEntries(path, searchOptions, filter);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path)
    {
        return GetFiles(path, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const SearchOptions searchOptions)
    {
        const auto filter = [](const std::filesystem::directory_entry& e)-> bool
        {
            return e.is_regular_file();
        };

        return file_manager_cpp::GetEntries(path, searchOptions, filter);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const Regex& regex)
    {
        return GetFiles(path, regex, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const Regex& regex,
                                                   const SearchOptions searchOptions)
    {
        const auto filter = [&regex](const std::filesystem::directory_entry& e)-> bool
        {
            return e.is_regular_file() && regex.Match(e.path().string());
        };

        return file_manager_cpp::GetEntries(path, searchOptions, filter);
    }

    void FileManager::CreateDirectory(const std::string& path)
    {
        std::filesystem::create_directory(path);
    }

    FileReader FileManager::OpenRead(const std::string& path)
    {
        return FileReader(std::ifstream(path, std::ios::binary));
    }

    FileWriter FileManager::OpenWrite(const std::string& path)
    {
        return FileWriter(std::ofstream(path, std::ios::binary));
    }

    void FileManager::Delete(const std::string& path)
    {
        std::filesystem::remove_all(path);
    }
}

#include <pluto/file/file_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>
#include <iostream>
#include <filesystem>
#include <regex>

namespace pluto
{
    class FileManager::Impl
    {
    public:
        explicit Impl(const std::string& dataDirectoryName)
        {
            if (!dataDirectoryName.empty())
            {
                std::filesystem::current_path(dataDirectoryName);
            }

            std::cout << "FileManager initialized!" << std::endl;
        }

        ~Impl()
        {
            std::cout << "FileManager terminated!" << std::endl;
        }

        bool Exists(const std::string& path) const
        {
            return std::filesystem::exists(path);
        }

        bool IsFile(const std::string& path) const
        {
            return std::filesystem::is_regular_file(path);
        }

        bool IsDirectory(const std::string& path) const
        {
            return std::filesystem::is_directory(path);
        }

        std::vector<std::string> GetDirectories(
            const std::string& path,
            const std::string& searchPattern,
            const SearchOptions searchOptions) const
        {
            if (searchOptions == TopDirectoryOnly)
            {
                return GetEntries(path, searchPattern, true, false, std::filesystem::directory_iterator(path));
            }
            return GetEntries(path, searchPattern, true, false, std::filesystem::recursive_directory_iterator(path));
        }

        std::vector<std::string> GetFiles(
            const std::string& path,
            const std::string& searchPattern,
            const SearchOptions searchOptions) const
        {
            if (searchOptions == TopDirectoryOnly)
            {
                return GetEntries(path, searchPattern, false, true, std::filesystem::directory_iterator(path));
            }
            return GetEntries(path, searchPattern, false, true, std::filesystem::recursive_directory_iterator(path));
        }

        std::fstream Open(const std::string& path) const
        {
            std::fstream file(path, std::ifstream::binary);
            return file;
        }

        std::ifstream OpenRead(const std::string& path) const
        {
            std::ifstream file(path, std::ifstream::binary);
            return file;
        }

        std::ofstream OpenWrite(const std::string& path) const
        {
            std::ofstream file(path, std::ifstream::binary);
            return file;
        }

        void Delete(const std::string& path) const
        {
            std::filesystem::remove_all(path);
        }

    private:
        template <class T>
        std::vector<std::string> GetEntries(
            const std::string& path,
            const std::string& searchPattern,
            const bool skipFiles,
            const bool skipDirectories,
            T it) const
        {
            std::vector<std::string> result;
            const std::regex searchRegex(searchPattern);
            for (auto& p : it)
            {
                if (!skipDirectories && !p.is_directory())
                {
                    continue;
                }
                if (!skipFiles && !p.is_regular_file())
                {
                    continue;
                }
                std::string pStr = p.path().string();
                if (searchPattern == ".*" || std::regex_match(pStr, searchRegex))
                {
                    result.push_back(pStr);
                }
            }
            return result;
        }
    };

    FileManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<FileManager> FileManager::Factory::Create(const std::string& dataDirectoryName) const
    {
        return std::make_unique<FileManager>(std::make_unique<Impl>(dataDirectoryName));
    }

    FileManager::FileManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    FileManager::~FileManager() = default;

    bool FileManager::Exists(const std::string& path) const
    {
        return impl->Exists(path);
    }

    bool FileManager::IsFile(const std::string& path) const
    {
        return impl->IsFile(path);
    }

    bool FileManager::IsDirectory(const std::string& path) const
    {
        return impl->IsDirectory(path);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path) const
    {
        return impl->GetDirectories(path, ".*", TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path,
                                                         const std::string& searchPattern) const
    {
        return impl->GetDirectories(path, searchPattern, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const std::string& searchPattern,
                                                         const SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, searchPattern, searchOptions);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path) const
    {
        return impl->GetFiles(path, ".*", TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const std::string& searchPattern) const
    {
        return impl->GetFiles(path, searchPattern, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const std::string& searchPattern,
                                                   const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, searchPattern, searchOptions);
    }

    std::fstream FileManager::Open(const std::string& path) const
    {
        return impl->Open(path);
    }

    std::ifstream FileManager::OpenRead(const std::string& path) const
    {
        return impl->OpenRead(path);
    }

    std::ofstream FileManager::OpenWrite(const std::string& path) const
    {
        return impl->OpenWrite(path);
    }

    void FileManager::Delete(const std::string& path) const
    {
        return impl->Delete(path);
    }
}

#include <pluto/file/file_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>
#include <filesystem>
#include <regex>

namespace pluto
{
    class FileManager::Impl
    {
    private:
        LogManager& logManager;

    public:
        explicit Impl(LogManager& logManager) : logManager(logManager)
        {
            logManager.LogInfo("FileManager Initialized!");
        }

        ~Impl()
        {
            logManager.LogInfo("FileManager Terminated!");
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

        std::ifstream ReadFile(const std::string& path) const
        {
            std::ifstream file(path, std::ifstream::binary);
            return file;
        }

        std::ofstream CreateFile(const std::string& path) const
        {
            std::ofstream file(path, std::ifstream::binary);
            return file;
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

    std::unique_ptr<FileManager> FileManager::Factory::Create() const
    {
        auto& logManager = diContainer.Resolve<LogManager>();
        return std::make_unique<FileManager>(std::make_unique<Impl>(logManager));
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

    std::ifstream FileManager::ReadFile(const std::string& path) const
    {
        return impl->ReadFile(path);
    }

    std::ofstream FileManager::CreateFile(const std::string& path) const
    {
        return impl->CreateFile(path);
    }
}

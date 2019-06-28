#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/di/di_container.h>

#include <iostream>
#include <filesystem>
#include <regex>

namespace pluto
{
    class FileManager::Impl
    {
    private:
        FileReader::Factory& fileReaderFactory;
        FileWriter::Factory& fileWriterFactory;

    public:
        explicit Impl(const std::string& dataDirectoryName, FileReader::Factory& fileReaderFactory,
                      FileWriter::Factory& fileWriterFactory) : fileReaderFactory(fileReaderFactory),
                                                                fileWriterFactory(fileWriterFactory)
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

        std::unique_ptr<FileReader> OpenRead(const std::string& path) const
        {
            return fileReaderFactory.Create(std::ifstream(path, std::ifstream::binary));
        }

        std::unique_ptr<FileWriter> OpenWrite(const std::string& path) const
        {
            return fileWriterFactory.Create(std::ofstream(path, std::ifstream::binary));
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
        auto& fileReaderFactory = diContainer.GetSingleton<FileReader::Factory>();
        auto& fileWriterFactory = diContainer.GetSingleton<FileWriter::Factory>();
        return std::make_unique<FileManager>(
            std::make_unique<Impl>(dataDirectoryName, fileReaderFactory, fileWriterFactory));
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

    std::unique_ptr<FileReader> FileManager::OpenRead(const std::string& path) const
    {
        return impl->OpenRead(path);
    }

    std::unique_ptr<FileWriter> FileManager::OpenWrite(const std::string& path) const
    {
        return impl->OpenWrite(path);
    }

    void FileManager::Delete(const std::string& path) const
    {
        return impl->Delete(path);
    }
}

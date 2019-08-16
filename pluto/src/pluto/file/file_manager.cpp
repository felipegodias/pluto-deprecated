#include "pluto/file/file_manager.h"
#include "pluto/file/file_reader.h"
#include "pluto/file/file_writer.h"
#include "pluto/file/path.h"
#include "pluto/service/service_collection.h"
#include "pluto/regex.h"

#include <algorithm>
#include <iostream>
#include <filesystem>

namespace pluto
{
    class FileManager::Impl
    {
    private:
        FileReader::Factory& fileReaderFactory;
        FileWriter::Factory& fileWriterFactory;

    public:
        explicit Impl(const std::string& rootPath, FileReader::Factory& fileReaderFactory,
                      FileWriter::Factory& fileWriterFactory)
            : fileReaderFactory(fileReaderFactory),
              fileWriterFactory(fileWriterFactory)
        {
            std::filesystem::current_path(rootPath);
            std::cout << "FileManager initialized!" << std::endl;
        }

        ~Impl()
        {
            std::cout << "FileManager terminated!" << std::endl;
        }

        std::string GetRootPath() const
        {
            return std::filesystem::current_path().string();
        }

        void SetRootPath(const std::string& value)
        {
            std::filesystem::current_path(value);
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

        std::vector<std::string> GetDirectories(const std::string& path, const SearchOptions searchOptions) const
        {
            const auto filter = [](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_directory();
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<std::string> GetDirectories(const std::string& path, const Regex& regex,
                                         const SearchOptions searchOptions) const
        {
            const auto filter = [&regex](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_directory() && regex.Match(e.path().string());
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<std::string> GetFiles(const std::string& path, const SearchOptions searchOptions) const
        {
            const auto filter = [](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_regular_file();
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<std::string> GetFiles(const std::string& path, const Regex& regex,
                                   const SearchOptions searchOptions) const
        {
            const auto filter = [&regex](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_regular_file() && regex.Match(e.path().string());
            };

            return GetEntries(path, searchOptions, filter);
        }

        void CreateDirectory(const std::string& path) const
        {
            std::filesystem::create_directory(path);
        }

        std::unique_ptr<FileReader> OpenRead(const std::string& path) const
        {
            return fileReaderFactory.Create(std::ifstream(path, std::ios::binary));
        }

        std::unique_ptr<FileWriter> OpenWrite(const std::string& path) const
        {
            return fileWriterFactory.Create(std::ofstream(path, std::ios::binary));
        }

        void Delete(const std::string& path) const
        {
            std::filesystem::remove_all(path);
        }

    private:
        template <typename Tf>
        std::vector<std::string> GetEntries(const std::string& path, const SearchOptions searchOptions, Tf filter) const
        {
            if (searchOptions == TopDirectoryOnly)
            {
                return FilterEntries(std::filesystem::directory_iterator(path), filter);
            }
            return FilterEntries(std::filesystem::recursive_directory_iterator(path), filter);
        }

        template <typename Ti, typename Tf>
        std::vector<std::string> FilterEntries(Ti iterator, Tf filter) const
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
    };

    FileManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<FileManager> FileManager::Factory::Create(const std::string& rootPath) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        FileReader::Factory& fileReaderFactory = serviceCollection.GetFactory<FileReader>();
        FileWriter::Factory& fileWriterFactory = serviceCollection.GetFactory<FileWriter>();
        return std::make_unique<FileManager>(
            std::make_unique<Impl>(rootPath, fileReaderFactory, fileWriterFactory));
    }

    FileManager::FileManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    FileManager::~FileManager() = default;

    std::string FileManager::GetRootPath() const
    {
        return impl->GetRootPath();
    }

    void FileManager::SetRootPath(const std::string& value)
    {
        impl->SetRootPath(value);
    }

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
        return impl->GetDirectories(path, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, searchOptions);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const Regex& regex) const
    {
        return impl->GetDirectories(path, regex, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const Regex& regex,
                                                  const SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, regex, searchOptions);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path) const
    {
        return impl->GetFiles(path, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, searchOptions);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const Regex& regex) const
    {
        return impl->GetFiles(path, regex, TopDirectoryOnly);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const Regex& regex,
                                            const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, regex, searchOptions);
    }

    void FileManager::CreateDirectory(const std::string& path) const
    {
        return impl->CreateDirectory(path);
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

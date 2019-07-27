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
        explicit Impl(const Path& rootPath, FileReader::Factory& fileReaderFactory,
                      FileWriter::Factory& fileWriterFactory) : fileReaderFactory(fileReaderFactory),
                                                                fileWriterFactory(fileWriterFactory)
        {
            std::filesystem::current_path(rootPath.Str());
            std::cout << "FileManager initialized!" << std::endl;
        }

        ~Impl()
        {
            std::cout << "FileManager terminated!" << std::endl;
        }

        Path GetRootPath() const
        {
            return Path(std::filesystem::current_path().string());
        }

        void SetRootPath(const Path& value)
        {
            std::filesystem::current_path(value.Str());
        }

        bool Exists(const Path& path) const
        {
            return std::filesystem::exists(path.Str());
        }

        bool IsFile(const Path& path) const
        {
            return std::filesystem::is_regular_file(path.Str());
        }

        bool IsDirectory(const Path& path) const
        {
            return std::filesystem::is_directory(path.Str());
        }

        std::vector<Path> GetDirectories(const Path& path, const SearchOptions searchOptions) const
        {
            const auto filter = [](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_directory();
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<Path> GetDirectories(const Path& path, const Regex& regex,
                                         const SearchOptions searchOptions) const
        {
            const auto filter = [&regex](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_directory() && regex.Match(e.path().string());
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<Path> GetFiles(const Path& path, const SearchOptions searchOptions) const
        {
            const auto filter = [](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_regular_file();
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<Path> GetFiles(const Path& path, const Regex& regex,
                                   const SearchOptions searchOptions) const
        {
            const auto filter = [&regex](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_regular_file() && regex.Match(e.path().string());
            };

            return GetEntries(path, searchOptions, filter);
        }

        void CreateDirectory(const Path& path) const
        {
            std::filesystem::create_directory(path.Str());
        }

        std::unique_ptr<FileReader> OpenRead(const Path& path) const
        {
            return fileReaderFactory.Create(std::ifstream(path.Str(), std::ios::binary));
        }

        std::unique_ptr<FileWriter> OpenWrite(const Path& path) const
        {
            return fileWriterFactory.Create(std::ofstream(path.Str(), std::ios::binary));
        }

        void Delete(const Path& path) const
        {
            std::filesystem::remove_all(path.Str());
        }

    private:
        template <typename Tf>
        std::vector<Path> GetEntries(const Path& path, const SearchOptions searchOptions, Tf filter) const
        {
            if (searchOptions == TopDirectoryOnly)
            {
                return FilterEntries(std::filesystem::directory_iterator(path.Str()), filter);
            }
            return FilterEntries(std::filesystem::recursive_directory_iterator(path.Str()), filter);
        }

        template <typename Ti, typename Tf>
        std::vector<Path> FilterEntries(Ti iterator, Tf filter) const
        {
            std::vector<Path> result;
            for (auto& it : iterator)
            {
                if (filter(it))
                {
                    result.push_back(Path(it.path().string()));
                }
            }
            return result;
        }
    };

    FileManager::Factory::Factory(ServiceCollection& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<FileManager> FileManager::Factory::Create(const Path& rootPath) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& fileReaderFactory = serviceCollection.GetService<FileReader::Factory>();
        auto& fileWriterFactory = serviceCollection.GetService<FileWriter::Factory>();
        return std::make_unique<FileManager>(
            std::make_unique<Impl>(rootPath, fileReaderFactory, fileWriterFactory));
    }

    FileManager::FileManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    FileManager::~FileManager() = default;

    Path FileManager::GetRootPath() const
    {
        return impl->GetRootPath();
    }

    void FileManager::SetRootPath(const Path& value)
    {
        impl->SetRootPath(value);
    }

    bool FileManager::Exists(const Path& path) const
    {
        return impl->Exists(path);
    }

    bool FileManager::IsFile(const Path& path) const
    {
        return impl->IsFile(path);
    }

    bool FileManager::IsDirectory(const Path& path) const
    {
        return impl->IsDirectory(path);
    }

    std::vector<Path> FileManager::GetDirectories(const Path& path) const
    {
        return impl->GetDirectories(path, TopDirectoryOnly);
    }

    std::vector<Path> FileManager::GetDirectories(const Path& path, const SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, searchOptions);
    }

    std::vector<Path> FileManager::GetDirectories(const Path& path, const Regex& regex) const
    {
        return impl->GetDirectories(path, regex, TopDirectoryOnly);
    }

    std::vector<Path> FileManager::GetDirectories(const Path& path, const Regex& regex,
                                                  const SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, regex, searchOptions);
    }

    std::vector<Path> FileManager::GetFiles(const Path& path) const
    {
        return impl->GetFiles(path, TopDirectoryOnly);
    }

    std::vector<Path> FileManager::GetFiles(const Path& path, const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, searchOptions);
    }

    std::vector<Path> FileManager::GetFiles(const Path& path, const Regex& regex) const
    {
        return impl->GetFiles(path, regex, TopDirectoryOnly);
    }

    std::vector<Path> FileManager::GetFiles(const Path& path, const Regex& regex,
                                            const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, regex, searchOptions);
    }

    void FileManager::CreateDirectory(const Path& path) const
    {
        return impl->CreateDirectory(path);
    }

    std::unique_ptr<FileReader> FileManager::OpenRead(const Path& path) const
    {
        return impl->OpenRead(path);
    }

    std::unique_ptr<FileWriter> FileManager::OpenWrite(const Path& path) const
    {
        return impl->OpenWrite(path);
    }

    void FileManager::Delete(const Path& path) const
    {
        return impl->Delete(path);
    }
}

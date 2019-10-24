#include "pluto/file/file_manager.h"
#include "pluto/file/file_stream.h"
#include "pluto/file/file_stream_reader.h"
#include "pluto/file/file_stream_writer.h"
#include "pluto/service/service_collection.h"
#include "pluto/regex.h"
#include "pluto/debug/assert.h"

#include <filesystem>

namespace pluto
{
    class FileManager::Impl
    {
        FileStream::Factory* fileStreamFactory;
        FileStreamReader::Factory* fileStreamReaderFactory;
        FileStreamWriter::Factory* fileStreamWriterFactory;

    public:
        Impl(FileStream::Factory& fileStreamFactory, FileStreamReader::Factory& fileStreamReaderFactory,
             FileStreamWriter::Factory& fileStreamWriterFactory)
            : fileStreamFactory(&fileStreamFactory),
              fileStreamReaderFactory(&fileStreamReaderFactory),
              fileStreamWriterFactory(&fileStreamWriterFactory)
        {
        }

        std::string GetRootPath() const
        {
            return std::filesystem::current_path().string();
        }

        void SetRootPath(const std::string& path) const
        {
            std::filesystem::current_path(path);
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

        std::vector<std::string> GetDirectories(const std::string& path) const
        {
            return GetDirectories(path, TopDirectoryOnly);
        }

        std::vector<std::string> GetDirectories(const std::string& path, SearchOptions searchOptions) const
        {
            const auto filter = [](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_directory();
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<std::string> GetDirectories(const std::string& path, const Regex& regex) const
        {
            return GetDirectories(path, regex, TopDirectoryOnly);
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

        std::vector<std::string> GetFiles(const std::string& path) const
        {
            return GetFiles(path, TopDirectoryOnly);
        }

        std::vector<std::string> GetFiles(const std::string& path, const SearchOptions searchOptions) const
        {
            const auto filter = [](const std::filesystem::directory_entry& e)-> bool
            {
                return e.is_regular_file();
            };

            return GetEntries(path, searchOptions, filter);
        }

        std::vector<std::string> GetFiles(const std::string& path, const Regex& regex) const
        {
            return GetFiles(path, regex, TopDirectoryOnly);
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

        std::unique_ptr<FileStream> Open(const std::string& path) const
        {
            return fileStreamFactory->Create(path);
        }

        std::unique_ptr<FileStreamReader> OpenRead(const std::string& path) const
        {
            ASSERT_THAT_IS_TRUE(IsFile(path));
            return fileStreamReaderFactory->Create(path);
        }

        std::unique_ptr<FileStreamWriter> OpenWrite(const std::string& path) const
        {
            return fileStreamWriterFactory->Create(path);
        }

        void Delete(const std::string& path) const
        {
            std::filesystem::remove_all(path);
        }

    private:
        template <typename TFilter>
        std::vector<std::string> GetEntries(const std::string& path, const SearchOptions searchOptions,
                                            TFilter filter) const
        {
            if (searchOptions == TopDirectoryOnly)
            {
                return FilterEntries(std::filesystem::directory_iterator(path), filter);
            }
            return FilterEntries(std::filesystem::recursive_directory_iterator(path), filter);
        }

        template <typename TIterator, typename TFilter>
        std::vector<std::string> FilterEntries(TIterator iterator, TFilter filter) const
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

    std::unique_ptr<FileManager> FileManager::Factory::Create() const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        FileStream::Factory& fileStreamFactory = serviceCollection.GetFactory<FileStream>();
        FileStreamReader::Factory& fileStreamReaderFactory = serviceCollection.GetFactory<FileStreamReader>();
        FileStreamWriter::Factory& fileStreamWriterFactory = serviceCollection.GetFactory<FileStreamWriter>();
        return std::make_unique<FileManager>(
            std::make_unique<Impl>(fileStreamFactory, fileStreamReaderFactory, fileStreamWriterFactory));
    }

    FileManager::~FileManager() = default;

    FileManager::FileManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    FileManager::FileManager(FileManager&& other) noexcept = default;

    FileManager& FileManager::operator=(FileManager&& rhs) noexcept = default;

    std::string FileManager::GetRootPath() const
    {
        return impl->GetRootPath();
    }

    void FileManager::SetRootPath(const std::string& path) const
    {
        impl->SetRootPath(path);
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
        return impl->GetDirectories(path);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, searchOptions);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const Regex& regex) const
    {
        return impl->GetDirectories(path, regex);
    }

    std::vector<std::string> FileManager::GetDirectories(const std::string& path, const Regex& regex,
                                                         const SearchOptions searchOptions) const
    {
        return impl->GetDirectories(path, regex, searchOptions);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path) const
    {
        return impl->GetFiles(path);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, searchOptions);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const Regex& regex) const
    {
        return impl->GetFiles(path, regex);
    }

    std::vector<std::string> FileManager::GetFiles(const std::string& path, const Regex& regex,
                                                   const SearchOptions searchOptions) const
    {
        return impl->GetFiles(path, regex, searchOptions);
    }

    void FileManager::CreateDirectory(const std::string& path) const
    {
        impl->CreateDirectory(path);
    }

    std::unique_ptr<FileStream> FileManager::Open(const std::string& path) const
    {
        return impl->Open(path);
    }

    std::unique_ptr<FileStreamReader> FileManager::OpenRead(const std::string& path) const
    {
        return impl->OpenRead(path);
    }

    std::unique_ptr<FileStreamWriter> FileManager::OpenWrite(const std::string& path) const
    {
        return impl->OpenWrite(path);
    }

    void FileManager::Delete(const std::string& path) const
    {
        impl->Delete(path);
    }
}

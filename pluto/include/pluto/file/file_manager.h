#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <vector>
#include <string>

namespace pluto
{
    class FileStream;
    class FileStreamWriter;
    class FileStreamReader;
    class Regex;

    class PLUTO_API FileManager final : public BaseService
    {
    public:
        enum SearchOptions
        {
            TopDirectoryOnly = 0,
            AllDirectories = 1,
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<FileManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~FileManager();

        explicit FileManager(std::unique_ptr<Impl> impl);

        FileManager(const FileManager& other) = delete;
        FileManager(FileManager&& other) noexcept;
        FileManager& operator=(const FileManager& rhs) = delete;
        FileManager& operator=(FileManager&& rhs) noexcept;

        std::string GetRootPath() const;

        void SetRootPath(const std::string& path) const;

        bool Exists(const std::string& path) const;

        bool IsFile(const std::string& path) const;

        bool IsDirectory(const std::string& path) const;

        std::vector<std::string> GetDirectories(const std::string& path) const;

        std::vector<std::string> GetDirectories(const std::string& path, SearchOptions searchOptions) const;

        std::vector<std::string> GetDirectories(const std::string& path, const Regex& regex) const;

        std::vector<std::string> GetDirectories(const std::string& path, const Regex& regex,
                                                SearchOptions searchOptions) const;

        std::vector<std::string> GetFiles(const std::string& path) const;

        std::vector<std::string> GetFiles(const std::string& path, SearchOptions searchOptions) const;

        std::vector<std::string> GetFiles(const std::string& path, const Regex& regex) const;

        std::vector<std::string> GetFiles(const std::string& path, const Regex& regex,
                                          SearchOptions searchOptions) const;

        void CreateDirectory(const std::string& path) const;

        std::unique_ptr<FileStream> Open(const std::string& path) const;

        std::unique_ptr<FileStreamReader> OpenRead(const std::string& path) const;

        std::unique_ptr<FileStreamWriter> OpenWrite(const std::string& path) const;

        void Delete(const std::string& path) const;
    };
}

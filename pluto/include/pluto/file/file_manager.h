#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <vector>
#include <string>

namespace pluto
{
    class FileWriter;
    class FileReader;
    class Regex;
    class Path;

    class PLUTO_API FileManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<FileManager> Create(const std::string& rootPath) const;
        };

        enum SearchOptions
        {
            TopDirectoryOnly = 0,
            AllDirectories = 1,
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit FileManager(std::unique_ptr<Impl> impl);

        ~FileManager();

        std::string GetRootPath() const;

        void SetRootPath(const std::string& value);

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

        std::unique_ptr<FileReader> OpenRead(const std::string& path) const;

        std::unique_ptr<FileWriter> OpenWrite(const std::string& path) const;

        void Delete(const std::string& path) const;
    };
}

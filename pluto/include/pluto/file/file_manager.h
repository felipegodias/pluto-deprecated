#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <vector>

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
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<FileManager> Create(const Path& rootPath) const;
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

        Path GetRootPath() const;

        void SetRootPath(const Path& value);

        bool Exists(const Path& path) const;

        bool IsFile(const Path& path) const;

        bool IsDirectory(const Path& path) const;

        std::vector<Path> GetDirectories(const Path& path) const;

        std::vector<Path> GetDirectories(const Path& path, SearchOptions searchOptions) const;

        std::vector<Path> GetDirectories(const Path& path, const Regex& regex) const;

        std::vector<Path> GetDirectories(const Path& path, const Regex& regex, SearchOptions searchOptions) const;

        std::vector<Path> GetFiles(const Path& path) const;

        std::vector<Path> GetFiles(const Path& path, SearchOptions searchOptions) const;

        std::vector<Path> GetFiles(const Path& path, const Regex& regex) const;

        std::vector<Path> GetFiles(const Path& path, const Regex& regex, SearchOptions searchOptions) const;

        void CreateDirectory(const Path& path) const;

        std::unique_ptr<FileReader> OpenRead(const Path& path) const;

        std::unique_ptr<FileWriter> OpenWrite(const Path& path) const;

        void Delete(const Path& path) const;
    };
}

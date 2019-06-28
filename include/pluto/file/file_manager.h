#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
#include <memory>
#include <string>
#include <vector>
#include <fstream>

namespace pluto
{
    class FileWriter;
    class FileReader;

    class PLUTO_API FileManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer & diContainer);
            std::unique_ptr<FileManager> Create(const std::string& dataDirectoryName) const;
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

        bool Exists(const std::string& path) const;

        bool IsFile(const std::string& path) const;

        bool IsDirectory(const std::string& path) const;

        std::vector<std::string> GetDirectories(const std::string& path) const;

        std::vector<std::string> GetDirectories(const std::string& path, const std::string& searchPattern) const;

        std::vector<std::string> GetDirectories(
            const std::string& path,
            const std::string& searchPattern,
            SearchOptions searchOptions) const;

        std::vector<std::string> GetFiles(const std::string& path) const;

        std::vector<std::string> GetFiles(const std::string& path, const std::string& searchPattern) const;

        std::vector<std::string> GetFiles(
            const std::string& path,
            const std::string& searchPattern,
            SearchOptions searchOptions) const;

        std::unique_ptr<FileReader> OpenRead(const std::string& path) const;

        std::unique_ptr<FileWriter> OpenWrite(const std::string& path) const;

        void Delete(const std::string& path) const;
    };
}

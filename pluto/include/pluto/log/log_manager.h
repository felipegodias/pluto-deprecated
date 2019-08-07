#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <string>
#include <memory>

namespace pluto
{
    class FileWriter;
    class Exception;

    class PLUTO_API LogManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<LogManager> Create(std::unique_ptr<FileWriter> logFile) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit LogManager(std::unique_ptr<Impl> impl);
        ~LogManager();
        void LogInfo(const std::string& message);
        void LogWarning(const std::string& message);
        void LogError(const std::string& message);
        void LogException(const Exception& exception);
    };
}

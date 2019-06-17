#include "pluto/log/log_manager.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace pluto
{
    class LogManager::Impl
    {
    private:
        std::shared_ptr<spdlog::logger> logger;

    public:
        explicit Impl(const std::string& logFileName)
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");
            logger = spdlog::stdout_color_mt("Pluto Engine");
            logger->set_level(spdlog::level::trace);
        }

        void LogInfo(const std::string& message) const
        {
            logger->info(message);
        }

        void LogWarning(const std::string& message) const
        {
            logger->warn(message);
        }

        void LogError(const std::string& message) const
        {
            logger->error(message);
        }
    };

    std::unique_ptr<LogManager> LogManager::Factory::Create(const std::string& logFileName)
    {
        return std::make_unique<LogManager>(std::make_unique<Impl>(logFileName));
    }

    LogManager::LogManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    LogManager::~LogManager() = default;

    void LogManager::LogInfo(const std::string& message)
    {
        impl->LogInfo(message);
    }

    void LogManager::LogWarning(const std::string& message)
    {
        impl->LogWarning(message);
    }

    void LogManager::LogError(const std::string& message)
    {
        impl->LogError(message);
    }
}

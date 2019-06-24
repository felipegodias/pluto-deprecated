#include <pluto/log/log_manager.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <fmt/ostream.h>
#include <boost/stacktrace.hpp>

namespace pluto
{
    class LogManager::Impl
    {
    private:
        std::unique_ptr<spdlog::logger> logger;

    public:
        explicit Impl(const std::string& logFileName)
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");
            const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            const auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFileName);
            spdlog::sinks_init_list sinkList{consoleSink, fileSink};
            logger = std::make_unique<spdlog::logger>("Pluto Engine", sinkList);
            logger->set_level(spdlog::level::trace);
            LogInfo("LogManager Initialized!");
        }

        ~Impl()
        {
            LogInfo("LogManager Terminated!");
        }

        void LogInfo(const std::string& message) const
        {
            logger->info(fmt::format("{0}\n{1}", message, boost::stacktrace::stacktrace()));
        }

        void LogWarning(const std::string& message) const
        {
            logger->warn(fmt::format("{0}\n{1}", message, boost::stacktrace::stacktrace()));
        }

        void LogError(const std::string& message) const
        {
            logger->error(fmt::format("{0}\n{1}", message, boost::stacktrace::stacktrace()));
        }
    };

    LogManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

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

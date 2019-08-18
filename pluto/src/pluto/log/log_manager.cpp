#include <pluto/log/log_manager.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_writer.h>
#include <pluto/service/service_collection.h>
#include <pluto/exception.h>
#include <pluto/stack_trace.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <fmt/ostream.h>

namespace pluto
{
    class LogManager::Impl
    {
        std::unique_ptr<FileWriter> logFile;
        std::unique_ptr<spdlog::logger> logger;
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
        std::shared_ptr<spdlog::sinks::ostream_sink_mt> fileSink;

    public:
        explicit Impl(std::unique_ptr<FileWriter> logFile)
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");

            consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            spdlog::sinks_init_list sinkList;
            if (logFile == nullptr)
            {
                sinkList = spdlog::sinks_init_list{consoleSink};
            }
            else
            {
                std::ofstream& ofs = logFile->GetStream();
                fileSink = std::make_shared<spdlog::sinks::ostream_sink_mt>(ofs);
                sinkList = spdlog::sinks_init_list{consoleSink, fileSink};
                this->logFile = std::move(logFile);
            }

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
            logger->info(fmt::format("{0}\n{1}", message, StackTrace(5)));
        }

        void LogWarning(const std::string& message) const
        {
            logger->warn(fmt::format("{0}\n{1}", message, StackTrace(5)));
        }

        void LogError(const std::string& message) const
        {
            logger->error(fmt::format("{0}\n{1}", message, StackTrace(5)));
        }

        void LogException(const Exception& exception)
        {
            logger->error(fmt::format("{0}\n{1}", exception.what(), exception.GetStackTrace()));
        }
    };

    LogManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<LogManager> LogManager::Factory::Create(std::unique_ptr<FileWriter> logFile) const
    {
        return std::make_unique<LogManager>(std::make_unique<Impl>(std::move(logFile)));
    }

    LogManager::LogManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
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

    void LogManager::LogException(const Exception& exception)
    {
        impl->LogException(exception);
    }
}

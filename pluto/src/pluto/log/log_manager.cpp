#include <pluto/log/log_manager.h>
#include <pluto/file/file_stream_writer.h>
#include <pluto/service/service_collection.h>
#include <pluto/exception.h>
#include <pluto/stack_trace.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ostream_sink.h>
#include <fmt/ostream.h>

namespace pluto
{
    class FileWriterSink final : public spdlog::sinks::base_sink<std::mutex>
    {
        std::unique_ptr<FileStreamWriter> fileWriter;

    public:
        ~FileWriterSink() = default;

        explicit FileWriterSink(std::unique_ptr<FileStreamWriter> writer)
            : fileWriter(std::move(writer))
        {
        }

        FileWriterSink(const FileWriterSink& other) = delete;
        FileWriterSink(FileWriterSink&& other) noexcept = default;
        FileWriterSink& operator=(const FileWriterSink& rhs) = delete;
        FileWriterSink& operator=(FileWriterSink&& rhs) noexcept = default;

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            fmt::memory_buffer formatted;
            formatter_->format(msg, formatted);
            fileWriter->Write(formatted.data(), static_cast<std::streamsize>(formatted.size()));
            fileWriter->Flush();
        }

        void flush_() override
        {
            fileWriter->Flush();
        }
    };

    class LogManager::Impl final
    {
        std::unique_ptr<spdlog::logger> logger;
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
        std::shared_ptr<FileWriterSink> fileSink;

    public:
        explicit Impl(std::unique_ptr<FileStreamWriter> logFile)
        {
#ifndef NDEBUG
            spdlog::set_pattern("%^[%T] %n: %v%$");

            consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            spdlog::sinks_init_list sinkList;
            if (logFile == nullptr)
            {
                sinkList = spdlog::sinks_init_list{consoleSink};
            }
            else
            {
                fileSink = std::make_shared<FileWriterSink>(std::move(logFile));
                sinkList = spdlog::sinks_init_list{consoleSink, fileSink};
            }

            logger = std::make_unique<spdlog::logger>("Pluto Engine", sinkList);
            logger->set_level(spdlog::level::trace);
#endif
            LogInfo("LogManager Initialized!");
        }

        ~Impl()
        {
            LogInfo("LogManager Terminated!");
        }

        void LogInfo(const std::string& message) const
        {
            if (logger != nullptr)
            {
                logger->info(fmt::format("{0}\n{1}", message, StackTrace(5)));
            }
        }

        void LogWarning(const std::string& message) const
        {
            if (logger != nullptr)
            {
                logger->warn(fmt::format("{0}\n{1}", message, StackTrace(5)));
            }
        }

        void LogError(const std::string& message) const
        {
            if (logger != nullptr)
            {
                logger->error(fmt::format("{0}\n{1}", message, StackTrace(5)));
            }
        }

        void LogException(const Exception& exception)
        {
            if (logger != nullptr)
            {
                logger->error(fmt::format("{0}\n{1}", exception.what(), exception.GetStackTrace()));
            }
        }
    };

    LogManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<LogManager> LogManager::Factory::Create(std::unique_ptr<FileStreamWriter> logFile) const
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

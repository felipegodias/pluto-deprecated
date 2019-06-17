#pragma once

#include "../api.h"
#include <string>
#include <memory>

namespace pluto
{
    class PLUTO_API LogManager
    {
    public:
        class Factory
        {
        public:
            static std::unique_ptr<LogManager> Create(const std::string& logFileName);
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
    };
}

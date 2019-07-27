#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <string>

namespace pluto
{
    class ServiceCollection;
    class FileReader;

    class PLUTO_API ConfigManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<ConfigManager> Create(FileReader* configFile) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit ConfigManager(std::unique_ptr<Impl> impl);
        ~ConfigManager();

        bool GetBool(const std::string& key, bool defaultValue) const;
        int GetInt(const std::string& key, int defaultValue) const;
        float GetFloat(const std::string& key, float defaultValue) const;
        std::string GetString(const std::string& key, const std::string& defaultValue) const;
    };
}

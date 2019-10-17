#include <pluto/config/config_manager.h>
#include <pluto/file/file_manager.h>
#include <pluto/file/file_reader.h>
#include <pluto/log/log_manager.h>
#include <pluto/service/service_collection.h>

#include <unordered_map>
#include <string>
#include <yaml-cpp/yaml.h>

namespace pluto
{
    class ConfigManager::Impl
    {
    private:
        LogManager& logManager;
        std::unordered_map<std::string, std::string> config;

    public:
        Impl(FileReader* configFile, LogManager& logManager)
            : logManager(
                logManager)
        {
            if (configFile != nullptr)
            {
                YAML::Node configYaml = YAML::Load(configFile->ReadAllText());
                for (YAML::const_iterator it = configYaml.begin(); it != configYaml.end(); ++it)
                {
                    if (!it->second.IsScalar())
                    {
                        continue;
                    }

                    config[it->first.as<std::string>()] = it->second.as<std::string>();
                }
            }

            logManager.LogInfo("ConfigManager Initialized!");
        }

        ~Impl()
        {
            logManager.LogInfo("ConfigManager Terminated!");
        }

        bool GetBool(const std::string& key, const bool defaultValue) const
        {
            const auto& it = config.find(key);
            if (it == config.end())
            {
                return defaultValue;
            }

            return std::stoi(it->second) != 0;
        }

        int GetInt(const std::string& key, const int defaultValue) const
        {
            const auto& it = config.find(key);
            if (it == config.end())
            {
                return defaultValue;
            }

            return std::stoi(it->second);
        }

        float GetFloat(const std::string& key, const float defaultValue) const
        {
            const auto& it = config.find(key);
            if (it == config.end())
            {
                return defaultValue;
            }

            return std::stof(it->second);
        }

        std::string GetString(const std::string& key, const std::string& defaultValue) const
        {
            const auto& it = config.find(key);
            if (it == config.end())
            {
                return defaultValue;
            }

            return it->second;
        }
    };

    ConfigManager::Factory::Factory(ServiceCollection& serviceCollection)
        : BaseFactory(serviceCollection)
    {
    }

    std::unique_ptr<ConfigManager> ConfigManager::Factory::Create(FileReader* configFile) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& logManager = serviceCollection.GetService<LogManager>();
        return std::make_unique<ConfigManager>(std::make_unique<Impl>(configFile, logManager));
    }

    ConfigManager::ConfigManager(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    ConfigManager::~ConfigManager() = default;

    bool ConfigManager::GetBool(const std::string& key, const bool defaultValue) const
    {
        return impl->GetBool(key, defaultValue);
    }

    int ConfigManager::GetInt(const std::string& key, const int defaultValue) const
    {
        return impl->GetInt(key, defaultValue);
    }

    float ConfigManager::GetFloat(const std::string& key, const float defaultValue) const
    {
        return impl->GetFloat(key, defaultValue);
    }

    std::string ConfigManager::GetString(const std::string& key, const std::string& defaultValue) const
    {
        return impl->GetString(key, defaultValue);
    }
}

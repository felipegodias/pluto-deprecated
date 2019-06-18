#include <pluto/config/config_manager.h>
#include <pluto/log/log_manager.h>
#include <pluto/di/di_container.h>
#include <unordered_map>
#include <string>

namespace pluto
{
    class ConfigManager::Impl
    {
    private:
        LogManager& logManager;
        std::unordered_map<std::string, std::string> config;

    public:
        Impl(const std::string& configFileName, LogManager& logManager) : logManager(logManager)
        {
            config.emplace("appName", "Pluto");
            config.emplace("defaultScreenWidth", "800");
            config.emplace("defaultScreenHeight", "600");

            logManager.LogInfo("ConfigManager Initialized!");
        }

        ~Impl()
        {
            logManager.LogInfo("ConfigManager Terminated!");
        }

        bool GetBool(const std::string& key) const
        {
            const std::string value = GetString(key);
            return std::stoi(value) != 0;
        }

        int GetInt(const std::string& key) const
        {
            const std::string value = GetString(key);
            return std::stoi(value);
        }

        float GetFloat(const std::string& key) const
        {
            const std::string value = GetString(key);
            return std::stof(value);
        }

        std::string GetString(const std::string& key) const
        {
            return config.at(key);
        }
    };

    ConfigManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<ConfigManager> ConfigManager::Factory::Create(const std::string& configFileName)
    {
        auto& logManager = diContainer.Resolve<LogManager>();
        return std::make_unique<ConfigManager>(std::make_unique<Impl>(configFileName, logManager));
    }

    ConfigManager::ConfigManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    ConfigManager::~ConfigManager() = default;

    bool ConfigManager::GetBool(const std::string& key) const
    {
        return impl->GetBool(key);
    }

    int ConfigManager::GetInt(const std::string& key) const
    {
        return impl->GetInt(key);
    }

    float ConfigManager::GetFloat(const std::string& key) const
    {
        return impl->GetFloat(key);
    }

    std::string ConfigManager::GetString(const std::string& key) const
    {
        return impl->GetString(key);
    }
}

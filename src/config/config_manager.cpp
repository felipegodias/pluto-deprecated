#include <pluto/config/config_manager.h>

namespace pluto
{
    class ConfigManager::Impl
    {
    public:
        explicit Impl(const std::string& configFileName)
        {
        }
    };

    ConfigManager::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<ConfigManager> ConfigManager::Factory::Create(const std::string& configFileName)
    {
        return std::make_unique<ConfigManager>(std::make_unique<Impl>(configFileName));
    }

    ConfigManager::ConfigManager(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    ConfigManager::~ConfigManager() = default;

    bool ConfigManager::GetBool(const std::string& key) const
    {
        return false;
    }

    int ConfigManager::GetInt(const std::string& key) const
    {
        return 0;
    }

    float ConfigManager::GetFloat(const std::string& key) const
    {
        return 0;
    }

    std::string ConfigManager::GetString(const std::string& key) const
    {
        return "";
    }
}

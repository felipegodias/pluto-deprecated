#pragma once

#include "../singleton.h"
#include "../base_factory.h"
#include <memory>
#include <string>

namespace pluto
{
    class DiContainer;

    class PLUTO_API ConfigManager final : public ISingleton
    {
    public:
        class Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<ConfigManager> Create(const std::string& configFileName);
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit ConfigManager(std::unique_ptr<Impl> impl);
        ~ConfigManager();

        bool GetBool(const std::string& key) const;
        int GetInt(const std::string& key) const;
        float GetFloat(const std::string& key) const;
        std::string GetString(const std::string& key) const;
    };
}

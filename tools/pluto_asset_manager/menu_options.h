#pragma once

#include <string>
#include <ostream>
#include <unordered_map>
#include <functional>

namespace pluto
{
    class MenuOptions
    {
        struct OptionData
        {
            std::string msg;
            std::function<void()> callback;
        };

        std::string title;
        std::unordered_map<int, OptionData> options;

    public:
        explicit MenuOptions(std::string title);

        void AddOption(int id, const std::string& msg, const std::function<void()>& callback);
        bool UseOption(int id) const;

        friend std::ostream& operator<<(std::ostream& os, const MenuOptions& menuOptions);
    };
}

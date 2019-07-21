#include "menu_options.h"

#include <algorithm>

namespace pluto
{
    MenuOptions::MenuOptions(std::string title) : title(std::move(title))
    {
    }

    void MenuOptions::AddOption(int id, const std::string& msg, const std::function<void()>& callback)
    {
        options.emplace(id, OptionData{msg, callback});
    }

    bool MenuOptions::UseOption(const int id) const
    {
        const auto it = options.find(id);
        if (it == options.end())
        {
            return false;
        }

        it->second.callback();
        return true;
    }

    std::ostream& operator<<(std::ostream& os, const MenuOptions& menuOptions)
    {
        std::vector<int> options;
        for (auto& it : menuOptions.options)
        {
            if (it.first == 0)
            {
                continue;
            }

            options.push_back(it.first);
        }
        std::sort(options.begin(), options.end());

        os << "*** " << menuOptions.title << " ***" << std::endl;
        for (auto& option : options)
        {
            const MenuOptions::OptionData& op = menuOptions.options.at(option);
            os << option << ": " << op.msg << std::endl;
        }
        const auto it = menuOptions.options.find(0);
        if (it != menuOptions.options.end())
        {
            os << "0: " << it->second.msg << std::endl;
        }

        return os;
    }
}

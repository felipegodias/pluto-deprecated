#pragma once

#include "api.h"
#include <string>
#include <vector>
#include <regex>

namespace pluto
{
    class PLUTO_API Regex
    {
    private:
        std::regex regex;

    public:
        Regex();
        explicit Regex(const std::string& pattern);
        explicit Regex(std::regex regex);
        ~Regex();

        Regex(const Regex& other);
        Regex(Regex&& other) noexcept;

        Regex& operator=(const Regex& rhs);
        Regex& operator=(Regex&& rhs) noexcept;

        bool Match(const std::string& str) const;
        std::vector<std::string> Search(std::string str) const;
        std::string Replace(const std::string& str, const std::string& fmt) const;
    };
}

#include <pluto/regex.h>

namespace pluto
{
    Regex::Regex() = default;

    Regex::Regex(const std::string& pattern) : regex(pattern)
    {
    }

    Regex::Regex(std::regex regex) : regex(std::move(regex))
    {
    }

    Regex::Regex(const Regex& other) : regex(other.regex)
    {
    }

    Regex::Regex(Regex&& other) noexcept
    {
    }

    Regex::~Regex() = default;

    Regex& Regex::operator=(const Regex& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        regex = rhs.regex;
        return *this;
    }

    Regex& Regex::operator=(Regex&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        regex = std::move(rhs.regex);
        return *this;
    }

    bool Regex::Match(const std::string& str) const
    {
        return std::regex_match(str, regex);
    }

    std::vector<std::string> Regex::Search(std::string str) const
    {
        std::vector<std::string> result;
        std::smatch match;
        while (std::regex_search(str, match, regex))
        {
            result.push_back(match[0]);
            str = match.suffix().str();
        }
        return result;
    }

    std::string Regex::Replace(const std::string& str, const std::string& fmt) const
    {
        return std::regex_replace(str, regex, fmt);
    }
}

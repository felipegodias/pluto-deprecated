#include "pluto/math/color.h"
#include "pluto/math/vector4f.h"
#include "pluto/exception.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace pluto
{
    const std::unordered_map<char, uint8_t> ASCII_HEX_TO_UINT8 = {
        {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6},
        {'7', 7}, {'8', 8}, {'9', 9}, {'a', 10}, {'A', 10}, {'b', 11}, {'B', 11},
        {'c', 12}, {'C', 12}, {'d', 13}, {'D', 13}, {'e', 14}, {'E', 14}, {'f', 15}, {'F', 15}
    };

    inline uint8_t HexToUInt8(const char c)
    {
        return ASCII_HEX_TO_UINT8.find(c) != ASCII_HEX_TO_UINT8.end() ? ASCII_HEX_TO_UINT8.at(c) : 0;
    }

    inline uint8_t NormColorFloat(const float v)
    {
        return std::clamp(static_cast<int>(v * 255), 0, 255);
    }

    inline uint8_t UInt8Lerp(const uint8_t from, const uint8_t to, const float t)
    {
        return from + static_cast<uint8_t>(static_cast<float>(to - from) * t);
    }

    const Color Color::BLACK = {0, 0, 0, 255};
    const Color Color::BLUE = {0, 0, 255, 255};
    const Color Color::CLEAR = {0, 0, 0, 0};
    const Color Color::CYAN = {0, 255, 255, 255};
    const Color Color::GREEN = {0, 255, 0, 255};
    const Color Color::GREY = {128, 128, 128, 128};
    const Color Color::MAGENTA = {255, 0, 255, 255};
    const Color Color::RED = {255, 0, 0, 255};
    const Color Color::WHITE = {255, 255, 255, 255};
    const Color Color::YELLOW = {255, 255, 0, 255};

    Color::Color() : r(0), g(0), b(0), a(0)
    {
    }

    Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a)
    {
    }

    Color::Color(const Vector4F& vector) : r(NormColorFloat(vector.x)),
                                           g(NormColorFloat(vector.y)),
                                           b(NormColorFloat(vector.z)),
                                           a(NormColorFloat(vector.w))
    {
    }

    Color::Color(const std::string& hex)
    {
        if (hex.size() != 8)
        {
            Exception::Throw(std::invalid_argument(""));
        }

        r = HexToUInt8(hex[0]) * 16 + HexToUInt8(hex[1]);
        g = HexToUInt8(hex[2]) * 16 + HexToUInt8(hex[3]);
        b = HexToUInt8(hex[4]) * 16 + HexToUInt8(hex[5]);
        a = HexToUInt8(hex[6]) * 16 + HexToUInt8(hex[7]);
    }

    bool Color::operator==(const Color& rhs) const
    {
        return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    uint8_t& Color::operator[](const uint8_t index)
    {
        switch (index)
        {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            Exception::Throw(std::out_of_range(""));
        }
        return r;
    }

    uint8_t Color::operator[](const uint8_t index) const
    {
        switch (index)
        {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            Exception::Throw(std::out_of_range(""));
        }
        return 0;
    }

    std::ostream& operator<<(std::ostream& os, const Color& color)
    {
        return os << "[" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "]";
    }

    std::string Color::Str() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    std::string Color::HexStr() const
    {
        std::stringstream ss;
        ss << std::hex << r;
        ss << std::hex << g;
        ss << std::hex << b;
        ss << std::hex << a;
        return ss.str();
    }

    uint8_t* Color::Data()
    {
        return &r;
    }

    const uint8_t* Color::Data() const
    {
        return &r;
    }

    Color Color::Lerp(const Color& from, const Color& to, const float t)
    {
        return Color(UInt8Lerp(from.r, to.r, t), UInt8Lerp(from.g, to.g, t), UInt8Lerp(from.b, to.b, t),
                     UInt8Lerp(from.a, to.a, t));
    }
}

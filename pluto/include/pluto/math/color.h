#pragma once

#include "../api.h"
#include <ostream>
#include <string>

namespace pluto
{
    class Vector4F;

    class PLUTO_API Color
    {
    public:
        static const Color BLACK;
        static const Color BLUE;
        static const Color CLEAR;
        static const Color CYAN;
        static const Color GREEN;
        static const Color GREY;
        static const Color MAGENTA;
        static const Color RED;
        static const Color WHITE;
        static const Color YELLOW;

        uint8_t r, g, b, a;

        Color();
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        Color(float r, float g, float b, float a);
        explicit Color(const std::string& hex);
        Color(const Color& other);
        Color(Color&& other) noexcept;

        ~Color();

        Color& operator=(const Color& rhs);
        Color& operator=(Color&& rhs) noexcept;

        bool operator==(const Color& rhs) const;
        bool operator!=(const Color& rhs) const;
        uint8_t& operator[](uint8_t index);
        uint8_t operator[](uint8_t index) const;
        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Color& vector);

        std::string Str() const;
        std::string HexStr() const;
        uint8_t* Data();
        const uint8_t* Data() const;

        static Color Lerp(const Color& from, const Color& to, float t);
    };
}

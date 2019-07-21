#pragma once

#include <functional>

namespace pluto
{
    class MenuOptions;

    class BaseMenu
    {
    public:
        virtual ~BaseMenu() = 0;
        BaseMenu();

        BaseMenu(const BaseMenu& other) = delete;
        BaseMenu(BaseMenu&& other) noexcept = delete;
        BaseMenu& operator=(const BaseMenu& rhs) = delete;
        BaseMenu& operator=(BaseMenu&& rhs) noexcept = delete;

        virtual const MenuOptions& GetCurrentMenuOptions() const = 0;
    };
}

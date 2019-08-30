#pragma once

#include <type_traits>

namespace pluto
{
    template <typename T1, typename T2>
    struct HasEqualTo
    {
    private:
        template <typename U1, typename U2>
        static auto test() -> decltype(std::declval<U1>() == std::declval<U2>(), std::true_type());

        template <typename>
        static std::false_type test();

    public:

        static constexpr bool VALUE = std::is_same<decltype(test<T1, T2>()), std::true_type>::value;
    };
}

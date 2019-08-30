#pragma once

#include "pluto/api.h"
#include "pluto/type_traits.h"

#include <string>

namespace pluto
{
    class PLUTO_API Assert final
    {
    public:
        Assert() = delete;

        template <typename TFirst, typename TSecond,
                  std::enable_if_t<std::is_arithmetic_v<TFirst> && std::is_arithmetic_v<TSecond>, bool>  = false>
        static void constexpr AreApproximatelyEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message);
        template <typename TFirst, typename TSecond,
                  std::enable_if_t<std::is_arithmetic_v<TFirst> && std::is_arithmetic_v<TSecond>, bool>  = false>
        static void constexpr AreApproximatelyNotEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message);

        template <typename TFirst, typename TSecond,
                  std::enable_if_t<HasEqualTo<TFirst, TSecond>::VALUE, bool>  = false>
        static void constexpr AreEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message);
        template <typename TFirst, typename TSecond,
                  std::enable_if_t<HasEqualTo<TFirst, TSecond>::VALUE, bool>  = false>
        static void constexpr AreNotEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message);

        template <typename T, std::enable_if_t<HasEqualTo<T, nullptr_t>::VALUE, bool>  = false>
        static void constexpr IsNull(T&& value, const std::string& message);
        template <typename T, std::enable_if_t<HasEqualTo<T, nullptr_t>::VALUE, bool>  = false>
        static void constexpr IsNotNull(T&& value, const std::string& message);

        static void constexpr IsTrue(bool condition, const std::string& message);
        static void constexpr IsFalse(bool condition, const std::string& message);
    };
}

#include "assert.inl"

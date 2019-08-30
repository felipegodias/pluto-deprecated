#pragma once

#include <limits>

namespace pluto
{
#ifdef NDEBUG

    template <typename TFirst, typename TSecond,
              typename std::enable_if<std::is_arithmetic_v<TFirst> && std::is_arithmetic_v<TSecond>, bool>::type>
    constexpr void Assert::AreApproximatelyEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
    }

    template <typename TFirst, typename TSecond,
              typename std::enable_if<std::is_arithmetic_v<TFirst> && std::is_arithmetic_v<TSecond>, bool>::type>
    constexpr void Assert::AreApproximatelyNotEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
    }

    template <typename TFirst, typename TSecond,
              typename std::enable_if<HasEqualTo<TFirst, TSecond>::VALUE, bool>::type>
    constexpr void Assert::AreEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
    }

    template <typename TFirst, typename TSecond,
              typename std::enable_if<HasEqualTo<TFirst, TSecond>::VALUE, bool>::type>
    constexpr void Assert::AreNotEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
    }

    template <typename T, typename std::enable_if<HasEqualTo<T, std::nullptr_t>::VALUE, bool>::type>
    constexpr void Assert::IsNull(T&& value, const std::string& message)
    {
    }

    template <typename T, typename std::enable_if<HasEqualTo<T, std::nullptr_t>::VALUE, bool>::type>
    constexpr void Assert::IsNotNull(T&& value, const std::string& message)
    {
    }

#else

    template <typename TFirst, typename TSecond,
              typename std::enable_if<std::is_arithmetic_v<TFirst> && std::is_arithmetic_v<TSecond>, bool>::type>
    constexpr void Assert::AreApproximatelyEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
        IsTrue(abs(std::forward<TFirst>(lhs) - std::forward<TSecond>(rhs)) <= std::numeric_limits<double>::epsilon(),
               message);
    }

    template <typename TFirst, typename TSecond,
              typename std::enable_if<std::is_arithmetic_v<TFirst> && std::is_arithmetic_v<TSecond>, bool>::type>
    constexpr void Assert::AreApproximatelyNotEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
        IsTrue(abs(std::forward<TFirst>(lhs) - std::forward<TSecond>(rhs)) > std::numeric_limits<double>::epsilon(),
               message);
    }

    template <typename TFirst, typename TSecond,
              typename std::enable_if<HasEqualTo<TFirst, TSecond>::VALUE, bool>::type>
    constexpr void Assert::AreEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
        IsTrue(std::forward<TFirst>(lhs) == std::forward<TSecond>(rhs), message);
    }

    template <typename TFirst, typename TSecond,
              typename std::enable_if<HasEqualTo<TFirst, TSecond>::VALUE, bool>::type>
    constexpr void Assert::AreNotEqual(TFirst&& lhs, TSecond&& rhs, const std::string& message)
    {
        IsTrue(!(std::forward<TFirst>(lhs) == std::forward<TSecond>(rhs)), message);
    }

    template <typename T, typename std::enable_if<HasEqualTo<T, std::nullptr_t>::VALUE, bool>::type>
    constexpr void Assert::IsNull(T&& value, const std::string& message)
    {
        IsTrue(std::forward<T>(value) == nullptr, message);
    }

    template <typename T, typename std::enable_if<HasEqualTo<T, std::nullptr_t>::VALUE, bool>::type>
    constexpr void Assert::IsNotNull(T&& value, const std::string& message)
    {
        IsTrue(!(std::forward<T>(value) == nullptr), message);
    }

#endif
}

#pragma once

#include "pluto/exception.h"

#ifdef NDEBUG
#define __PLUTO_ASSERT_THAT__(expr, msg)
#else
#define __PLUTO_ASSERT_THAT__(expr, msg) if (!(expr)) { abort(); Exception::Throw(std::runtime_error((msg))); }
#endif

#define ASSERT_THAT_IS_TRUE(condition)\
__PLUTO_ASSERT_THAT__((condition), "Assert Failed! Condition was expected to be true but is false.")

#define ASSERT_THAT_IS_FALSE(condition)\
__PLUTO_ASSERT_THAT__(!(condition), "Assert Failed! Condition was expected to be false but is true.")

#define ASSERT_THAT_IS_NULL(value)\
__PLUTO_ASSERT_THAT__((value) == nullptr, "Assert Failed! Value was expected to be null but it is not.")

#define ASSERT_THAT_IS_NOT_NULL(value)\
__PLUTO_ASSERT_THAT__((value) != nullptr, "Assert Failed! Value was expected to be not null but it is null.")

#define ASSERT_THAT_ARE_EQUAL(lhs, rhs)\
__PLUTO_ASSERT_THAT__((lhs) == (rhs), "Assert Failed! Values were expected to be equal but are diferent.")

#define ASSERT_THAT_NOT_EQUAL(lhs, rhs) \
__PLUTO_ASSERT_THAT__((lhs) != (rhs), "Assert Failed! Values were expected to be diferent but are equal.")

#define ASSERT_THAT_ARE_APPROXIMATELY_EQUAL(lhs, rhs, e) \
__PLUTO_ASSERT_THAT__(abs((lhs) - (rhs)) <= (e), "Assert Failed! Values were expected to be aproximately equal but are diferent.")

#define ASSERT_THAT_ARE_APPROXIMATELY_NOT_EQUAL(lhs, rhs, e) \
__PLUTO_ASSERT_THAT__(abs((lhs) - (rhs)) > (e), "Assert Failed! Values were expected to be diferent but are aproximately equal.")

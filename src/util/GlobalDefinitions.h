// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cassert>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

// Add this to the function signature to force inlining the function
#define ALWAYS_INLINE __attribute__((always_inline))

// Casts a pointer to another pointer type and asserts that the cast was successful in debug mode
template <typename T> ALWAYS_INLINE static T spice_pointer_cast(auto source) {
  static_assert(std::is_pointer_v<decltype(source)>, "Source is not a pointer type");
  assert(dynamic_cast<T>(source) != nullptr);
  return static_cast<T>(source);
}

// Fail with an assertion error message
#define assert_fail(msg) assert(false && (msg))

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

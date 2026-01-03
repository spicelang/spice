// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cassert>
#include <type_traits>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif

// Add this to the function signature to force inlining the function
#define ALWAYS_INLINE __attribute__((always_inline))

// Casts a pointer to another pointer type and asserts that the cast was successful in debug mode
template <typename DstT, typename SrcT>
ALWAYS_INLINE static DstT spice_pointer_cast(SrcT source)
  requires(std::is_pointer_v<SrcT> && std::is_pointer_v<DstT>)
{
  assert(dynamic_cast<DstT>(source) != nullptr);
  return static_cast<DstT>(source);
}

// Fail with an assertion error message
#define assert_fail(msg) assert(false && (msg))

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

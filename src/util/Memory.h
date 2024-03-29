// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <atomic>
#include <cassert>
#include <cstdlib>
#include <new>

namespace spice::compiler {

// Typedefs
using byte = uint8_t;

class MemoryManager {
public:
  [[nodiscard]] virtual byte *allocate(size_t size) const = 0;
  virtual void deallocate(byte *ptr) const = 0;
};

class DefaultMemoryManager : public MemoryManager {
public:
  [[nodiscard]] byte *allocate(size_t size) const override { return static_cast<byte *>(malloc(size)); }
  void deallocate(byte *ptr) const override { free(static_cast<void *>(ptr)); }
};

} // namespace spice::compiler

// Overload new and delete operators for debugging purposes
#ifdef false
void *operator new(std::size_t n) noexcept(false) { return malloc(n); }
void operator delete(void *ptr) noexcept { free(ptr); }
#endif
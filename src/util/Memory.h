// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdlib>

namespace spice::compiler {

// Typedefs
using byte = uint8_t;

class MemoryManager {
protected:
  ~MemoryManager() = default;

public:
  [[nodiscard]] virtual byte *allocate(size_t size) const = 0;
  virtual void deallocate(byte *ptr) const = 0;
};

class DefaultMemoryManager final : public MemoryManager {
public:
  [[nodiscard]] byte *allocate(const size_t size) const override { return static_cast<byte *>(malloc(size)); }
  void deallocate(byte *ptr) const override { free(ptr); }
};

} // namespace spice::compiler

// Overload new and delete operators for debugging purposes
#ifdef SPICE_NEW_DELETE_OVERLOADED
void *operator new(size_t n) noexcept(false) { return malloc(n); }
void operator delete(void *ptr) noexcept { free(ptr); }
#endif

// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <algorithm>

namespace spice::compiler {

/**
 * RAII helper to save a value and restore it when the object of this class goes out of scope
 *
 * @tparam T Value type
 */
template <typename T> struct SaveAndRestore {
  // Constructors
  explicit SaveAndRestore(T &value) : value(value), oldValue(value) {}
  SaveAndRestore(T &value, const T &newValue) : value(value), oldValue(value) { value = newValue; }
  SaveAndRestore(T &value, T &&newValue) : value(value), oldValue(std::move(value)) { value = std::move(newValue); }

  // Destructor
  ~SaveAndRestore() { value = std::move(oldValue); }

  // Public methods
  const T &get() { return oldValue; }

private:
  // Private members
  T &value;
  T oldValue;
};

} // namespace spice::compiler

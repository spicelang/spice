// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

namespace spice::compiler {

/**
 * Factory class for managing program-wide organized threads and their ids
 */
class ThreadFactory {
public:
  // Constructors
  ThreadFactory() = default;
  ThreadFactory(const ThreadFactory &) = delete;

  // Public methods
  int getNextFunctionSuffix();
  [[nodiscard]] bool isUsingThreads() const;

private:
  // Members
  int nextFctSuffix = 0;
};

} // namespace spice::compiler
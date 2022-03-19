// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

/**
 * Factory class for managing program-wide organized threads and their ids
 */
class ThreadFactory {
public:
  // Public methods
  int getNextFunctionSuffix();

private:
  // Members
  int nextFctSuffix = 0;
};
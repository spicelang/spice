// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>

/**
 * Jobs:
 * - Visit the import statements of a source file and register the imported files as dependencies to the current one
 * - Load the exposed symbols of the imported file and register them in the name registry of the current source file
 */
class ImportCollector : private CompilerPass {
public:
  // Constructors

private:
  // Private members

};

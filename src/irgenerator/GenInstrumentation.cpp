// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IRGenerator.h"
#include "exception/CompilerError.h"

#include <driver/Driver.h>

namespace spice::compiler {

void IRGenerator::enableFunctionInstrumentation(llvm::Function *function) const {
  switch (cliOptions.instrumentation.sanitizer) {
  case Sanitizer::NONE: // GCOV_EXCL_LINE
    break;              // GCOV_EXCL_LINE
  case Sanitizer::ADDRESS:
    function->addFnAttr(llvm::Attribute::SanitizeAddress);
    break;
  case Sanitizer::THREAD:
    function->addFnAttr(llvm::Attribute::SanitizeThread);
    break;
  case Sanitizer::MEMORY:
    function->addFnAttr(llvm::Attribute::SanitizeMemory);
    break;
  case Sanitizer::TYPE:
    function->addFnAttr(llvm::Attribute::SanitizeType);
    break;
  default:                                                             // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "Unhandled sanitizer type"); // GCOV_EXCL_LINE
  }
}

} // namespace spice::compiler

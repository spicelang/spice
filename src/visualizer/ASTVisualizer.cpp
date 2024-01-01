// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "ASTVisualizer.h"

#include <cxxabi.h>

#include <util/CommonUtil.h>

namespace spice::compiler {

std::string ASTVisualizer::getSpaces() const {
  std::string spaces;
  for (int i = 0; i < currentTabs; i++)
    spaces += " ";
  return spaces;
} // GCOVR_EXCL_LINE (false positive)

std::string ASTVisualizer::demangleTypeName(const char *mangledName) {
  int status;
  char *demangled = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
  if (status == 0) {
    std::string result(demangled);
    free(demangled);
    return result;
  }
  return mangledName;
}

} // namespace spice::compiler
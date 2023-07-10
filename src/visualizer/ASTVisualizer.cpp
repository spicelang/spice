// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ASTVisualizer.h"

#include <util/CommonUtil.h>

namespace spice::compiler {

std::string ASTVisualizer::getSpaces() const {
  std::string spaces;
  for (int i = 0; i < currentTabs; i++)
    spaces += " ";
  return spaces;
} // GCOVR_EXCL_LINE (false positive)

} // namespace spice::compiler
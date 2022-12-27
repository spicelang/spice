// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ASTVisualizer.h"

#include <util/CommonUtil.h>

std::string ASTVisualizer::getSpaces() const {
  std::string spaces;
  for (int i = 0; i < currentTabs; i++)
    spaces += " ";
  return spaces;
}
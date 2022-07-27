// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ASTVisualizerVisitor.h"

#include <util/CommonUtil.h>
#include <ast/AstNodes.h>

std::string ASTVisualizerVisitor::getSpaces() const {
  std::string spaces;
  for (int i = 0; i < currentTabs; i++)
    spaces += " ";
  return spaces;
}
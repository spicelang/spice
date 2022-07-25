// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CodeLoc.h"
std::string CodeLoc::toString() const {
  return "L" + std::to_string(line) + "C" + std::to_string(col);
}

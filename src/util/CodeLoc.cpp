// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CodeLoc.h"

std::string CodeLoc::toString() const { return "L" + std::to_string(line) + "C" + std::to_string(col); }

std::string CodeLoc::toPrettyString() const { return sourceFilePath + ":" + std::to_string(line) + ":" + std::to_string(col); }

std::string CodeLoc::toPrettyLine() const { return "l" + std::to_string(line); }
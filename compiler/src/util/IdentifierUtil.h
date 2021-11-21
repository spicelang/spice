// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>
#include <tree/TerminalNode.h>

/**
 * Util class for identifier-related work
 */
class IdentifierUtil {
public:
    static std::string getVarNameFromIdentList(std::vector<antlr4::tree::TerminalNode*>);
};
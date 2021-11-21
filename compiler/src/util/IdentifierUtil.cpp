// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "IdentifierUtil.h"

std::string IdentifierUtil::getVarNameFromIdentList(std::vector<antlr4::tree::TerminalNode*> identifiers) {
    std::string variableName = identifiers[0]->toString();
    for (int i = 1; i < identifiers.size(); i++)
        variableName += "." + identifiers[i]->toString();
    return variableName;
}

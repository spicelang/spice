// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <analyzer/SymbolTableEntry.h>
#include <analyzer/SymbolTable.h>

/**
 * Util class for identifier-related work
 */
class IdentifierUtil {
public:
    static std::string getVarNameFromIdentList(std::vector<antlr4::tree::TerminalNode*>);
    static SymbolTableEntry* getSymbolTableEntryByIdenList(const antlr4::Token&, SymbolTable*, std::vector<std::string>);
    static SymbolTableEntry* getSymbolTableEntryByIdenList(SymbolTable*, std::vector<antlr4::tree::TerminalNode*>);
};
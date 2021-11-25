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
    static SymbolTableEntry* getSymbolTableEntryByIdenList(const antlr4::Token&, SymbolTable* subTable,
                                                           std::vector<std::string> idenList);
    static SymbolTableEntry* getSymbolTableEntryByIdenList(SymbolTable* subTable,
                                                           std::vector<antlr4::tree::TerminalNode*> idenList);
};
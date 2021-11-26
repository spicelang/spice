// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "IdentifierUtil.h"

/**
 * Concat identifier list and join them with a dot delimiter
 *
 * @param identifiers Identifier list in form of antlr terminal nodes
 * @return String output
 */
std::string IdentifierUtil::getVarNameFromIdentList(std::vector<antlr4::tree::TerminalNode*> identifiers) {
    std::string variableName = identifiers[0]->toString();
    for (int i = 1; i < identifiers.size(); i++)
        variableName += "." + identifiers[i]->toString();
    return variableName;
}

/**
 * Retrieves symbol table entries by an identifier list. This method supports nested struct field discovery
 *
 * @param token Start token for potential error message outputs
 * @param subTable Symbol table, representing the current scope
 * @param idenList List of identifiers
 * @return Symbol table
 */
SymbolTableEntry* IdentifierUtil::getSymbolTableEntryByIdenList(const antlr4::Token& token, SymbolTable* subTable,
                                                                 std::vector<std::string> idenList) {
    // Check the variable and return it if
    SymbolTableEntry* entry = subTable->lookup(idenList[0]);
    // Check if the entry exists
    if (!entry)
        throw SemanticError(token, REFERENCED_UNDEFINED_VARIABLE,
                            "Variable " + idenList[0] + " was referenced before declared");
    // If it is no struct and the idenList has one item, return immediately
    if (entry->getType().getSuperType() != TYPE_STRUCT && entry->getType().getSuperType() != TYPE_STRUCT_PTR) {
        if (idenList.size() == 1) return entry;
        throw SemanticError(token, REFERENCED_UNDEFINED_STRUCT_FIELD,
                            "Cannot access nested fields in '" + entry->getName() + "'");
    }
    // It is a struct, but this was the list item of idenList, return the struct
    if (idenList.size() == 1) return entry;
    // It is a struct and we need a field
    std::string structName = entry->getType().getSubType();
    SymbolTable* structTable = subTable->lookupTable("struct:" + structName);
    // Check if the table exists
    if (!structTable)
        throw SemanticError(token, REFERENCED_UNDEFINED_STRUCT_FIELD,
                            "Referenced undefined struct '" + structName + "'");
    // Remove first element of idenList
    idenList.erase(idenList.begin());
    // Recurse
    return getSymbolTableEntryByIdenList(token, structTable, idenList);
}

/**
 * Retrieves symbol table entries by an identifier list. This method supports nested struct field discovery
 *
 * @param subTable Symbol table, representing the current scope
 * @param idenList List of identifiers
 * @return Symbol table
 */
SymbolTableEntry* IdentifierUtil::getSymbolTableEntryByIdenList(SymbolTable* subTable,
                                                                std::vector<antlr4::tree::TerminalNode*> idenList) {
    std::vector<std::string> stringVector;
    stringVector.reserve(idenList.size()); // Set fixed size due to performance reasons
    for (auto& segment : idenList) stringVector.push_back(segment->toString());
    return getSymbolTableEntryByIdenList(*idenList[0]->getSymbol(), subTable, stringVector);
}
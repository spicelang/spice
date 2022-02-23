// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <map>
#include <vector>
#include <queue>

#include <symbol/SymbolTableEntry.h>
#include <symbol/SymbolType.h>
#include <util/FunctionSignature.h>

#include <llvm/IR/BasicBlock.h>

#include "../../lib/json/json.hpp"

/**
 * Class for storing information about symbols of the AST. Symbol tables are meant to be arranged in a tree structure,
 * so that you can navigate with the getParent() and getChild() methods up and down the tree.
 */
class SymbolTable {
public:
    // Constructors
    explicit SymbolTable(SymbolTable* parent, bool inMainSourceFile) : parent(parent), inMainSourceFile(inMainSourceFile) {};

    // Public methods
    void insert(const std::string&, const SymbolType&, SymbolSpecifiers, SymbolState, const antlr4::Token&, bool);

    SymbolTableEntry* lookup(const std::string& symbolName);
    SymbolTableEntry* lookupByIndexInCurrentScope(unsigned int);
    SymbolTable* lookupTable(const std::string& tableName);
    SymbolTable* lookupTableWithSignature(const std::string& signature);
    SymbolTable* createChildBlock(const std::string& tableName);
    void mountChildBlock(const std::string& tableName, SymbolTable* symbolTable);
    void renameChildBlock(const std::string& oldName, const std::string& newName);

    SymbolTable* getParent();
    SymbolTable* getChild(const std::string& tableName);

    std::map<std::string, SymbolTableEntry>& getSymbols();

    unsigned int getFieldCount();

    void insertFunctionDeclaration(const std::string& functionName, const std::vector<SymbolType>& argTypes);
    std::vector<SymbolType> getFunctionDeclaration(const std::string& functionName);

    void insertProcedureDeclaration(const std::string& procedureName, const std::vector<SymbolType>& argTypes);
    std::vector<SymbolType> getProcedureDeclaration(const std::string& procedureName);

    void updateSymbolTypes(const SymbolType& oldType, const SymbolType& newType);

    void pushSignature(const FunctionSignature& signature);
    FunctionSignature popSignature();

    void printCompilerWarnings();

    nlohmann::json toJSON();

    void setImported();
    [[nodiscard]] bool isImported() const;

private:
    // Members
    SymbolTable* parent;
    std::map<std::string, SymbolTable> children;
    std::map<std::string, SymbolTableEntry> symbols;
    std::map<std::string, std::vector<SymbolType>> functionDeclarations;
    std::map<std::string, std::vector<SymbolType>> procedureDeclarations;
    std::vector<std::string> paramNames;
    std::queue<FunctionSignature> functionSignatures;
    bool inMainSourceFile;
    bool imported = false;
};
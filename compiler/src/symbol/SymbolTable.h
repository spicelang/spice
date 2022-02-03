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
    void insert(const std::string&, SymbolType, SymbolSpecifiers, SymbolState, const antlr4::Token&, bool);

    SymbolTableEntry* lookup(const std::string&);
    SymbolTableEntry* lookupByIndexInCurrentScope(unsigned int);
    SymbolTable* lookupTable(const std::string&);
    SymbolTable* lookupTableWithSignature(const std::string&);

    void update(const std::string&, SymbolState);
    void update(const std::string&, const SymbolType&);

    SymbolTable* createChildBlock(const std::string&);
    void mountChildBlock(const std::string&, SymbolTable*);
    void renameChildBlock(const std::string&, const std::string&);

    SymbolTable* getParent();
    SymbolTable* getChild(const std::string&);

    unsigned int getFieldCount();

    void insertFunctionDeclaration(const std::string&, const std::vector<SymbolType>&);
    std::vector<SymbolType> getFunctionDeclaration(const std::string&);

    void insertProcedureDeclaration(const std::string&, const std::vector<SymbolType>&);
    std::vector<SymbolType> getProcedureDeclaration(const std::string&);

    void updateSymbolTypes(const SymbolType&, const SymbolType&);

    void pushSignature(const FunctionSignature&);
    FunctionSignature popSignature();

    void setContinueBlock(llvm::BasicBlock*);
    llvm::BasicBlock* getContinueBlock() const;

    void setBreakBlock(llvm::BasicBlock*);
    llvm::BasicBlock* getBreakBlock() const;

    void printCompilerWarnings();

    nlohmann::ordered_json toJSON();

    void setImported();
    bool isImported() const;

private:
    // Members
    SymbolTable* parent;
    std::map<std::string, SymbolTable> children;
    std::map<std::string, SymbolTableEntry> symbols;
    std::map<std::string, std::vector<SymbolType>> functionDeclarations;
    std::map<std::string, std::vector<SymbolType>> procedureDeclarations;
    std::vector<std::string> paramNames;
    std::queue<FunctionSignature> functionSignatures;
    llvm::BasicBlock* continueBlock = nullptr;
    llvm::BasicBlock* breakBlock = nullptr;
    bool inMainSourceFile;
    bool imported = false;
};
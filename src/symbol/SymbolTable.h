// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <queue>
#include <string>
#include <vector>

#include <symbol/Capture.h>
#include <symbol/Function.h>
#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolTableEntry.h>
#include <symbol/SymbolType.h>

#include <llvm/IR/BasicBlock.h>

#include "../../lib/json/json.hpp"

/**
 * Class for storing information about symbols of the AST. Symbol tables are meant to be arranged in a tree structure,
 * so that you can navigate with the getParent() and getChild() methods up and down the tree.
 */
class SymbolTable {
public:
  // Constructors
  explicit SymbolTable(SymbolTable *parent, bool inMainSourceFile) : parent(parent), inMainSourceFile(inMainSourceFile){};

  // Public methods
  void insert(const std::string &name, const SymbolType &type, SymbolSpecifiers specifiers, SymbolState state,
              const antlr4::Token &token, bool isParameter);

  SymbolTableEntry *lookup(const std::string &symbolName);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupByIndexInCurrentScope(unsigned int);
  SymbolTableEntry *lookupGlobalByName(const std::string &globalName, bool skipThisScope = false);
  Capture *lookupCapture(const std::string &symbolName);
  Capture *lookupCaptureStrict(const std::string &symbolName);
  SymbolTable *lookupTable(const std::string &tableName);
  SymbolTable *lookupTableWithSignature(const std::string &signature);
  SymbolTable *createChildBlock(const std::string &tableName);
  void mountChildBlock(const std::string &tableName, SymbolTable *symbolTable);
  void renameChildBlock(const std::string &oldName, const std::string &newName);

  [[nodiscard]] SymbolTable *getParent() const;
  SymbolTable *getChild(const std::string &tableName);

  std::map<std::string, SymbolTableEntry> &getSymbols();
  std::map<std::string, Capture> &getCaptures();

  [[nodiscard]] unsigned int getFieldCount() const;

  void insertFunction(const Function &function, ErrorFactory *err, const antlr4::Token &token);
  const Function *matchFunction(const std::string &functionName, const SymbolType &thisType,
                                const std::vector<SymbolType> &argTypes, ErrorFactory *errorFactory, const antlr4::Token &token);
  Function *popFunctionAccessPointer();

  void printCompilerWarnings();

  [[nodiscard]] nlohmann::json toJSON() const;

  void setImported();
  [[nodiscard]] bool isImported() const;

  void setCapturingRequired();

private:
  // Members
  SymbolTable *parent;
  std::map<std::string, SymbolTable> children;
  std::map<std::string, SymbolTableEntry> symbols;
  std::map<std::string, Capture> captures;
  std::map<std::string, Function> functions;
  std::queue<Function *> functionAccessPointers;
  bool inMainSourceFile;
  bool imported = false;
  bool requiresCapturing = false;

  // Private methods
  void insertSubstantiatedFunction(const Function &function, ErrorFactory *err, const antlr4::Token &token);
};
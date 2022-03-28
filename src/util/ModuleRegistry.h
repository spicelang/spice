// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <string>
#include <vector>

#include "Token.h"
#include "symbol/SymbolTable.h"

/**
 * Class to remember source files, which have already been imported. This is required to detect circular dependencies
 */
class ModuleRegistry {
public:
  // Public methods
  void pushToImportPath(const std::string &modulePath);
  void popFromImportPath();
  void addToCompiledModules(const std::string &modulePath, SymbolTable *symbolTable);
  [[nodiscard]] bool causesCircularImport(const std::string &modulePath) const;
  [[nodiscard]] bool isAlreadyCompiled(const std::string &modulePath) const;
  [[nodiscard]] SymbolTable *getSymbolTable(const std::string &modulePath) const;

private:
  // Members
  std::map<std::string, SymbolTable *> compiledModules;
  std::stack<std::string> currentImportPath;
};
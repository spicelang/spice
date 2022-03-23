// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/SymbolType.h>

class Function {
public:
  // Constructors
  explicit Function(std::string name, SymbolType *thisType, SymbolType *returnType, std::vector<SymbolType *> argTypes)
      : name(std::move(name)), thisType(thisType), returnType(returnType), argTypes(std::move(argTypes)) {}

  // Public methods
  std::string getName();
  SymbolType *getThisType();
  SymbolType *getReturnType();
  std::vector<SymbolType *> getArgTypes();
  std::string getMangledName();
  [[nodiscard]] bool isFunction() const;
  [[nodiscard]] bool isProcedure() const;
  [[nodiscard]] bool isMethodFunction() const;
  [[nodiscard]] bool isMethodProcedure() const;

private:
  // Members
  std::string name;
  SymbolType *thisType = nullptr;
  SymbolType *returnType = nullptr;
  std::vector<SymbolType *> argTypes;
};
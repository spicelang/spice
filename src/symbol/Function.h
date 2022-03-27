// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

typedef std::vector<std::pair<SymbolType, bool>> ArgList;

class Function {
public:
  // Constructors
  explicit Function(std::string name, SymbolSpecifiers specifiers, SymbolType thisType, SymbolType returnType,
                    std::vector<std::pair<SymbolType, bool>> argTypes, std::vector<SymbolType> templateTypes)
      : name(std::move(name)), specifiers(specifiers), thisType(std::move(thisType)), returnType(std::move(returnType)),
        argTypes(std::move(argTypes)), templateTypes(std::move(templateTypes)) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] SymbolType getThisType() const;
  [[nodiscard]] SymbolType getReturnType() const;
  [[nodiscard]] std::vector<SymbolType> getArgTypes() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] bool isFunction() const;
  [[nodiscard]] bool isProcedure() const;
  [[nodiscard]] bool isMethodFunction() const;
  [[nodiscard]] bool isMethodProcedure() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] std::vector<Function> substantiate() const;
  [[nodiscard]] bool isSubstantiated() const;
  void setUsed();
  [[nodiscard]] bool isUsed() const;

private:
  // Members
  std::string name;
  SymbolSpecifiers specifiers;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType returnType = SymbolType(TY_DYN);
  std::vector<std::pair<SymbolType, bool>> argTypes;
  std::vector<SymbolType> templateTypes;
  bool used = false;
};
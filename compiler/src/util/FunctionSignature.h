// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <symbol/SymbolTableEntry.h>

/**
 * Util class for generating function/procedure signatures
 */
class FunctionSignature {
public:
    // Constructors
    explicit FunctionSignature(std::string& functionName) : functionName(functionName) {}
    FunctionSignature(std::string& functionName, std::vector<SymbolType> paramTypes) :
            functionName(functionName),
            paramTypes(std::move(paramTypes)) {}

    // Public methods
    std::string toString();
    std::string getFunctionName();
    std::vector<SymbolType> getParamTypes();

private:
    // Members
    std::string functionName;
    std::vector<SymbolType> paramTypes;
};
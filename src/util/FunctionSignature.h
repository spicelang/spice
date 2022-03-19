// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <symbol/SymbolType.h>

/**
 * Util class for generating function/procedure signatures
 */
class FunctionSignature {
public:
    // Constructors
    explicit FunctionSignature(std::string& functionName) : functionName(functionName) {}
    FunctionSignature(std::string& functionName, std::vector<SymbolType>  paramTypes) :
            functionName(functionName), paramTypes(std::move(paramTypes)) {}

    // Public methods
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] std::string getFunctionName() const;
    [[nodiscard]] std::vector<SymbolType> getParamTypes() const;

private:
    // Members
    std::string functionName;
    std::vector<SymbolType> paramTypes;
};
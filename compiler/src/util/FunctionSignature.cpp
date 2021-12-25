// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "FunctionSignature.h"

/**
 * Retrieve the signature of a function/procedure
 *
 * @return Signature of the function/procedure in form of a string
 */
std::string FunctionSignature::toString() {
    std::string params;
    if (!paramTypes.empty()) {
        for (auto& param : paramTypes)
            params.append(param.getName() + ",");
        params.pop_back();
    }
    return functionName + "(" + params + ")";
}

/**
 * Retrieve all parameter symbol types as vector
 *
 * @return Param types as vector
 */
std::vector<SymbolType> FunctionSignature::getParamTypes() {
    return paramTypes;
}

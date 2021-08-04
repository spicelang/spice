// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "FunctionSignature.h"

std::string FunctionSignature::toString() {
    std::string params;
    if (!paramTypes.empty()) {
        std::string typeName;
        for (auto& param : paramTypes) {
            switch (param) {
                case TYPE_DOUBLE: {
                    typeName = "double";
                    break;
                }
                case TYPE_INT: {
                    typeName = "int";
                    break;
                }
                case TYPE_STRING: {
                    typeName = "string";
                    break;
                }
                case TYPE_BOOL: {
                    typeName = "bool";
                    break;
                }
                case TYPE_DYN: {
                    throw std::runtime_error("Internal compiler error: Parameter type was dyn when generating signature");
                }
                case TYPE_FUNCTION: {
                    // Functions as parameters for functions are currently not supported, but in for completeness
                    typeName = "function";
                    break;
                }
                case TYPE_PROCEDURE: {
                    // Procedures as parameters for functions are currently not supported, but in for completeness
                    typeName = "procedure";
                    break;
                }
            }
            params.append(typeName + ",");
        }
        params.pop_back();
    }

    return functionName + "(" + params + ")";
}
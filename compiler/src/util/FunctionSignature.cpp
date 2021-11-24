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
        std::string typeName;
        for (auto& param : paramTypes) {
            switch (param.getSuperType()) {
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
                case TYPE_DOUBLE_PTR:
                    typeName = "double*";
                    break;
                case TYPE_INT_PTR:
                    typeName = "int*";
                    break;
                case TYPE_STRING_PTR:
                    typeName = "string*";
                    break;
                case TYPE_BOOL_PTR:
                    typeName = "bool*";
                    break;
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
                case TYPE_STRUCT:
                    typeName = "struct(" + param.getSubType() + "";
                    break;
                case TYPE_IMPORT:
                    typeName = "import";
                    break;
            }
            params.append(typeName + ",");
        }
        params.pop_back();
    }

    return functionName + "(" + params + ")";
}
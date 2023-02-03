// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <model/GenericType.h>

namespace spice::compiler {

// Forward declarations
class SymbolType;

/**
 * Helper class for FunctionManager and StructManager to match generic types.
 * This is a tricky task, because the types can be indirect or/and nested.
 */
class TypeMatcher {
public:
  static bool matchRequestedToCandidateType(SymbolType candidateType, SymbolType requestedType, TypeMapping &typeMapping,
                                            std::function<const GenericType *(const std::string &)> &resolveGenericType);
  static void substantiateTypeWithTypeMapping(SymbolType &symbolType, const TypeMapping &typeMapping);
};

} // namespace spice::compiler
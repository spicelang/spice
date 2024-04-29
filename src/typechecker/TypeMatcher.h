// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <model/GenericType.h>
#include <symboltablebuilder/QualType.h>

namespace spice::compiler {

/**
 * Helper class for FunctionManager and StructManager to match generic types.
 * This is a tricky task, because the types can be indirect or/and nested.
 */
class TypeMatcher {
public:
  // Typedefs
  using ResolverFct = std::function<const GenericType *(const std::string &)>;

  // Public methods
  static bool matchRequestedToCandidateTypes(const std::vector<Type> &candidateType, const std::vector<Type> &reqTypes,
                                             TypeMapping &typeMapping, ResolverFct &resolverFct, bool strictSpecifiers);
  static bool matchRequestedToCandidateType(Type candidateType, Type requestedType, TypeMapping &typeMapping,
                                            ResolverFct &resolverFct, bool strictSpecifierMatching);
  static void substantiateTypesWithTypeMapping(std::vector<Type> &symbolTypes, const TypeMapping &typeMapping);
  static void substantiateTypeWithTypeMapping(Type &type, const TypeMapping &typeMapping);
};

} // namespace spice::compiler
// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

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
  static bool matchRequestedToCandidateTypes(const QualTypeList &candidateTypes, const QualTypeList &reqTypes,
                                             TypeMapping &typeMapping, ResolverFct &resolverFct, bool strictQualifiers);
  static bool matchRequestedToCandidateType(QualType candidateType, QualType requestedType, TypeMapping &typeMapping,
                                            ResolverFct &resolverFct, bool strictQualifierMatching);
  static void substantiateTypesWithTypeMapping(QualTypeList &qualTypes, const TypeMapping &typeMapping, const ASTNode *node);
  static void substantiateTypeWithTypeMapping(QualType &type, const TypeMapping &typeMapping, const ASTNode *node);
};

} // namespace spice::compiler
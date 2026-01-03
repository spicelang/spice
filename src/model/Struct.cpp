// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "Struct.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

static constexpr auto STRUCT_SCOPE_PREFIX = "struct:";

/**
 * Retrieve the name of the scope, where members and methods are placed. This is used to navigate to the scope of the struct
 * from the parent scope.
 *
 * @return Name of the struct scope
 */
std::string Struct::getScopeName() const {
  const std::string &appendix = isGenericSubstantiation() ? getSignature() : name;
  return STRUCT_SCOPE_PREFIX + appendix;
}

/**
 * Retrieve the name of the scope, where members and methods are placed. This is used to navigate to the scope of the struct
 * from the parent scope.
 *
 * @return Name of the struct scope
 */
std::string Struct::getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes) {
  return STRUCT_SCOPE_PREFIX + getSignature(name, concreteTemplateTypes);
}

/**
 * Checks at least one field is a reference.
 * This is used to prohibit constant instantiations.
 *
 * @return Has reference as field type or not
 */
bool Struct::hasReferenceFields() const {
  return std::ranges::any_of(fieldTypes, [](const QualType &fieldType) { return fieldType.isRef(); });
}

} // namespace spice::compiler
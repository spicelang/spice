// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "Struct.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

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
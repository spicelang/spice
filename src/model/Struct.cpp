// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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

/**
 * Checks if there is at least one user-defined ctor procedure present, that is no copy ctor.
 *
 * @return Ctor present or not
 */
bool Struct::hasUserDefinedCtor() const {
  return false;
}

/**
 *  Checks if there is a user-defined copy ctor procedure present
 *
 * @return Copy ctor present or not
 */
bool Struct::hasUserDefinedCopyCtor() const {
  return false;
}


} // namespace spice::compiler
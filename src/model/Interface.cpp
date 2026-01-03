// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "Interface.h"

namespace spice::compiler {

static constexpr auto INTERFACE_SCOPE_PREFIX = "interface:";

/**
 * Retrieve the name of the scope, where signatures are placed. This is used to navigate to the scope of the interface
 * from the parent scope.
 *
 * @return Name of the interface scope
 */
std::string Interface::getScopeName() const {
  const std::string &appendix = isGenericSubstantiation() ? getSignature() : name;
  return INTERFACE_SCOPE_PREFIX + appendix;
}

/**
 * Retrieve the name of the scope, where signatures are placed. This is used to navigate to the scope of the interface
 * from the parent scope.
 *
 * @return Name of the interface scope
 */
std::string Interface::getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes) {
  return INTERFACE_SCOPE_PREFIX + getSignature(name, concreteTemplateTypes);
}

} // namespace spice::compiler

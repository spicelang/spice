// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeNameDisambiguator.h"

namespace spice::compiler {

// Static member initialization
std::unordered_map<std::string, std::vector<uint64_t>> TypeNameDisambiguator::claimedTypeIds = {};

/**
 * Get the disambiguation suffix for a struct/interface type with the given name and type id.
 * The first type id to claim a name keeps it unchanged (empty suffix); every further distinct type id sharing the name
 * receives a deterministic, dot-separated suffix (e.g. ".1", ".2"). A dot is used on purpose: it is not a valid Spice
 * identifier character, so the disambiguated name can never collide with a genuine, user-declared type name.
 *
 * @param name Simple type name
 * @param typeId Custom type id of the type
 * @return Disambiguation suffix (empty for the first claimer of the name)
 */
std::string TypeNameDisambiguator::getDisambiguationSuffix(const std::string &name, uint64_t typeId) {
  std::vector<uint64_t> &ids = claimedTypeIds[name];

  // Find the index of the type id among the ones already claiming this name
  size_t index = ids.size();
  for (size_t i = 0; i < ids.size(); i++) {
    if (ids.at(i) == typeId) {
      index = i;
      break;
    }
  }
  // If not seen yet, claim the name
  if (index == ids.size())
    ids.push_back(typeId);

  // The first claimer keeps the plain name to avoid churn for the common, non-colliding case
  if (index == 0)
    return "";
  return "." + std::to_string(index);
}

/**
 * Clear the disambiguation registry. Must be called between compilations.
 */
void TypeNameDisambiguator::clear() { claimedTypeIds.clear(); }

} // namespace spice::compiler

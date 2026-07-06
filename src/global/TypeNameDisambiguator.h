// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace spice::compiler {

/**
 * Two structs or interfaces that share a simple name but stem from independent declarations (e.g. a package-private
 * `Node<T>` declared separately in two source files, see issue #1253) are distinct types, but render to the same
 * name. That collision surfaces both as a type-checker conflation (same manifestation signature) and as a linker/codegen
 * clash (same mangled name). To keep them apart without affecting the common, non-colliding case, this registry hands
 * out a deterministic disambiguation suffix for the second and any further distinct type sharing a given name. The
 * first type to claim a name keeps it unchanged, so nothing changes unless a name is actually reused.
 *
 * The state is process-global (like TypeRegistry) and therefore must be cleared between compilations, since custom type
 * ids restart at the same value for every compilation.
 */
class TypeNameDisambiguator {
public:
  // Constructors
  TypeNameDisambiguator() = delete;
  TypeNameDisambiguator(const TypeNameDisambiguator &) = delete;

  // Public methods
  static std::string getDisambiguationSuffix(const std::string &name, uint64_t typeId);
  static void clear();

private:
  // Private members
  // Maps a simple type name to the list of distinct type ids claiming it, in first-seen order
  static std::unordered_map<std::string, std::vector<uint64_t>> claimedTypeIds;
};

} // namespace spice::compiler

// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <mutex>
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
 *
 * Note on the parallel back end: the suffix a type receives depends on the order in which the names are claimed. In
 * practice every struct/interface name is already claimed while the type checker builds signatures and diagnostics, so
 * the back end only ever reads back an existing claim. A name that is claimed for the very first time during back-end
 * name mangling would however get a scheduling-dependent index, which stays consistent within one compilation but may
 * differ between runs. Making the claim order independent of the scheduling (e.g. by ranking claimants by type id over
 * the complete, front-end-known set of types) is a follow-up.
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
  // The name mangler queries this registry, so it is reached from the IR generator and therefore from the worker threads
  // of the parallel back end.
  static std::mutex claimedTypeIdsMutex;
};

} // namespace spice::compiler

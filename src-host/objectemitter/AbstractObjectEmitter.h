// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <string>

// Forward declarations
namespace llvm {
  class Module;
} // namespace llvm

namespace spice::compiler {

/**
 * Abstract interface implemented by every concrete object emitter (LLVM CodeGen, TPDE, ...).
 *
 * Kept intentionally free of Spice-internal headers so it can be inherited by TUs that are compiled
 * with -fno-rtti (in particular the TPDE emitter, which has to match TPDE's compile-option ABI).
 */
class AbstractObjectEmitter {
public:
  // Destructor
  virtual ~AbstractObjectEmitter() = default;

  // Delete copies — object emitters own compile-time state and are always used as short-lived
  // stack objects at their construction site.
  AbstractObjectEmitter(const AbstractObjectEmitter &) = delete;
  AbstractObjectEmitter &operator=(const AbstractObjectEmitter &) = delete;

  /**
   * Emit an object file for the module associated with this emitter.
   *
   * @param objectPath Filesystem path where the object file should be written.
   */
  virtual void emit(const std::filesystem::path &objectPath) const = 0;

  /**
   * Write a textual representation of the emitter's output into @p output. LLVM CodeGen emits
   * a real assembly listing here; TPDE (which produces object bytes directly) returns a
   * placeholder note.
   *
   * @param output Destination string, cleared/filled by the emitter.
   */
  virtual void getASMString(std::string &output) const = 0;

protected:
  AbstractObjectEmitter() = default;
};

} // namespace spice::compiler

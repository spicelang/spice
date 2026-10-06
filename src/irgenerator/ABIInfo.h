// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include <llvm/IR/DataLayout.h>
#include <llvm/TargetParser/Triple.h>

// Forward declarations
namespace llvm {
class Type;
} // namespace llvm

namespace spice::compiler {

enum class ReturnABIKind : uint8_t {
  DIRECT,   // Returned as is
  COERCED,  // Returned in registers as another type with the same memory representation
  INDIRECT, // Returned via a hidden sret pointer to caller-allocated memory, passed as first argument
};

struct ReturnABIInfo {
  ReturnABIKind kind = ReturnABIKind::DIRECT;
  llvm::Type *type = nullptr;        // Type of the return value in memory
  llvm::Type *coercedType = nullptr; // Type, that is returned in registers (only set for COERCED)

  [[nodiscard]] bool isDirect() const { return kind == ReturnABIKind::DIRECT; }
  [[nodiscard]] bool isCoerced() const { return kind == ReturnABIKind::COERCED; }
  [[nodiscard]] bool isIndirect() const { return kind == ReturnABIKind::INDIRECT; }
};

/**
 * Lowers return values to the C calling convention of the target, like Clang does it. Aggregates, that do not fit into
 * the return registers, are returned via an sret pointer, small ones are coerced to the types, that are returned in the
 * return registers. Types, that are non-trivial for the purpose of calls (custom copy ctor or dtor), are always returned
 * via an sret pointer, like in the Itanium C++ ABI.
 */
class ABIInfo {
public:
  // Constructors
  ABIInfo(const llvm::Triple &targetTriple, const llvm::DataLayout &dataLayout);

  // Public methods
  [[nodiscard]] ReturnABIInfo classifyReturnType(llvm::Type *type, bool isNonTrivial) const;

private:
  // Private methods
  [[nodiscard]] ReturnABIInfo classifyReturnTypeX86_64SysV(llvm::Type *type) const;
  [[nodiscard]] ReturnABIInfo classifyReturnTypeWin64(llvm::Type *type) const;
  [[nodiscard]] ReturnABIInfo classifyReturnTypeAArch64(llvm::Type *type) const;
  [[nodiscard]] ReturnABIInfo classifyReturnTypeWebAssembly(llvm::Type *type) const;
  [[nodiscard]] llvm::Type *getIntegerTypeAtOffset(llvm::Type *type, uint64_t offset) const;
  [[nodiscard]] llvm::Type *getSSETypeAtOffset(llvm::Type *type, uint64_t offset) const;
  [[nodiscard]] llvm::Type *getScalarTypeAtOffset(llvm::Type *type, uint64_t offset) const;
  [[nodiscard]] bool bitsContainNoUserData(llvm::Type *type, uint64_t startBit, uint64_t endBit) const;
  void collectScalars(llvm::Type *type, uint64_t offset, std::vector<std::pair<llvm::Type *, uint64_t>> &scalars) const;
  [[nodiscard]] bool isHomogeneousFPAggregate(llvm::Type *type, llvm::Type *&baseType, uint64_t &memberCount) const;
  [[nodiscard]] llvm::Type *getSingleElementType(llvm::Type *type) const;
  [[nodiscard]] static ReturnABIInfo direct(llvm::Type *type);
  [[nodiscard]] static ReturnABIInfo coerced(llvm::Type *type, llvm::Type *coercedType);
  [[nodiscard]] static ReturnABIInfo indirect(llvm::Type *type);

  // Private members
  const llvm::Triple &targetTriple;
  const llvm::DataLayout &dataLayout;
};

} // namespace spice::compiler

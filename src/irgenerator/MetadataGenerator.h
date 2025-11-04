// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <llvm/IR/Instructions.h>
#include <llvm/IR/MDBuilder.h>

namespace spice::compiler {

// Forward declarations
class IRGenerator;
enum class Likelihood : uint8_t;
class QualType;

class MetadataGenerator {
public:
  // Constructors
  explicit MetadataGenerator(IRGenerator *irGenerator);

  // Public methods
  void generateBranchWeightsMetadata(llvm::BranchInst *jumpInst, Likelihood likeliness);

private:
  // Private members
  IRGenerator *irGenerator;
  llvm::MDBuilder mdBuilder;
};

} // namespace spice::compiler

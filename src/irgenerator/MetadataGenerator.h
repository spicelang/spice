// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <llvm/IR/MDBuilder.h>

// Forward declarations
namespace llvm {
class BranchInst;
} // namespace llvm

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
  void generateTypeMetadata(llvm::Instruction *inst, const QualType &type);
  void generateTBAAMetadata(llvm::Instruction *inst, const QualType &type);

private:
  // Private members
  IRGenerator *irGenerator;
  llvm::MDBuilder mdBuilder;
  llvm::MDNode *tbaaRoot;
  llvm::MDNode *omnipotentByte;
};

} // namespace spice::compiler

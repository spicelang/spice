// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "MetadataGenerator.h"

#include "irgenerator/IRGenerator.h"

namespace spice::compiler {

MetadataGenerator::MetadataGenerator(IRGenerator *irGenerator)
    : irGenerator(irGenerator), mdBuilder(irGenerator->context) {}

void MetadataGenerator::generateBranchWeightsMetadata(llvm::BranchInst *jumpInst, Likelihood likeliness) {
  if (likeliness == Likelihood::UNSPECIFIED)
    return;

  const bool likely = likeliness == Likelihood::LIKELY;
  llvm::MDNode *profMetadata = likely ? mdBuilder.createLikelyBranchWeights() : mdBuilder.createUnlikelyBranchWeights();
  jumpInst->setMetadata(llvm::LLVMContext::MD_prof, profMetadata);
}

} // namespace spice::compiler

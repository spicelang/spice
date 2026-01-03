// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "MetadataGenerator.h"

#include <global/TypeRegistry.h>
#include <irgenerator/IRGenerator.h>
#include <util/CustomHashFunctions.h>

#include <llvm/IR/Module.h>

namespace spice::compiler {

MetadataGenerator::MetadataGenerator(IRGenerator *irGenerator)
    : irGenerator(irGenerator), mdBuilder(irGenerator->context), tbaaRoot(mdBuilder.createTBAARoot("Simple Spice TBAA")),
      omnipotentByte(mdBuilder.createTBAAScalarTypeNode("omnipotent byte", tbaaRoot)) {}

void MetadataGenerator::generateBranchWeightsMetadata(llvm::BranchInst *jumpInst, Likelihood likeliness) {
  assert(likeliness != Likelihood::UNSPECIFIED);
  const bool likely = likeliness == Likelihood::LIKELY;
  llvm::MDNode *profMetadata = likely ? mdBuilder.createLikelyBranchWeights() : mdBuilder.createUnlikelyBranchWeights();
  jumpInst->setMetadata(llvm::LLVMContext::MD_prof, profMetadata);
}

void MetadataGenerator::generateTypeMetadata(llvm::Instruction *inst, const QualType &type) {
  const uint64_t typeHash = TypeRegistry::getTypeHash(*type.getType());

  llvm::MDString *typeName = mdBuilder.createString(type.getName(true, true));
  llvm::ConstantAsMetadata *typeId = mdBuilder.createConstant(irGenerator->builder.getInt64(typeHash));
  llvm::MDNode *typeMetadata = llvm::MDNode::get(irGenerator->context, {typeId, typeName});

  inst->setMetadata(llvm::LLVMContext::MD_type, typeMetadata);
}

void MetadataGenerator::generateTBAAMetadata(llvm::Instruction *inst, const QualType &type) {
  const std::string typeName = type.getName(true, true);
  llvm::MDNode *tbaaTypeNode = mdBuilder.createTBAAScalarTypeNode(typeName, omnipotentByte);
  // Seems so new, that the verifier does not accept it
  //const llvm::TypeSize typeSize = irGenerator->module->getDataLayout().getTypeAllocSize(llvmType);
  //const bool isImmutable = type.isConst();
  //llvm::MDNode *tbaaAccessTag = mdBuilder.createTBAAAccessTag(tbaaTypeNode, tbaaTypeNode, 0, typeSize, isImmutable);
  llvm::ConstantAsMetadata *offset = llvm::ConstantAsMetadata::get(irGenerator->builder.getInt64(0));
  llvm::MDNode *tbaaAccessTag = llvm::MDNode::get(irGenerator->context, {tbaaTypeNode, tbaaTypeNode, offset});

  inst->setMetadata(llvm::LLVMContext::MD_tbaa, tbaaAccessTag);
}

} // namespace spice::compiler

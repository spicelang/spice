// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <irgenerator/NameMangling.h>

namespace spice::compiler {

void IRGenerator::ensureRTTIRuntime() {
  // Request RTTI runtime
  sourceFile->requestRuntimeModule(RuntimeModule::RTTI_RT);
  // Add external global pointer for TypeInfo vtable
  const std::string mangledName = NameMangling::mangleVTable("TypeInfo");
  typeInfoExtPtr = module->getOrInsertGlobal(mangledName, builder.getPtrTy());
}

llvm::Constant *IRGenerator::generateTypeInfoName(const StructBase *spiceStruct) {
  // Resolve mangled type info name and mangled global name
  std::string typeInfoName = spiceStruct->name;
  const std::string globalName = NameMangling::mangleTypeInfoName(typeInfoName);

  // Generate global string constant
  llvm::Constant *globalAddr = builder.CreateGlobalStringPtr(typeInfoName, globalName, 0, module);
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(globalName));

  return global;
}

llvm::Constant *IRGenerator::generateTypeInfo(const StructBase *spiceStruct) {
  // Generate type info name
  llvm::Constant *typeInfoName = generateTypeInfoName(spiceStruct);
  // Generate LLVM type for type info
  const std::string mangledName = NameMangling::mangleTypeInfo(spiceStruct->name);
  std::vector<llvm::Type *> elementTypes;
  
  llvm::StructType *structType = llvm::StructType::get(context, elementTypes);
  // Generate global variable
  return module->getOrInsertGlobal(mangledName, structType);
}

llvm::Constant *IRGenerator::generateVTable(const StructBase *spiceStruct) {
  // Make sure that the RTTI runtime is present
  ensureRTTIRuntime();

  // Generate type info data structures
  generateTypeInfoName(spiceStruct);
  generateTypeInfo(spiceStruct);

  // Generate VTable itself
  // ToDo
  return nullptr;
}

} // namespace spice::compiler

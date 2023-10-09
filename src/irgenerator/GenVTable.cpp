// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <irgenerator/NameMangling.h>

namespace spice::compiler {

void IRGenerator::ensureRTTIRuntime() {
  assert(sourceFile->isRuntimeModuleAvailable(RuntimeModule::RTTI_RT));

  // Add external global pointer for TypeInfo vtable
  const std::string mangledName = NameMangling::mangleVTable("TypeInfo");
  typeInfoExtPtr = module->getOrInsertGlobal(mangledName, builder.getPtrTy());
}

llvm::Constant *IRGenerator::generateTypeInfoName(StructBase *spiceStruct) {
  // Resolve mangled type info name and mangled global name
  std::string typeInfoName = spiceStruct->name;
  const std::string globalName = NameMangling::mangleTypeInfoName(typeInfoName);

  // Generate global string constant
  spiceStruct->typeInfoName = builder.CreateGlobalStringPtr(typeInfoName, globalName, 0, module);
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(globalName));

  return global;
}

llvm::Constant *IRGenerator::generateTypeInfo(StructBase *spiceStruct) {
  assert(spiceStruct->entry != nullptr);
  const bool isInterface = spiceStruct->entry->getType().is(TY_INTERFACE);

  // Generate type info name
  llvm::Constant *typeInfoName = generateTypeInfoName(spiceStruct);

  // Generate LLVM type for type info
  const std::string mangledName = NameMangling::mangleTypeInfo(spiceStruct->name);
  llvm::PointerType *ptrTy = builder.getPtrTy();
  llvm::StructType *structType = llvm::StructType::get(context, {ptrTy, ptrTy});

  // Generate type info values
  std::vector<llvm::Constant *> fieldValues;
  fieldValues.push_back(llvm::ConstantExpr::getInBoundsGetElementPtr(ptrTy, typeInfoExtPtr, builder.getInt32(2)));
  fieldValues.push_back(typeInfoName);
  spiceStruct->typeInfo = llvm::ConstantStruct::get(structType, fieldValues);

  // Generate global variable
  module->getOrInsertGlobal(mangledName, structType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  global->setInitializer(spiceStruct->typeInfo);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(mangledName));

  return global;
}

llvm::Constant *IRGenerator::generateVTable(StructBase *spiceStruct) {
  // Make sure that the RTTI runtime is present
  ensureRTTIRuntime();

  // Generate type info data structures
  generateTypeInfo(spiceStruct);

  // Generate VTable type
  std::vector<llvm::Type *> fieldTypes;
  // ToDo
  llvm::StructType *structType = llvm::StructType::get(context, fieldTypes);

  // Generate VTable values
  spiceStruct->vtable = llvm::ConstantStruct::get(structType, {});

  const std::string mangledName = NameMangling::mangleVTable(spiceStruct->name);
  module->getOrInsertGlobal(mangledName, structType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  global->setInitializer(spiceStruct->vtable);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(mangledName));

  return global;
}

} // namespace spice::compiler

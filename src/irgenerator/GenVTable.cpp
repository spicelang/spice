// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <irgenerator/NameMangling.h>

namespace spice::compiler {

void IRGenerator::ensureRTTIRuntime() {
  // If we currently compile the RTTI runtime, we don't need to do anything here
  if (sourceFile->isRttiRT())
    return;

  // Add external global pointer for TypeInfo vtable
  assert(sourceFile->isRuntimeModuleAvailable(RuntimeModule::RTTI_RT));
  const std::string mangledName = NameMangling::mangleVTable("TypeInfo");
  typeInfoExtPtr = module->getOrInsertGlobal(mangledName, builder.getPtrTy());
}

llvm::Constant *IRGenerator::generateTypeInfoName(StructBase *spiceStruct) {
  // Resolve mangled type info name and mangled global name
  const std::string globalName = NameMangling::mangleTypeInfoName(spiceStruct);

  // Generate global string constant
  builder.CreateGlobalStringPtr(NameMangling::mangleTypeInfoValue(spiceStruct->name), globalName, 0, module);
  llvm::GlobalVariable *global = module->getNamedGlobal(globalName);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(globalName));

  return spiceStruct->typeInfoName = global;
}

llvm::Constant *IRGenerator::generateTypeInfo(StructBase *spiceStruct) {
  assert(spiceStruct->entry != nullptr);

  // Generate type info name
  llvm::Constant *typeInfoName = generateTypeInfoName(spiceStruct);

  // Generate LLVM type for type info
  const std::string mangledName = NameMangling::mangleTypeInfo(spiceStruct);
  llvm::PointerType *ptrTy = builder.getPtrTy();

  std::vector<SymbolType> interfaceTypes;
  if (spiceStruct->entry->getType().is(TY_STRUCT)) {
    auto spiceStructEnsured = static_cast<Struct *>(spiceStruct);
    interfaceTypes = spiceStructEnsured->interfaceTypes;
  }

  // Build type info LLVM type
  std::vector<llvm::Type *> typeInfoFieldTypes = {ptrTy, ptrTy};
  for (const SymbolType &interfaceType : interfaceTypes)
    typeInfoFieldTypes.push_back(ptrTy);
  llvm::StructType *structType = llvm::StructType::get(context, typeInfoFieldTypes);

  // Generate type info values
  llvm::Constant *typeInfoVTable = llvm::Constant::getNullValue(ptrTy);
  if (!sourceFile->isRttiRT())
    typeInfoVTable = llvm::ConstantExpr::getInBoundsGetElementPtr(ptrTy, typeInfoExtPtr, builder.getInt64(2));
  std::vector<llvm::Constant *> fieldValues;
  fieldValues.push_back(typeInfoVTable);
  fieldValues.push_back(typeInfoName);
  for (const SymbolType &interfaceType : interfaceTypes) {
    Interface *interface = interfaceType.getInterface(nullptr);
    assert(interface != nullptr && interface->typeInfo != nullptr);
    fieldValues.push_back(interface->typeInfo);
  }
  llvm::Constant *typeInfo = llvm::ConstantStruct::get(structType, fieldValues);

  // Generate global variable
  module->getOrInsertGlobal(mangledName, structType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  global->setInitializer(typeInfo);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(mangledName));
  global->setAlignment(llvm::MaybeAlign(8));

  return spiceStruct->typeInfo = global;
}

llvm::Constant *IRGenerator::generateVTable(StructBase *spiceStruct) {
  // Make sure that the RTTI runtime is present
  ensureRTTIRuntime();

  // Retrieve virtual method count
  const std::vector<Function *> virtualMethods = spiceStruct->scope->getVirtualMethods();
  const size_t virtualMethodCount = virtualMethods.size();
  const size_t arrayElementCount = virtualMethodCount + 2; // +2 for nullptr and TypeInfo

  // Generate type info data structures
  generateTypeInfo(spiceStruct);

  // Generate VTable type
  llvm::PointerType *ptrTy = llvm::PointerType::get(context, 0);
  llvm::ArrayType *vtableArrayTy = llvm::ArrayType::get(ptrTy, arrayElementCount);
  llvm::StructType *vtableTy = llvm::StructType::get(context, vtableArrayTy, false);

  const std::string mangledName = NameMangling::mangleVTable(spiceStruct);
  module->getOrInsertGlobal(mangledName, vtableTy);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
  global->setLinkage(llvm::GlobalValue::LinkOnceODRLinkage);
  global->setComdat(module->getOrInsertComdat(mangledName));
  global->setAlignment(llvm::MaybeAlign(8));

  return global;
}

void IRGenerator::generateVTableInitializer(StructBase *spiceStruct) {
  // Retrieve virtual method count
  assert(spiceStruct->scope);
  const std::vector<Function *> virtualMethods = spiceStruct->scope->getVirtualMethods();
  const size_t virtualMethodCount = virtualMethods.size();
  const size_t arrayElementCount = virtualMethodCount + 2; // +2 for nullptr and TypeInfo

  // Generate VTable type
  llvm::PointerType *ptrTy = llvm::PointerType::get(context, 0);
  llvm::ArrayType *vtableArrayTy = llvm::ArrayType::get(ptrTy, arrayElementCount);
  llvm::StructType *vtableTy = llvm::StructType::get(context, vtableArrayTy, false);

  // Generate VTable values
  std::vector<llvm::Constant *> arrayValues;
  arrayValues.push_back(llvm::Constant::getNullValue(ptrTy)); // nullptr as safety guard
  arrayValues.push_back(spiceStruct->typeInfo);               // TypeInfo to identify the type for the VTable
  for (Function *virtualMethod : virtualMethods) {
    assert(spiceStruct->scope->type == ScopeType::INTERFACE || virtualMethod->llvmFunction != nullptr);
    arrayValues.push_back(virtualMethod->llvmFunction ?: llvm::Constant::getNullValue(ptrTy));
  }

  // Generate VTable struct
  std::vector<llvm::Constant *> fieldValues;
  fieldValues.push_back(llvm::ConstantArray::get(vtableArrayTy, arrayValues));
  spiceStruct->vtable = llvm::ConstantStruct::get(vtableTy, fieldValues);

  const std::string mangledName = NameMangling::mangleVTable(spiceStruct);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  assert(global != nullptr);
  global->setInitializer(spiceStruct->vtable);
}

} // namespace spice::compiler
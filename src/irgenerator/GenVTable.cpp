// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <llvm/IR/Module.h>

#include <SourceFile.h>
#include <driver/Driver.h>
#include <irgenerator/NameMangling.h>

namespace spice::compiler {

llvm::Constant *IRGenerator::generateTypeInfoName(StructBase *spiceStruct) const {
  // Resolve mangled type info name and mangled global name
  const std::string globalName = NameMangling::mangleTypeInfoName(spiceStruct);

  // Generate global string constant
  llvm::GlobalVariable *global =
      builder.CreateGlobalString(NameMangling::mangleTypeInfoValue(spiceStruct->name), globalName, 0, module);

  // If the output should be comparable, fix alignment to 4 bytes
  if (cliOptions.comparableOutput)
    global->setAlignment(llvm::Align(4));

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::ExternalLinkage);
  // MachO does not support comdat annotations
  if (doesTargetSupportComDat())
    global->setComdat(module->getOrInsertComdat(globalName));

  return spiceStruct->vTableData.typeInfoName = global;
}

llvm::Constant *IRGenerator::generateTypeInfo(StructBase *spiceStruct) const {
  // Generate type info name
  llvm::Constant *typeInfoName = generateTypeInfoName(spiceStruct);

  // Generate LLVM type for type info
  const std::string mangledName = NameMangling::mangleTypeInfo(spiceStruct);
  llvm::PointerType *ptrTy = builder.getPtrTy();

  QualTypeList interfaceTypes;
  if (spiceStruct->entry->getQualType().is(TY_STRUCT)) {
    const auto spiceStructEnsured = reinterpret_cast<Struct *>(spiceStruct);
    interfaceTypes = spiceStructEnsured->interfaceTypes;
  }

  // Build type info LLVM type
  std::vector<llvm::Type *> typeInfoFieldTypes = {ptrTy, ptrTy};
  for (size_t i = 0; i < interfaceTypes.size(); i++)
    typeInfoFieldTypes.push_back(ptrTy);
  spiceStruct->vTableData.typeInfoType = llvm::StructType::get(context, typeInfoFieldTypes);

  // Generate type info values
  llvm::Constant *typeInfoVTable = llvm::Constant::getNullValue(ptrTy);
  if (!sourceFile->isRttiRT()) {
    // Add external global pointer for TypeInfo vtable
    assert(sourceFile->isRuntimeModuleAvailable(RuntimeModule::RTTI_RT));
    const std::string typeInfoMangledName = NameMangling::mangleVTable("TypeInfo");
    llvm::Constant *typeInfoExtPtr = module->getOrInsertGlobal(typeInfoMangledName, builder.getPtrTy());
    typeInfoVTable = llvm::ConstantExpr::getInBoundsGetElementPtr(ptrTy, typeInfoExtPtr, builder.getInt64(2));
  }
  std::vector<llvm::Constant *> fieldValues;
  fieldValues.push_back(typeInfoVTable);
  fieldValues.push_back(typeInfoName);
  for (const QualType &interfaceType : interfaceTypes) {
    const Interface *interface = interfaceType.getInterface(nullptr);
    assert(interface != nullptr && interface->vTableData.typeInfo != nullptr);
    const std::string interfaceMangledName = NameMangling::mangleTypeInfo(interface);
    llvm::Constant *global = module->getOrInsertGlobal(interfaceMangledName, builder.getPtrTy());
    fieldValues.push_back(global);
  }
  llvm::Constant *typeInfo = llvm::ConstantStruct::get(spiceStruct->vTableData.typeInfoType, fieldValues);

  // Generate global variable
  module->getOrInsertGlobal(mangledName, spiceStruct->vTableData.typeInfoType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  global->setInitializer(typeInfo);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(llvm::GlobalValue::ExternalLinkage);
  if (doesTargetSupportComDat())
    global->setComdat(module->getOrInsertComdat(mangledName));
  global->setAlignment(llvm::MaybeAlign(8));

  return spiceStruct->vTableData.typeInfo = global;
}

llvm::Constant *IRGenerator::generateVTable(StructBase *spiceStruct) const {
  // Retrieve virtual method count
  const std::vector<Function *> virtualMethods = spiceStruct->scope->getVirtualMethods();
  const size_t virtualMethodCount = virtualMethods.size();
  const size_t arrayElementCount = virtualMethodCount + 2; // +2 for nullptr and TypeInfo

  // Generate type info data structures
  generateTypeInfo(spiceStruct);

  // Generate VTable type
  llvm::ArrayType *vtableArrayTy = llvm::ArrayType::get(builder.getPtrTy(), arrayElementCount);
  spiceStruct->vTableData.vtableType = llvm::StructType::get(context, vtableArrayTy, false);

  const std::string mangledName = NameMangling::mangleVTable(spiceStruct);
  module->getOrInsertGlobal(mangledName, spiceStruct->vTableData.vtableType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);

  // Set global attributes
  global->setConstant(true);
  global->setDSOLocal(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
  global->setLinkage(llvm::GlobalValue::ExternalLinkage);
  if (doesTargetSupportComDat())
    global->setComdat(module->getOrInsertComdat(mangledName));
  global->setAlignment(llvm::MaybeAlign(8));

  return spiceStruct->vTableData.vtable = global;
}

void IRGenerator::generateVTableInitializer(const StructBase *spiceStruct) const {
  // Retrieve virtual method count
  assert(spiceStruct->scope);
  const std::vector<Function *> virtualMethods = spiceStruct->scope->getVirtualMethods();
  const size_t virtualMethodCount = virtualMethods.size();
  const size_t arrayElementCount = virtualMethodCount + 2; // +2 for nullptr and TypeInfo

  // Generate VTable type
  llvm::PointerType *ptrTy = builder.getPtrTy();
  llvm::ArrayType *vtableArrayTy = llvm::ArrayType::get(ptrTy, arrayElementCount);
  assert(spiceStruct->vTableData.vtableType);

  // Generate VTable values
  std::vector<llvm::Constant *> arrayValues;
  arrayValues.push_back(llvm::Constant::getNullValue(ptrTy)); // nullptr as safety guard
  arrayValues.push_back(spiceStruct->vTableData.typeInfo);    // TypeInfo to identify the type for the VTable
  for (Function *virtualMethod : virtualMethods) {
    assert(spiceStruct->scope->type == ScopeType::INTERFACE || virtualMethod->llvmFunction != nullptr);
    arrayValues.push_back(virtualMethod->llvmFunction ? virtualMethod->llvmFunction : llvm::Constant::getNullValue(ptrTy));
  }

  // Generate VTable struct
  std::vector<llvm::Constant *> fieldValues;
  fieldValues.push_back(llvm::ConstantArray::get(vtableArrayTy, arrayValues));
  llvm::Constant *initializer = llvm::ConstantStruct::get(spiceStruct->vTableData.vtableType, fieldValues);

  const std::string mangledName = NameMangling::mangleVTable(spiceStruct);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  assert(global != nullptr);
  global->setInitializer(initializer);
}

bool IRGenerator::doesTargetSupportComDat() const { return cliOptions.targetTriple.getObjectFormat() != llvm::Triple::MachO; }

} // namespace spice::compiler
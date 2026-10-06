// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <SourceFile.h>
#include <driver/Driver.h>
#include <irgenerator/NameMangling.h>

#include <llvm/IR/Module.h>

namespace spice::compiler {

llvm::Constant *IRGenerator::generateTypeInfoName(StructBase *spiceStruct) const {
  // Resolve mangled type info name and mangled global name
  const std::string globalName = NameMangling::mangleTypeInfoName(spiceStruct);

  // Generate global string constant
  const std::string mangledName = NameMangling::mangleTypeInfoValue(spiceStruct->name);
  llvm::GlobalVariable *global = builder.CreateGlobalString(mangledName, globalName, 0, module);

  // If the output should be comparable, fix alignment to 4 bytes
  if (cliOptions.comparableOutput)
    global->setAlignment(llvm::Align(4));

  const bool isPublic = spiceStruct->entry->getQualType().isPublic();

  // Set global attributes
  global->setConstant(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(getVTableLinkageType(isPublic));
  global->setDSOLocal(isSymbolDSOLocal(isPublic));
  attachComdatToSymbol(global, globalName, isPublic);

  return spiceStruct->vTableData.typeInfoName = global;
}

llvm::Constant *IRGenerator::generateTypeInfo(StructBase *spiceStruct) const {
  // Generate type info name
  llvm::Constant *typeInfoName = generateTypeInfoName(spiceStruct);

  // Generate LLVM type for type info
  const std::string mangledName = NameMangling::mangleTypeInfo(spiceStruct);
  llvm::PointerType *ptrTy = builder.getPtrTy();

  QualTypeList interfaceTypes;
  if (spiceStruct->entry->getQualType().is(TY_STRUCT))
    interfaceTypes = static_cast<Struct *>(spiceStruct)->interfaceTypes;

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
    assert(interface != nullptr);
    const std::string interfaceMangledName = NameMangling::mangleTypeInfo(interface);
    llvm::Constant *global = module->getOrInsertGlobal(interfaceMangledName, builder.getPtrTy());
    fieldValues.push_back(global);
  }
  llvm::Constant *typeInfo = llvm::ConstantStruct::get(spiceStruct->vTableData.typeInfoType, fieldValues);

  // Generate global variable
  module->getOrInsertGlobal(mangledName, spiceStruct->vTableData.typeInfoType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);
  global->setInitializer(typeInfo);

  const bool isPublic = spiceStruct->entry->getQualType().isPublic();

  // Set global attributes
  global->setConstant(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::None);
  global->setLinkage(getVTableLinkageType(isPublic));
  global->setDSOLocal(isSymbolDSOLocal(isPublic));
  global->setAlignment(llvm::MaybeAlign(8));
  attachComdatToSymbol(global, mangledName, isPublic);

  return spiceStruct->vTableData.typeInfo = global;
}

llvm::Constant *IRGenerator::generateVTable(StructBase *spiceStruct) const {
  // Generate type info data structures
  generateTypeInfo(spiceStruct);

  // Generate VTable type
  spiceStruct->vTableData.vtableType = getVTableType(spiceStruct);

  const std::string mangledName = NameMangling::mangleVTable(spiceStruct);
  module->getOrInsertGlobal(mangledName, spiceStruct->vTableData.vtableType);
  llvm::GlobalVariable *global = module->getNamedGlobal(mangledName);

  const bool isPublic = spiceStruct->entry->getQualType().isPublic();

  // Set global attributes
  global->setConstant(true);
  global->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
  global->setLinkage(getVTableLinkageType(isPublic));
  global->setDSOLocal(isSymbolDSOLocal(isPublic));
  global->setAlignment(llvm::MaybeAlign(8));
  attachComdatToSymbol(global, mangledName, isPublic);

  return spiceStruct->vTableData.vtable = global;
}

void IRGenerator::generateVTableInitializer(const StructBase *spiceStruct) {
  // Retrieve virtual method count
  assert(spiceStruct->scope);
  const std::vector<const Function *> virtualMethods = spiceStruct->scope->getVirtualMethods();
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
  for (const Function *virtualMethod : virtualMethods) {
    llvm::Function *llvmFunc = getLLVMFunction(virtualMethod);
    assert(spiceStruct->scope->type == ScopeType::INTERFACE || llvmFunc != nullptr);
    // Virtual calls expect the return type of the interface method, so methods with another return type need a thunk
    if (llvmFunc != nullptr && !virtualMethod->virtualReturnType.is(TY_DYN))
      llvmFunc = getOrCreateCovariantReturnThunk(virtualMethod, llvmFunc);
    arrayValues.push_back(llvmFunc ? llvmFunc : llvm::Constant::getNullValue(ptrTy));
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

llvm::Function *IRGenerator::getOrCreateCovariantReturnThunk(const Function *method, llvm::Function *target) {
  // A struct method may implement an interface method, that returns an interface, by returning a struct, which implements
  // this interface. Virtual calls expect the interface to be returned, which might be returned in another way than the
  // struct. Therefore, the VTable points to a thunk, that calls the method and returns the interface part of the struct.
  const std::string thunkName = target->getName().str() + ".covthunk";
  // The result is handed out as non-const pointer, which misc-const-correctness does not recognize as pointee mutation
  // NOLINTNEXTLINE(misc-const-correctness)
  if (llvm::Function *existing = module->getFunction(thunkName))
    return existing;

  // Build the thunk signature: the target's params with the return type of the interface method
  const QualType &returnType = method->returnType;
  const QualType &virtualReturnType = method->virtualReturnType;
  const unsigned int targetArgOffset = getArgOffset(returnType);
  const llvm::FunctionType *targetType = target->getFunctionType();
  const std::vector<llvm::Type *> paramTypes(targetType->param_begin() + targetArgOffset, targetType->param_end());
  llvm::FunctionType *thunkType = getFunctionType(virtualReturnType, paramTypes);
  llvm::Function *thunk = llvm::Function::Create(thunkType, llvm::Function::PrivateLinkage, thunkName, module);
  thunk->setDSOLocal(true);
  addCommonFctAttrs(thunk);
  if (const ReturnABIInfo returnABI = getReturnABIInfo(virtualReturnType); returnABI.isIndirect())
    addSRetParamAttrs(thunk, returnABI.memoryType);

  // Save insert markers, because the thunk body might be emitted in the middle of generating another function
  llvm::BasicBlock *bOrig = builder.GetInsertBlock();
  llvm::BasicBlock *allocaInsertBlockOrig = allocaInsertBlock;
  llvm::AllocaInst *allocaInsertInstOrig = allocaInsertInst;
  const bool blockAlreadyTerminatedOrig = blockAlreadyTerminated;
  const llvm::DebugLoc debugLocOrig = builder.getCurrentDebugLocation();
  builder.SetCurrentDebugLocation(llvm::DebugLoc());

  llvm::BasicBlock *bEntry = createBlock("entry");
  switchToBlock(bEntry, thunk);
  allocaInsertBlock = bEntry;
  allocaInsertInst = nullptr;

  // Forward all arguments except the sret pointer of the thunk
  const unsigned int thunkArgOffset = getArgOffset(virtualReturnType);
  std::vector<llvm::Value *> fwdArgs;
  fwdArgs.reserve(paramTypes.size());
  for (size_t i = 0; i < paramTypes.size(); i++)
    fwdArgs.push_back(thunk->getArg(thunkArgOffset + i));
  llvm::Value *resultAddr = nullptr;
  llvm::CallInst *call = insertCall(target, fwdArgs, returnType, resultAddr);
  if (resultAddr == nullptr) {
    resultAddr = insertAlloca(call->getType());
    insertStore(call, resultAddr);
  }

  // Return the interface part of the returned struct
  llvm::Value *interfacePtr = getUpcastedStructPtr(resultAddr, virtualReturnType, returnType);
  insertReturn(insertLoad(virtualReturnType.toLLVMType(sourceFile), interfacePtr));

  // Restore insert markers
  if (bOrig != nullptr)
    builder.SetInsertPoint(bOrig);
  builder.SetCurrentDebugLocation(debugLocOrig);
  blockAlreadyTerminated = blockAlreadyTerminatedOrig;
  allocaInsertBlock = allocaInsertBlockOrig;
  allocaInsertInst = allocaInsertInstOrig;

  return thunk;
}

llvm::StructType *IRGenerator::getVTableType(const StructBase *spiceStruct) const {
  // Retrieve virtual method count
  const size_t virtualMethodCount = spiceStruct->scope->getVirtualMethods().size();
  const size_t arrayElementCount = virtualMethodCount + 2; // +2 for nullptr and TypeInfo

  return llvm::StructType::get(context, llvm::ArrayType::get(builder.getPtrTy(), arrayElementCount), false);
}

llvm::Constant *IRGenerator::getVTableAddressPoint(const StructBase *spiceStruct) const {
  // Look up the VTable by name in the current module instead of using vTableData.vtable. The latter belongs to the module
  // of the source file that defines the struct, so for a struct from another source file a declaration is inserted.
  llvm::StructType *vtableType = getVTableType(spiceStruct);
  llvm::Constant *vtable = module->getOrInsertGlobal(NameMangling::mangleVTable(spiceStruct), vtableType);

  // The address point of the VTable is behind the nullptr and the TypeInfo
  return llvm::ConstantExpr::getInBoundsGetElementPtr(
      vtableType, vtable, llvm::ArrayRef<llvm::Constant *>({builder.getInt64(0), builder.getInt32(0), builder.getInt32(2)}));
}

} // namespace spice::compiler

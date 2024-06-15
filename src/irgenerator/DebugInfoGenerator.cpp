// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "DebugInfoGenerator.h"

#include <ast/ASTNodes.h>
#include <driver/Driver.h>
#include <irgenerator/IRGenerator.h>
#include <irgenerator/NameMangling.h>
#include <model/Function.h>
#include <model/Struct.h>
#include <util/CustomHashFunctions.h>
#include <util/FileUtil.h>

#include <llvm/BinaryFormat/Dwarf.h>

namespace spice::compiler {

void DebugInfoGenerator::initialize(const std::string &sourceFileName, std::filesystem::path sourceFileDir) {
  llvm::Module *module = irGenerator->module;
  llvm::LLVMContext &context = irGenerator->context;
  const std::string producerString = "spice version " + std::string(SPICE_VERSION) + " (https://github.com/spicelang/spice)";

  // Create DIBuilder
  diBuilder = std::make_unique<llvm::DIBuilder>(*module);

  // Create compilation unit
  std::filesystem::path absolutePath = std::filesystem::absolute(sourceFileDir / sourceFileName);
  absolutePath.make_preferred();
  sourceFileDir.make_preferred();
  llvm::DIFile *cuDiFile = diBuilder->createFile(absolutePath.string(), sourceFileDir.string());
  compileUnit = diBuilder->createCompileUnit(
      llvm::dwarf::DW_LANG_C_plus_plus_14, cuDiFile, producerString, irGenerator->cliOptions.optLevel > OptLevel::O0, "", 0, "",
      llvm::DICompileUnit::FullDebug, 0, false, false, llvm::DICompileUnit::DebugNameTableKind::None);

  module->addModuleFlag(llvm::Module::Max, "Dwarf Version", llvm::dwarf::DWARF_VERSION);
  module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
  module->addModuleFlag(llvm::Module::Error, "wchar_size", 4);
  module->addModuleFlag(llvm::Module::Min, "PIC Level", llvm::PICLevel::BigPIC);
  module->addModuleFlag(llvm::Module::Max, "PIE Level", llvm::PIELevel::Large);
  module->addModuleFlag(llvm::Module::Max, "uwtable", 2);
  module->addModuleFlag(llvm::Module::Max, "frame-pointer", 2);

  llvm::NamedMDNode *identifierMetadata = module->getOrInsertNamedMetadata("llvm.ident");
  identifierMetadata->addOperand(llvm::MDNode::get(context, llvm::MDString::get(context, producerString)));

  // Create another DIFile as scope for subprograms
  diFile = diBuilder->createFile(sourceFileName, sourceFileDir.string());

  pointerWidth = irGenerator->module->getDataLayout().getPointerSizeInBits();

  // Initialize primitive debug types
  doubleTy = diBuilder->createBasicType("double", 64, llvm::dwarf::DW_ATE_float);
  intTy = diBuilder->createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);
  uIntTy = diBuilder->createBasicType("unsigned int", 32, llvm::dwarf::DW_ATE_unsigned);
  shortTy = diBuilder->createBasicType("short", 16, llvm::dwarf::DW_ATE_signed);
  uShortTy = diBuilder->createBasicType("unsigned short", 16, llvm::dwarf::DW_ATE_unsigned);
  longTy = diBuilder->createBasicType("long", 64, llvm::dwarf::DW_ATE_signed);
  uLongTy = diBuilder->createBasicType("unsigned long", 64, llvm::dwarf::DW_ATE_unsigned);
  byteTy = diBuilder->createBasicType("byte", 8, llvm::dwarf::DW_ATE_unsigned);
  charTy = diBuilder->createBasicType("char", 8, llvm::dwarf::DW_ATE_unsigned_char);
  stringTy = diBuilder->createPointerType(charTy, pointerWidth);
  boolTy = diBuilder->createBasicType("bool", 8, llvm::dwarf::DW_ATE_boolean);
  voidTy = diBuilder->createBasicType("void", 0, llvm::dwarf::DW_ATE_unsigned);

  // Initialize fat ptr type
  llvm::PointerType *ptrTy = irGenerator->builder.getPtrTy();
  if (!irGenerator->llvmTypes.fatPtrType)
    irGenerator->llvmTypes.fatPtrType = llvm::StructType::get(context, {ptrTy, ptrTy});

  const llvm::DataLayout &dataLayout = module->getDataLayout();
  const llvm::StructLayout *structLayout = dataLayout.getStructLayout(irGenerator->llvmTypes.fatPtrType);
  const uint32_t alignInBits = dataLayout.getABITypeAlign(irGenerator->llvmTypes.fatPtrType).value();
  const uint32_t ptrAlignInBits = dataLayout.getABITypeAlign(ptrTy).value();

  fatPtrTy = diBuilder->createStructType(diFile, "_fat_ptr", diFile, 0, structLayout->getSizeInBits(), alignInBits,
                                         llvm::DINode::FlagTypePassByValue | llvm::DINode::FlagNonTrivial, nullptr, {}, 0,
                                         nullptr, "_fat_ptr");

  llvm::DIType *voidPtrDIType = diBuilder->createPointerType(voidTy, pointerWidth, ptrAlignInBits);
  llvm::DIDerivedType *firstType = diBuilder->createMemberType(fatPtrTy, "first", diFile, 0, pointerWidth, ptrAlignInBits, 0,
                                                               llvm::DINode::FlagZero, voidPtrDIType);
  llvm::DIDerivedType *secondType = diBuilder->createMemberType(fatPtrTy, "second", diFile, 0, pointerWidth, ptrAlignInBits,
                                                                pointerWidth, llvm::DINode::FlagZero, voidPtrDIType);
  fatPtrTy->replaceElements(llvm::MDTuple::get(context, {firstType, secondType}));
}

void DebugInfoGenerator::generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc, bool isLambda) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  const ASTNode *node = spiceFunc->declNode;
  const uint32_t lineNo = spiceFunc->getDeclCodeLoc().line;

  // Prepare flags
  llvm::DIScope *scope = diFile;
  llvm::DINode::DIFlags flags = llvm::DINode::FlagPrototyped;
  if (spiceFunc->entry && spiceFunc->entry->getQualType().isPublic())
    flags |= llvm::DINode::FlagPublic;

  // Prepare spFlags
  llvm::DISubprogram::DISPFlags spFlags = llvm::DISubprogram::SPFlagDefinition;
  if (isLambda)
    spFlags |= llvm::DISubprogram::SPFlagLocalToUnit;
  if (spiceFunc->isVirtual) {
    if (spiceFunc->thisType.is(TY_INTERFACE))
      spFlags |= llvm::DISubprogram::SPFlagPureVirtual;
    else
      spFlags |= llvm::DISubprogram::SPFlagVirtual;
  }

  // Collect arguments
  std::vector<llvm::Metadata *> argTypes;
  if (spiceFunc->isProcedure())
    argTypes.push_back(voidTy);
  else
    argTypes.push_back(getDITypeForQualType(node, spiceFunc->returnType)); // Add result type
  if (spiceFunc->isMethod())
    argTypes.push_back(getDITypeForQualType(node, spiceFunc->thisType)); // Add this type
  if (isLambda) {
    llvm::DICompositeType *captureStructType = generateCaptureStructDebugInfo(spiceFunc);
    scope = captureStructType;
    llvm::DIType *captureStructPtr = diBuilder->createPointerType(captureStructType, pointerWidth);
    argTypes.push_back(captureStructPtr); // Add this type
  }
  for (const QualType &argType : spiceFunc->getParamTypes()) // Add arg types
    argTypes.push_back(getDITypeForQualType(node, argType));

  // Create function type
  llvm::DISubroutineType *functionTy = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(argTypes));

  const std::string mangledName = spiceFunc->getMangledName();
  llvm::DISubprogram *subprogram;
  const std::string &name = spiceFunc->name;
  if (spiceFunc->isMethod()) {
    subprogram = diBuilder->createMethod(scope, name, mangledName, diFile, lineNo, functionTy, 0, 0, nullptr, flags, spFlags);
  } else {
    subprogram = diBuilder->createFunction(scope, name, mangledName, diFile, lineNo, functionTy, lineNo, flags, spFlags);
  }
  subprogram->replaceRetainedNodes({});

  // Add debug info to LLVM function
  llvmFunction->setSubprogram(subprogram);
  // Add scope to lexicalBlocks
  lexicalBlocks.push(subprogram);
}

void DebugInfoGenerator::concludeFunctionDebugInfo() {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  assert(!lexicalBlocks.empty());
  lexicalBlocks.pop();
  assert(lexicalBlocks.empty());
}

void DebugInfoGenerator::pushLexicalBlock(const ASTNode *node) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  const uint32_t line = node->codeLoc.line;
  const uint32_t col = node->codeLoc.col;
  llvm::DILexicalBlock *lexicalBlock = diBuilder->createLexicalBlock(lexicalBlocks.top(), diFile, line, col);
  lexicalBlocks.push(lexicalBlock);
}

void DebugInfoGenerator::popLexicalBlock() {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  assert(!lexicalBlocks.empty());
  lexicalBlocks.pop();
}

llvm::DICompositeType *DebugInfoGenerator::generateCaptureStructDebugInfo(const Function *spiceFunc) {
  const CaptureMap &captures = spiceFunc->bodyScope->symbolTable.captures;
  const ASTNode *node = spiceFunc->declNode;
  const uint32_t lineNo = node->codeLoc.line;

  // Get LLVM type for struct
  std::vector<llvm::Type *> fieldTypes;
  std::vector<SymbolTableEntry *> fieldEntries;
  QualTypeList fieldSymbolTypes;
  for (const auto &[_, capture] : captures) {
    QualType captureType = capture.capturedSymbol->getQualType();

    // Capture by reference
    if (capture.getMode() == BY_REFERENCE)
      captureType = captureType.toRef(node);

    fieldEntries.push_back(capture.capturedSymbol);
    fieldSymbolTypes.push_back(captureType);
    fieldTypes.push_back(captureType.toLLVMType(irGenerator->sourceFile));
  }
  llvm::StructType *structType = llvm::StructType::get(irGenerator->context, fieldTypes);
  const llvm::StructLayout *structLayout = irGenerator->module->getDataLayout().getStructLayout(structType);
  const size_t alignInBits = irGenerator->module->getDataLayout().getABITypeAlign(structType).value();

  llvm::DIScope *scope = lexicalBlocks.top();
  llvm::DICompositeType *structDiType =
      diBuilder->createClassType(scope, "", diFile, lineNo, structLayout->getSizeInBits(), alignInBits, 0,
                                 llvm::DINode::FlagTypePassByValue | llvm::DINode::FlagNonTrivial, nullptr, {});

  std::vector<llvm::Metadata *> fieldDITypes;
  for (size_t i = 0; i < fieldEntries.size(); i++) {
    llvm::DIType *fieldDiType = getDITypeForQualType(node, fieldSymbolTypes.at(i));
    const std::string &fieldName = fieldEntries.at(i)->name;
    const size_t offsetInBits = structLayout->getElementOffsetInBits(i);
    const size_t fieldSize = fieldDiType->getSizeInBits();
    const size_t fieldAlign = fieldDiType->getAlignInBits();
    llvm::DIDerivedType *fieldDiDerivedType = diBuilder->createMemberType(
        structDiType, fieldName, diFile, lineNo, fieldSize, fieldAlign, offsetInBits, llvm::DINode::FlagZero, fieldDiType);
    fieldDITypes.push_back(fieldDiDerivedType);
  }
  structDiType->replaceElements(llvm::MDTuple::get(irGenerator->context, fieldDITypes));

  return structDiType;
}

void DebugInfoGenerator::generateGlobalVarDebugInfo(llvm::GlobalVariable *global, const SymbolTableEntry *globalEntry) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  const uint32_t lineNo = globalEntry->getDeclCodeLoc().line;
  llvm::StringRef name = global->getName();
  llvm::DIType *type = getDITypeForQualType(globalEntry->declNode, globalEntry->getQualType());
  const bool isLocal = globalEntry->getQualType().isPublic();

  global->addDebugInfo(diBuilder->createGlobalVariableExpression(compileUnit, name, name, diFile, lineNo, type, isLocal));
}

void DebugInfoGenerator::generateGlobalStringDebugInfo(llvm::GlobalVariable *global, const std::string &name, size_t length,
                                                       const CodeLoc &codeLoc) {
  const uint32_t lineNo = codeLoc.line;
  const size_t sizeInBits = (length + 1) * 8; // +1 because of null-terminator

  llvm::DIStringType *stringType = diBuilder->createStringType(name, sizeInBits);
  global->addDebugInfo(diBuilder->createGlobalVariableExpression(compileUnit, name, name, diFile, lineNo, stringType, true));
}

void DebugInfoGenerator::generateLocalVarDebugInfo(const std::string &varName, llvm::Value *address, const size_t argNumber,
                                                   bool moveToPrev) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  // Get symbol table entry
  SymbolTableEntry *variableEntry = irGenerator->currentScope->lookupStrict(varName);
  assert(variableEntry != nullptr);
  // Build debug info
  llvm::DIScope *scope = lexicalBlocks.top();
  llvm::DIType *diType = getDITypeForQualType(variableEntry->declNode, variableEntry->getQualType());
  const uint32_t lineNo = variableEntry->declNode->codeLoc.line;

  llvm::DILocalVariable *varInfo;
  if (argNumber != SIZE_MAX)
    varInfo = diBuilder->createParameterVariable(scope, varName, argNumber, diFile, lineNo, diType);
  else
    varInfo = diBuilder->createAutoVariable(scope, varName, diFile, lineNo, diType);
  llvm::DIExpression *expr = diBuilder->createExpression();
  llvm::DILocation *debugLocation = irGenerator->builder.getCurrentDebugLocation();
  assert(debugLocation != nullptr);
  llvm::Instruction *inst = diBuilder->insertDeclare(address, varInfo, expr, debugLocation, irGenerator->allocaInsertBlock);
  if (moveToPrev)
    inst->moveBefore(irGenerator->builder.GetInsertPoint()->getPrevNonDebugInstruction());
}

void DebugInfoGenerator::setSourceLocation(const ASTNode *node) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  assert(!lexicalBlocks.empty());
  llvm::DIScope *scope = lexicalBlocks.top();
  llvm::DILocation *codeLoc = llvm::DILocation::get(scope->getContext(), node->codeLoc.line, node->codeLoc.col, scope);
  irGenerator->builder.SetCurrentDebugLocation(codeLoc);
}

void DebugInfoGenerator::finalize() {
  if (irGenerator->cliOptions.generateDebugInfo)
    diBuilder->finalize();
}

llvm::DIType *DebugInfoGenerator::getDITypeForQualType(const ASTNode *node, const QualType &ty) { // NOLINT(*-no-recursion)
  // Pointer type
  if (ty.isPtr()) {
    llvm::DIType *pointeeTy = getDITypeForQualType(node, ty.getContained());
    return diBuilder->createPointerType(pointeeTy, pointerWidth);
  }

  // Reference type
  if (ty.isRef()) {
    llvm::DIType *referencedType = getDITypeForQualType(node, ty.getContained());
    return diBuilder->createReferenceType(llvm::dwarf::DW_TAG_reference_type, referencedType, pointerWidth);
  }

  // Array type
  if (ty.isArray()) {
    llvm::DIType *itemTy = getDITypeForQualType(node, ty.getContained());
    const size_t size = ty.getArraySize();
    llvm::DINodeArray subscripts = diBuilder->getOrCreateArray({});
    return diBuilder->createArrayType(size, 0, itemTy, subscripts);
  }

  // Primitive types
  llvm::DIType *baseDiType;
  switch (ty.getSuperType()) {
  case TY_DOUBLE:
    baseDiType = doubleTy;
    break;
  case TY_INT:
    baseDiType = ty.isSigned() ? intTy : uIntTy;
    break;
  case TY_SHORT:
    baseDiType = ty.isSigned() ? shortTy : uShortTy;
    break;
  case TY_LONG:
    baseDiType = ty.isSigned() ? longTy : uLongTy;
    break;
  case TY_BYTE:
    baseDiType = byteTy;
    break;
  case TY_CHAR:
    baseDiType = charTy;
    break;
  case TY_STRING:
    baseDiType = stringTy;
    break;
  case TY_BOOL:
    baseDiType = boolTy;
    break;
  case TY_STRUCT: {
    // Do cache lookup
    const size_t hashKey = std::hash<QualType>{}(ty);
    if (structTypeCache.contains(hashKey))
      return structTypeCache.at(hashKey);

    // Cache miss, generate struct type
    Struct *spiceStruct = ty.getStruct(node);
    assert(spiceStruct != nullptr);

    // Retrieve information about the struct
    const uint32_t lineNo = spiceStruct->getDeclCodeLoc().line;
    llvm::Type *structType = spiceStruct->entry->getQualType().toLLVMType(irGenerator->sourceFile);
    assert(structType != nullptr);
    const llvm::DataLayout &dataLayout = irGenerator->module->getDataLayout();
    const llvm::StructLayout *structLayout = dataLayout.getStructLayout(reinterpret_cast<llvm::StructType *>(structType));
    const uint32_t alignInBits = dataLayout.getABITypeAlign(structType).value();

    // Create struct ty
    const std::string mangledName = NameMangling::mangleStruct(*spiceStruct);
    llvm::DICompositeType *structDiType = diBuilder->createStructType(
        diFile, spiceStruct->name, diFile, lineNo, structLayout->getSizeInBits(), alignInBits,
        llvm::DINode::FlagTypePassByReference | llvm::DINode::FlagNonTrivial, nullptr, {}, 0, nullptr, mangledName);
    baseDiType = structDiType;

    // Insert into cache
    structTypeCache.insert({hashKey, structDiType});

    // Collect DI types for fields
    std::vector<llvm::Metadata *> fieldTypes;
    for (size_t i = 0; i < spiceStruct->scope->getFieldCount(); i++) {
      // Get field entry
      SymbolTableEntry *fieldEntry = spiceStruct->scope->symbolTable.lookupStrictByIndex(i);
      assert(fieldEntry != nullptr && fieldEntry->isField());
      if (fieldEntry->isImplicitField)
        continue;

      const QualType &fieldType = fieldEntry->getQualType();
      const uint32_t fieldLineNo = fieldEntry->declNode->codeLoc.line;
      const size_t offsetInBits = structLayout->getElementOffsetInBits(i);

      llvm::DIType *fieldDiType = getDITypeForQualType(node, fieldType);
      llvm::DIDerivedType *fieldDiDerivedType =
          diBuilder->createMemberType(structDiType, fieldEntry->name, diFile, fieldLineNo, fieldDiType->getSizeInBits(),
                                      fieldDiType->getAlignInBits(), offsetInBits, llvm::DINode::FlagZero, fieldDiType);

      fieldTypes.push_back(fieldDiDerivedType);
    }

    structDiType->replaceElements(llvm::MDTuple::get(irGenerator->context, fieldTypes));
    break;
  }
  case TY_INTERFACE: {
    // Do cache lookup
    const size_t hashKey = std::hash<QualType>{}(ty);
    if (structTypeCache.contains(hashKey))
      return structTypeCache.at(hashKey);

    // Cache miss, generate interface type
    Interface *spiceInterface = ty.getInterface(node);
    assert(spiceInterface != nullptr);

    // Retrieve information about the interface
    const uint32_t lineNo = spiceInterface->getDeclCodeLoc().line;
    llvm::Type *interfaceType = spiceInterface->entry->getQualType().toLLVMType(irGenerator->sourceFile);
    assert(interfaceType != nullptr);
    llvm::DataLayout dataLayout = irGenerator->module->getDataLayout();
    const llvm::StructLayout *structLayout = dataLayout.getStructLayout(reinterpret_cast<llvm::StructType *>(interfaceType));
    const uint32_t alignInBits = dataLayout.getABITypeAlign(interfaceType).value();

    // Create interface ty
    const std::string mangledName = NameMangling::mangleInterface(*spiceInterface);
    llvm::DICompositeType *interfaceDiType = diBuilder->createStructType(
        diFile, spiceInterface->name, diFile, lineNo, structLayout->getSizeInBits(), alignInBits,
        llvm::DINode::FlagTypePassByReference | llvm::DINode::FlagNonTrivial, nullptr, {}, 0, nullptr, mangledName);

    // Set vtable holder to itself for interfaces
    interfaceDiType->replaceVTableHolder(interfaceDiType);
    baseDiType = interfaceDiType;

    // Insert into cache
    structTypeCache.insert({hashKey, interfaceDiType});

    break;
  }
  case TY_FUNCTION: // fall-through
  case TY_PROCEDURE:
    baseDiType = fatPtrTy;
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Debug Info Type fallthrough"); // GCOV_EXCL_LINE
  }

  if (ty.isConst())
    baseDiType = diBuilder->createQualifiedType(llvm::dwarf::DW_TAG_const_type, baseDiType);

  return baseDiType;
}

} // namespace spice::compiler
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "DebugInfoGenerator.h"

#include <ast/ASTNodes.h>
#include <irgenerator/IRGenerator.h>
#include <irgenerator/NameMangling.h>
#include <model/Function.h>
#include <model/Struct.h>
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
      llvm::dwarf::DW_LANG_C17, cuDiFile, producerString, irGenerator->cliOptions.optLevel > OptLevel::O0, "", 0, "",
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
  llvm::Type *ptrTy = llvm::PointerType::get(irGenerator->context, 0);
  if (!irGenerator->llvmTypes.fatPtrType)
    irGenerator->llvmTypes.fatPtrType = llvm::StructType::get(irGenerator->context, {ptrTy, ptrTy});

  const llvm::StructLayout *structLayout =
      irGenerator->module->getDataLayout().getStructLayout(irGenerator->llvmTypes.fatPtrType);
  const uint32_t alignInBits = irGenerator->module->getDataLayout().getABITypeAlign(irGenerator->llvmTypes.fatPtrType).value();
  const uint32_t ptrAlignInBits = irGenerator->module->getDataLayout().getABITypeAlign(ptrTy).value();

  fatPtrTy = diBuilder->createStructType(diFile, "_fat_ptr", diFile, 0, structLayout->getSizeInBits(), alignInBits,
                                         llvm::DINode::FlagTypePassByValue | llvm::DINode::FlagNonTrivial, nullptr, {}, 0,
                                         nullptr, "_fat_ptr");

  llvm::DIType *voidPtrDIType = diBuilder->createPointerType(voidTy, pointerWidth, ptrAlignInBits);
  llvm::DIDerivedType *firstType = diBuilder->createMemberType(fatPtrTy, "first", diFile, 0, pointerWidth, ptrAlignInBits, 0,
                                                               llvm::DINode::FlagZero, voidPtrDIType);
  llvm::DIDerivedType *secondType = diBuilder->createMemberType(fatPtrTy, "second", diFile, 0, pointerWidth, ptrAlignInBits,
                                                                pointerWidth, llvm::DINode::FlagZero, voidPtrDIType);
  fatPtrTy->replaceElements(llvm::MDTuple::get(irGenerator->context, {firstType, secondType}));
}

void DebugInfoGenerator::generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc, bool isLambda) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  const ASTNode *node = spiceFunc->declNode;
  const size_t lineNo = spiceFunc->getDeclCodeLoc().line;

  // Prepare information
  llvm::DIScope *scope = diFile;
  llvm::DINode::DIFlags flags = llvm::DINode::FlagPrototyped;
  llvm::DISubprogram::DISPFlags spFlags = llvm::DISubprogram::SPFlagDefinition;
  if (isLambda)
    spFlags |= llvm::DISubprogram::SPFlagLocalToUnit;

  // Collect arguments
  std::vector<llvm::Metadata *> argTypes;
  if (spiceFunc->isProcedure())
    argTypes.push_back(voidTy);
  else
    argTypes.push_back(getDITypeForSymbolType(node, spiceFunc->returnType)); // Add result type
  if (spiceFunc->isMethod())
    argTypes.push_back(getDITypeForSymbolType(node, spiceFunc->thisType)); // Add this type
  if (isLambda) {
    llvm::DICompositeType *captureStructType = generateCaptureStructDebugInfo(spiceFunc);
    scope = captureStructType;
    llvm::DIType *captureStructPtr = diBuilder->createPointerType(captureStructType, pointerWidth);
    argTypes.push_back(captureStructPtr); // Add this type
  }
  for (const SymbolType &argType : spiceFunc->getParamTypes()) // Add arg types
    argTypes.push_back(getDITypeForSymbolType(node, argType));

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
}

void DebugInfoGenerator::pushLexicalBlock(const ASTNode *node) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  const size_t line = node->codeLoc.line;
  const size_t col = node->codeLoc.col;
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
  const std::unordered_map<std::string, Capture> &captures = spiceFunc->bodyScope->symbolTable.captures;
  const CaptureMode captureMode = spiceFunc->bodyScope->symbolTable.capturingMode;
  const ASTNode *node = spiceFunc->declNode;
  const size_t lineNo = node->codeLoc.line;

  // Get LLVM type for struct
  std::vector<llvm::Type *> fieldTypes;
  std::vector<SymbolTableEntry *> fieldEntries;
  std::vector<SymbolType> fieldSymbolTypes;
  for (const auto &[_, capture] : captures) {
    SymbolType captureType = capture.capturedEntry->getType();

    // Capture by reference
    if (captureMode == BY_REFERENCE)
      captureType = captureType.toReference(node);

    fieldEntries.push_back(capture.capturedEntry);
    fieldSymbolTypes.push_back(captureType);
    fieldTypes.push_back(captureType.toLLVMType(irGenerator->context, irGenerator->currentScope));
  }
  llvm::StructType *structType = llvm::StructType::get(irGenerator->context, fieldTypes, CAPTURES_PARAM_NAME);
  const llvm::StructLayout *structLayout = irGenerator->module->getDataLayout().getStructLayout(structType);
  const size_t alignInBits = irGenerator->module->getDataLayout().getABITypeAlign(structType).value();

  llvm::DIScope *scope = lexicalBlocks.top();
  llvm::DICompositeType *structDiType =
      diBuilder->createClassType(scope, "", diFile, lineNo, structLayout->getSizeInBits(), alignInBits, 0,
                                 llvm::DINode::FlagTypePassByValue | llvm::DINode::FlagNonTrivial, nullptr, {});

  std::vector<llvm::Metadata *> fieldDITypes;
  for (size_t i = 0; i < fieldEntries.size(); i++) {
    llvm::DIType *fieldDiType = getDITypeForSymbolType(node, fieldSymbolTypes.at(i));
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

  const size_t lineNo = globalEntry->getDeclCodeLoc().line;
  llvm::StringRef name = global->getName();
  llvm::DIType *type = getDITypeForSymbolType(globalEntry->declNode, globalEntry->getType());
  const bool isLocal = globalEntry->getType().isPublic();

  global->addDebugInfo(diBuilder->createGlobalVariableExpression(compileUnit, name, name, diFile, lineNo, type, isLocal));
}

void DebugInfoGenerator::generateGlobalStringDebugInfo(llvm::GlobalVariable *global, const std::string &name, size_t length,
                                                       const CodeLoc &codeLoc) {
  const size_t lineNo = codeLoc.line;
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
  llvm::DIType *diType = getDITypeForSymbolType(variableEntry->declNode, variableEntry->getType());
  const size_t lineNo = variableEntry->declNode->codeLoc.line;

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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
llvm::DIType *DebugInfoGenerator::getDITypeForSymbolType(const ASTNode *node, const SymbolType &symbolType) const {
  // Pointer or reference type
  if (symbolType.isPtr() || symbolType.isRef()) {
    llvm::DIType *pointeeTy = getDITypeForSymbolType(node, symbolType.getContainedTy());
    return diBuilder->createPointerType(pointeeTy, pointerWidth);
  }

  // Array type
  if (symbolType.isArray()) {
    llvm::DIType *itemTy = getDITypeForSymbolType(node, symbolType.getContainedTy());
    const size_t size = symbolType.getArraySize();
    llvm::DINodeArray subscripts = diBuilder->getOrCreateArray({});
    return diBuilder->createArrayType(size, 0, itemTy, subscripts);
  }

  // Primitive types
  llvm::DIType *baseDiType;
  switch (symbolType.getSuperType()) {
  case TY_DOUBLE:
    baseDiType = doubleTy;
    break;
  case TY_INT:
    baseDiType = symbolType.isSigned() ? intTy : uIntTy;
    break;
  case TY_SHORT:
    baseDiType = symbolType.isSigned() ? shortTy : uShortTy;
    break;
  case TY_LONG:
    baseDiType = symbolType.isSigned() ? longTy : uLongTy;
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
    Struct *spiceStruct = symbolType.getStruct(node);
    assert(spiceStruct != nullptr);

    // Check if we already know the DI type
    if (spiceStruct->diType != nullptr) {
      baseDiType = spiceStruct->diType;
      break;
    }

    // Retrieve information about the struct
    const size_t lineNo = spiceStruct->getDeclCodeLoc().line;
    llvm::Type *structType = spiceStruct->entry->getType().toLLVMType(irGenerator->context, irGenerator->currentScope);
    assert(structType != nullptr);
    const llvm::StructLayout *structLayout =
        irGenerator->module->getDataLayout().getStructLayout(reinterpret_cast<llvm::StructType *>(structType));
    const uint32_t alignInBits = irGenerator->module->getDataLayout().getABITypeAlign(structType).value();

    // Create struct type
    const std::string mangledName = NameMangling::mangleStruct(*spiceStruct);
    llvm::DICompositeType *structDiType = diBuilder->createStructType(
        diFile, spiceStruct->name, diFile, lineNo, structLayout->getSizeInBits(), alignInBits,
        llvm::DINode::FlagTypePassByValue | llvm::DINode::FlagNonTrivial, nullptr, {}, 0, nullptr, mangledName);
    baseDiType = spiceStruct->diType = structDiType;

    // Collect DI types for fields
    std::vector<llvm::Metadata *> fieldTypes;
    for (size_t i = 0; i < spiceStruct->scope->getFieldCount(); i++) {
      // Get field entry
      SymbolTableEntry *fieldEntry = spiceStruct->scope->symbolTable.lookupStrictByIndex(i);
      assert(fieldEntry != nullptr && fieldEntry->isField());
      if (fieldEntry->isImplicitField)
        continue;

      const SymbolType fieldType = fieldEntry->getType();
      const size_t fieldLineNo = fieldEntry->declNode->codeLoc.line;
      const size_t offsetInBits = structLayout->getElementOffsetInBits(i);

      llvm::DIType *fieldDiType = getDITypeForSymbolType(node, fieldType);
      llvm::DIDerivedType *fieldDiDerivedType =
          diBuilder->createMemberType(structDiType, fieldEntry->name, diFile, fieldLineNo, fieldDiType->getSizeInBits(),
                                      fieldDiType->getAlignInBits(), offsetInBits, llvm::DINode::FlagZero, fieldDiType);

      fieldTypes.push_back(fieldDiDerivedType);
    }

    structDiType->replaceElements(llvm::MDTuple::get(irGenerator->context, fieldTypes));
    break;
  }
  case TY_INTERFACE: {
    Interface *spiceInterface = symbolType.getInterface(node);
    assert(spiceInterface != nullptr);

    // Check if we already know the DI type
    if (spiceInterface->diType != nullptr) {
      baseDiType = spiceInterface->diType;
      break;
    }

    // Retrieve information about the interface
    const size_t lineNo = spiceInterface->getDeclCodeLoc().line;
    llvm::Type *interfaceType = spiceInterface->entry->getType().toLLVMType(irGenerator->context, irGenerator->currentScope);
    assert(interfaceType != nullptr);
    const llvm::StructLayout *structLayout =
        irGenerator->module->getDataLayout().getStructLayout(static_cast<llvm::StructType *>(interfaceType));
    const uint32_t alignInBits = irGenerator->module->getDataLayout().getABITypeAlign(interfaceType).value();

    // Create interface type
    const std::string mangledName = NameMangling::mangleInterface(*spiceInterface);
    llvm::DICompositeType *interfaceDiType = diBuilder->createStructType(
        diFile, spiceInterface->name, diFile, lineNo, structLayout->getSizeInBits(), alignInBits,
        llvm::DINode::FlagTypePassByValue | llvm::DINode::FlagNonTrivial, nullptr, {}, 0, nullptr, mangledName);
    baseDiType = spiceInterface->diType = interfaceDiType;
    break;
  }
  case TY_FUNCTION: // fall-through
  case TY_PROCEDURE:
    baseDiType = fatPtrTy;
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Debug Info Type fallthrough"); // GCOV_EXCL_LINE
  }

  if (symbolType.isConst())
    baseDiType = diBuilder->createQualifiedType(llvm::dwarf::DW_TAG_const_type, baseDiType);

  return baseDiType;
}
#pragma clang diagnostic pop

} // namespace spice::compiler
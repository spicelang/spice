// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "DebugInfoGenerator.h"

#include <ast/ASTNodes.h>
#include <irgenerator/IRGenerator.h>
#include <model/Function.h>

#include <llvm/BinaryFormat/Dwarf.h>

namespace spice::compiler {

void DebugInfoGenerator::initialize(const std::string &sourceFileName, const std::string &sourceFileDir) {
  llvm::Module *module = irGenerator->module;
  llvm::LLVMContext &context = irGenerator->context;
  const std::string producerString = "spice version " + std::string(SPICE_VERSION);

  module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
  module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", llvm::dwarf::DWARF_VERSION);
  module->addModuleFlag(llvm::Module::Error, "PIC Level", llvm::PICLevel::BigPIC);

  llvm::NamedMDNode *identifierMetadata = module->getOrInsertNamedMetadata("llvm.ident");
  identifierMetadata->addOperand(llvm::MDNode::get(context, llvm::MDString::get(context, producerString)));

  // Create DIBuilder
  diBuilder = std::make_unique<llvm::DIBuilder>(*module);

  // Create compilation unit
  diFile = diBuilder->createFile(sourceFileName, sourceFileDir);
  compileUnit = diBuilder->createCompileUnit(llvm::dwarf::DW_LANG_C11, diFile, producerString,
                                             irGenerator->cliOptions.optLevel > 0, "", 0, "", llvm::DICompileUnit::FullDebug, 0,
                                             false, false, llvm::DICompileUnit::DebugNameTableKind::None);

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
  stringTy = diBuilder->createBasicType("string", 8, llvm::dwarf::DW_ATE_ASCII);
  boolTy = diBuilder->createBasicType("bool", 1, llvm::dwarf::DW_ATE_boolean);
}

void DebugInfoGenerator::generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;
  // Create function type
  std::vector<llvm::Metadata *> argTypes;
  argTypes.push_back(getDITypeForSymbolType(spiceFunc->returnType)); // Add result type
  for (const SymbolType &argType : spiceFunc->getParamTypes())       // Add arg types
    argTypes.push_back(getDITypeForSymbolType(argType));
  llvm::DISubroutineType *functionTy = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(argTypes));

  llvm::DIFile *unit = diBuilder->createFile(compileUnit->getFilename(), compileUnit->getDirectory());
  const size_t lineNumber = spiceFunc->getDeclCodeLoc().line;

  llvm::DISubprogram *subprogram =
      diBuilder->createFunction(unit, spiceFunc->name, spiceFunc->getMangledName(), unit, lineNumber, functionTy, lineNumber,
                                llvm::DINode::FlagZero, llvm::DISubprogram::SPFlagDefinition);

  // Add debug info to LLVM function
  llvmFunction->setSubprogram(subprogram);
  // Add scope to lexicalBlocks
  lexicalBlocks.push(subprogram);
}

/*llvm::DIType *IRGenerator::generateStructDebugInfo(llvm::StructType *llvmStructTy, const Struct *spiceStruct) const {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  size_t lineNumber = spiceStruct->getDeclCodeLoc().line;
  size_t sizeInBits = module->getDataLayout().getTypeSizeInBits(llvmStructTy);
  llvm::DINode::DIFlags flags = spiceStruct->getSpecifiers().isPublic() ? llvm::DINode::FlagPublic : llvm::DINode::FlagPrivate;
  llvm::DINodeArray elements = diBuilder->getOrCreateArray({}); // ToDo: fill
  return diBuilder->createStructType(unit, spiceStruct->getName(), unit, lineNumber, sizeInBits, 0, flags, nullptr, elements);
}*/

void DebugInfoGenerator::generateGlobalVarDebugInfo(llvm::GlobalVariable *global, const SymbolTableEntry *globalEntry) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;
  llvm::DIFile *unit = diBuilder->createFile(compileUnit->getFilename(), compileUnit->getDirectory());
  const size_t lineNumber = globalEntry->getDeclCodeLoc().line;
  llvm::StringRef name = global->getName();
  llvm::DIType *type = getDITypeForSymbolType(globalEntry->getType());
  const bool isLocal = globalEntry->specifiers.isPublic();

  global->addDebugInfo(diBuilder->createGlobalVariableExpression(unit, name, name, unit, lineNumber, type, isLocal));
}

void DebugInfoGenerator::generateLocalVarDebugInfo(const CodeLoc &codeLoc, const std::string &varName, llvm::Value *address,
                                                   const size_t argNumber, bool moveToPrev) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;
  // Get symbol table entry
  SymbolTableEntry *variableEntry = irGenerator->currentScope->lookupStrict(varName);
  assert(variableEntry != nullptr);
  // Build debug info
  llvm::DIFile *unit = diBuilder->createFile(compileUnit->getFilename(), compileUnit->getDirectory());
  llvm::DIScope *scope = lexicalBlocks.top();
  llvm::DIType *diType = getDITypeForSymbolType(variableEntry->getType());
  llvm::DILocalVariable *varInfo;
  if (argNumber != SIZE_MAX)
    varInfo = diBuilder->createParameterVariable(scope, varName, argNumber, unit, codeLoc.line, diType);
  else
    varInfo = diBuilder->createAutoVariable(scope, varName, unit, codeLoc.line, diType);
  llvm::DIExpression *expr = diBuilder->createExpression();
  auto inst = diBuilder->insertDeclare(address, varInfo, expr, irGenerator->builder.getCurrentDebugLocation(),
                                       irGenerator->allocaInsertBlock);
  if (moveToPrev)
    inst->moveBefore(irGenerator->builder.GetInsertPoint()->getPrevNonDebugInstruction());
}

void DebugInfoGenerator::setSourceLocation(const ASTNode *node) {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  if (lexicalBlocks.empty()) {
    irGenerator->builder.SetCurrentDebugLocation(llvm::DebugLoc());
    return;
  }

  llvm::DIScope *scope = lexicalBlocks.top();
  auto codeLoc = llvm::DILocation::get(scope->getContext(), node->codeLoc.line, node->codeLoc.col, scope);
  irGenerator->builder.SetCurrentDebugLocation(codeLoc);
}

void DebugInfoGenerator::concludeFunctionDebugInfo() {
  if (!irGenerator->cliOptions.generateDebugInfo)
    return;

  assert(!lexicalBlocks.empty());
  lexicalBlocks.pop();
}

void DebugInfoGenerator::finalize() {
  if (irGenerator->cliOptions.generateDebugInfo)
    diBuilder->finalize();
}

llvm::DIType *DebugInfoGenerator::getDITypeForSymbolType(const SymbolType &symbolType) const { // NOLINT(misc-no-recursion)
  if (symbolType.isPointer()) {                                                                // Pointer type
    llvm::DIType *pointeeTy = getDITypeForSymbolType(symbolType.getContainedTy());
    const unsigned int pointerWidth = irGenerator->module->getDataLayout().getPointerSizeInBits();
    return diBuilder->createPointerType(pointeeTy, pointerWidth);
  }

  if (symbolType.isArray()) { // Array type
    llvm::DIType *itemTy = getDITypeForSymbolType(symbolType.getContainedTy());
    const size_t size = symbolType.getArraySize();
    llvm::DINodeArray subscripts = diBuilder->getOrCreateArray({});
    return diBuilder->createArrayType(size, 0, itemTy, subscripts);
  }

  // Primitive types
  switch (symbolType.getSuperType()) {
  case TY_DOUBLE:
    return doubleTy;
  case TY_INT:
    return symbolType.isSigned() ? intTy : uIntTy;
  case TY_SHORT:
    return symbolType.isSigned() ? shortTy : uShortTy;
  case TY_LONG:
    return symbolType.isSigned() ? longTy : uLongTy;
  case TY_BYTE:
    return byteTy;
  case TY_CHAR:
    return charTy;
  case TY_STRING:
    return stringTy;
  case TY_BOOL:
    return boolTy;
  default:
    return nullptr;
  }
}

} // namespace spice::compiler
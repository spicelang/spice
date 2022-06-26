// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

#include <stdexcept>

#include <analyzer/AnalyzerVisitor.h>
#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/IRError.h>
#include <exception/SemanticError.h>
#include <symbol/Function.h>
#include <symbol/Struct.h>
#include <symbol/SymbolTable.h>
#include <util/FileUtil.h>
#include <util/ScopeIdUtil.h>
#include <util/ThreadFactory.h>

#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>

GeneratorVisitor::GeneratorVisitor(const std::shared_ptr<llvm::LLVMContext> &context,
                                   const std::shared_ptr<llvm::IRBuilder<>> &builder, ThreadFactory &threadFactory,
                                   const LinkerInterface &linker, const CliOptions &cliOptions, const SourceFile &sourceFile,
                                   const std::string &objectFile)
    : objectFile(objectFile), cliOptions(cliOptions), linker(linker), context(context), builder(builder),
      threadFactory(threadFactory) {
  // Enrich options
  this->requiresMainFct = sourceFile.parent == nullptr;
  this->currentScope = this->rootScope = sourceFile.symbolTable.get();

  // Create error factory for this specific file
  this->err = std::make_unique<ErrorFactory>(sourceFile.filePath);

  // Create LLVM base components
  module = std::make_unique<llvm::Module>(FileUtil::getFileName(sourceFile.filePath), *context);
  conversionsManager = std::make_unique<OpRuleConversionsManager>(builder, err.get());

  // Initialize LLVM
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  // Configure output target
  module->setTargetTriple(cliOptions.targetTriple);

  // Search after selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw err->get(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  llvm::TargetOptions opt;
  llvm::Optional rm = llvm::Optional<llvm::Reloc::Model>();
  targetMachine = target->createTargetMachine(cliOptions.targetTriple, "generic", "", opt, rm);

  module->setDataLayout(targetMachine->createDataLayout());
}

void GeneratorVisitor::optimize() {
  if (cliOptions.printDebugOutput)
    std::cout << "\nOptimizing on level " + std::to_string(cliOptions.optLevel) << " ...\n"; // GCOV_EXCL_LINE

  llvm::LoopAnalysisManager loopAnalysisMgr;
  llvm::FunctionAnalysisManager functionAnalysisMgr;
  llvm::CGSCCAnalysisManager cgsccAnalysisMgr;
  llvm::ModuleAnalysisManager moduleAnalysisMgr;
  llvm::PassBuilder passBuilder(targetMachine);

  functionAnalysisMgr.registerPass([&] { return passBuilder.buildDefaultAAPipeline(); });

  passBuilder.registerModuleAnalyses(moduleAnalysisMgr);
  passBuilder.registerCGSCCAnalyses(cgsccAnalysisMgr);
  passBuilder.registerFunctionAnalyses(functionAnalysisMgr);
  passBuilder.registerLoopAnalyses(loopAnalysisMgr);
  passBuilder.crossRegisterProxies(loopAnalysisMgr, functionAnalysisMgr, cgsccAnalysisMgr, moduleAnalysisMgr);

  // Run passes
  llvm::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
  llvm::ModulePassManager modulePassMgr = passBuilder.buildPerModuleDefaultPipeline(llvmOptLevel);
  modulePassMgr.addPass(llvm::AlwaysInlinerPass());
  modulePassMgr.run(*module, moduleAnalysisMgr);
}

void GeneratorVisitor::emit() {
  // GCOV_EXCL_START
  if (cliOptions.printDebugOutput)
    std::cout << "\nEmitting object file for triplet '" << cliOptions.targetTriple << "' to path: " << objectFile << "\n";
  // GCOV_EXCL_STOP

  // Open file output stream
  std::error_code errorCode;
  llvm::raw_fd_ostream dest(objectFile, errorCode, llvm::sys::fs::OF_None);
  if (errorCode)
    throw err->get(CANT_OPEN_OUTPUT_FILE, "File '" + objectFile + "' could not be opened"); // GCOV_EXCL_LINE

  llvm::legacy::PassManager passManager;
  if (targetMachine->addPassesToEmitFile(passManager, dest, nullptr, llvm::CGFT_ObjectFile))
    throw err->get(WRONG_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(*module);
  dest.flush();
}

void GeneratorVisitor::dumpIR() { module->print(llvm::outs(), nullptr); } // GCOV_EXCL_LINE

std::string GeneratorVisitor::getIRString() {
  std::string output;
  llvm::raw_string_ostream oss(output);
  module->print(oss, nullptr);
  return oss.str();
}

std::any GeneratorVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
  std::any result = SpiceBaseVisitor::visitEntry(ctx);

  // Verify module to detect IR code bugs
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyModule(*module, &oss))
    throw err->get(*ctx->start, INVALID_MODULE, oss.str());

  return result;
}

std::any GeneratorVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  if (requiresMainFct) { // Only create main function when it is required
    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    assert(currentScope != nullptr);

    // Visit arguments
    std::vector<std::string> argNames;
    std::vector<llvm::Type *> argTypes;
    if (ctx->argLstDef()) {
      argNames.reserve(ctx->argLstDef()->declStmt().size());
      argTypes.reserve(ctx->argLstDef()->declStmt().size());
      for (const auto &arg : ctx->argLstDef()->declStmt()) {
        currentVarName = arg->IDENTIFIER()->toString();
        argNames.push_back(currentVarName);
        auto argType = any_cast<llvm::Type *>(visit(arg->dataType()));
        argTypes.push_back(argType);
      }
    }

    // Build function itself
    llvm::Type *returnType = builder->getInt32Ty();
    llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, argTypes, false);
    llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, MAIN_FUNCTION_NAME, module.get());
    llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
    allocaInsertInst = nullptr;
    fct->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store function arguments
    for (auto &arg : fct->args()) {
      unsigned argNo = arg.getArgNo();
      std::string argName = argNames[argNo];
      llvm::Type *argType = fct->getFunctionType()->getParamType(argNo);
      llvm::Value *memAddress = insertAlloca(argType, argName);
      SymbolTableEntry *argSymbol = currentScope->lookup(argName);
      assert(argSymbol != nullptr);
      argSymbol->updateAddress(memAddress);
      argSymbol->updateLLVMType(argType);
      builder->CreateStore(&arg, memAddress);
    }

    // Declare result variable and set it to 0 for positive return code
    llvm::Value *memAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
    SymbolTableEntry *returnSymbol = currentScope->lookup(RETURN_VARIABLE_NAME);
    assert(returnSymbol != nullptr);
    returnSymbol->updateAddress(memAddress);
    returnSymbol->updateLLVMType(returnType);
    builder->CreateStore(builder->getInt32(0), returnSymbol->getAddress());

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Generate return statement for result variable
    if (!blockAlreadyTerminated) {
      llvm::Value *result = returnSymbol->getAddress();
      builder->CreateRet(builder->CreateLoad(result->getType()->getPointerElementType(), result));
    }

    // Verify function
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyFunction(*fct, &oss))
      throw err->get(*ctx->start, INVALID_FUNCTION, oss.str());

    // Change scope back
    currentScope = currentScope->getParent();
    assert(currentScope != nullptr);
  }

  return nullptr;
}

std::any GeneratorVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  // Check if this is a global function or a method
  std::string functionName = ctx->IDENTIFIER().back()->toString();
  bool isMethod = ctx->IDENTIFIER().size() > 1;

  // Change to the (potentially generic) struct scope
  SymbolTable *accessScope = currentScope;
  if (isMethod)
    accessScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER().front()->toString());
  assert(accessScope != nullptr);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Function> *manifestations = accessScope->getFunctionManifestations(*ctx->start);

  if (manifestations) {
    for (const auto &[mangledName, spiceFunc] : *manifestations) {
      // Check if the function is substantiated
      if (!spiceFunc.isFullySubstantiated())
        continue;

      // Do not generate this function if it is private and used by nobody
      if (!spiceFunc.isUsed() && !spiceFunc.getSpecifiers().isPublic())
        continue;

      std::vector<std::string> argNames;
      std::vector<llvm::Type *> argTypes;

      if (isMethod) { // Change to the struct scope
        std::string structSignature = Struct::getSignature(spiceFunc.getThisType().getBaseType().getSubType(),
                                                           spiceFunc.getThisType().getBaseType().getTemplateTypes());
        // Get the LLVM type of the struct symbol
        SymbolTableEntry *thisEntry = currentScope->lookup(structSignature);
        assert(thisEntry);
        argNames.push_back(THIS_VARIABLE_NAME);
        argTypes.push_back(thisEntry->getLLVMType()->getPointerTo());
        // Change scope to struct
        currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope);
      }

      // Change scope
      currentScope = currentScope->getChild(spiceFunc.getSignature());
      assert(currentScope);

      // Get return type
      llvm::Type *returnType = getTypeForSymbolType(spiceFunc.getReturnType(), currentScope);

      // Arguments
      unsigned int currentArgIndex = 0;
      if (ctx->argLstDef()) {
        std::vector<SymbolType> argSymbolTypes = spiceFunc.getArgTypes();
        for (; currentArgIndex < argSymbolTypes.size(); currentArgIndex++) {
          currentVarName = ctx->argLstDef()->declStmt()[currentArgIndex]->IDENTIFIER()->toString();
          argNames.push_back(currentVarName);
          argTypes.push_back(getTypeForSymbolType(argSymbolTypes[currentArgIndex], currentScope));
        }
      }

      // Check if function is public and/or explicit inlined
      llvm::GlobalValue::LinkageTypes linkage = llvm::Function::InternalLinkage;
      bool explicitInlined = false;
      if (ctx->declSpecifiers()) {
        for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
          if (specifier->INLINE()) {
            explicitInlined = true;
          } else if (specifier->PUBLIC()) {
            linkage = llvm::Function::ExternalLinkage;
          }
        }
      }

      // Create function itself
      llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, argTypes, false);
      module->getOrInsertFunction(mangledName, fctType);
      llvm::Function *fct = module->getFunction(mangledName);
      fct->setLinkage(linkage);
      if (explicitInlined)
        fct->addFnAttr(llvm::Attribute::AlwaysInline);

      // Create entry block
      llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
      allocaInsertInst = nullptr;
      fct->getBasicBlockList().push_back(bEntry);
      moveInsertPointToBlock(bEntry);

      // Store mandatory function args
      for (auto &arg : fct->args()) {
        unsigned int argNo = arg.getArgNo();
        std::string argName = argNames[argNo];
        llvm::Type *argType = fct->getFunctionType()->getParamType(argNo);
        llvm::Value *memAddress = insertAlloca(argType, argName);
        SymbolTableEntry *argEntry = currentScope->lookup(argName);
        assert(argEntry != nullptr);
        argEntry->updateAddress(memAddress);
        argEntry->updateLLVMType(argType);
        builder->CreateStore(&arg, memAddress);
      }

      // Store the default values for optional function args
      if (ctx->argLstDef()) {
        for (; currentArgIndex < ctx->argLstDef()->declStmt().size(); currentArgIndex++) {
          visit(ctx->argLstDef()->declStmt()[currentArgIndex]);
        }
      }

      // Declare result variable
      llvm::Value *returnMemAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
      SymbolTableEntry *returnSymbol = currentScope->lookup(RETURN_VARIABLE_NAME);
      assert(returnSymbol != nullptr);
      returnSymbol->updateAddress(returnMemAddress);
      returnSymbol->updateLLVMType(returnType);

      // Generate IR for function body
      visit(ctx->stmtLst());

      // Generate return statement for result variable
      if (!blockAlreadyTerminated) {
        llvm::Value *result = returnSymbol->getAddress();
        builder->CreateRet(builder->CreateLoad(result->getType()->getPointerElementType(), result));
      }

      // Verify function
      std::string output;
      llvm::raw_string_ostream oss(output);
      if (llvm::verifyFunction(*fct, &oss))
        throw err->get(*ctx->start, INVALID_FUNCTION, oss.str());

      // Change scope back
      currentScope = currentScope->getParent();
      assert(currentScope);

      // Leave the struct scope
      if (isMethod) {
        currentScope = currentScope->getParent();
        assert(currentScope);
      }
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  std::string procedureName = ctx->IDENTIFIER().back()->toString();
  bool isMethod = ctx->IDENTIFIER().size() > 1;

  // Change to the (potentially generic) struct scope
  SymbolTable *accessScope = currentScope;
  if (isMethod)
    accessScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER().front()->toString());
  assert(accessScope != nullptr);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Function> *manifestations = accessScope->getFunctionManifestations(*ctx->start);

  if (manifestations) {
    for (const auto &[mangledName, spiceProc] : *manifestations) {
      // Check if the function is substantiated
      if (!spiceProc.isFullySubstantiated())
        continue;

      // Do not generate this function if it is private and used by nobody
      if (!spiceProc.isUsed() && !spiceProc.getSpecifiers().isPublic())
        continue;

      std::vector<std::string> argNames;
      std::vector<llvm::Type *> argTypes;

      if (isMethod) { // Change to the struct scope
        std::string structSignature = Struct::getSignature(spiceProc.getThisType().getBaseType().getSubType(),
                                                           spiceProc.getThisType().getBaseType().getTemplateTypes());
        // Get the LLVM type of the struct symbol
        SymbolTableEntry *thisEntry = currentScope->lookup(structSignature);
        assert(thisEntry);
        argNames.push_back(THIS_VARIABLE_NAME);
        argTypes.push_back(thisEntry->getLLVMType()->getPointerTo());
        // Change scope to struct
        currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope);
      }

      // Change scope
      currentScope = currentScope->getChild(spiceProc.getSignature());
      assert(currentScope);

      // Arguments
      unsigned int currentArgIndex = 0;
      if (ctx->argLstDef()) {
        std::vector<SymbolType> argSymbolTypes = spiceProc.getArgTypes();
        for (; currentArgIndex < argSymbolTypes.size(); currentArgIndex++) {
          currentVarName = ctx->argLstDef()->declStmt()[currentArgIndex]->IDENTIFIER()->toString();
          argNames.push_back(currentVarName);
          argTypes.push_back(getTypeForSymbolType(argSymbolTypes[currentArgIndex], currentScope));
        }
      }

      // Check if function is public and/or explicit inlined
      llvm::GlobalValue::LinkageTypes linkage = llvm::Function::InternalLinkage;
      bool explicitInlined = false;
      if (ctx->declSpecifiers()) {
        for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
          if (specifier->INLINE()) {
            explicitInlined = true;
          } else if (specifier->PUBLIC()) {
            linkage = llvm::Function::ExternalLinkage;
          }
        }
      }

      // Create procedure itself
      llvm::FunctionType *procType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), argTypes, false);
      module->getOrInsertFunction(mangledName, procType);
      llvm::Function *proc = module->getFunction(mangledName);
      proc->setLinkage(linkage);
      if (explicitInlined)
        proc->addFnAttr(llvm::Attribute::AlwaysInline);

      // Create entry block
      llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
      allocaInsertInst = nullptr;
      proc->getBasicBlockList().push_back(bEntry);
      moveInsertPointToBlock(bEntry);

      // Store mandatory procedure args
      for (auto &arg : proc->args()) {
        unsigned int argNo = arg.getArgNo();
        std::string argName = argNames[argNo];
        llvm::Type *argType = proc->getFunctionType()->getParamType(argNo);
        llvm::Value *memAddress = insertAlloca(argType, argName);
        SymbolTableEntry *argSymbol = currentScope->lookup(argName);
        assert(argSymbol != nullptr);
        argSymbol->updateAddress(memAddress);
        argSymbol->updateLLVMType(argType);
        builder->CreateStore(&arg, memAddress);
      }

      // Store the default values for optional procedure args
      if (ctx->argLstDef()) {
        for (; currentArgIndex < ctx->argLstDef()->declStmt().size(); currentArgIndex++) {
          visit(ctx->argLstDef()->declStmt()[currentArgIndex]);
        }
      }

      // Generate IR for procedure body
      visit(ctx->stmtLst());

      // Create return
      builder->CreateRetVoid();

      // Verify procedure
      std::string output;
      llvm::raw_string_ostream oss(output);
      if (llvm::verifyFunction(*proc, &oss))
        throw err->get(*ctx->start, INVALID_FUNCTION, oss.str());

      // Change scope back
      currentScope = currentScope->getParent();
      assert(currentScope);

      // Leave the struct scope
      if (isMethod) {
        currentScope = currentScope->getParent();
        assert(currentScope);
      }
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  // Get function name
  std::string functionName = ctx->IDENTIFIER()->toString();
  std::vector<SymbolType> symbolTypes;

  // Pop function declaration pointer from the stack
  std::map<std::string, Function> *manifestations = currentScope->getFunctionManifestations(*ctx->start);
  assert(!manifestations->empty());
  Function spiceFunc = manifestations->begin()->second;

  // Get LLVM return type
  llvm::Type *returnType;
  if (ctx->dataType()) {
    returnType = any_cast<llvm::Type *>(visit(ctx->dataType()));
    SymbolTable *functionTable = currentScope->getChild(spiceFunc.getSignature());
    assert(functionTable != nullptr);
    SymbolTableEntry *returnEntry = functionTable->lookup(RETURN_VARIABLE_NAME);
    assert(returnEntry != nullptr);
    symbolTypes.push_back(returnEntry->getType());
  } else {
    returnType = llvm::Type::getVoidTy(*context);
  }

  // Get LLVM arg types
  std::vector<llvm::Type *> argTypes;
  if (ctx->typeLstEllipsis()) {
    for (const auto &arg : ctx->typeLstEllipsis()->typeLst()->dataType()) {
      auto argType = any_cast<llvm::Type *>(visit(arg));
      argTypes.push_back(argType);
    }
  }
  std::vector<SymbolType> argSymbolTypes = spiceFunc.getArgTypes();
  symbolTypes.insert(std::end(symbolTypes), std::begin(argSymbolTypes), std::end(argSymbolTypes));

  // Get vararg
  bool isVararg = ctx->typeLstEllipsis() && ctx->typeLstEllipsis()->ELLIPSIS();

  // Declare function
  llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, argTypes, isVararg);
  module->getOrInsertFunction(functionName, functionType);
  if (ctx->DLL())
    module->getFunction(functionName)->setDLLStorageClass(llvm::GlobalValue::DLLImportStorageClass);

  return nullptr;
}

std::any GeneratorVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) {
  // Get struct name
  std::string structName = ctx->IDENTIFIER()->toString();

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Struct> *manifestations = currentScope->getStructManifestations(*ctx->start);
  if (manifestations) {
    for (const auto &[mangledName, spiceStruct] : *manifestations) {
      // Check if the struct is substantiated
      if (!spiceStruct.isFullySubstantiated())
        continue;

      // Do not generate this struct if it is private and used by nobody
      if (!spiceStruct.isUsed() && !spiceStruct.getSpecifiers().isPublic())
        continue;

      // Change scope
      currentScope = currentScope->getChild(STRUCT_SCOPE_PREFIX + spiceStruct.getSignature());
      assert(currentScope);

      // Collect concrete field types
      std::vector<llvm::Type *> fieldTypes;
      for (const auto &field : ctx->field()) {
        std::string fieldName = field->IDENTIFIER()->toString();
        SymbolTableEntry *fieldEntry = currentScope->lookup(fieldName);
        assert(fieldEntry && !fieldEntry->getType().is(TY_GENERIC));
        fieldTypes.push_back(getTypeForSymbolType(fieldEntry->getType(), currentScope));
      }

      // Return to old scope
      currentScope = currentScope->getParent();
      assert(currentScope);

      // Create global struct
      llvm::StructType *structType = llvm::StructType::create(*context, fieldTypes, mangledName);
      SymbolTableEntry *structSymbol = currentScope->lookup(spiceStruct.getSignature());
      assert(structSymbol);
      structSymbol->updateLLVMType(structType);
    }
  }

  return nullptr;
}

std::any GeneratorVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  std::string varName = currentVarName = ctx->IDENTIFIER()->toString();

  // Get symbol table entry and the symbol specifiers
  SymbolTableEntry *globalVarEntry = currentScope->lookup(varName);
  assert(globalVarEntry != nullptr);
  SymbolSpecifiers specifiers = globalVarEntry->getSpecifiers();
  llvm::GlobalValue::LinkageTypes linkage =
      specifiers.isPublic() ? llvm::GlobalValue::LinkageTypes::ExternalLinkage : llvm::GlobalValue::LinkageTypes::InternalLinkage;

  // Create correctly signed LLVM type from the data type
  currentVarSigned = specifiers.isSigned();
  auto varType = any_cast<llvm::Type *>(visit(ctx->dataType()));
  globalVarEntry->updateLLVMType(varType);

  // Create global variable
  llvm::Value *memAddress = module->getOrInsertGlobal(varName, varType);
  globalVarEntry->updateAddress(memAddress);
  // Set some attributes to it
  llvm::GlobalVariable *global = module->getNamedGlobal(varName);
  global->setLinkage(linkage);
  global->setConstant(specifiers.isConst());

  if (ctx->value()) { // Variable is initialized here
    visit(ctx->value());
    constNegate = ctx->MINUS();
    global->setInitializer(currentConstValue);
    constNegate = false;
  }

  return nullptr;
}

std::any GeneratorVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  // Create threaded function
  std::string threadedFctName = "_thread" + std::to_string(threadFactory.getNextFunctionSuffix());
  llvm::Type *voidPtrTy = builder->getInt8PtrTy();
  llvm::FunctionType *threadFctTy = llvm::FunctionType::get(voidPtrTy, voidPtrTy, false);
  llvm::Function *threadFct = llvm::Function::Create(threadFctTy, llvm::Function::InternalLinkage, threadedFctName, module.get());

  // Change scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->getChild(scopeId);
  assert(currentScope != nullptr);

  // Collect arg names, types and addresses from captures of the nested scope
  std::vector<std::string> argStructFieldNames;
  std::vector<llvm::Type *> argStructFieldTypes;
  std::vector<llvm::Value *> argStructFieldPointers;
  for (const auto &capture : currentScope->getCaptures()) {
    argStructFieldNames.push_back(capture.first);
    argStructFieldTypes.push_back(capture.second.getEntry()->getLLVMType()->getPointerTo());
    argStructFieldPointers.push_back(capture.second.getEntry()->getAddress());
  }

  // Create arg struct instance
  llvm::StructType *argStructTy = llvm::StructType::get(*context, argStructFieldTypes, false);
  llvm::Value *argStruct = insertAlloca(argStructTy);

  // Fill the struct with the argument values
  for (int i = 0; i < argStructFieldNames.size(); i++) {
    llvm::Value *fieldAddress = builder->CreateStructGEP(argStructTy, argStruct, i);
    builder->CreateStore(argStructFieldPointers[i], fieldAddress);
  }

  // Get current basic block to return to later
  llvm::BasicBlock *bOriginal = builder->GetInsertBlock();

  // Create entry block for thread function
  llvm::BasicBlock *allocaInsertBlockBackup = allocaInsertBlock;
  llvm::Instruction *allocaInsertInstBackup = allocaInsertInst;
  llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
  allocaInsertInst = nullptr;
  threadFct->getBasicBlockList().push_back(bEntry);
  moveInsertPointToBlock(bEntry);

  // Store function args
  llvm::Value *recArgStructPtr = builder->CreatePointerCast(threadFct->args().begin(), argStructTy->getPointerTo());
  unsigned int i = 0;
  for (const auto &capture : currentScope->getCaptures()) {
    std::string argName = argStructFieldNames[i];
    llvm::Value *memAddress = builder->CreateStructGEP(argStructTy, recArgStructPtr, i);
    memAddress = builder->CreateLoad(memAddress->getType()->getPointerElementType(), memAddress);
    // Push address to each capture to ensure that the address is valid and known to the inner function
    capture.second.getEntry()->pushAddress(memAddress);
    i++;
  }

  // Insert instructions into thread function
  visit(ctx->stmtLst());

  // Pop address from each capture to ensure that the address is valid and known to the outer function
  for (const auto &capture : currentScope->getCaptures())
    capture.second.getEntry()->popAddress();

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Insert return statement and verify function
  llvm::Value *voidPtrNull = llvm::Constant::getNullValue(llvm::Type::getInt8Ty(*context)->getPointerTo());
  builder->CreateRet(voidPtrNull);

  // Verify function
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyFunction(*threadFct, &oss))
    throw err->get(*ctx->start, INVALID_FUNCTION, oss.str());

  // Change back to the original basic block
  moveInsertPointToBlock(bOriginal);

  // Restore alloca insert block and inst
  allocaInsertBlock = allocaInsertBlockBackup;
  allocaInsertInst = allocaInsertInstBackup;

  // Create pthread instance
  llvm::Type *pthreadTy = builder->getInt8PtrTy();
  llvm::Value *pthread = builder->CreateAlloca(pthreadTy); // Caution: Do not replace with insertAlloca() due to thread safety!

  // Get function pthread_create
  llvm::Function *ptCreateFct = module->getFunction("pthread_create");
  if (!ptCreateFct) { // Declare function if not done already
    std::vector<llvm::Type *> argTypes = {pthreadTy->getPointerTo(), voidPtrTy, threadFctTy->getPointerTo(), voidPtrTy};
    llvm::FunctionType *ptCreateFctTy = llvm::FunctionType::get(builder->getInt32Ty(), argTypes, false);
    module->getOrInsertFunction("pthread_create", ptCreateFctTy);
    ptCreateFct = module->getFunction("pthread_create");
  }
  assert(ptCreateFct != nullptr);

  // Create call to pthread_create
  llvm::Value *args[4] = {pthread, voidPtrNull, threadFct, builder->CreatePointerCast(argStruct, voidPtrTy)};
  builder->CreateCall(ptCreateFct, args);

  // Return the thread id as i8**
  return pthread;
}

std::any GeneratorVisitor::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {
  // Change scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->getChild(scopeId);
  assert(currentScope != nullptr);

  // Visit instructions in the block
  visit(ctx->stmtLst());

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  return nullptr;
}

std::any GeneratorVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto head = ctx->forHead();

  // Create blocks
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "for.cond");
  llvm::BasicBlock *bPost = llvm::BasicBlock::Create(*context, "for.post");
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "for");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "for.end");

  // Change scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->getChild(scopeId);
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bPost);

  // Execute pre-loop stmts
  visit(head->declStmt());
  // Jump into condition block
  createBr(bCond);

  // Get parent function
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Fill condition block
  parentFct->getBasicBlockList().push_back(bCond);
  moveInsertPointToBlock(bCond);
  auto condValuePtr = any_cast<llvm::Value *>(visit(head->assignExpr()[0]));
  llvm::Value *condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
  // Jump to loop body or to loop end
  createCondBr(condValue, bLoop, bEnd);

  // Fill loop block
  parentFct->getBasicBlockList().push_back(bLoop);
  moveInsertPointToBlock(bLoop);
  // Generate IR for nested statements
  visit(ctx->stmtLst());
  // Jump into post block
  createBr(bPost);

  // Fill loop post block
  parentFct->getBasicBlockList().push_back(bPost);
  moveInsertPointToBlock(bPost);
  // Run post-loop actions
  visit(head->assignExpr()[1]);
  // Jump into condition block
  createBr(bCond);

  // Fill loop end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Pop basic blocks from break and continue stacks
  breakBlocks.pop();
  continueBlocks.pop();

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto head = ctx->foreachHead();

  // Create blocks
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "foreach.loop");
  llvm::BasicBlock *bInc = llvm::BasicBlock::Create(*context, "foreach.inc");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "foreach.end");

  // Change scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->getChild(scopeId);
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bInc);

  // Initialize loop variables
  llvm::Value *indexVariablePtr;
  if (head->declStmt().size() >= 2) {
    auto indexVariableName = any_cast<std::string>(visit(ctx->foreachHead()->declStmt().front()));
    SymbolTableEntry *indexVariableEntry = currentScope->lookup(indexVariableName);
    assert(indexVariableEntry != nullptr);
    indexVariablePtr = indexVariableEntry->getAddress();

    // Initialize variable with 0
    if (!head->declStmt().front()->assignExpr())
      builder->CreateStore(builder->getInt32(0), indexVariablePtr);
  } else {
    std::string indexVariableName = FOREACH_DEFAULT_IDX_VARIABLE_NAME;
    // Create local variable for
    llvm::Type *indexVariableType = llvm::Type::getInt32Ty(*context);
    indexVariablePtr = insertAlloca(indexVariableType, indexVariableName);
    SymbolTableEntry *indexVariableEntry = currentScope->lookup(indexVariableName);
    assert(indexVariableEntry != nullptr);
    indexVariableEntry->updateAddress(indexVariablePtr);
    indexVariableEntry->updateLLVMType(indexVariableType);
    indexVariableEntry->setUsed();
    // Initialize variable with 0
    builder->CreateStore(builder->getInt32(0), indexVariablePtr);
  }
  auto itemVariableName = any_cast<std::string>(visit(ctx->foreachHead()->declStmt().back()));
  SymbolTableEntry *itemVariableEntry = currentScope->lookup(itemVariableName);
  assert(itemVariableEntry != nullptr);
  llvm::Value *itemVariablePtr = itemVariableEntry->getAddress();

  // Do loop variable initialization
  auto valuePtr = any_cast<llvm::Value *>(visit(ctx->foreachHead()->assignExpr()));
  llvm::Value *value = builder->CreateLoad(valuePtr->getType()->getPointerElementType(), valuePtr);
  llvm::Value *maxIndex = builder->getInt32(value->getType()->getArrayNumElements() - 1);
  // Load the first item into item variable
  llvm::Value *index = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr);
  llvm::Value *indices[2] = {builder->getInt32(0), index};
  llvm::Value *itemPtr = builder->CreateInBoundsGEP(valuePtr->getType()->getPointerElementType(), valuePtr, indices);
  llvm::Value *newItemValue = builder->CreateLoad(itemPtr->getType()->getPointerElementType(), itemPtr);
  builder->CreateStore(newItemValue, itemVariablePtr);
  createBr(bLoop);

  // Get parent function
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Fill loop block
  parentFct->getBasicBlockList().push_back(bLoop);
  moveInsertPointToBlock(bLoop);
  // Generate IR for nested statements
  visit(ctx->stmtLst());
  // Check if the index variable reached the size -2
  if (!blockAlreadyTerminated) {
    index = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr);
    llvm::Value *cond = builder->CreateICmpSLT(index, maxIndex);
    createCondBr(cond, bInc, bEnd);
  }

  // Fill inc block
  parentFct->getBasicBlockList().push_back(bInc);
  moveInsertPointToBlock(bInc);
  // Increment index variable
  index = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr, "idx");
  index = builder->CreateAdd(index, builder->getInt32(1), "idx.inc");
  builder->CreateStore(index, indexVariablePtr);
  // Load new item into item variable
  indices[1] = index;
  itemPtr = builder->CreateInBoundsGEP(valuePtr->getType()->getPointerElementType(), valuePtr, indices);
  newItemValue = builder->CreateLoad(itemPtr->getType()->getPointerElementType(), itemPtr);
  builder->CreateStore(newItemValue, itemVariablePtr);
  createBr(bLoop);

  // Fill loop end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Pop basic blocks from break and continue stacks
  breakBlocks.pop();
  continueBlocks.pop();

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "while.cond");
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "while");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "while.end");

  // Change scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->getChild(scopeId);
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bCond);

  // Jump into condition block
  createBr(bCond);

  // Fill condition block
  parentFct->getBasicBlockList().push_back(bCond);
  moveInsertPointToBlock(bCond);
  auto condValuePtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
  llvm::Value *condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
  createCondBr(condValue, bLoop, bEnd);

  // Fill loop block
  parentFct->getBasicBlockList().push_back(bLoop);
  moveInsertPointToBlock(bLoop);
  // Generate IR for nested statements
  visit(ctx->stmtLst());
  // Jump into condition block
  createBr(bCond);

  // Fill loop end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  for (const auto &child : ctx->children) {
    if (!blockAlreadyTerminated)
      visit(child);
  }
  return nullptr;
}

std::any GeneratorVisitor::visitTypeAlts(SpiceParser::TypeAltsContext *ctx) {
  return nullptr; // Noop
}

std::any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  // Change scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->getChild(scopeId);
  assert(currentScope != nullptr);

  auto condValuePtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
  llvm::Value *condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *bThen = llvm::BasicBlock::Create(*context, "if.then");
  llvm::BasicBlock *bElse = llvm::BasicBlock::Create(*context, "if.else");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "if.end");

  // Check if condition is fulfilled
  createCondBr(condValue, bThen, ctx->elseStmt() ? bElse : bEnd);

  // Fill then block
  parentFct->getBasicBlockList().push_back(bThen);
  moveInsertPointToBlock(bThen);
  // Generate IR for nested statements
  visit(ctx->stmtLst());
  createBr(bEnd);

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Fill else block
  if (ctx->elseStmt()) {
    parentFct->getBasicBlockList().push_back(bElse);
    moveInsertPointToBlock(bElse);
    visit(ctx->elseStmt()); // Generate IR for else block
    createBr(bEnd);
  }

  // Fill end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Return conditional value as result for the if stmt
  return condValue;
}

std::any GeneratorVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  if (ctx->ifStmt()) { // It is an else if branch
    visit(ctx->ifStmt());
  } else { // It is an else branch
    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    assert(currentScope != nullptr);

    // Generate IR for nested statements
    visit(ctx->stmtLst());

    // Change scope back
    currentScope = currentScope->getParent();
    assert(currentScope != nullptr);
  }
  return nullptr;
}

std::any GeneratorVisitor::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  // Only generate assertions with -O0
  if (cliOptions.optLevel == 0) {
    // Visit the assignExpr
    auto condValuePtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
    llvm::Value *condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
    llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock *bThen = llvm::BasicBlock::Create(*context, "if.then");
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "if.end");

    // Check if condition is fulfilled
    createCondBr(condValue, bEnd, bThen);

    // Fill then block
    parentFct->getBasicBlockList().push_back(bThen);
    moveInsertPointToBlock(bThen);
    // Generate IR for assertion error
    llvm::Function *printfFct = retrievePrintfFct();
    std::string errorMsg = "Assertion failed: Condition '" + ctx->assignExpr()->getText() + "' evaluated to false.";
    llvm::Value *templateString = builder->CreateGlobalStringPtr(errorMsg);
    builder->CreateCall(printfFct, templateString);
    // Generate call to exit
    llvm::Function *exitFct = retrieveExitFct();
    builder->CreateCall(exitFct, builder->getInt32(1));
    // Create unreachable instruction
    builder->CreateUnreachable();

    // Fill end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);
  }

  return nullptr;
}

std::any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  currentVarName = lhsVarName = ctx->IDENTIFIER()->toString();
  llvm::Type *varType = lhsType = any_cast<llvm::Type *>(visit(ctx->dataType()));

  // Get variable entry
  SymbolTableEntry *entry = currentScope->lookup(lhsVarName);
  assert(entry != nullptr);
  llvm::Value *memAddress = insertAlloca(varType, lhsVarName);
  entry->updateAddress(memAddress);
  entry->updateLLVMType(varType);
  if (ctx->assignExpr()) {
    // Visit right side
    auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
    llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
    builder->CreateStore(rhs, memAddress, entry->isVolatile());
  }

  lhsType = nullptr; // Reset nullptr

  // Return the variable name
  return lhsVarName;
}

std::any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  // Noop
  return nullptr;
}

std::any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);

  // Check if a value is attached to the return statement
  llvm::Value *returnValuePtr = nullptr;
  if (ctx->assignExpr()) {
    assert(returnVarEntry != nullptr);
    // Set the expected type of the value
    lhsType = getTypeForSymbolType(returnVarEntry->getType(), currentScope);
    // Visit return value
    returnValuePtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
  } else if (returnVarEntry != nullptr) { // Function. Procedures do not have a return variable
    returnValuePtr = returnVarEntry->getAddress();
  }

  // Set block to terminated
  blockAlreadyTerminated = true;

  // No return value
  if (returnValuePtr == nullptr) {
    builder->CreateRetVoid();
    return nullptr;
  }

  // Return value
  llvm::Value *returnValue = builder->CreateLoad(returnValuePtr->getType()->getPointerElementType(), returnValuePtr);
  builder->CreateRet(returnValue);
  return returnValuePtr;
}

std::any GeneratorVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  // Get number, how many loops we want to break
  int breakCount = 1;
  if (ctx->INTEGER())
    breakCount = std::stoi(ctx->INTEGER()->toString());

  // Get destination block
  for (int i = 1; i < breakCount; i++)
    breakBlocks.pop();

  // Jump to destination block
  createBr(breakBlocks.top());
  return nullptr;
}

std::any GeneratorVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  // Get number, how many loops we want to continue
  int continueCount = 1;
  if (ctx->INTEGER())
    continueCount = std::stoi(ctx->INTEGER()->toString());

  // Get destination block
  for (int i = 1; i < continueCount; i++)
    continueBlocks.pop();

  // Jump to destination block
  createBr(continueBlocks.top());
  return nullptr;
}

std::any GeneratorVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  if (ctx->printfCall())
    return visit(ctx->printfCall());
  if (ctx->sizeOfCall())
    return visit(ctx->sizeOfCall());
  if (ctx->lenCall())
    return visit(ctx->lenCall());
  if (ctx->tidCall())
    return visit(ctx->tidCall());
  if (ctx->joinCall())
    return visit(ctx->joinCall());
  throw std::runtime_error("Internal compiler error: Could not find builtin function"); // GCOV_EXCL_LINE
}

std::any GeneratorVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  // Declare if not declared already
  llvm::Function *printfFct = retrievePrintfFct();

  std::vector<llvm::Value *> printfArgs;
  std::string stringTemplate = ctx->STRING_LITERAL()->toString();
  stringTemplate = std::regex_replace(stringTemplate, std::regex("\\\\n"), "\n");
  stringTemplate = std::regex_replace(stringTemplate, std::regex("\\\\a"), "\a");
  stringTemplate = stringTemplate.substr(1, stringTemplate.size() - 2);
  printfArgs.push_back(builder->CreateGlobalStringPtr(stringTemplate));
  for (const auto &arg : ctx->assignExpr()) {
    // Visit argument
    auto argValPtr = any_cast<llvm::Value *>(visit(arg));

    llvm::Value *argVal;
    if (argValPtr->getType()->getPointerElementType()->isArrayTy()) { // Convert array type to pointer type
      llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(0)};
      llvm::Type *targetType = argValPtr->getType()->getPointerElementType();
      argVal = builder->CreateInBoundsGEP(targetType, argValPtr, indices);
    } else {
      argVal = builder->CreateLoad(argValPtr->getType()->getPointerElementType(), argValPtr);
    }

    if (argVal == nullptr)
      throw err->get(*arg->start, PRINTF_NULL_TYPE, "'" + arg->getText() + "' is null");

    // Cast all integer types to 32 bit
    if (argVal->getType()->isIntegerTy(1) || argVal->getType()->isIntegerTy(8) || argVal->getType()->isIntegerTy(16))
      argVal = builder->CreateZExtOrTrunc(argVal, llvm::Type::getInt32Ty(*context));

    printfArgs.push_back(argVal);
  }
  return static_cast<llvm::Value *>(builder->CreateCall(printfFct, printfArgs));
}

std::any GeneratorVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  unsigned int size;
  if (ctx->assignExpr()) { // Assign expression
    // Visit the argument
    auto valuePtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
    llvm::Value *value = builder->CreateLoad(valuePtr->getType()->getPointerElementType(), valuePtr);
    llvm::Type *valueTy = value->getType();

    // Calculate size at compile-time
    size = getSizeOfType(valueTy);
  } else { // Type
    auto llvmType = std::any_cast<llvm::Type *>(visit(ctx->dataType()));
    size = llvmType->getScalarSizeInBits();
  }

  // Save size
  llvm::Value *resultPtr = insertAlloca(builder->getInt32Ty());
  builder->CreateStore(builder->getInt32(size), resultPtr);

  // Return address to where the size is saved
  return resultPtr;
}

std::any GeneratorVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) {
  // Visit the argument
  auto valuePtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
  llvm::Value *value = builder->CreateLoad(valuePtr->getType()->getPointerElementType(), valuePtr);

  // Get array size
  unsigned int size = value->getType()->getArrayNumElements();

  // Store the result
  llvm::Value *resultPtr = insertAlloca(builder->getInt32Ty());
  builder->CreateStore(builder->getInt32(size), resultPtr);

  // Return address to where the size is saved
  return resultPtr;
}

std::any GeneratorVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) {
  // Declare if not declared already
  std::string psFctName = "pthread_self";
  llvm::Function *psFct = module->getFunction(psFctName);
  if (!psFct) {
    llvm::FunctionType *psFctTy = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(*context), {}, false);
    module->getOrInsertFunction(psFctName, psFctTy);
    psFct = module->getFunction(psFctName);
  }

  // Create call to function
  llvm::Value *result = builder->CreateCall(psFct);
  result = builder->CreatePtrToInt(result, builder->getInt32Ty());

  // Store the result
  llvm::Value *resultPtr = insertAlloca(result->getType());
  builder->CreateStore(result, resultPtr);

  // Return address to where the tid is saved
  return resultPtr;
}

std::any GeneratorVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) {
  // Declare if not declared already
  std::string pjFctName = "pthread_join";
  llvm::Function *pjFct = module->getFunction(pjFctName);
  if (!pjFct) {
    llvm::FunctionType *pjFctTy =
        llvm::FunctionType::get(builder->getInt32Ty(), {builder->getInt8PtrTy(), builder->getInt8PtrTy()->getPointerTo()}, false);
    module->getOrInsertFunction(pjFctName, pjFctTy);
    pjFct = module->getFunction(pjFctName);
  }

  unsigned int joinCount = 0;
  for (const auto &assignExpr : ctx->assignExpr()) {
    // Check if it is an id or an array of ids
    auto threadIdPtr = any_cast<llvm::Value *>(visit(assignExpr));
    assert(threadIdPtr != nullptr && threadIdPtr->getType()->isPointerTy());
    llvm::Type *threadIdPtrTy = threadIdPtr->getType()->getPointerElementType();
    if (threadIdPtr->getType()->getPointerElementType()->isArrayTy()) { // Array of ids
      for (int i = 0; i < threadIdPtrTy->getArrayNumElements(); i++) {
        // Get thread id that has to be joined
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(i)};
        llvm::Value *threadIdAddr = builder->CreateGEP(threadIdPtrTy, threadIdPtr, indices);
        llvm::Value *threadId = builder->CreateLoad(threadIdAddr->getType()->getPointerElementType(), threadIdAddr);

        // Create call to pthread_join
        llvm::Value *voidPtrPtrNull = llvm::Constant::getNullValue(llvm::Type::getInt8PtrTy(*context)->getPointerTo());
        builder->CreateCall(pjFct, {threadId, voidPtrPtrNull});

        joinCount++;
      }
    } else { // Single id
      // Get thread id that has to be joined
      llvm::Value *threadId = builder->CreateLoad(threadIdPtrTy, threadIdPtr);

      // Create call to pthread_join
      llvm::Value *voidPtrPtrNull = llvm::Constant::getNullValue(llvm::Type::getInt8PtrTy(*context)->getPointerTo());
      builder->CreateCall(pjFct, {threadId, voidPtrPtrNull});

      joinCount++;
    }
  }

  // Return the number of threads that were joined
  llvm::Value *result = builder->getInt32(joinCount);
  llvm::Value *resultPtr = insertAlloca(result->getType());
  builder->CreateStore(result, resultPtr);
  return resultPtr;
}

std::any GeneratorVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  // Check if there is an assign operator applied
  if (ctx->assignOp()) { // This is an assignment or compound assignment
    lhsType = nullptr;   // Reset lhs type

    // Get value of right side
    auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->assignExpr()));
    llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

    // Visit the left side
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->prefixUnaryExpr()));
    lhsVarName = currentVarName;

    // Take a look at the operator
    if (ctx->assignOp()->ASSIGN()) { // Simple assign
      builder->CreateStore(rhs, lhsPtr);
    } else { // Compound assign
      // Get symbol table entry
      SymbolTableEntry *variableEntry = currentScope->lookup(lhsVarName);
      assert(variableEntry != nullptr);

      // Get value of left side
      llvm::Value *lhs;
      if (variableEntry->isGlobal()) {
        lhs = module->getNamedGlobal(lhsVarName);
      } else {
        lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
      }

      // Decide what to do, based on the operator
      if (ctx->assignOp()->PLUS_EQUAL()) {
        rhs = conversionsManager->getPlusEqualInst(lhs, rhs, *ctx->assignOp()->PLUS_EQUAL()->getSymbol());
      } else if (ctx->assignOp()->MINUS_EQUAL()) {
        rhs = conversionsManager->getMinusEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->MUL_EQUAL()) {
        rhs = conversionsManager->getMulEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->DIV_EQUAL()) {
        rhs = conversionsManager->getDivEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->REM_EQUAL()) {
        rhs = conversionsManager->getRemEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->SHL_EQUAL()) {
        rhs = conversionsManager->getSHLEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->SHR_EQUAL()) {
        rhs = conversionsManager->getSHREqualInst(lhs, rhs);
      } else if (ctx->assignOp()->AND_EQUAL()) {
        rhs = conversionsManager->getAndEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->OR_EQUAL()) {
        rhs = conversionsManager->getOrEqualInst(lhs, rhs);
      } else if (ctx->assignOp()->XOR_EQUAL()) {
        rhs = conversionsManager->getXorEqualInst(lhs, rhs);
      }
      builder->CreateStore(rhs, lhsPtr, variableEntry->isVolatile());
    }
    return lhsPtr;
  } else if (ctx->ternaryExpr()) {
    std::any rhs = visit(ctx->ternaryExpr());
    lhsType = nullptr; // Reset lhs type
    return rhs;
  } else if (ctx->threadDef()) {
    std::any rhs = visit(ctx->threadDef());
    lhsType = nullptr; // Reset lhs type
    return rhs;
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any GeneratorVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  if (ctx->logicalOrExpr().size() > 1) {
    auto conditionPtr = any_cast<llvm::Value *>(visit(ctx->logicalOrExpr()[0]));
    llvm::Value *condition = builder->CreateLoad(conditionPtr->getType()->getPointerElementType(), conditionPtr);
    llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock *bThen = llvm::BasicBlock::Create(*context, "tern.then");
    llvm::BasicBlock *bElse = llvm::BasicBlock::Create(*context, "tern.else");
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "tern.end");

    // Conditional jump to respective block
    createCondBr(condition, bThen, bElse);

    // Fill then block
    parentFct->getBasicBlockList().push_back(bThen);
    moveInsertPointToBlock(bThen);
    auto thenValuePtr = any_cast<llvm::Value *>(visit(ctx->logicalOrExpr()[1]));
    llvm::Value *thenValue = builder->CreateLoad(thenValuePtr->getType()->getPointerElementType(), thenValuePtr);
    createBr(bEnd);

    // Fill else block
    parentFct->getBasicBlockList().push_back(bElse);
    moveInsertPointToBlock(bElse);
    auto elseValuePtr = any_cast<llvm::Value *>(visit(ctx->logicalOrExpr()[2]));
    llvm::Value *elseValue = builder->CreateLoad(elseValuePtr->getType()->getPointerElementType(), elseValuePtr);
    createBr(bEnd);

    // Fill end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);

    // Setup phi value
    llvm::PHINode *phi = builder->CreatePHI(thenValue->getType(), 2, "phi");
    phi->addIncoming(thenValue, bThen);
    phi->addIncoming(elseValue, bElse);

    llvm::Value *resultPtr = insertAlloca(phi->getType());
    builder->CreateStore(phi, resultPtr);
    return resultPtr;
  }
  return visit(ctx->logicalOrExpr()[0]);
}

std::any GeneratorVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  if (ctx->logicalAndExpr().size() > 1) {
    // Prepare for short-circuiting
    std::pair<llvm::Value *, llvm::BasicBlock *> incomingBlocks[ctx->logicalAndExpr().size()];
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "lor.end");
    llvm::Function *parentFunction = builder->GetInsertBlock()->getParent();

    // Visit the first condition
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->logicalAndExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

    // Prepare the blocks
    incomingBlocks[0] = {lhs, builder->GetInsertBlock()};
    for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "lor." + std::to_string(i));
      parentFunction->getBasicBlockList().push_back(bb);
      incomingBlocks[i] = {nullptr, bb};
    }
    createCondBr(lhs, bEnd, incomingBlocks[1].second);

    // Create a block for every other condition
    for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
      moveInsertPointToBlock(incomingBlocks[i].second);
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->logicalAndExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
      incomingBlocks[i].first = rhs;
      incomingBlocks[i].second = builder->GetInsertBlock();
      if (i < ctx->logicalAndExpr().size() - 1) {
        createCondBr(rhs, bEnd, incomingBlocks[i + 1].second);
      } else {
        createBr(bEnd);
      }
    }

    // Get the result with the phi node
    parentFunction->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);
    llvm::PHINode *phi = builder->CreatePHI(lhs->getType(), ctx->logicalAndExpr().size(), "lor_phi");
    for (const auto &incomingBlock : incomingBlocks)
      phi->addIncoming(incomingBlock.first, incomingBlock.second);

    // Store the result
    llvm::Value *resultPtr = insertAlloca(phi->getType());
    builder->CreateStore(phi, resultPtr);
    return resultPtr;
  }
  return visit(ctx->logicalAndExpr()[0]);
}

std::any GeneratorVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  if (ctx->bitwiseOrExpr().size() > 1) {
    // Prepare for short-circuiting
    std::pair<llvm::Value *, llvm::BasicBlock *> incomingBlocks[ctx->bitwiseOrExpr().size()];
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "land.end");
    llvm::Function *parentFunction = builder->GetInsertBlock()->getParent();

    // Visit the first condition
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->bitwiseOrExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

    // Prepare the blocks
    incomingBlocks[0] = {lhs, builder->GetInsertBlock()};
    for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "land." + std::to_string(i));
      parentFunction->getBasicBlockList().push_back(bb);
      incomingBlocks[i] = {nullptr, bb};
    }
    createCondBr(lhs, incomingBlocks[1].second, bEnd);

    // Create a block for every other condition
    for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
      moveInsertPointToBlock(incomingBlocks[i].second);
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->bitwiseOrExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
      incomingBlocks[i].first = rhs;
      incomingBlocks[i].second = builder->GetInsertBlock();
      if (i < ctx->bitwiseOrExpr().size() - 1) {
        createCondBr(rhs, incomingBlocks[i + 1].second, bEnd);
      } else {
        createBr(bEnd);
      }
    }

    // Get the result with the phi node
    parentFunction->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);
    llvm::PHINode *phi = builder->CreatePHI(lhs->getType(), ctx->bitwiseOrExpr().size(), "land_phi");
    for (const auto &incomingBlock : incomingBlocks)
      phi->addIncoming(incomingBlock.first, incomingBlock.second);

    // Store the result
    llvm::Value *resultPtr = insertAlloca(phi->getType());
    builder->CreateStore(phi, resultPtr);
    return resultPtr;
  }
  return visit(ctx->bitwiseOrExpr()[0]);
}

std::any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  if (ctx->bitwiseXorExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->bitwiseXorExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    for (int i = 1; i < ctx->bitwiseXorExpr().size(); i++) {
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->bitwiseXorExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
      lhs = conversionsManager->getBitwiseOrInst(lhs, rhs);
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(ctx->bitwiseXorExpr()[0]);
}

std::any GeneratorVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  if (ctx->bitwiseAndExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->bitwiseAndExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->bitwiseAndExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
      lhs = conversionsManager->getBitwiseXorInst(lhs, rhs);
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(ctx->bitwiseAndExpr()[0]);
}

std::any GeneratorVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  if (ctx->equalityExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->equalityExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    for (int i = 1; i < ctx->equalityExpr().size(); i++) {
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->equalityExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
      lhs = conversionsManager->getBitwiseAndInst(lhs, rhs);
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(ctx->equalityExpr()[0]);
}

std::any GeneratorVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  if (ctx->relationalExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->relationalExpr()[0]));
    auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->relationalExpr()[1]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

    if (ctx->EQUAL()) { // Equality expr is: relationalExpr EQUAL relationalExpr
      llvm::Value *result = conversionsManager->getEqualInst(lhs, rhs, *ctx->EQUAL()->getSymbol());
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    } else if (ctx->NOT_EQUAL()) { // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
      llvm::Value *result = conversionsManager->getNotEqualInst(lhs, rhs, *ctx->NOT_EQUAL()->getSymbol());
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    }
  }
  return visit(ctx->relationalExpr()[0]);
}

std::any GeneratorVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  if (ctx->shiftExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->shiftExpr()[0]));
    auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->shiftExpr()[1]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

    if (ctx->LESS()) { // Relational expr is: shiftExpr LESS shiftExpr
      llvm::Value *result = conversionsManager->getLessInst(lhs, rhs);
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    } else if (ctx->GREATER()) { // Relational expr is: shiftExpr GREATER shiftExpr
      llvm::Value *result = conversionsManager->getGreaterInst(lhs, rhs);
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    } else if (ctx->LESS_EQUAL()) { // Relational expr is: shiftExpr LESS_EQUAL shiftExpr
      llvm::Value *result = conversionsManager->getLessEqualInst(lhs, rhs);
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    } else if (ctx->GREATER_EQUAL()) { // Relational expr is: shiftExpr GREATER_EQUAL shiftExpr
      llvm::Value *result = conversionsManager->getGreaterEqualInst(lhs, rhs);
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    }
  }
  return visit(ctx->shiftExpr()[0]);
}

std::any GeneratorVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  // Check if there is a shift operation attached
  if (ctx->additiveExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->additiveExpr()[0]));
    auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->additiveExpr()[1]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

    if (ctx->SHL()) { // Shift expr is: additiveExpr SHL additiveExpr
      llvm::Value *result = conversionsManager->getShiftLeftInst(lhs, rhs);
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    } else if (ctx->SHR()) { // Shift expr is: additiveExpr SHR additiveExpr
      llvm::Value *result = conversionsManager->getShiftRightInst(lhs, rhs);
      llvm::Value *resultPtr = insertAlloca(result->getType());
      builder->CreateStore(result, resultPtr);
      return resultPtr;
    }
  }
  return visit(ctx->additiveExpr()[0]);
}

std::any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  // Check if at least one additive operator is applied
  if (ctx->multiplicativeExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->multiplicativeExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    unsigned int operatorIndex = 1;
    for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
      auto op = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[operatorIndex]);
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->multiplicativeExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

      if (op->getSymbol()->getType() == SpiceParser::PLUS)
        lhs = conversionsManager->getPlusInst(lhs, rhs, *op->getSymbol());
      else if (op->getSymbol()->getType() == SpiceParser::MINUS)
        lhs = conversionsManager->getMinusInst(lhs, rhs);

      operatorIndex += 2;
    }

    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(ctx->multiplicativeExpr()[0]);
}

std::any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  // Check if at least one multiplicative operator is applied
  if (ctx->castExpr().size() > 1) {
    auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->castExpr()[0]));
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
    unsigned int operatorIndex = 1;
    for (int i = 1; i < ctx->castExpr().size(); i++) {
      auto op = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[operatorIndex]);
      auto rhsPtr = any_cast<llvm::Value *>(visit(ctx->castExpr()[i]));
      llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

      if (op->getSymbol()->getType() == SpiceParser::MUL)
        lhs = conversionsManager->getMulInst(lhs, rhs, *op->getSymbol());
      else if (op->getSymbol()->getType() == SpiceParser::DIV)
        lhs = conversionsManager->getDivInst(lhs, rhs);
      else if (op->getSymbol()->getType() == SpiceParser::REM)
        lhs = conversionsManager->getRemInst(lhs, rhs);

      operatorIndex += 2;
    }

    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(ctx->castExpr()[0]);
}

std::any GeneratorVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  auto value = visit(ctx->prefixUnaryExpr());

  if (ctx->LPAREN()) { // Cast operator is applied
    auto dstTy = any_cast<llvm::Type *>(visit(ctx->dataType()));
    auto rhsPtr = any_cast<llvm::Value *>(value);
    llvm::Value *rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
    llvm::Value *result = conversionsManager->getCastInst(dstTy, rhs);
    llvm::Value *resultPtr = insertAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
  }

  return value;
}

std::any GeneratorVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  currentVarName = "";         // Reset the current variable name
  scopePrefix = "";            // Reset the scope prefix
  scopePath.clear();           // Clear the scope path
  structAccessIndices.clear(); // Clear struct access indices

  auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->postfixUnaryExpr()));

  if (!ctx->prefixUnaryOp().empty()) {
    // Load the value
    llvm::Value *lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

    bool isVolatile = false;
    unsigned int tokenCounter = 0;
    while (tokenCounter < ctx->children.size() - 1) {
      auto token = dynamic_cast<SpiceParser::PrefixUnaryOpContext *>(ctx->children[tokenCounter]);

      // Insert conversion instructions depending on the used operator
      if (token->MINUS()) { // Consider - operator
        lhs = conversionsManager->getPrefixMinusInst(lhs);
      } else if (token->PLUS_PLUS()) { // Consider ++ operator
        lhs = conversionsManager->getPrefixPlusPlusInst(lhs);

        // Store the new value volatile
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        if (!isVolatile && lhsVarEntry && lhsVarEntry->isVolatile())
          isVolatile = true;
      } else if (token->MINUS_MINUS()) { // Consider -- operator
        lhs = conversionsManager->getPrefixMinusMinusInst(lhs);

        // Store the new value volatile
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        if (!isVolatile && lhsVarEntry && lhsVarEntry->isVolatile())
          isVolatile = true;
      } else if (token->NOT()) { // Consider ! operator
        lhs = conversionsManager->getPrefixNotInst(lhs);
      } else if (token->BITWISE_NOT()) { // Consider ~ operator
        lhs = conversionsManager->getPrefixBitwiseNotInst(lhs);
      } else if (token->MUL()) { // Consider * operator
        // Indirect pointer
        lhsPtr = lhs;
        lhs = builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
      } else if (token->BITWISE_AND()) { // Consider & operator
        // Create a reference
        lhs = lhsPtr;
        lhsPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, lhsPtr);
      } else if (token->LOGICAL_AND()) { // Consider doubled & operator
        // First reference
        lhs = lhsPtr;
        lhsPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, lhsPtr);
        // Second reference
        lhs = lhsPtr;
        lhsPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, lhsPtr);
      }
      tokenCounter++;
    }

    // Store the value back again
    builder->CreateStore(lhs, lhsPtr, isVolatile);
  }

  return lhsPtr;
}

std::any GeneratorVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto lhsPtr = any_cast<llvm::Value *>(visit(ctx->atomicExpr()));

  if (ctx->children.size() > 1) {
    // Load the value
    llvm::Value *lhs = nullptr;
    if (lhsPtr)
      lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

    unsigned int tokenCounter = 1;
    while (tokenCounter < ctx->children.size()) {
      auto token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
      assert(token != nullptr);
      unsigned long long symbolType = token->getSymbol()->getType();
      if (symbolType == SpiceParser::LBRACKET) { // Consider subscript operator
        tokenCounter++;                          // Consume LBRACKET

        std::string arrayName = currentVarName; // Save array name
        ScopePath scopePathBackup = scopePath;  // Save scope path
        scopePrefix += "[idx]";

        // Get the index value
        auto assignExpr = dynamic_cast<SpiceParser::AssignExprContext *>(ctx->children[tokenCounter]);
        auto indexValue = any_cast<llvm::Value *>(visit(assignExpr));
        indexValue = builder->CreateLoad(indexValue->getType()->getPointerElementType(), indexValue);
        tokenCounter++; // Consume assignExpr

        // Get array item
        if (lhsPtr->getType()->getPointerElementType()->isArrayTy()) {
          llvm::Value *indices[2] = {builder->getInt32(0), indexValue};
          lhsPtr = builder->CreateInBoundsGEP(lhsPtr->getType()->getPointerElementType(), lhsPtr, indices);
        } else {
          if (lhs == nullptr)
            lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
          lhsPtr = builder->CreateInBoundsGEP(lhs->getType()->getPointerElementType(), lhs, indexValue);
        }

        currentVarName = arrayName;  // Restore current var name
        scopePath = scopePathBackup; // Restore scope path

        lhs = nullptr;                             // Set lhs to nullptr to prevent a store
      } else if (symbolType == SpiceParser::DOT) { // Consider member access
        tokenCounter++;                            // Consume dot
        scopePrefix += ".";
        auto postfixUnary = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(ctx->children[tokenCounter]);
        lhsPtr = any_cast<llvm::Value *>(visit(postfixUnary));
        lhs = nullptr;
      } else if (symbolType == SpiceParser::PLUS_PLUS) { // Consider ++ operator
        assert(lhs != nullptr);
        // Get the lhs variable entry
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        // Save the new value to the old pointer
        llvm::Value *newLhsValue = conversionsManager->getPostfixPlusPlusInst(lhs);
        builder->CreateStore(newLhsValue, lhsPtr, lhsVarEntry && lhsVarEntry->isVolatile());
        // Allocate new space and continue working with the new memory slot
        lhsPtr = insertAlloca(lhs->getType());
      } else if (symbolType == SpiceParser::MINUS_MINUS) { // Consider -- operator
        assert(lhs != nullptr);
        // Get the lhs variable entry
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        // Save the new value to the old pointer
        llvm::Value *newLhsValue = conversionsManager->getPostfixMinusMinusInst(lhs);
        builder->CreateStore(newLhsValue, lhsPtr, lhsVarEntry && lhsVarEntry->isVolatile());
        // Allocate new space and continue working with the new memory slot
        lhsPtr = insertAlloca(lhs->getType());
      }
      tokenCounter++;
    }

    // Store the value back again
    if (lhs != nullptr)
      builder->CreateStore(lhs, lhsPtr);
  }

  return lhsPtr;
}

std::any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  if (ctx->value())
    return visit(ctx->value());
  allArgsHardcoded = false; // To prevent arrays from being defined globally when depending on other values (vars, calls, etc.)
  if (ctx->IDENTIFIER()) {
    currentVarName = ctx->IDENTIFIER()->toString();
    scopePrefix += currentVarName;

    // Load symbol table entry
    SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
    SymbolTableEntry *entry = accessScope->lookup(currentVarName);

    if (!entry)
      return static_cast<llvm::Value *>(nullptr);

    // Check if this an external global var
    if (accessScope->isImported(currentScope) && entry->isGlobal())
      entry = initExtGlobal(currentVarName, scopePrefix);

    llvm::Value *memAddress = entry->getAddress();
    if (entry->getType().isBaseType(TY_STRUCT)) { // If base type is a struct
      if (structAccessIndices.empty()) {          // No struct was seen before
        // Initialize GEP calculation
        structAccessIndices.push_back(builder->getInt32(0)); // To indirect pointer input of GEP
        // Set the access type and address
        structAccessType = entry->getLLVMType();
        structAccessAddress = entry->getAddress();
      } else { // This is a struct field in a struct
        // Just add the index to the index list
        unsigned int fieldIndex = entry->getOrderIndex();
        structAccessIndices.push_back(builder->getInt32(fieldIndex));
        SymbolType tmpType = entry->getType();
        while (tmpType.isPointer()) {
          // Execute GEP with the collected indices to indirect the pointer
          structAccessAddress = builder->CreateInBoundsGEP(structAccessType, structAccessAddress, structAccessIndices);
          // Load the value and store it as new address
          structAccessAddress = builder->CreateLoad(structAccessAddress->getType()->getPointerElementType(), structAccessAddress);
          // Set new struct access type
          SymbolTableEntry *nestedStructEntry = accessScope->lookup(entry->getType().getBaseType().getSubType());
          assert(nestedStructEntry != nullptr);
          structAccessType = nestedStructEntry->getLLVMType();
          // Reset index list
          structAccessIndices.clear();
          structAccessIndices.push_back(builder->getInt32(0));
          // Unpack symbol type
          tmpType = tmpType.getContainedTy();
        }
      }
    } else if (!structAccessIndices.empty()) { // A struct was met already, so this is a struct field
      unsigned int fieldIndex = entry->getOrderIndex();
      // Push field index to index list
      structAccessIndices.push_back(builder->getInt32(fieldIndex));
      // Unpack address
      while (structAccessType->isPointerTy()) {
        structAccessAddress = builder->CreateLoad(structAccessType, structAccessAddress);
        structAccessType = structAccessType->getPointerElementType();
      }
      // Execute GEP calculation
      memAddress = builder->CreateInBoundsGEP(structAccessType, structAccessAddress, structAccessIndices);
      // Clear index list
      structAccessIndices.clear();
    } else {
      // For the case that in the current scope there is a variable with the same name, but it is initialized later, so the
      // symbol above in the hierarchy is meant to be used.
      while (entry && !memAddress && accessScope->getParent() && !entry->getType().is(TY_IMPORT)) {
        accessScope = accessScope->getParent();
        entry = accessScope->lookup(currentVarName);
        if (!entry)
          break;
        memAddress = entry->getAddress();
      }
      assert(entry != nullptr);
    }

    // Retrieve scope for the new scope path fragment
    if (entry->getType().is(TY_IMPORT)) { // Import
      accessScope = accessScope->lookupTable(entry->getName());
    } else if (entry->getType().isBaseType(TY_STRUCT)) { // Struct
      std::string structSignature =
          Struct::getSignature(entry->getType().getBaseType().getSubType(), entry->getType().getBaseType().getTemplateTypes());
      accessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
    }
    assert(accessScope);

    // Otherwise, push the current scope to the scope path
    scopePath.pushFragment(currentVarName, accessScope);

    return memAddress;
  }
  if (ctx->builtinCall())
    return visit(ctx->builtinCall());
  return visit(ctx->assignExpr());
}

std::any GeneratorVisitor::visitValue(SpiceParser::ValueContext *ctx) {
  // Primitive value
  if (ctx->primitiveValue()) {
    // Visit the primitive value
    currentConstValue = any_cast<llvm::Constant *>(visit(ctx->primitiveValue()));

    // If global variable value, return value immediately, because it is already a pointer
    if (currentScope == rootScope)
      return currentConstValue;

    // Store the value to a tmp variable
    llvm::Value *llvmValuePtr = insertAlloca(currentConstValue->getType());
    builder->CreateStore(currentConstValue, llvmValuePtr);
    return llvmValuePtr;
  }

  // Function call
  if (ctx->functionCall())
    return visit(ctx->functionCall());

  // Array initialization
  if (ctx->arrayInitialization())
    return visit(ctx->arrayInitialization());

  // Struct instantiation
  if (ctx->structInstantiation())
    return visit(ctx->structInstantiation());

  if (ctx->NIL()) {
    auto nilType = any_cast<llvm::Type *>(visit(ctx->dataType()));
    currentConstValue = llvm::Constant::getNullValue(nilType);

    // If global variable value, return value immediately, because it is already a pointer
    if (currentScope == rootScope)
      return currentConstValue;

    // Store the value to a tmp variable
    llvm::Value *llvmValuePtr = insertAlloca(currentConstValue->getType());
    builder->CreateStore(currentConstValue, llvmValuePtr);
    return llvmValuePtr;
  }

  return nullptr;
}

std::any GeneratorVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Get function access pointer
  Function *spiceFunc = currentScope->getFunctionAccessPointer(*ctx->IDENTIFIER().back()->getSymbol());
  assert(spiceFunc != nullptr);
  bool isMethod = spiceFunc->isMethodFunction() || spiceFunc->isMethodProcedure();
  bool constructorCall = false;

  // Load the 'this' value if it is a pointer
  llvm::Value *thisValuePtr;
  for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
    std::string identifier = ctx->IDENTIFIER()[i]->toString();
    SymbolTableEntry *symbolEntry = accessScope->lookup(identifier);

    if (i < ctx->IDENTIFIER().size() - 1) {
      if (!symbolEntry)
        throw err->get(*ctx->IDENTIFIER()[i]->getSymbol(), REFERENCED_UNDEFINED_FUNCTION,
                       "Symbol '" + scopePath.getScopePrefix() + identifier + "' was used before defined");
      thisValuePtr = symbolEntry->getAddress();
    } else if (symbolEntry != nullptr && symbolEntry->getType().getBaseType().is(TY_STRUCT)) {
      Struct *spiceStruct = currentScope->getStructAccessPointer(*ctx->start);
      assert(spiceStruct != nullptr);

      // Check if the struct is defined
      symbolEntry = accessScope->lookup(spiceStruct->getSignature());
      assert(symbolEntry != nullptr);
      llvm::Type *structType = symbolEntry->getLLVMType();

      // Get address of variable in memory
      assert(!lhsVarName.empty());
      SymbolTableEntry *assignVarEntry = currentScope->lookup(lhsVarName);
      assert(assignVarEntry != nullptr);
      thisValuePtr = assignVarEntry->getAddress();

      // Get struct table
      SymbolTable *structTable = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + spiceStruct->getSignature());
      assert(structTable != nullptr);

      // Fill the struct with the stated values
      if (ctx->argLst()) {
        for (unsigned int i = 0; i < ctx->argLst()->assignExpr().size(); i++) {
          // Set address to the struct instance field
          SymbolTableEntry *fieldEntry = structTable->lookupByIndex(i);
          assert(fieldEntry);
          // Visit assignment
          auto assignmentPtr = any_cast<llvm::Value *>(visit(ctx->argLst()->assignExpr()[i]));
          llvm::Value *assignment = builder->CreateLoad(assignmentPtr->getType()->getPointerElementType(), assignmentPtr);
          // Get pointer to struct element
          llvm::Value *fieldAddress = builder->CreateStructGEP(structType, thisValuePtr, i);
          fieldEntry->updateAddress(fieldAddress);
          // Store value to address
          builder->CreateStore(assignment, fieldAddress);
        }
      }

      constructorCall = true;
    } else {
      continue;
    }

    std::string tableName =
        symbolEntry->getType().is(TY_IMPORT) ? identifier : STRUCT_SCOPE_PREFIX + symbolEntry->getType().getBaseType().getName();
    accessScope = accessScope->lookupTable(tableName);
    assert(accessScope != nullptr);
    scopePath.pushFragment(identifier, accessScope);
  }

  // Load this value ptr if necessary
  if (isMethod && thisValuePtr->getType()->getPointerElementType()->isPointerTy())
    thisValuePtr = builder->CreateLoad(thisValuePtr->getType()->getPointerElementType(), thisValuePtr);

  // Check if function exists in the current module
  bool functionFound = false;
  std::string fctIdentifier = spiceFunc->getMangledName();
  for (const auto &function : module->getFunctionList()) { // Search for function definition
    if (function.getName() == fctIdentifier) {
      functionFound = true;
      break;
    } else if (function.getName() == spiceFunc->getName()) {
      functionFound = true;
      fctIdentifier = spiceFunc->getName();
      break;
    }
  }

  if (!functionFound) { // Not found => Declare function, which will be linked in
    SymbolType returnSymbolType = spiceFunc->getReturnType();
    std::vector<SymbolType> argSymbolTypes = spiceFunc->getArgTypes();

    llvm::Type *returnType =
        returnSymbolType.is(TY_DYN) ? llvm::Type::getVoidTy(*context) : getTypeForSymbolType(returnSymbolType, accessScope);

    std::vector<llvm::Type *> argTypes;
    if (isMethod)
      argTypes.push_back(thisValuePtr->getType());
    for (auto &argSymbolType : argSymbolTypes)
      argTypes.push_back(getTypeForSymbolType(argSymbolType, accessScope));

    llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, argTypes, false);
    module->getOrInsertFunction(fctIdentifier, fctType);
  }

  // Get the declared function and its type
  llvm::Function *fct = module->getFunction(fctIdentifier);
  assert(fct != nullptr);
  llvm::FunctionType *fctType = fct->getFunctionType();

  // Fill argument list
  int argIndex = 0;
  std::vector<llvm::Value *> argValues;
  if (isMethod) { // If it is a method, pass 'this' as implicit first argument
    argValues.push_back(thisValuePtr);
    argIndex++;
  }
  if (ctx->argLst()) {
    for (const auto &arg : ctx->argLst()->assignExpr()) {
      // Get expected arg type
      llvm::Type *expectedArgType = fctType->getParamType(argIndex);
      // Get the actual arg value
      auto actualArgPtr = any_cast<llvm::Value *>(visit(arg));
      if (!compareLLVMTypes(actualArgPtr->getType()->getPointerElementType(), expectedArgType)) {
        argValues.push_back(doImplicitCast(actualArgPtr, expectedArgType));
      } else {
        argValues.push_back(builder->CreateLoad(actualArgPtr->getType()->getPointerElementType(), actualArgPtr));
      }
      argIndex++;
    }
  }

  // Create the function call
  llvm::Value *resultValue = builder->CreateCall(fct, argValues);

  // Consider constructor calls
  if (constructorCall) {
    return thisValuePtr;
  } else if (!resultValue->getType()->isSized()) {
    // Set return type bool for procedures
    resultValue = builder->getTrue();
  }

  llvm::Value *resultPtr = insertAlloca(resultValue->getType());
  builder->CreateStore(resultValue, resultPtr);
  return resultPtr;
}

std::any GeneratorVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  // Get data type
  size_t actualItemCount = ctx->argLst() ? ctx->argLst()->assignExpr().size() : 0;
  size_t arraySize = lhsType != nullptr && lhsType->isArrayTy() ? lhsType->getArrayNumElements() : actualItemCount;
  auto arrayType = static_cast<llvm::ArrayType *>(lhsType);

  // Fill items with the stated values
  if (ctx->argLst()) {
    // Visit all args to check if they are hardcoded or not
    std::vector<llvm::Value *> itemValuePointers;
    std::vector<llvm::Constant *> itemConstants;
    allArgsHardcoded = true;
    for (size_t i = 0; i < std::min(ctx->argLst()->assignExpr().size(), arraySize); i++) {
      auto itemValuePtr = any_cast<llvm::Value *>(visit(ctx->argLst()->assignExpr()[i]));
      itemValuePointers.push_back(itemValuePtr);
      itemConstants.push_back(currentConstValue);
    }

    llvm::Type *itemType;
    if (arrayType == nullptr) {
      itemType = itemValuePointers[0]->getType()->getPointerElementType();
      arrayType = llvm::ArrayType::get(itemType, arraySize);
    } else {
      itemType = arrayType->getArrayElementType();
    }

    // Decide if the array can be defined globally
    llvm::Value *arrayAddress;
    if (!itemType->isStructTy() && allArgsHardcoded) { // All args hardcoded => array can be defined globally
      // Fill up the rest of the items
      if (itemConstants.size() < arraySize) {
        llvm::Constant *constantValue = getDefaultValueForType(itemType);
        for (size_t i = itemConstants.size(); i < arraySize; i++)
          itemConstants.push_back(constantValue);
      }

      // Create hardcoded array
      llvm::Constant *constArray = llvm::ConstantArray::get(arrayType, itemConstants);
      // Create global variable
      std::string globalVarName = lhsVarName;
      if (globalVarName.empty()) { // Get unused anonymous global var name
        unsigned int suffixNumber = 0;
        do {
          globalVarName = "anonymous." + std::to_string(suffixNumber);
          suffixNumber++;
        } while (module->getNamedGlobal(globalVarName) != nullptr);
      }
      arrayAddress = module->getOrInsertGlobal(globalVarName, arrayType);
      // Set some attributes to it
      llvm::GlobalVariable *global = module->getNamedGlobal(globalVarName);
      global->setConstant(true);
      global->setInitializer(constArray);
    } else { // Some args are not hardcoded => fallback to individual indexing
      // Allocate array
      arrayAddress = insertAlloca(arrayType, currentVarName);

      // Insert all given values
      size_t valueIndex = 0;
      for (; valueIndex < std::min(ctx->argLst()->assignExpr().size(), arraySize); valueIndex++) {
        llvm::Value *itemValuePtr = itemValuePointers[valueIndex];
        llvm::Value *itemValue = builder->CreateLoad(itemValuePtr->getType()->getPointerElementType(), itemValuePtr);
        // Calculate item address
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(valueIndex)};
        llvm::Value *itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, indices);
        // Store value to item address
        builder->CreateStore(itemValue, itemAddress);
      }

      // Fill up the rest of the values with default values
      for (; valueIndex < arraySize; valueIndex++) {
        // ToDo: Fill up with default values
      }
    }
    return arrayAddress;
  }

  // Empty array: '{}'
  assert(arrayType != nullptr);
  // assert(!currentVarName.empty()); // Empty var names cause problems
  return insertAlloca(arrayType, currentVarName);
}

std::any GeneratorVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  // Get struct name in format a.b.c and struct scope
  std::string structName;
  SymbolTable *structScope = currentScope;
  for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
    std::string identifier = ctx->IDENTIFIER()[i]->toString();
    structName += structName.empty() ? identifier : "." + identifier;
    if (i < ctx->IDENTIFIER().size() - 1) {
      SymbolTableEntry *entry = structScope->lookup(identifier);
      if (!entry)
        throw err->get(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                       "Struct '" + structName + "' was used before defined");
      if (entry->getType().is(TY_IMPORT)) {
        structScope = structScope->lookupTable(identifier);
      } else if (entry->getType().is(TY_STRUCT)) {
        structScope = structScope->lookupTable(STRUCT_SCOPE_PREFIX + identifier);
      } else {
        throw err->get(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                       "The variable '" + identifier + "' is of type " + entry->getType().getName(false) +
                           ". Expected struct or import");
      }
    }
  }

  // Get struct from struct access pointer
  Struct *spiceStruct = currentScope->getStructAccessPointer(*ctx->start);
  assert(spiceStruct);

  // Check if the struct is defined
  SymbolTableEntry *structSymbol = structScope->lookup(spiceStruct->getSignature());
  assert(structSymbol);
  llvm::Type *structType = structSymbol->getLLVMType();

  // Allocate space for the struct in memory
  llvm::Value *structAddress = insertAlloca(structType);

  // Get struct table
  SymbolTable *structTable = structScope->lookupTable(STRUCT_SCOPE_PREFIX + spiceStruct->getSignature());
  assert(structTable);

  // Fill the struct with the stated values
  if (ctx->argLst()) {
    for (unsigned int i = 0; i < ctx->argLst()->assignExpr().size(); i++) {
      // Set address to the struct instance field
      SymbolTableEntry *fieldEntry = structTable->lookupByIndex(i);
      assert(fieldEntry);
      // Visit assignment
      auto assignmentPtr = any_cast<llvm::Value *>(visit(ctx->argLst()->assignExpr()[i]));
      llvm::Value *assignment = builder->CreateLoad(assignmentPtr->getType()->getPointerElementType(), assignmentPtr);
      // Get pointer to struct element
      llvm::Value *fieldAddress = builder->CreateStructGEP(structType, structAddress, i);
      fieldEntry->updateAddress(fieldAddress);
      // Store value to address
      builder->CreateStore(assignment, fieldAddress);
    }
  }

  return structAddress;
}

std::any GeneratorVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) {
  // Value is a double constant
  if (ctx->DOUBLE()) {
    currentSymbolType = SymbolType(TY_DOUBLE);
    double value = std::stod(ctx->DOUBLE()->toString());
    if (constNegate)
      value *= -1;
    return static_cast<llvm::Constant *>(llvm::ConstantFP::get(*context, llvm::APFloat(value)));
  }

  // Value is an integer constant
  if (ctx->INTEGER()) {
    currentSymbolType = SymbolType(TY_INT);
    int value = std::stoi(ctx->INTEGER()->toString());
    if (constNegate)
      value *= -1;
    if (currentVarSigned) {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), value));
    } else {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), value));
    }
  }

  // Value is a short constant
  if (ctx->SHORT()) {
    currentSymbolType = SymbolType(TY_SHORT);
    int value = std::stoi(ctx->SHORT()->toString());
    if (constNegate)
      value *= -1;
    if (currentVarSigned) {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::getSigned(llvm::Type::getInt16Ty(*context), value));
    } else {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::get(llvm::Type::getInt16Ty(*context), value));
    }
  }

  // Value is a long constant
  if (ctx->LONG()) {
    currentSymbolType = SymbolType(TY_LONG);
    long long value = std::stoll(ctx->LONG()->toString());
    if (constNegate)
      value = -value;
    if (currentVarSigned) {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::getSigned(llvm::Type::getInt64Ty(*context), value));
    } else {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), value));
    }
  }

  // Value is a char constant
  if (ctx->CHAR_LITERAL()) {
    currentSymbolType = SymbolType(TY_CHAR);
    char value = ctx->CHAR_LITERAL()->toString()[1];
    if (currentVarSigned) {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::getSigned(llvm::Type::getInt8Ty(*context), value));
    } else {
      return static_cast<llvm::Constant *>(llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), value));
    }
  }

  // Value is a string constant
  if (ctx->STRING_LITERAL()) {
    currentSymbolType = SymbolType(TY_STRING);
    std::string value = ctx->STRING_LITERAL()->toString();
    value = std::regex_replace(value, std::regex("\\\\n"), "\n");
    value = value.substr(1, value.size() - 2);
    return static_cast<llvm::Constant *>(builder->CreateGlobalStringPtr(value, "", 0, module.get()));
  }

  // Value is a boolean constant with value false
  if (ctx->FALSE()) {
    currentSymbolType = SymbolType(TY_BOOL);
    return static_cast<llvm::Constant *>(builder->getFalse());
  }

  // Value is a boolean constant with value true
  if (ctx->TRUE()) {
    currentSymbolType = SymbolType(TY_BOOL);
    return static_cast<llvm::Constant *>(builder->getTrue());
  }

  throw std::runtime_error("Internal compiler error: Primitive data type generator fall-through"); // GCOV_EXCL_LINE
}

std::any GeneratorVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) {
  currentSymbolType = any_cast<SymbolType>(visit(ctx->baseDataType()));

  size_t tokenCounter = 1;
  while (tokenCounter < ctx->children.size()) {
    auto token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
    if (token->getSymbol()->getType() == SpiceParser::MUL) { // Consider de-referencing operators
      currentSymbolType = currentSymbolType.toPointer(err.get(), *token->getSymbol());
    } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider array bracket pairs
      tokenCounter++;                                                    // Consume LBRACKET
      token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
      unsigned int size = 0;                                       // Default to 0 when no size is attached
      if (token->getSymbol()->getType() == SpiceParser::INTEGER) { // Size is attached
        size = std::stoi(token->toString());
        tokenCounter++; // Consume INTEGER
      }
      currentSymbolType = currentSymbolType.toArray(err.get(), *token->getSymbol(), size);
    }
    tokenCounter++;
  }

  // Come up with the LLVM type
  llvm::Type *type = getTypeForSymbolType(currentSymbolType, currentScope);
  // Throw an error if something went wrong.
  // This should technically never occur because of the semantic analysis
  if (!type)
    throw err->get(*ctx->baseDataType()->getStart(), UNEXPECTED_DYN_TYPE_IR, // GCOV_EXCL_LINE
                   "Internal compiler error: Dyn was other");                // GCOV_EXCL_LINE
  return type;
}

std::any GeneratorVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  if (ctx->TYPE_DOUBLE())
    return SymbolType(TY_DOUBLE);
  if (ctx->TYPE_INT())
    return SymbolType(TY_INT);
  if (ctx->TYPE_SHORT())
    return SymbolType(TY_SHORT);
  if (ctx->TYPE_LONG())
    return SymbolType(TY_LONG);
  if (ctx->TYPE_BYTE())
    return SymbolType(TY_BYTE);
  if (ctx->TYPE_CHAR())
    return SymbolType(TY_CHAR);
  if (ctx->TYPE_STRING())
    return SymbolType(TY_STRING);
  if (ctx->TYPE_BOOL())
    return SymbolType(TY_BOOL);
  if (ctx->TYPE_DYN()) { // Data type is type inferred
    assert(!currentVarName.empty());
    SymbolTableEntry *symbolTableEntry = currentScope->lookup(currentVarName);
    assert(symbolTableEntry != nullptr);
    return symbolTableEntry->getType();
  }
  if (ctx->customDataType()) // Struct type or generic type
    return visit(ctx->customDataType());
  throw std::runtime_error("Internal compiler error: Base datatype generator fall-through");
}

std::any GeneratorVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  // Get type name in format: a.b.c<d>
  std::string typeName = ctx->getText();

  // Search in symbol for a struct
  SymbolTableEntry *typeEntry = currentScope->lookup(typeName);
  if (typeEntry != nullptr)
    return SymbolType(TY_STRUCT, typeName);

  // Search in generic types
  GenericType *genericType = currentScope->lookupGenericType(typeName);
  assert(genericType);
  return SymbolType(genericType->getTypeChain());
}

void GeneratorVisitor::createBr(llvm::BasicBlock *targetBlock) {
  if (blockAlreadyTerminated)
    return;
  builder->CreateBr(targetBlock);
  blockAlreadyTerminated = true;
}

void GeneratorVisitor::createCondBr(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) {
  if (blockAlreadyTerminated)
    return;
  builder->CreateCondBr(condition, trueBlock, falseBlock);
  blockAlreadyTerminated = true;
}

void GeneratorVisitor::moveInsertPointToBlock(llvm::BasicBlock *block) {
  builder->SetInsertPoint(block);
  blockAlreadyTerminated = false;
}

llvm::Value *GeneratorVisitor::insertAlloca(llvm::Type *llvmType, const std::string &varName, llvm::Value *arraySize) {
  if (allocaInsertInst != nullptr) {
    llvm::AllocaInst *allocaInst = builder->CreateAlloca(llvmType, arraySize, varName);
    allocaInst->moveAfter(allocaInsertInst);
    allocaInsertInst = allocaInst;
  } else {
    // Save current basic block and move insert cursor to entry block of the current function
    llvm::BasicBlock *currentBlock = builder->GetInsertBlock();
    builder->SetInsertPoint(allocaInsertBlock);

    allocaInsertInst = builder->CreateAlloca(llvmType, arraySize, varName);

    // Restore old basic block
    builder->SetInsertPoint(currentBlock);
  }
  return static_cast<llvm::Value *>(allocaInsertInst);
}

llvm::Function *GeneratorVisitor::retrievePrintfFct() {
  std::string printfFctName = "printf";
  llvm::Function *printfFct = module->getFunction(printfFctName);
  if (printfFct)
    return printfFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *printfFctTy = llvm::FunctionType::get(builder->getInt32Ty(), builder->getInt8PtrTy(), true);
  module->getOrInsertFunction(printfFctName, printfFctTy);
  return module->getFunction(printfFctName);
}

llvm::Function *GeneratorVisitor::retrieveExitFct() {
  std::string exitFctName = "exit";
  llvm::Function *exitFct = module->getFunction(exitFctName);
  if (exitFct)
    return exitFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *exitFctTy = llvm::FunctionType::get(builder->getVoidTy(), builder->getInt32Ty(), false);
  module->getOrInsertFunction(exitFctName, exitFctTy);
  return module->getFunction(exitFctName);
}

llvm::Type *GeneratorVisitor::getTypeForSymbolType(SymbolType symbolType, SymbolTable *accessScope) {
  currentSymbolType = symbolType;

  // Get base symbol type
  std::stack<std::pair<SymbolSuperType, size_t>> pointerArrayList;
  while (symbolType.isPointer() || symbolType.isArray()) {
    SymbolSuperType superType = symbolType.isPointer() ? TY_PTR : TY_ARRAY;
    size_t arraySize = symbolType.isPointer() ? 0 : symbolType.getArraySize();
    pointerArrayList.push(std::make_pair(superType, arraySize));
    symbolType = symbolType.getContainedTy();
  }

  llvm::Type *llvmBaseType;
  switch (symbolType.getSuperType()) {
  case TY_DOUBLE: {
    llvmBaseType = llvm::Type::getDoubleTy(*context);
    break;
  }
  case TY_INT: {
    llvmBaseType = llvm::Type::getInt32Ty(*context);
    break;
  }
  case TY_SHORT: {
    llvmBaseType = llvm::Type::getInt16Ty(*context);
    break;
  }
  case TY_LONG: {
    llvmBaseType = llvm::Type::getInt64Ty(*context);
    break;
  }
  case TY_BYTE: // fallthrough
  case TY_CHAR: {
    llvmBaseType = llvm::Type::getInt8Ty(*context);
    break;
  }
  case TY_STRING: {
    llvmBaseType = llvm::Type::getInt8PtrTy(*context);
    break;
  }
  case TY_BOOL: {
    llvmBaseType = llvm::Type::getInt1Ty(*context);
    break;
  }
  case TY_STRUCT: {
    std::string structSignature = Struct::getSignature(symbolType.getSubType(), symbolType.getTemplateTypes());
    SymbolTableEntry *structSymbol = accessScope->lookup(structSignature);
    assert(structSymbol);
    llvmBaseType = structSymbol->getLLVMType();
    assert(llvmBaseType);
    break;
  }
  default:
    assert(!symbolType.is(TY_GENERIC));
    throw std::runtime_error("Internal compiler error: Cannot determine LLVM type of " + symbolType.getName(true));
  }

  // Consider pointer/array hierarchy
  while (!pointerArrayList.empty()) {
    if (pointerArrayList.top().first == TY_PTR || pointerArrayList.top().second == 0) { // Pointer
      llvmBaseType = llvmBaseType->getPointerTo();
    } else { // Otherwise, use the array type with a fixed size
      llvmBaseType = llvm::ArrayType::get(llvmBaseType, pointerArrayList.top().second);
    }
    pointerArrayList.pop();
  }

  return llvmBaseType;
}

llvm::Constant *GeneratorVisitor::getDefaultValueForType(llvm::Type *type) {
  // Double
  if (OpRuleConversionsManager::isDouble(type))
    return llvm::ConstantFP::get(*context, llvm::APFloat(0.0));

  // Int
  if (OpRuleConversionsManager::isInt(type))
    return builder->getInt32(0);

  // Short
  if (OpRuleConversionsManager::isShort(type))
    return builder->getInt16(0);

  // Long
  if (OpRuleConversionsManager::isLong(type))
    return builder->getInt64(0);

  // Byte or char
  if (OpRuleConversionsManager::isByteOrChar(type))
    return builder->getInt8(0);

  // String
  if (OpRuleConversionsManager::isString(type))
    return builder->CreateGlobalStringPtr("", "", 0, module.get());

  // Bool
  if (OpRuleConversionsManager::isBool(type))
    return builder->getInt1(false);

  throw std::runtime_error("Internal compiler error: Cannot determine default value for type"); // GCOV_EXCL_LINE
}

SymbolTableEntry *GeneratorVisitor::initExtGlobal(const std::string &globalName, const std::string &fqGlobalName) {
  Capture *capture = currentScope->lookupCapture(fqGlobalName);
  SymbolTableEntry *global = capture->getEntry();

  // Check if the entry is already initialized
  if (capture->getState() == INITIALIZED)
    return global;

  // Declare the global also in the current module and update the address of the symbol accordingly
  llvm::Value *memAddress = module->getOrInsertGlobal(globalName, global->getLLVMType());
  global->updateAddress(memAddress);

  return global;
}

bool GeneratorVisitor::compareLLVMTypes(llvm::Type *lhs, llvm::Type *rhs) {
  if (lhs->getTypeID() != rhs->getTypeID())
    return false;
  if (lhs->getTypeID() == llvm::Type::PointerTyID)
    return compareLLVMTypes(lhs->getPointerElementType(), rhs->getPointerElementType());
  if (lhs->getTypeID() == llvm::Type::ArrayTyID)
    return compareLLVMTypes(lhs->getArrayElementType(), rhs->getArrayElementType());
  return true;
}

unsigned int GeneratorVisitor::getSizeOfType(llvm::Type *llvmType) {
  if (llvmType->isArrayTy()) {
    return llvmType->getArrayNumElements() * llvmType->getArrayElementType()->getScalarSizeInBits();
  } else if (llvmType->isStructTy()) {
    unsigned int size = 0;
    for (int i = 0; i < llvmType->getNumContainedTypes(); ++i) {
      llvm::Type *containedType = llvmType->getContainedType(i);
      size += getSizeOfType(containedType);
    }
    return size;
  } else if (llvmType->isPointerTy()) {
    return module->getDataLayout().getPointerSizeInBits();
  }
  return llvmType->getScalarSizeInBits();
}

llvm::Value *GeneratorVisitor::doImplicitCast(llvm::Value *srcValue, llvm::Type *dstType) {
  // Unpack the pointers until a pointer of another type is met
  unsigned int loadCounter = 0;
  while (srcValue->getType()->getPointerElementType()->isPointerTy()) {
    srcValue = builder->CreateLoad(srcValue->getType()->getPointerElementType(), srcValue);
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstType->isPointerTy() && srcValue->getType()->getPointerElementType()->isArrayTy()) {
    llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(0)};
    llvm::Type *actualArgType = srcValue->getType()->getPointerElementType();
    srcValue = builder->CreateInBoundsGEP(actualArgType, srcValue, indices);
  } else {
    llvm::Type *actualArgType = srcValue->getType()->getPointerElementType();
    srcValue = builder->CreateLoad(actualArgType, srcValue);
    srcValue = builder->CreateBitCast(srcValue, dstType);
  }
  // Pack the pointers together again
  for (; loadCounter > 0; loadCounter--) {
    llvm::Value *newActualArg = insertAlloca(srcValue->getType());
    builder->CreateStore(srcValue, newActualArg);
    srcValue = newActualArg;
  }
  return srcValue;
}

llvm::OptimizationLevel GeneratorVisitor::getLLVMOptLevelFromSpiceOptLevel() const {
  switch (cliOptions.optLevel) { // Get LLVM opt level from Spice opt level
  case 1:
    return llvm::OptimizationLevel::O1;
  case 2:
    return llvm::OptimizationLevel::O2;
  case 3:
    return llvm::OptimizationLevel::O3;
  case 4:
    return llvm::OptimizationLevel::Os;
  case 5:
    return llvm::OptimizationLevel::Oz;
  default:
    return llvm::OptimizationLevel::O0;
  }
}
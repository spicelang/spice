// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

#include <stdexcept>

#include <analyzer/AnalyzerVisitor.h>
#include <cli/CliInterface.h>
#include <dependency/RuntimeModuleManager.h>
#include <dependency/SourceFile.h>
#include <exception/IRError.h>
#include <symbol/Function.h>
#include <symbol/Struct.h>
#include <symbol/SymbolTable.h>
#include <util/FileUtil.h>
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

GeneratorVisitor::GeneratorVisitor(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, ThreadFactory &threadFactory,
                                   const LinkerInterface &linker, const CliOptions &cliOptions, SourceFile &sourceFile,
                                   const std::string &objectFile)
    : objectFile(objectFile), sourceFile(sourceFile), cliOptions(cliOptions), linker(linker), context(context), builder(builder),
      threadFactory(threadFactory) {
  // Enrich options
  this->requiresMainFct = sourceFile.parent == nullptr;
  this->currentScope = this->rootScope = sourceFile.symbolTable.get();

  // Create LLVM base components
  module = std::make_unique<llvm::Module>(FileUtil::getFileName(sourceFile.filePath), *context);

  // Initialize generator helper objects
  stdFunctionManager = std::make_unique<StdFunctionManager>(this);
  conversionsManager = std::make_unique<OpRuleConversionsManager>(this);

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
    throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

  llvm::TargetOptions opt;
  llvm::Optional rm = llvm::Optional<llvm::Reloc::Model>();
  targetMachine = target->createTargetMachine(cliOptions.targetTriple, "generic", "", opt, rm);

  module->setDataLayout(targetMachine->createDataLayout());

  // Initialize debug info generator
  if (cliOptions.generateDebugInfo) {
    module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
    module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", llvm::dwarf::DWARF_VERSION);
    module->addModuleFlag(llvm::Module::Error, "PIC Level", llvm::PICLevel::BigPIC);

    auto identifierMetadata = module->getOrInsertNamedMetadata("llvm.ident");
    llvm::MDNode *n = llvm::MDNode::get(*context, llvm::MDString::get(*context, "spice version " + std::string(SPICE_VERSION)));
    identifierMetadata->addOperand(n);

    initializeDIBuilder(sourceFile.fileName, sourceFile.fileDir);
  }
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
    throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + objectFile + "' could not be opened"); // GCOV_EXCL_LINE

  llvm::legacy::PassManager passManager;
  if (targetMachine->addPassesToEmitFile(passManager, dest, nullptr, llvm::CGFT_ObjectFile))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

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

void GeneratorVisitor::dumpAsm() {
  llvm::legacy::PassManager passManager;
  if (targetMachine->addPassesToEmitFile(passManager, llvm::outs(), nullptr, llvm::CGFT_AssemblyFile))
    throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(*module);
  llvm::outs().flush();
}

std::any GeneratorVisitor::visitEntry(EntryNode *node) {
  // Visit children
  AstVisitor::visitEntry(node);

  // Reset the AST
  node->reset();

  // Check if we need to re-analyze
  if (!secondRun) {
    secondRun = true;
    return true;
  }

  // Finalize debug info generation
  if (cliOptions.generateDebugInfo)
    diBuilder->finalize();

  // Verify module
  if (!cliOptions.disableVerifier) {
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyModule(*module, &oss))
      throw IRError(node->codeLoc, INVALID_MODULE, oss.str());
  }

  return false;
}

std::any GeneratorVisitor::visitMainFctDef(MainFctDefNode *node) {
  if (!secondRun)
    return nullptr;

  // Only create main function when it is required
  if (!requiresMainFct)
    return nullptr;

  // Change scope to function scope
  currentScope = node->fctScope;
  assert(currentScope != nullptr);

  // Visit arguments
  std::vector<std::string> argNames;
  std::vector<llvm::Type *> argTypes;
  if (node->paramLst()) {
    argNames.reserve(node->paramLst()->params().size());
    argTypes.reserve(node->paramLst()->params().size());
    for (const auto &param : node->paramLst()->params()) {
      currentVarName = param->varName;
      argNames.push_back(currentVarName);
      SymbolTableEntry *argSymbol = node->fctScope->lookup(currentVarName);
      assert(argSymbol != nullptr);
      currentConstSigned = argSymbol->specifiers.isSigned();
      auto argType = any_cast<llvm::Type *>(visit(param->dataType()));
      argTypes.push_back(argType);
    }
  }

  // Build function itself
  llvm::Type *returnType = builder->getInt32Ty();
  llvm::FunctionType *fctType = llvm::FunctionType::get(returnType, argTypes, false);
  llvm::Function *fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, MAIN_FUNCTION_NAME, module.get());

  // Add debug info
  if (cliOptions.generateDebugInfo) {
    // Get arg types
    std::vector<std::pair<SymbolType, bool>> argSymbolTypes;
    for (const auto &argName : argNames) {
      SymbolTableEntry *argEntry = node->fctScope->lookup(argName);
      assert(argEntry != nullptr);
      argSymbolTypes.emplace_back(argEntry->type, true);
    }
    // Build spice function
    SymbolSpecifiers specifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    Function spiceFunc("main", specifiers, SymbolType(TY_DYN), SymbolType(TY_INT), argSymbolTypes, {}, node);
    // Add debug info
    generateFunctionDebugInfo(fct, &spiceFunc);
    setSourceLocation(node);
  }

  // Create entry block
  std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry." + codeLine);
  fct->getBasicBlockList().push_back(bEntry);
  moveInsertPointToBlock(bEntry);
  allocaInsertInst = nullptr;

  // Store function arguments
  for (auto &arg : fct->args()) {
    unsigned argNo = arg.getArgNo();
    std::string argName = argNames[argNo];
    llvm::Type *argType = fctType->getParamType(argNo);
    llvm::Value *memAddress = insertAlloca(argType, argName);
    SymbolTableEntry *argSymbol = node->fctScope->lookup(argName);
    assert(argSymbol != nullptr);
    argSymbol->updateAddress(memAddress);

    if (cliOptions.generateDebugInfo)
      generateDeclDebugInfo(node->codeLoc, argName, memAddress);

    builder->CreateStore(&arg, memAddress);
  }

  // Declare result variable and set it to 0 for positive return code
  llvm::Value *memAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
  SymbolTableEntry *returnSymbol = node->fctScope->lookup(RETURN_VARIABLE_NAME);
  assert(returnSymbol != nullptr);
  returnSymbol->updateAddress(memAddress);
  builder->CreateStore(builder->getInt32(0), returnSymbol->getAddress());

  // Reset stack state
  stackState = nullptr;

  // Generate IR for function body
  visit(node->stmtLst());

  // Generate return statement for result variable
  if (!blockAlreadyTerminated) {
    std::vector<SymbolTableEntry *> varsToDestruct = node->fctScope->getVarsGoingOutOfScope(true);
    if (!varsToDestruct.empty()) {
      llvm::BasicBlock *predecessor = builder->GetInsertBlock();
      // Generate cleanup block
      llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup." + codeLine);
      moveInsertPointToBlock(bCleanup);

      // Generate cleanup instructions (e.g. dtor calls)
      bool destructorCalled = false;
      for (SymbolTableEntry *varEntry : varsToDestruct)
        destructorCalled |= insertDestructorCall(varEntry->getDeclCodeLoc(), varEntry);

      if (destructorCalled) {
        fct->getBasicBlockList().push_back(bCleanup);
        moveInsertPointToBlock(predecessor);
        builder->CreateBr(bCleanup);
        moveInsertPointToBlock(bCleanup);
      } else {
        moveInsertPointToBlock(predecessor);
      }
    }

    // Restore stack if necessary
    if (stackState != nullptr) {
      builder->CreateCall(stdFunctionManager->getStackRestoreIntrinsic(), {stackState});
      stackState = nullptr;
    }

    // Create return stmt
    llvm::Value *result = returnSymbol->getAddress();
    builder->CreateRet(builder->CreateLoad(fct->getReturnType(), result));
  }

  // Conclude debug information
  if (cliOptions.generateDebugInfo)
    debugInfo.lexicalBlocks.pop();

  // Verify function
  if (!cliOptions.disableVerifier && !cliOptions.generateDebugInfo) { // Verifying while generating debug info throws errors
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyFunction(*fct, &oss))
      throw IRError(node->codeLoc, INVALID_FUNCTION, oss.str());
  }

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any GeneratorVisitor::visitFctDef(FctDefNode *node) {
  if (!secondRun)
    return nullptr;

  setSourceLocation(node);

  // Change to the (potentially generic) struct scope
  SymbolTable *fctParentScope = node->isMethod ? node->structScope : currentScope;
  assert(fctParentScope != nullptr);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Function> *manifestations = fctParentScope->getFunctionManifestations(node->codeLoc);

  // Set the symbolTypeIndex to 0
  node->symbolTypeIndex = 0;

  if (manifestations) {
    for (const auto &[mangledName, spiceFunc] : *manifestations) {
      // Check if the function is substantiated
      if (!spiceFunc.isFullySubstantiated())
        continue;

      // Do not generate this function if it is private and used by nobody
      if (!spiceFunc.isUsed && !spiceFunc.getSpecifiers().isPublic())
        continue;

      std::vector<std::string> argNames;
      std::vector<llvm::Type *> argTypes;

      if (node->isMethod) { // Change to the struct scope
        std::string structSignature = Struct::getSignature(spiceFunc.getThisType().getBaseType().getSubType(),
                                                           spiceFunc.getThisType().getBaseType().getTemplateTypes());
        // Get the LLVM type of the struct symbol
        SymbolType thisSymbolType = spiceFunc.getThisType();
        argNames.emplace_back(THIS_VARIABLE_NAME);
        llvm::Type *thisType = thisSymbolType.toLLVMType(*context, fctParentScope)->getPointerTo();
        argTypes.push_back(thisType);
        // Change scope to struct
        currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope);
      }

      // Change scope
      currentScope = currentScope->getChild(spiceFunc.getSignature());
      assert(currentScope);

      // Get return type
      llvm::Type *returnType = spiceFunc.getReturnType().toLLVMType(*context, currentScope);

      // Arguments
      unsigned int currentArgIndex = 0;
      if (node->hasParams) {
        std::vector<SymbolType> argSymbolTypes = spiceFunc.getParamTypes();
        for (; currentArgIndex < argSymbolTypes.size(); currentArgIndex++) {
          currentVarName = node->paramLst()->params()[currentArgIndex]->varName;
          argNames.push_back(currentVarName);
          SymbolTableEntry *argSymbol = currentScope->lookup(currentVarName);
          assert(argSymbol != nullptr);
          currentConstSigned = argSymbol->specifiers.isSigned();
          argTypes.push_back(argSymbolTypes[currentArgIndex].toLLVMType(*context, currentScope));
        }
      }

      // Check if function is public and/or explicit inlined
      llvm::GlobalValue::LinkageTypes linkage = llvm::Function::InternalLinkage;
      bool explicitInlined = false;
      if (node->specifierLst()) {
        for (const auto &specifier : node->specifierLst()->specifiers()) {
          if (specifier->type == SpecifierNode::TY_INLINE) {
            explicitInlined = true;
          } else if (specifier->type == SpecifierNode::TY_PUBLIC) {
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

      // Add debug info
      if (cliOptions.generateDebugInfo) {
        generateFunctionDebugInfo(fct, &spiceFunc);
        setSourceLocation(node);
      }

      // Create entry block
      std::string codeLine = node->codeLoc.toPrettyLine();
      llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry." + codeLine);
      allocaInsertInst = nullptr;
      fct->getBasicBlockList().push_back(bEntry);
      moveInsertPointToBlock(bEntry);

      // Store mandatory function args
      for (auto &arg : fct->args()) {
        unsigned int argNo = arg.getArgNo();
        std::string argName = argNames[argNo];
        llvm::Type *argType = fctType->getParamType(argNo);
        llvm::Value *memAddress = insertAlloca(argType, argName);
        SymbolTableEntry *argEntry = currentScope->lookup(argName);
        assert(argEntry != nullptr);
        argEntry->updateAddress(memAddress);

        if (cliOptions.generateDebugInfo)
          generateDeclDebugInfo(node->codeLoc, argName, memAddress);

        builder->CreateStore(&arg, memAddress);
      }

      // Store the default values for optional function args
      if (node->paramLst()) {
        for (; currentArgIndex < node->paramLst()->params().size(); currentArgIndex++)
          visit(node->paramLst()->params()[currentArgIndex]);
      }

      // Declare result variable
      llvm::Value *returnMemAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
      SymbolTableEntry *returnSymbol = currentScope->lookup(RETURN_VARIABLE_NAME);
      assert(returnSymbol != nullptr);
      returnSymbol->updateAddress(returnMemAddress);

      // Reset stack state
      stackState = nullptr;

      // Generate IR for function body
      visit(node->stmtLst());

      // Generate return statement for result variable
      if (!blockAlreadyTerminated) {
        std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
        if (!varsToDestruct.empty()) {
          llvm::BasicBlock *predecessor = builder->GetInsertBlock();
          // Generate cleanup block
          llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup." + codeLine);
          moveInsertPointToBlock(bCleanup);

          // Generate cleanup instructions (e.g. dtor calls)
          bool destructorCalled = false;
          for (SymbolTableEntry *varEntry : varsToDestruct)
            destructorCalled |= insertDestructorCall(varEntry->getDeclCodeLoc(), varEntry);

          if (destructorCalled) {
            fct->getBasicBlockList().push_back(bCleanup);
            moveInsertPointToBlock(predecessor);
            builder->CreateBr(bCleanup);
            moveInsertPointToBlock(bCleanup);
          } else {
            moveInsertPointToBlock(predecessor);
          }
        }

        // Restore stack if necessary
        if (stackState != nullptr) {
          builder->CreateCall(stdFunctionManager->getStackRestoreIntrinsic(), {stackState});
          stackState = nullptr;
        }

        // Create return stmt
        llvm::Value *result = returnSymbol->getAddress();
        builder->CreateRet(builder->CreateLoad(fct->getReturnType(), result));
      }

      // Conclude debug information
      if (cliOptions.generateDebugInfo)
        debugInfo.lexicalBlocks.pop();

      // Verify function
      if (!cliOptions.disableVerifier && !cliOptions.generateDebugInfo) { // Verifying while generating debug info throws errors
        std::string output;
        llvm::raw_string_ostream oss(output);
        if (llvm::verifyFunction(*fct, &oss))
          throw IRError(node->codeLoc, INVALID_FUNCTION, oss.str());
      }

      // Change scope back
      currentScope = currentScope->parent;
      assert(currentScope);

      // Leave the struct scope
      if (node->isMethod) {
        currentScope = node->structScope->parent;
        assert(currentScope);
      }

      // Increase the symbolTypeIndex
      node->symbolTypeIndex++;
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitProcDef(ProcDefNode *node) {
  if (!secondRun)
    return nullptr;

  setSourceLocation(node);

  // Change to the (potentially generic) struct scope
  SymbolTable *procParentScope = node->isMethod ? node->structScope : currentScope;
  assert(procParentScope != nullptr);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Function> *manifestations = procParentScope->getFunctionManifestations(node->codeLoc);

  // Set the symbolTypeIndex to 0
  node->symbolTypeIndex = 0;

  if (manifestations) {
    for (const auto &[mangledName, spiceProc] : *manifestations) {
      // Check if the function is substantiated
      if (!spiceProc.isFullySubstantiated())
        continue;

      // Do not generate this function if it is private and used by nobody
      if (!spiceProc.isUsed && !spiceProc.getSpecifiers().isPublic())
        continue;

      std::vector<std::string> argNames;
      std::vector<llvm::Type *> argTypes;

      if (node->isMethod) { // Change to the struct scope
        std::string structSignature = Struct::getSignature(spiceProc.getThisType().getBaseType().getSubType(),
                                                           spiceProc.getThisType().getBaseType().getTemplateTypes());
        // Get the LLVM type of the struct symbol
        argNames.emplace_back(THIS_VARIABLE_NAME);
        llvm::Type *thisType = spiceProc.getThisType().toLLVMType(*context, procParentScope)->getPointerTo();
        argTypes.push_back(thisType);
        // Change scope to struct
        currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope);
      }

      // Change scope
      currentScope = currentScope->getChild(spiceProc.getSignature());
      assert(currentScope);

      // Arguments
      unsigned int currentArgIndex = 0;
      if (node->paramLst()) {
        std::vector<SymbolType> argSymbolTypes = spiceProc.getParamTypes();
        for (; currentArgIndex < argSymbolTypes.size(); currentArgIndex++) {
          currentVarName = node->paramLst()->params()[currentArgIndex]->varName;
          argNames.push_back(currentVarName);
          SymbolTableEntry *argSymbol = currentScope->lookup(currentVarName);
          assert(argSymbol != nullptr);
          currentConstSigned = argSymbol->specifiers.isSigned();
          argTypes.push_back(argSymbolTypes[currentArgIndex].toLLVMType(*context, currentScope));
        }
      }

      // Check if function is public and/or explicit inlined
      llvm::GlobalValue::LinkageTypes linkage = llvm::Function::InternalLinkage;
      bool explicitInlined = false;
      if (node->specifierLst()) {
        for (const auto &specifier : node->specifierLst()->specifiers()) {
          if (specifier->type == SpecifierNode::TY_INLINE) {
            explicitInlined = true;
          } else if (specifier->type == SpecifierNode::TY_PUBLIC) {
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

      // Add debug info
      if (cliOptions.generateDebugInfo) {
        generateFunctionDebugInfo(proc, &spiceProc);
        setSourceLocation(node);
      }

      // Create entry block
      std::string codeLine = node->codeLoc.toPrettyLine();
      llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry." + codeLine);
      allocaInsertInst = nullptr;
      proc->getBasicBlockList().push_back(bEntry);
      moveInsertPointToBlock(bEntry);

      // Store mandatory procedure args
      for (auto &arg : proc->args()) {
        unsigned int argNo = arg.getArgNo();
        std::string argName = argNames[argNo];
        llvm::Type *argType = procType->getParamType(argNo);
        llvm::Value *memAddress = insertAlloca(argType, argName);
        SymbolTableEntry *argSymbol = currentScope->lookup(argName);
        assert(argSymbol != nullptr);
        argSymbol->updateAddress(memAddress);

        if (cliOptions.generateDebugInfo)
          generateDeclDebugInfo(node->codeLoc, argName, memAddress);

        builder->CreateStore(&arg, memAddress);
      }

      // Store the default values for optional procedure args
      if (node->paramLst()) {
        for (; currentArgIndex < node->paramLst()->params().size(); currentArgIndex++)
          visit(node->paramLst()->params()[currentArgIndex]);
      }

      // Reset stack state
      stackState = nullptr;

      // Generate IR for procedure body
      visit(node->stmtLst());

      // Create return
      if (!blockAlreadyTerminated) {
        std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
        if (!varsToDestruct.empty()) {
          llvm::BasicBlock *predecessor = builder->GetInsertBlock();
          // Generate cleanup block
          llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup." + codeLine);
          moveInsertPointToBlock(bCleanup);

          // Generate cleanup instructions (e.g. dtor calls)
          bool destructorCalled = false;
          for (SymbolTableEntry *varEntry : varsToDestruct)
            destructorCalled |= insertDestructorCall(varEntry->getDeclCodeLoc(), varEntry);

          if (destructorCalled) {
            proc->getBasicBlockList().push_back(bCleanup);
            moveInsertPointToBlock(predecessor);
            builder->CreateBr(bCleanup);
            moveInsertPointToBlock(bCleanup);
          } else {
            moveInsertPointToBlock(predecessor);
          }
        }

        // Restore stack if necessary
        if (stackState != nullptr) {
          builder->CreateCall(stdFunctionManager->getStackRestoreIntrinsic(), {stackState});
          stackState = nullptr;
        }

        // Create return stmt
        builder->CreateRetVoid();
      }

      // Conclude debug information
      if (cliOptions.generateDebugInfo)
        debugInfo.lexicalBlocks.pop();

      // Verify procedure
      if (!cliOptions.disableVerifier && !cliOptions.generateDebugInfo) { // Verifying while generating debug info throws errors
        std::string output;
        llvm::raw_string_ostream oss(output);
        if (llvm::verifyFunction(*proc, &oss))
          throw IRError(node->codeLoc, INVALID_FUNCTION, oss.str());
      }

      // Change scope back
      currentScope = currentScope->parent;
      assert(currentScope);

      // Leave the struct scope
      if (node->isMethod) {
        currentScope = node->structScope->parent;
        assert(currentScope);
      }

      // Increase the symbolTypeIndex
      node->symbolTypeIndex++;
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitStructDef(StructDefNode *node) {
  if (secondRun)
    return nullptr;

  setSourceLocation(node);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Struct> *manifestations = currentScope->getStructManifestations(node->codeLoc);
  if (manifestations) {
    for (const auto &[mangledName, spiceStruct] : *manifestations) {
      // Check if the struct is substantiated
      if (!spiceStruct.isFullySubstantiated())
        continue;

      // Do not generate this struct if it is private and used by nobody
      if (!spiceStruct.isUsed && !spiceStruct.getSpecifiers().isPublic())
        continue;

      // Change scope
      currentScope = currentScope->getChild(STRUCT_SCOPE_PREFIX + spiceStruct.getSignature());
      assert(currentScope);

      // Create global struct
      llvm::StructType *structType = llvm::StructType::create(*context, mangledName);
      SymbolTableEntry *structSymbol = currentScope->lookup(spiceStruct.getSignature());
      assert(structSymbol);
      structSymbol->setStructLLVMType(structType);

      // Collect concrete field types
      std::vector<llvm::Type *> fieldTypes;
      for (const auto &field : node->fields()) {
        SymbolTableEntry *fieldEntry = currentScope->lookup(field->name);
        assert(fieldEntry && !fieldEntry->type.is(TY_GENERIC));
        currentConstSigned = fieldEntry->specifiers.isSigned();
        fieldTypes.push_back(fieldEntry->type.toLLVMType(*context, currentScope));
      }

      // Set field types to struct type
      structType->setBody(fieldTypes);

      // Return to old scope
      currentScope = currentScope->parent;
      assert(currentScope);
    }
  }

  return nullptr;
}

std::any GeneratorVisitor::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (secondRun)
    return nullptr;

  setSourceLocation(node);

  currentVarName = node->varName;

  // Get symbol table entry and the symbol specifiers
  SymbolTableEntry *globalVarEntry = currentScope->lookup(node->varName);
  assert(globalVarEntry != nullptr);
  SymbolSpecifiers specifiers = globalVarEntry->specifiers;
  llvm::GlobalValue::LinkageTypes linkage =
      specifiers.isPublic() ? llvm::GlobalValue::LinkageTypes::ExternalLinkage : llvm::GlobalValue::LinkageTypes::InternalLinkage;

  // Create correctly signed LLVM type from the data type
  currentConstSigned = specifiers.isSigned();
  auto varType = any_cast<llvm::Type *>(visit(node->dataType()));

  // Create global variable
  llvm::Value *memAddress = module->getOrInsertGlobal(node->varName, varType);
  globalVarEntry->updateAddress(memAddress);
  // Set some attributes to it
  llvm::GlobalVariable *global = module->getNamedGlobal(node->varName);
  global->setLinkage(linkage);
  global->setConstant(specifiers.isConst());

  // Add debug info
  if (cliOptions.generateDebugInfo)
    generateGlobalVarDebugInfo(global, globalVarEntry);

  if (node->value()) { // Variable is initialized here
    visit(node->value());
    global->setInitializer(currentConstValue);
  } else {
    llvm::Constant *defaultValue = getDefaultValueForSymbolType(globalVarEntry->type);
    global->setInitializer(defaultValue);
  }

  return nullptr;
}

std::any GeneratorVisitor::visitExtDecl(ExtDeclNode *node) {
  if (secondRun)
    return nullptr;

  setSourceLocation(node);

  // Get function name
  std::vector<SymbolType> symbolTypes;

  // Pop function declaration pointer from the stack
  std::map<std::string, Function> *manifestations = currentScope->getFunctionManifestations(node->codeLoc);
  assert(!manifestations->empty());
  Function spiceFunc = manifestations->begin()->second;

  // Get LLVM return type
  llvm::Type *returnType;
  if (node->returnType()) {
    returnType = any_cast<llvm::Type *>(visit(node->returnType()));
    SymbolTable *functionTable = currentScope->getChild(spiceFunc.getSignature());
    assert(functionTable != nullptr);
    SymbolTableEntry *returnEntry = functionTable->lookup(RETURN_VARIABLE_NAME);
    assert(returnEntry != nullptr);
    symbolTypes.push_back(returnEntry->type);
  } else {
    returnType = llvm::Type::getVoidTy(*context);
  }

  // Get LLVM arg types
  std::vector<llvm::Type *> argTypes;
  if (node->argTypeLst()) {
    for (const auto &arg : node->argTypeLst()->dataTypes()) {
      auto argType = any_cast<llvm::Type *>(visit(arg));
      argTypes.push_back(argType);
    }
  }
  std::vector<SymbolType> argSymbolTypes = spiceFunc.getParamTypes();
  symbolTypes.insert(std::end(symbolTypes), std::begin(argSymbolTypes), std::end(argSymbolTypes));

  // Declare function
  llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, argTypes, node->isVarArg);
  module->getOrInsertFunction(node->extFunctionName, functionType);
  if (node->isDll)
    module->getFunction(node->extFunctionName)->setDLLStorageClass(llvm::GlobalValue::DLLImportStorageClass);

  return nullptr;
}

std::any GeneratorVisitor::visitThreadDef(ThreadDefNode *node) {
  setSourceLocation(node);

  // Create threaded function
  std::string threadedFctName = "_thread" + std::to_string(threadFactory.getNextFunctionSuffix());
  llvm::Type *voidPtrTy = builder->getInt8PtrTy();
  llvm::FunctionType *threadFctTy = llvm::FunctionType::get(voidPtrTy, voidPtrTy, false);
  llvm::Function *threadFct = llvm::Function::Create(threadFctTy, llvm::Function::InternalLinkage, threadedFctName, module.get());

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);

  // Collect arg names, types and addresses from captures of the nested scope
  std::vector<std::string> argStructFieldNames;
  std::vector<llvm::Type *> argStructFieldTypes;
  std::vector<llvm::Value *> argStructFieldPointers;
  for (const auto &capture : currentScope->getCaptures()) {
    argStructFieldNames.push_back(capture.first);
    argStructFieldTypes.push_back(capture.second.capturedEntry->type.toLLVMType(*context, currentScope)->getPointerTo());
    argStructFieldPointers.push_back(capture.second.capturedEntry->getAddress());
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
  std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "thread.entry." + codeLine);
  allocaInsertInst = nullptr;
  threadFct->getBasicBlockList().push_back(bEntry);
  moveInsertPointToBlock(bEntry);

  // Store function args
  llvm::Value *recArgStructPtr = builder->CreatePointerCast(threadFct->args().begin(), argStructTy->getPointerTo());
  unsigned int i = 0;
  for (const auto &[name, capture] : currentScope->getCaptures()) {
    std::string argName = argStructFieldNames[i];
    llvm::Value *memAddress = builder->CreateStructGEP(argStructTy, recArgStructPtr, i);
    llvm::Type *loadType = capture.capturedEntry->type.toLLVMType(*context, currentScope)->getPointerTo();
    memAddress = builder->CreateLoad(loadType, memAddress);
    // Push address to each capture to ensure that the address is valid and known to the inner function
    capture.capturedEntry->pushAddress(memAddress);
    i++;
  }

  // Insert instructions into thread function
  visit(node->stmtLst());

  // Pop address from each capture to ensure that the address is valid and known to the outer function
  for (const auto &capture : currentScope->getCaptures())
    capture.second.capturedEntry->popAddress();

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  // Insert return statement and verify function
  llvm::Value *voidPtrNull = llvm::Constant::getNullValue(llvm::Type::getInt8Ty(*context)->getPointerTo());
  builder->CreateRet(voidPtrNull);

  // Verify function
  std::string output;
  llvm::raw_string_ostream oss(output);
  if (llvm::verifyFunction(*threadFct, &oss))
    throw IRError(node->codeLoc, INVALID_FUNCTION, oss.str());

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

std::any GeneratorVisitor::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  setSourceLocation(node);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);

  // Visit instructions in the block
  visit(node->stmtLst());

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  return nullptr;
}

std::any GeneratorVisitor::visitForLoop(ForLoopNode *node) {
  setSourceLocation(node);

  // Create blocks
  std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "for.cond." + codeLine);
  llvm::BasicBlock *bInc = llvm::BasicBlock::Create(*context, "for.inc." + codeLine);
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "for." + codeLine);
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "for.end." + codeLine);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bInc);

  // Execute pre-loop stmts
  visit(node->initDecl());
  // Jump into condition block
  createBr(bCond);

  // Get parent function
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Fill loop block
  parentFct->getBasicBlockList().push_back(bLoop);
  moveInsertPointToBlock(bLoop);
  // Generate IR for nested statements
  visit(node->stmtLst());
  // Jump into post block
  createBr(bInc);

  // Fill inc block
  parentFct->getBasicBlockList().push_back(bInc);
  moveInsertPointToBlock(bInc);
  // Run inc actions
  visit(node->incAssign());
  // Jump into condition block
  createBr(bCond);

  // Fill condition block
  parentFct->getBasicBlockList().push_back(bCond);
  moveInsertPointToBlock(bCond);
  llvm::Value *condValue = resolveValue(node->condAssign());
  // Jump to loop body or to loop end
  createCondBr(condValue, bLoop, bEnd);

  // Fill loop end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Pop basic blocks from break and continue stacks
  breakBlocks.pop();
  continueBlocks.pop();

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitForeachLoop(ForeachLoopNode *node) {
  setSourceLocation(node);

  // Create blocks
  std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "foreach.cond." + codeLine);
  llvm::BasicBlock *bInc = llvm::BasicBlock::Create(*context, "foreach.inc." + codeLine);
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "foreach.loop." + codeLine);
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "foreach.end." + codeLine);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bInc);

  // Get array variable entry
  llvm::Value *arrayValuePtr = resolveAddress(node->arrayAssign());
  SymbolTableEntry *arrayVarEntry = currentScope->lookup(currentVarName);
  bool dynamicallySized = arrayVarEntry && arrayVarEntry->type.is(TY_PTR) && arrayVarEntry->type.getDynamicArraySize() != nullptr;

  // Initialize loop variables
  llvm::Value *idxVarPtr;
  llvm::Type *idxVarType;
  if (node->idxVarDecl()) {
    auto idxVarName = any_cast<std::string>(visit(node->idxVarDecl()));
    SymbolTableEntry *idxVarEntry = currentScope->lookup(idxVarName);
    assert(idxVarEntry != nullptr);
    idxVarPtr = idxVarEntry->getAddress();
    idxVarType = idxVarEntry->type.toLLVMType(*context, currentScope);
  } else {
    std::string indexVariableName = FOREACH_DEFAULT_IDX_VARIABLE_NAME;
    // Create local variable for
    idxVarType = llvm::Type::getInt32Ty(*context);
    idxVarPtr = insertAlloca(idxVarType, indexVariableName);
    SymbolTableEntry *idxVarEntry = currentScope->lookup(indexVariableName);
    assert(idxVarEntry != nullptr);
    idxVarEntry->updateAddress(idxVarPtr);
    idxVarEntry->isUsed = true;
    // Initialize variable with 0
    builder->CreateStore(builder->getInt32(0), idxVarPtr);
  }
  auto itemVarName = any_cast<std::string>(visit(node->itemDecl()));
  SymbolTableEntry *itemVarEntry = currentScope->lookup(itemVarName);
  assert(itemVarEntry != nullptr);
  llvm::Value *itemVarPtr = itemVarEntry->getAddress();

  // Do loop variable initialization
  SymbolType arrSymbolType = node->arrayAssign()->getEvaluatedSymbolType();
  llvm::Type *arrayValueType = arrSymbolType.toLLVMType(*context, currentScope);
  llvm::Value *arrayValue = builder->CreateLoad(arrayValueType, arrayValuePtr);
  llvm::Value *arraySizeValue = dynamicallySized ? arrayVarEntry->type.getDynamicArraySize()
                                                 : builder->getInt32(arrayValue->getType()->getArrayNumElements());

  // Cast to i32 type if necessary
  if (arraySizeValue->getType()->getIntegerBitWidth() != 32)
    arraySizeValue = builder->CreateSExtOrTrunc(arraySizeValue, builder->getInt32Ty());

  // Load the first item into item variable
  llvm::Value *index = builder->CreateLoad(idxVarType, idxVarPtr);

  llvm::Value *itemPtr;
  llvm::Value *indices[2] = {builder->getInt32(0), index};
  if (dynamicallySized) {
    arrayValuePtr = builder->CreateLoad(arrayValueType, arrayValuePtr);
    arrayValueType = arrSymbolType.getContainedTy().toLLVMType(*context, currentScope);
    itemPtr = builder->CreateInBoundsGEP(arrayValueType, arrayValuePtr, index);
  } else {
    itemPtr = builder->CreateInBoundsGEP(arrayValueType, arrayValuePtr, indices);
  }
  llvm::Type *itemPtrType = arrSymbolType.getContainedTy().toLLVMType(*context, currentScope);
  llvm::Value *newItemValue = builder->CreateLoad(itemPtrType, itemPtr);
  builder->CreateStore(newItemValue, itemVarPtr);
  createBr(bLoop);

  // Get parent function
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Fill loop block
  parentFct->getBasicBlockList().push_back(bLoop);
  moveInsertPointToBlock(bLoop);
  // Generate IR for nested statements
  visit(node->stmtLst());
  // Jump to condition check
  if (!blockAlreadyTerminated)
    builder->CreateBr(bInc);

  // Fill inc block
  parentFct->getBasicBlockList().push_back(bInc);
  moveInsertPointToBlock(bInc);
  // Increment index variable
  index = builder->CreateLoad(idxVarType, idxVarPtr, "idx");
  index = builder->CreateAdd(index, builder->getInt32(1), "idx.inc");
  builder->CreateStore(index, idxVarPtr);
  // Load new item into item variable
  indices[1] = index;
  if (dynamicallySized) {
    itemPtr = builder->CreateInBoundsGEP(arrayValueType, arrayValuePtr, index);
  } else {
    itemPtr = builder->CreateInBoundsGEP(arrayValueType, arrayValuePtr, indices);
  }
  newItemValue = builder->CreateLoad(itemPtrType, itemPtr);
  builder->CreateStore(newItemValue, itemVarPtr);
  createBr(bCond);

  // Fill cond block
  parentFct->getBasicBlockList().push_back(bCond);
  moveInsertPointToBlock(bCond);
  // Check condition
  index = builder->CreateLoad(idxVarType, idxVarPtr);
  llvm::Value *cond = builder->CreateICmpULT(index, arraySizeValue);
  createCondBr(cond, bLoop, bEnd);

  // Fill loop end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Pop basic blocks from break and continue stacks
  breakBlocks.pop();
  continueBlocks.pop();

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitWhileLoop(WhileLoopNode *node) {
  setSourceLocation(node);

  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Create blocks
  std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "while.cond." + codeLine);
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "while." + codeLine);
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "while.end." + codeLine);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bCond);

  // Jump into condition block
  createBr(bCond);

  // Fill condition block
  parentFct->getBasicBlockList().push_back(bCond);
  moveInsertPointToBlock(bCond);
  llvm::Value *condValue = resolveValue(node->condition());
  createCondBr(condValue, bLoop, bEnd);

  // Fill loop block
  parentFct->getBasicBlockList().push_back(bLoop);
  moveInsertPointToBlock(bLoop);
  // Generate IR for nested statements
  visit(node->stmtLst());
  // Jump into condition block
  createBr(bCond);

  // Fill loop end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitStmtLst(StmtLstNode *node) {
  for (const auto &child : node->children) {
    if (!blockAlreadyTerminated) {
      visit(child);
      lhsVarName = currentVarName = "";
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitTypeAltsLst(TypeAltsLstNode * /*node*/) {
  return nullptr; // Noop
}

std::any GeneratorVisitor::visitIfStmt(IfStmtNode *node) {
  setSourceLocation(node);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);

  llvm::Value *condValue = resolveValue(node->condition());
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Create blocks
  std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bThen = llvm::BasicBlock::Create(*context, "if.then." + codeLine);
  llvm::BasicBlock *bElse = llvm::BasicBlock::Create(*context, "if.else." + codeLine);
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "if.end." + codeLine);

  // Check if condition is fulfilled
  createCondBr(condValue, bThen, node->elseStmt() ? bElse : bEnd);

  // Fill then block
  parentFct->getBasicBlockList().push_back(bThen);
  moveInsertPointToBlock(bThen);
  // Generate IR for nested statements
  visit(node->stmtLst());
  createBr(bEnd);

  // Change scope back
  currentScope = currentScope->parent;
  assert(currentScope != nullptr);

  // Fill else block
  if (node->elseStmt()) {
    parentFct->getBasicBlockList().push_back(bElse);
    moveInsertPointToBlock(bElse);
    visit(node->elseStmt()); // Generate IR for else block
    createBr(bEnd);
  }

  // Fill end block
  parentFct->getBasicBlockList().push_back(bEnd);
  moveInsertPointToBlock(bEnd);

  // Return conditional value as result for the if stmt
  return condValue;
}

std::any GeneratorVisitor::visitElseStmt(ElseStmtNode *node) {
  setSourceLocation(node);

  if (node->ifStmt()) { // It is an else if branch
    visit(node->ifStmt());
  } else { // It is an else branch
    // Change scope
    currentScope = currentScope->getChild(node->getScopeId());
    assert(currentScope != nullptr);

    // Generate IR for nested statements
    visit(node->stmtLst());

    // Change scope back
    currentScope = currentScope->parent;
    assert(currentScope != nullptr);
  }
  return nullptr;
}

std::any GeneratorVisitor::visitAssertStmt(AssertStmtNode *node) {
  setSourceLocation(node);

  // Only generate assertions with -O0
  if (cliOptions.optLevel == 0) {
    // Visit the assignExpr
    llvm::Value *condValue = resolveValue(node->assignExpr());
    llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    std::string codeLine = node->codeLoc.toPrettyLine();
    llvm::BasicBlock *bThen = llvm::BasicBlock::Create(*context, "assert.then." + codeLine);
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "assert.end." + codeLine);

    // Check if condition is fulfilled
    createCondBr(condValue, bEnd, bThen);

    // Fill then block
    parentFct->getBasicBlockList().push_back(bThen);
    moveInsertPointToBlock(bThen);
    // Generate IR for assertion error
    llvm::Function *printfFct = stdFunctionManager->getPrintfFct();
    std::string errorMsg = "Assertion failed: Condition '" + node->expressionString + "' evaluated to false.";
    llvm::Value *templateString = builder->CreateGlobalStringPtr(errorMsg);
    builder->CreateCall(printfFct, templateString);
    // Generate call to exit
    llvm::Function *exitFct = stdFunctionManager->getExitFct();
    builder->CreateCall(exitFct, builder->getInt32(0));
    // Create unreachable instruction
    builder->CreateUnreachable();

    // Fill end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);
  }

  return nullptr;
}

std::any GeneratorVisitor::visitDeclStmt(DeclStmtNode *node) {
  setSourceLocation(node);

  // Get variable entry
  SymbolTableEntry *entry = currentScope->lookup(node->varName);
  assert(entry != nullptr);
  currentConstSigned = entry->specifiers.isSigned();

  // Set lhsVarName and currentVarName to varName
  currentVarName = lhsVarName = node->varName;

  // Get data type
  llvm::Type *varType = lhsType = any_cast<llvm::Type *>(visit(node->dataType()));
  entry->updateType(currentSymbolType, true);

  // Restore lhsVarName and currentVarName
  currentVarName = lhsVarName = node->varName;

  llvm::Value *memAddress = nullptr;
  if (node->assignExpr()) { // Declaration with assignment
    if (varType->isStructTy()) {
      llvm::Value *rhsAddress = resolveAddress(node->assignExpr());
      if (entry->getAddress() != nullptr) { // Struct contents need be copied
        llvm::Function *memcpyFct = stdFunctionManager->getMemcpyIntrinsic();
        unsigned int typeSize = module->getDataLayout().getTypeSizeInBits(varType);
        llvm::Value *structSize = builder->getInt64(typeSize);
        llvm::Value *isVolatile = builder->getInt1(entry->isVolatile);
        memAddress = insertAlloca(varType, node->varName);
        builder->CreateCall(memcpyFct, {memAddress, rhsAddress, structSize, isVolatile});
      } else { // Struct contents need not be copied
        memAddress = rhsAddress;
      }
    } else {
      llvm::Value *value = resolveValue(node->assignExpr());
      memAddress = insertAlloca(varType, node->varName);
      builder->CreateStore(value, memAddress);
    }
  } else { // Declaration with default value
    if (entry->type.is(TY_PTR) && entry->type.getDynamicArraySize() != nullptr) {
      llvm::Type *itemType = entry->type.getContainedTy().toLLVMType(*context, nullptr);
      dynamicArraySize = entry->type.getDynamicArraySize();
      llvm::Value *value = allocateDynamicallySizedArray(itemType);
      memAddress = insertAlloca(value->getType(), node->varName);
      builder->CreateStore(value, memAddress, entry->isVolatile);
    } else {
      llvm::Value *value = getDefaultValueForSymbolType(currentSymbolType);
      memAddress = insertAlloca(varType, node->varName);

      // Save default value to address
      builder->CreateStore(value, memAddress, entry->isVolatile);
    }
  }

  if (cliOptions.generateDebugInfo)
    generateDeclDebugInfo(node->codeLoc, node->varName, memAddress, true);

  // Update address in symbol table
  entry->updateAddress(memAddress);

  lhsType = nullptr; // Reset nullptr

  // Return the variable name
  return node->varName;
}

std::any GeneratorVisitor::visitImportStmt(ImportStmtNode * /*node*/) {
  // Noop
  return nullptr;
}

std::any GeneratorVisitor::visitReturnStmt(ReturnStmtNode *node) {
  setSourceLocation(node);

  SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);

  // Check if a value is attached to the return statement
  llvm::Value *returnValuePtr = nullptr;
  llvm::Type *returnValueType;
  if (node->assignExpr()) {
    assert(returnVarEntry != nullptr);
    // Set the expected type of the value
    lhsType = returnVarEntry->type.toLLVMType(*context, currentScope);
    // Visit return value
    returnValuePtr = resolveAddress(node->assignExpr());
    returnValueType = node->assignExpr()->getEvaluatedSymbolType().toLLVMType(*context, currentScope);
  } else if (returnVarEntry != nullptr) { // Function. Procedures do not have a return variable
    returnValuePtr = returnVarEntry->getAddress();
    returnValueType = returnVarEntry->type.toLLVMType(*context, currentScope);
  }

  // Insert destructor calls to variables, going out of scope
  std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
  if (!varsToDestruct.empty()) {
    llvm::BasicBlock *predecessor = builder->GetInsertBlock();
    // Generate cleanup block
    llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup");
    moveInsertPointToBlock(bCleanup);

    // Generate cleanup instructions (e.g. dtor calls)
    bool destructorCalled = false;
    for (SymbolTableEntry *varEntry : varsToDestruct)
      destructorCalled |= insertDestructorCall(varEntry->getDeclCodeLoc(), varEntry);

    if (destructorCalled) {
      predecessor->getParent()->getBasicBlockList().push_back(bCleanup);
      moveInsertPointToBlock(predecessor);
      builder->CreateBr(bCleanup);
      moveInsertPointToBlock(bCleanup);
    } else {
      moveInsertPointToBlock(predecessor);
    }
  }

  // Set block to terminated
  blockAlreadyTerminated = true;

  // No return value
  if (returnValuePtr == nullptr) {
    builder->CreateRetVoid();
    return nullptr;
  }

  // Return value
  llvm::Value *returnValue = builder->CreateLoad(returnValueType, returnValuePtr);
  builder->CreateRet(returnValue);
  return returnValuePtr;
}

std::any GeneratorVisitor::visitBreakStmt(BreakStmtNode *node) {
  setSourceLocation(node);

  // Get destination block
  for (int i = 1; i < node->breakTimes; i++)
    breakBlocks.pop();

  // Jump to destination block
  createBr(breakBlocks.top());
  return nullptr;
}

std::any GeneratorVisitor::visitContinueStmt(ContinueStmtNode *node) {
  setSourceLocation(node);

  // Get destination block
  for (int i = 1; i < node->continueTimes; i++)
    continueBlocks.pop();

  // Jump to destination block
  createBr(continueBlocks.top());
  return nullptr;
}

std::any GeneratorVisitor::visitPrintfCall(PrintfCallNode *node) {
  // Declare if not declared already
  llvm::Function *printfFct = stdFunctionManager->getPrintfFct();

  std::vector<llvm::Value *> printfArgs;
  printfArgs.push_back(builder->CreateGlobalStringPtr(node->templatedString));
  for (const auto &arg : node->assignExpr()) {
    SymbolType argSymbolType = arg->getEvaluatedSymbolType();
    llvm::Type *targetType = argSymbolType.toLLVMType(*context, currentScope);

    llvm::Value *argVal;
    if (argSymbolType.isArray()) { // Convert array type to pointer type
      llvm::Value *argValPtr = resolveAddress(arg);
      llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(0)};
      argVal = builder->CreateInBoundsGEP(targetType, argValPtr, indices);
    } else if (argSymbolType.isStringStruct()) {
      argVal = materializeString(resolveAddress(arg));
    } else {
      argVal = resolveValue(arg);
    }

    // Cast all integer types to 32 bit
    if (argVal->getType()->isIntegerTy(8) || argVal->getType()->isIntegerTy(16))
      argVal = builder->CreateSExt(argVal, llvm::Type::getInt32Ty(*context));
    if (argVal->getType()->isIntegerTy(1))
      argVal = builder->CreateZExt(argVal, llvm::Type::getInt32Ty(*context));

    printfArgs.push_back(argVal);
  }

  llvm::Value *returnValue = builder->CreateCall(printfFct, printfArgs);
  return returnValue;
}

std::any GeneratorVisitor::visitSizeofCall(SizeofCallNode *node) {
  llvm::Type *type;
  if (node->isType) { // Size of type
    type = any_cast<llvm::Type *>(visit(node->dataType()));
  } else { // Size of value
    // Visit the argument
    llvm::Value *value = resolveValue(node->assignExpr());
    type = value->getType();
  }
  // Calculate size at compile-time
  unsigned int size = module->getDataLayout().getTypeSizeInBits(type);

  // Save size
  llvm::Value *resultPtr = insertAlloca(builder->getInt32Ty());
  builder->CreateStore(builder->getInt32(size), resultPtr);

  // Return address to where the size is saved
  return resultPtr;
}

std::any GeneratorVisitor::visitLenCall(LenCallNode *node) {
  // Visit the argument
  llvm::Value *value = resolveValue(node->assignExpr());

  llvm::Value *sizeValue;
  if (value->getType()->isArrayTy()) {
    // Get array size
    sizeValue = builder->getInt32(value->getType()->getArrayNumElements());
  } else {
    // Get pointer size / dynamic array size
    sizeValue = dynamicArraySize;
  }

  // Store the result
  llvm::Value *resultPtr = insertAlloca(builder->getInt32Ty());
  builder->CreateStore(sizeValue, resultPtr);

  // Return address to where the size is saved
  return resultPtr;
}

std::any GeneratorVisitor::visitTidCall(TidCallNode *node) {
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

std::any GeneratorVisitor::visitJoinCall(JoinCallNode *node) {
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
  for (const auto &assignExpr : node->assignExpressions()) {
    // Check if it is an id or an array of ids
    auto threadIdPtr = resolveAddress(assignExpr);
    assert(threadIdPtr != nullptr && threadIdPtr->getType()->isPointerTy());
    SymbolType threadIdSymbolType = assignExpr->getEvaluatedSymbolType();
    llvm::Type *threadIdPtrTy = threadIdSymbolType.toLLVMType(*context, currentScope);
    if (threadIdPtrTy->isArrayTy()) { // Array of ids
      for (int i = 0; i < threadIdPtrTy->getArrayNumElements(); i++) {
        // Get thread id that has to be joined
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(i)};
        llvm::Value *threadIdAddr = builder->CreateGEP(threadIdPtrTy, threadIdPtr, indices);
        llvm::Type *threadIdType = threadIdSymbolType.getContainedTy().toLLVMType(*context, currentScope);
        llvm::Value *threadId = builder->CreateLoad(threadIdType, threadIdAddr);

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

std::any GeneratorVisitor::visitAssignExpr(AssignExprNode *node) {
  setSourceLocation(node);

  // Check if there is an assign operator applied
  if (node->hasOperator) { // This is an assignment or compound assignment
    lhsType = nullptr;     // Reset lhs type

    // Retrieve the lhs variable name
    if (!node->lhs()->postfixUnaryExpr()->atomicExpr()->identifier.empty())
      lhsVarName = node->lhs()->postfixUnaryExpr()->atomicExpr()->identifier;

    // Get value of right side
    llvm::Value *rhs = resolveValue(node->rhs());
    SymbolType rhsTy = node->rhs()->getEvaluatedSymbolType();

    // Visit the left side
    llvm::Value *lhsPtr = resolveAddress(node->lhs());
    SymbolType lhsTy = node->lhs()->getEvaluatedSymbolType();
    lhsVarName = currentVarName;

    // Take a look at the operator
    if (node->op == AssignExprNode::OP_ASSIGN) { // Simple assign
      builder->CreateStore(rhs, lhsPtr);
    } else { // Compound assign
      // Get symbol table entry
      SymbolTableEntry *variableEntry = currentScope->lookup(lhsVarName);
      assert(variableEntry != nullptr);

      // Get value of left side
      llvm::Value *lhs;
      if (variableEntry->isGlobal) {
        lhs = module->getNamedGlobal(lhsVarName);
      } else {
        lhs = builder->CreateLoad(lhsTy.toLLVMType(*context, currentScope), lhsPtr);
      }

      // Decide what to do, based on the operator
      PtrAndValue result = {nullptr, nullptr};
      switch (node->op) {
      case AssignExprNode::OP_PLUS_EQUAL:
        result = conversionsManager->getPlusEqualInst({lhsPtr, lhs}, rhs, lhsTy, rhsTy, currentScope, node->codeLoc);
        break;
      case AssignExprNode::OP_MINUS_EQUAL:
        rhs = conversionsManager->getMinusEqualInst(lhs, rhs, lhsTy, rhsTy, currentScope);
        break;
      case AssignExprNode::OP_MUL_EQUAL:
        result = conversionsManager->getMulEqualInst({lhsPtr, lhs}, rhs, lhsTy, rhsTy, currentScope, node->codeLoc);
        break;
      case AssignExprNode::OP_DIV_EQUAL:
        rhs = conversionsManager->getDivEqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      case AssignExprNode::OP_REM_EQUAL:
        rhs = conversionsManager->getRemEqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      case AssignExprNode::OP_SHL_EQUAL:
        rhs = conversionsManager->getSHLEqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      case AssignExprNode::OP_SHR_EQUAL:
        rhs = conversionsManager->getSHREqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      case AssignExprNode::OP_AND_EQUAL:
        rhs = conversionsManager->getAndEqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      case AssignExprNode::OP_OR_EQUAL:
        rhs = conversionsManager->getOrEqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      case AssignExprNode::OP_XOR_EQUAL:
        rhs = conversionsManager->getXorEqualInst(lhs, rhs, lhsTy, rhsTy);
        break;
      default:
        throw std::runtime_error("Assign op fall-through");
      }

      if (result.ptr) {
        variableEntry->updateAddress(result.ptr);
        return result.ptr;
      }
      if (result.value)
        rhs = result.value;

      builder->CreateStore(rhs, lhsPtr, variableEntry->isVolatile);
    }

    return lhsPtr;
  } else if (node->ternaryExpr()) {
    return visit(node->ternaryExpr());
  } else if (node->threadDef()) {
    return visit(node->threadDef());
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any GeneratorVisitor::visitTernaryExpr(TernaryExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    llvm::Value *conditionPtr = resolveAddress(node->operands()[0]);
    llvm::Value *trueValuePtr;
    llvm::Value *falseValuePtr;
    if (node->isShortened) {
      trueValuePtr = conditionPtr;
      falseValuePtr = resolveAddress(node->operands()[1]);
    } else {
      trueValuePtr = resolveAddress(node->operands()[1]);
      falseValuePtr = resolveAddress(node->operands()[2]);
    }

    llvm::Type *conditionType = node->operands().front()->getEvaluatedSymbolType().toLLVMType(*context, currentScope);
    llvm::Value *condition = builder->CreateLoad(conditionType, conditionPtr);
    return builder->CreateSelect(condition, trueValuePtr, falseValuePtr);
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitLogicalOrExpr(LogicalOrExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    std::string codeLine = node->codeLoc.toPrettyLine();
    // Prepare for short-circuiting
    std::pair<llvm::Value *, llvm::BasicBlock *> incomingBlocks[node->operands().size()];
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "lor.end." + codeLine);
    llvm::Function *parentFunction = builder->GetInsertBlock()->getParent();

    // Visit the first condition
    llvm::Value *lhs = resolveValue(node->operands().front());

    // Prepare the blocks
    incomingBlocks[0] = {lhs, builder->GetInsertBlock()};
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "lor." + std::to_string(i) + "." + codeLine);
      parentFunction->getBasicBlockList().push_back(bb);
      incomingBlocks[i] = {nullptr, bb};
    }
    createCondBr(lhs, bEnd, incomingBlocks[1].second);

    // Create a block for every other condition
    for (int i = 1; i < node->operands().size(); i++) {
      moveInsertPointToBlock(incomingBlocks[i].second);
      llvm::Value *rhs = resolveValue(node->operands()[i]);
      incomingBlocks[i].first = rhs;
      incomingBlocks[i].second = builder->GetInsertBlock();
      if (i < node->operands().size() - 1) {
        createCondBr(rhs, bEnd, incomingBlocks[i + 1].second);
      } else {
        createBr(bEnd);
      }
    }

    // Get the result with the phi node
    parentFunction->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);
    llvm::PHINode *phi = builder->CreatePHI(lhs->getType(), node->operands().size(), "lor_phi");
    for (const auto &incomingBlock : incomingBlocks)
      phi->addIncoming(incomingBlock.first, incomingBlock.second);

    // Store the result
    llvm::Value *resultPtr = insertAlloca(phi->getType());
    builder->CreateStore(phi, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitLogicalAndExpr(LogicalAndExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    std::string codeLine = node->codeLoc.toPrettyLine();
    // Prepare for short-circuiting
    std::pair<llvm::Value *, llvm::BasicBlock *> incomingBlocks[node->operands().size()];
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "land.end." + codeLine);
    llvm::Function *parentFunction = builder->GetInsertBlock()->getParent();

    // Visit the first condition
    llvm::Value *lhs = resolveValue(node->operands().front());

    // Prepare the blocks
    incomingBlocks[0] = {lhs, builder->GetInsertBlock()};
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "land." + std::to_string(i) + "." + codeLine);
      parentFunction->getBasicBlockList().push_back(bb);
      incomingBlocks[i] = {nullptr, bb};
    }
    createCondBr(lhs, incomingBlocks[1].second, bEnd);

    // Create a block for every other condition
    for (int i = 1; i < node->operands().size(); i++) {
      moveInsertPointToBlock(incomingBlocks[i].second);
      llvm::Value *rhs = resolveValue(node->operands()[i]);
      incomingBlocks[i].first = rhs;
      incomingBlocks[i].second = builder->GetInsertBlock();
      if (i < node->operands().size() - 1) {
        createCondBr(rhs, incomingBlocks[i + 1].second, bEnd);
      } else {
        createBr(bEnd);
      }
    }

    // Get the result with the phi node
    parentFunction->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);
    llvm::PHINode *phi = builder->CreatePHI(lhs->getType(), node->operands().size(), "land_phi");
    for (const auto &incomingBlock : incomingBlocks)
      phi->addIncoming(incomingBlock.first, incomingBlock.second);

    // Store the result
    llvm::Value *resultPtr = insertAlloca(phi->getType());
    builder->CreateStore(phi, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    BitwiseXorExprNode *lhsOperand = node->operands().front();
    llvm::Value *lhs = resolveValue(lhsOperand);
    for (int i = 1; i < node->operands().size(); i++) {
      BitwiseXorExprNode *rhsOperand = node->operands()[i];
      llvm::Value *rhs = resolveValue(rhsOperand);
      lhs = conversionsManager->getBitwiseOrInst(lhs, rhs, lhsOperand->getEvaluatedSymbolType(),
                                                 rhsOperand->getEvaluatedSymbolType());
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    BitwiseAndExprNode *lhsOperand = node->operands().front();
    llvm::Value *lhs = resolveValue(lhsOperand);
    for (int i = 1; i < node->operands().size(); i++) {
      BitwiseAndExprNode *rhsOperand = node->operands()[i];
      llvm::Value *rhs = resolveValue(rhsOperand);
      lhs = conversionsManager->getBitwiseXorInst(lhs, rhs, lhsOperand->getEvaluatedSymbolType(),
                                                  rhsOperand->getEvaluatedSymbolType());
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    EqualityExprNode *lhsOperand = node->operands().front();
    llvm::Value *lhs = resolveValue(lhsOperand);
    for (int i = 1; i < node->operands().size(); i++) {
      EqualityExprNode *rhsOperand = node->operands()[i];
      llvm::Value *rhs = resolveValue(rhsOperand);
      lhs = conversionsManager->getBitwiseAndInst(lhs, rhs, lhsOperand->getEvaluatedSymbolType(),
                                                  rhsOperand->getEvaluatedSymbolType());
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitEqualityExpr(EqualityExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    RelationalExprNode *lhsOperand = node->operands()[0];
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Value *lhs = resolveValue(lhsOperand);

    RelationalExprNode *rhsOperand = node->operands()[1];
    SymbolType rhsSymbolType = rhsOperand->getEvaluatedSymbolType();
    llvm::Value *rhs = resolveValue(rhsOperand);

    llvm::Value *result;
    switch (node->op) {
    case EqualityExprNode::OP_EQUAL:
      result = conversionsManager->getEqualInst(lhs, rhs, lhsSymbolType, rhsSymbolType, node->codeLoc);
      break;
    case EqualityExprNode::OP_NOT_EQUAL:
      result = conversionsManager->getNotEqualInst(lhs, rhs, lhsSymbolType, rhsSymbolType, node->codeLoc);
      break;
    default:
      throw std::runtime_error("Equality expr fall-through");
    }
    llvm::Value *resultPtr = insertAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitRelationalExpr(RelationalExprNode *node) {
  setSourceLocation(node);

  if (node->operands().size() > 1) {
    ShiftExprNode *lhsOperand = node->operands()[0];
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Value *lhs = resolveValue(lhsOperand);

    ShiftExprNode *rhsOperand = node->operands()[1];
    SymbolType rhsSymbolType = rhsOperand->getEvaluatedSymbolType();
    llvm::Value *rhs = resolveValue(rhsOperand);

    llvm::Value *result;
    switch (node->op) {
    case RelationalExprNode::OP_LESS:
      result = conversionsManager->getLessInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
      break;
    case RelationalExprNode::OP_GREATER:
      result = conversionsManager->getGreaterInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
      break;
    case RelationalExprNode::OP_LESS_EQUAL:
      result = conversionsManager->getLessEqualInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
      break;
    case RelationalExprNode::OP_GREATER_EQUAL:
      result = conversionsManager->getGreaterEqualInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
      break;
    default:
      throw std::runtime_error("Relational expr fall-through");
    }
    llvm::Value *resultPtr = insertAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitShiftExpr(ShiftExprNode *node) {
  setSourceLocation(node);

  // Check if there is a shift operation attached
  if (node->operands().size() > 1) {
    AdditiveExprNode *lhsOperand = node->operands()[0];
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Value *lhs = resolveValue(lhsOperand);

    AdditiveExprNode *rhsOperand = node->operands()[1];
    SymbolType rhsSymbolType = rhsOperand->getEvaluatedSymbolType();
    llvm::Value *rhs = resolveValue(rhsOperand);

    llvm::Value *result;
    switch (node->op) {
    case ShiftExprNode::OP_SHIFT_LEFT:
      result = conversionsManager->getShiftLeftInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
      break;
    case ShiftExprNode::OP_SHIFT_RIGHT:
      result = conversionsManager->getShiftRightInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
      break;
    default:
      throw std::runtime_error("Shift expr fall-through");
    }
    llvm::Value *resultPtr = insertAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitAdditiveExpr(AdditiveExprNode *node) {
  setSourceLocation(node);

  // Check if at least one additive operator is applied
  if (!node->opQueue.empty()) {
    MultiplicativeExprNode *lhsOperand = node->operands().front();
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Value *lhs = resolveValue(lhsOperand);

    auto opQueue = node->opQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      MultiplicativeExprNode *rhsOperand = node->operands()[operandIndex++];
      assert(rhsOperand != nullptr);
      SymbolType rhsSymbolType = rhsOperand->getEvaluatedSymbolType();

      llvm::Value *rhs;
      PtrAndValue result = {nullptr, nullptr};
      switch (opQueue.front().first) {
      case AdditiveExprNode::OP_PLUS:
        if (lhsSymbolType.isStringStruct()) {
          llvm::Value *lhsPtr = insertAlloca(lhs->getType());
          builder->CreateStore(lhs, lhsPtr);
          lhs = materializeString(lhsPtr);
        }
        rhs = rhsSymbolType.isStringStruct() ? materializeString(resolveAddress(rhsOperand)) : resolveValue(rhsOperand);
        result = conversionsManager->getPlusInst(lhs, rhs, lhsSymbolType, rhsSymbolType, currentScope, rhsOperand->codeLoc);
        break;
      case AdditiveExprNode::OP_MINUS:
        rhs = resolveValue(rhsOperand);
        lhs = conversionsManager->getMinusInst(lhs, rhs, lhsSymbolType, rhsSymbolType, currentScope);
        break;
      default:
        throw std::runtime_error("Additive expr fall-through");
      }

      lhsSymbolType = opQueue.front().second;

      if (result.ptr) {
        lhs = builder->CreateLoad(lhsSymbolType.toLLVMType(*context, currentScope), result.ptr);
      } else if (result.value) {
        lhs = result.value;
      }

      opQueue.pop();
    }

    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitMultiplicativeExpr(MultiplicativeExprNode *node) {
  setSourceLocation(node);

  // Check if at least one multiplicative operator is applied
  if (!node->opQueue.empty()) {
    CastExprNode *lhsOperand = node->operands().front();
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Value *lhs = resolveValue(lhsOperand);

    llvm::Value *resultPtr = nullptr;

    auto opQueue = node->opQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      CastExprNode *rhsOperand = node->operands()[operandIndex++];
      assert(rhsOperand != nullptr);
      SymbolType rhsSymbolType = rhsOperand->getEvaluatedSymbolType();
      llvm::Value *rhs = resolveValue(rhsOperand);

      PtrAndValue result = {nullptr, nullptr};
      switch (opQueue.front().first) {
      case MultiplicativeExprNode::OP_MUL: {
        llvm::Value *lhsPtr = resultPtr ?: resolveAddress(lhsOperand);
        llvm::Value *rhsPtr = resolveAddress(rhsOperand);
        result = conversionsManager->getMulInst({lhsPtr, lhs}, {rhsPtr, rhs}, lhsSymbolType, rhsSymbolType, currentScope,
                                                rhsOperand->codeLoc);
        break;
      }
      case MultiplicativeExprNode::OP_DIV:
        lhs = conversionsManager->getDivInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
        break;
      case MultiplicativeExprNode::OP_REM:
        lhs = conversionsManager->getRemInst(lhs, rhs, lhsSymbolType, rhsSymbolType);
        break;
      default:
        throw std::runtime_error("Multiplicative expr fall-through");
      }

      lhsSymbolType = opQueue.front().second;

      if (result.ptr) {
        resultPtr = result.ptr;
        lhs = builder->CreateLoad(lhsSymbolType.toLLVMType(*context, currentScope), resultPtr);
      } else if (result.value) {
        lhs = result.value;
      }

      opQueue.pop();
    }

    resultPtr = resultPtr ?: insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitCastExpr(CastExprNode *node) {
  setSourceLocation(node);

  if (node->isCasted) { // Cast operator is applied
    SymbolType lhsSymbolType = node->getEvaluatedSymbolType();

    PrefixUnaryExprNode *rhsOperand = node->prefixUnaryExpr();
    SymbolType rhsSymbolType = rhsOperand->getEvaluatedSymbolType();
    llvm::Value *rhs = resolveValue(rhsOperand);

    llvm::Value *result = conversionsManager->getCastInst(rhs, lhsSymbolType, rhsSymbolType, currentScope);

    llvm::Value *resultPtr = insertAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
  }

  return visit(node->prefixUnaryExpr());
}

std::any GeneratorVisitor::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  setSourceLocation(node);

  currentVarName = "";           // Reset the current variable name
  scopePath.clear();             // Clear the scope path
  structAccessAddress = nullptr; // Clear struct access address

  if (!node->opStack.empty()) {
    // Load the value
    PostfixUnaryExprNode *lhsOperand = node->postfixUnaryExpr();
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Type *lhsTy = lhsSymbolType.toLLVMType(*context, currentScope);
    llvm::Value *lhsPtr = resolveAddress(lhsOperand);
    llvm::Value *lhs = nullptr;

    bool isVolatile = false;
    bool storeValue = true;

    auto opStack = node->opStack;
    while (!opStack.empty()) {
      switch (opStack.top().first) {
      case PrefixUnaryExprNode::OP_MINUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        lhs = conversionsManager->getPrefixMinusInst(lhs, lhsSymbolType);
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PrefixUnaryExprNode::OP_PLUS_PLUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        lhs = conversionsManager->getPrefixPlusPlusInst(lhs, lhsSymbolType);

        // Store the new value volatile
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        if (!isVolatile && lhsVarEntry && lhsVarEntry->isVolatile)
          isVolatile = true;
        break;
      }
      case PrefixUnaryExprNode::OP_MINUS_MINUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        lhs = conversionsManager->getPrefixMinusMinusInst(lhs, lhsSymbolType);

        // Store the new value volatile
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        if (!isVolatile && lhsVarEntry && lhsVarEntry->isVolatile)
          isVolatile = true;
        break;
      }
      case PrefixUnaryExprNode::OP_NOT: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        lhs = conversionsManager->getPrefixNotInst(lhs, lhsSymbolType);
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PrefixUnaryExprNode::OP_BITWISE_NOT: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        lhs = conversionsManager->getPrefixBitwiseNotInst(lhs, lhsSymbolType);
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PrefixUnaryExprNode::OP_INDIRECTION: {
        if (!lhs) {
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
          lhsSymbolType = lhsSymbolType.getContainedTy();
          lhsTy = lhsSymbolType.toLLVMType(*context, currentScope);
        }
        // Indirect pointer
        lhsPtr = lhs;
        lhs = builder->CreateLoad(lhsTy, lhs);
        break;
      }
      case PrefixUnaryExprNode::OP_ADDRESS_OF: {
        // Create a reference
        lhs = lhsPtr;
        lhsPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, lhsPtr);
        storeValue = false;
        break;
      }
      default:
        throw std::runtime_error("PrefixUnary fall-through");
      }

      lhsSymbolType = opStack.top().second;
      lhsTy = lhsSymbolType.toLLVMType(*context, currentScope);
      opStack.pop();
    }

    // Store the value back again
    if (storeValue)
      builder->CreateStore(lhs, lhsPtr, isVolatile);

    return lhsPtr;
  }

  return visit(node->postfixUnaryExpr());
}

std::any GeneratorVisitor::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  setSourceLocation(node);

  if (!node->opQueue.empty()) {
    // Retrieve the address and the value if required
    AtomicExprNode *lhsOperand = node->atomicExpr();
    SymbolType lhsSymbolType = lhsOperand->getEvaluatedSymbolType();
    llvm::Type *lhsTy = lhsSymbolType.is(TY_IMPORT) ? nullptr : lhsSymbolType.toLLVMType(*context, currentScope);
    llvm::Value *lhsPtr = resolveAddress(lhsOperand);
    llvm::Value *lhs = nullptr;

    size_t subscriptCounter = 0;
    size_t memberAccessCounter = 0;

    auto opQueue = node->opQueue;
    while (!opQueue.empty()) {
      switch (opQueue.front().first) {
      case PostfixUnaryExprNode::OP_SUBSCRIPT: {

        // Save variables to restore later
        std::string currentVarNameBackup = currentVarName;
        ScopePath scopePathBackup = scopePath;
        llvm::Value *structAccessAddressBackup = structAccessAddress;
        llvm::Type *structAccessTypeBackup = structAccessType;

        // Get the index value
        AssignExprNode *indexExpr = node->assignExpr()[subscriptCounter++];
        llvm::Value *indexValue = resolveValue(indexExpr);

        // Restore variables
        currentVarName = currentVarNameBackup;
        scopePath = scopePathBackup;
        structAccessAddress = structAccessAddressBackup;
        structAccessType = structAccessTypeBackup;

        if (lhsSymbolType.isArray() && lhsSymbolType.getArraySize() > 0) { // Array
          lhsTy = lhsSymbolType.toLLVMType(*context, currentScope);
          // Calculate address of array item
          llvm::Value *indices[2] = {builder->getInt32(0), indexValue};
          lhsPtr = builder->CreateInBoundsGEP(lhsTy, lhsPtr, indices);
          // lhsPtr = builder->CreateInBoundsGEP(lhsTy, lhsPtr, indexValue);
          structAccessType = lhsSymbolType.getContainedTy().toLLVMType(*context, currentScope);
        } else { // Pointer
          lhsTy = lhsSymbolType.toLLVMType(*context, currentScope);
          lhsPtr = builder->CreateLoad(lhsTy, lhsPtr);
          lhsTy = lhsSymbolType.getContainedTy().toLLVMType(*context, currentScope);
          // Calculate address of pointer offset
          lhsPtr = builder->CreateInBoundsGEP(lhsTy, lhsPtr, indexValue);
          structAccessType = lhsTy;
        }
        structAccessAddress = lhsPtr;

        lhs = nullptr;
        break;
      }
      case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
        // Auto de-reference pointer
        while (lhsPtr && lhsSymbolType.isPointer()) {
          lhsTy = lhsSymbolType.toLLVMType(*context, currentScope);
          lhsPtr = structAccessAddress = builder->CreateLoad(lhsTy, lhsPtr);
          lhsSymbolType = lhsSymbolType.getContainedTy();
          structAccessType = lhsSymbolType.toLLVMType(*context, currentScope);
        }

        PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];

        // Propagate strings to string objects
        if (lhsSymbolType.is(TY_STRING)) {
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
          lhsPtr = conversionsManager->propagateValueToStringObject(currentScope, lhsSymbolType, lhsPtr, lhs, rhs->codeLoc);
          currentSymbolType = SymbolType(TY_STRUCT, std::string("String"));
          SymbolTable *stringRuntimeScope = sourceFile.getRuntimeModuleScope(STRING_RT);
          assert(stringRuntimeScope != nullptr);
          scopePath.clear();
          scopePath.pushFragment(STRING_RT_IMPORT_NAME, stringRuntimeScope);
        }
        currentThisValuePtr = structAccessAddress = lhsPtr;

        // Visit identifier after the dot
        lhsPtr = resolveAddress(rhs);

        lhs = nullptr;
        break;
      }
      case PostfixUnaryExprNode::OP_SCOPE_ACCESS: {
        // Visit identifier after the double colon
        PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];
        lhsPtr = resolveAddress(rhs);

        lhs = nullptr;
        break;
      }
      case PostfixUnaryExprNode::OP_PLUS_PLUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        // Get the lhs variable entry
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        // Save the new value to the old pointer
        llvm::Value *newLhsValue = conversionsManager->getPostfixPlusPlusInst(lhs, lhsSymbolType);
        builder->CreateStore(newLhsValue, lhsPtr, lhsVarEntry && lhsVarEntry->isVolatile);
        // Allocate new space and continue working with the new memory slot
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PostfixUnaryExprNode::OP_MINUS_MINUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsTy, lhsPtr);
        // Get the lhs variable entry
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        // Save the new value to the old pointer
        llvm::Value *newLhsValue = conversionsManager->getPostfixMinusMinusInst(lhs, lhsSymbolType);
        builder->CreateStore(newLhsValue, lhsPtr, lhsVarEntry && lhsVarEntry->isVolatile);
        // Allocate new space and continue working with the new memory slot
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      default:
        throw std::runtime_error("PostfixUnary fall-through");
      }

      lhsSymbolType = opQueue.front().second;
      opQueue.pop();
    }

    // Store the value back again
    if (lhs != nullptr)
      builder->CreateStore(lhs, lhsPtr);

    return lhsPtr;
  }

  return visit(node->atomicExpr());
}

std::any GeneratorVisitor::visitAtomicExpr(AtomicExprNode *node) {
  setSourceLocation(node);

  if (node->value())
    return visit(node->value());

  allArgsHardcoded = false; // To prevent arrays from being defined globally when depending on other values (vars, calls, etc.)

  if (!node->identifier.empty()) {
    currentVarName = node->identifier;

    // Retrieve access scope
    SymbolTable *accessScope = scopePath.isEmpty() ? currentScope : scopePath.getCurrentScope();
    bool importedScope = accessScope->isImported(currentScope);

    // Get identifier entry
    SymbolTableEntry *entry = accessScope->lookup(node->identifier);
    assert(entry != nullptr);

    // Import or enum
    if (entry->type.isOneOf({TY_IMPORT, TY_ENUM})) {
      std::string scopeName = entry->type.is(TY_ENUM) ? ENUM_SCOPE_PREFIX + node->identifier : node->identifier;
      SymbolTable *newScope = accessScope->lookupTable(scopeName);
      assert(newScope != nullptr);
      scopePath.pushFragment(node->identifier, newScope);
      return static_cast<llvm::Value *>(nullptr);
    }

    // Global variables
    if (entry->isGlobal) {
      // Initialize if it is an external global var
      if (importedScope)
        entry = initExtGlobal(node->identifier, scopePath.getScopePrefix(true) + node->identifier);

      // Return the address if modifiable
      if (!entry->specifiers.isConst())
        return entry->getAddress();

      // Save value of global to a new address to get it modifiable
      llvm::Type *globalTy = entry->type.toLLVMType(*context, accessScope);
      llvm::Value *globalValue = builder->CreateLoad(globalTy, entry->getAddress());
      llvm::Value *memoryAddress = insertAlloca(globalTy);
      builder->CreateStore(globalValue, memoryAddress);
      return memoryAddress;
    }

    // Struct or Struct* or Struct** or ...
    if (entry->type.isBaseType(TY_STRUCT)) {
      // Retrieve struct scope
      SymbolType entryBaseType = entry->type.getBaseType();
      std::string structSignature = Struct::getSignature(entryBaseType.getSubType(), entryBaseType.getTemplateTypes());
      SymbolTable *structScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
      assert(structScope != nullptr);
      scopePath.pushFragment(node->identifier, structScope);

      if (structAccessAddress == nullptr) {
        // Auto de-referencing is done by the dot operator
        structAccessType = node->getEvaluatedSymbolType().toLLVMType(*context, accessScope);
        structAccessAddress = entry->getAddress();
        assert(structAccessAddress != nullptr && structAccessType != nullptr);
      } else {
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(entry->orderIndex)};
        SymbolTableEntry *structEntry = accessScope->lookup(entry->type.getName());
        // Auto de-referencing is done by the dot operator
        structAccessAddress = builder->CreateInBoundsGEP(structAccessType, structAccessAddress, indices);
        structAccessType = node->getEvaluatedSymbolType().toLLVMType(*context, accessScope);
        assert(structAccessAddress != nullptr && structAccessType != nullptr);
      }
      return structAccessAddress;
    }

    // Other types
    if (accessScope->scopeType == SCOPE_STRUCT) { // Struct field
      // Retrieve field index
      llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(entry->orderIndex)};

      // Calculate field address
      llvm::Value *fieldAddress = builder->CreateInBoundsGEP(structAccessType, structAccessAddress, indices);

      structAccessAddress = nullptr;
      structAccessType = nullptr;
      return fieldAddress;
    }

    // Check if enum item
    if (accessScope->scopeType == SCOPE_ENUM) {
      auto itemNode = dynamic_cast<const EnumItemNode *>(entry->declNode);
      llvm::Type *llvmType = entry->type.toLLVMType(*context, currentScope);
      llvm::Value *memAddress = insertAlloca(llvmType);
      builder->CreateStore(builder->getInt32(itemNode->itemValue), memAddress);
      return memAddress;
    }

    // If the address is known, return it
    if (entry->getAddress())
      return entry->getAddress();

    // For the case that in the current scope there is a variable with the same name, but it is initialized later, so the
    // symbol above in the hierarchy is meant to be used.
    SymbolTableEntry *entryAbove = entry;
    while (entryAbove && entryAbove->getAddress() == nullptr && accessScope->parent) {
      accessScope = accessScope->parent;
      entryAbove = accessScope->lookup(currentVarName);
      assert(entryAbove != nullptr);
      if (entryAbove->getAddress())
        return entryAbove->getAddress();
    }

    // If no variable was found, that has a valid address => allocate space for the original entry
    llvm::Type *llvmType = entry->type.toLLVMType(*context, currentScope);
    llvm::Value *memAddress = insertAlloca(llvmType, lhsVarName);
    entry->updateAddress(memAddress);
    return memAddress;
  }

  if (node->printfCall())
    return visit(node->printfCall());

  if (node->sizeofCall())
    return visit(node->sizeofCall());

  if (node->lenCall())
    return visit(node->lenCall());

  if (node->tidCall())
    return visit(node->tidCall());

  if (node->joinCall())
    return visit(node->joinCall());

  return visit(node->assignExpr());
}

std::any GeneratorVisitor::visitValue(ValueNode *node) {
  setSourceLocation(node);

  // Primitive value
  if (node->primitiveValue()) {
    // Visit the primitive value
    currentConstValue = any_cast<llvm::Constant *>(visit(node->primitiveValue()));

    // If global variable value, return value immediately, because it is already a pointer
    if (currentScope == rootScope)
      return currentConstValue;

    // Return empty std::any to signalize the resolveValue and resolveAddress functions to take the currentConstValue at resolving
    return {};
  }

  // Function call
  if (node->functionCall())
    return visit(node->functionCall());

  // Array initialization
  if (node->arrayInitialization())
    return visit(node->arrayInitialization());

  // Struct instantiation
  if (node->structInstantiation())
    return visit(node->structInstantiation());

  if (node->isNil) {
    auto nilType = any_cast<llvm::Type *>(visit(node->nilType()));
    currentConstValue = llvm::Constant::getNullValue(nilType);

    // If global variable value, return value immediately, because it is already a pointer
    if (currentScope == rootScope)
      return currentConstValue;

    // Store the value to a tmp variable
    llvm::Value *llvmValuePtr = insertAlloca(currentConstValue->getType(), lhsVarName);
    builder->CreateStore(currentConstValue, llvmValuePtr);
    return llvmValuePtr;
  }

  return nullptr;
}

std::any GeneratorVisitor::visitPrimitiveValue(PrimitiveValueNode *node) {
  // Value is a double constant
  if (node->type == PrimitiveValueNode::TYPE_DOUBLE) {
    currentSymbolType = SymbolType(TY_DOUBLE);
    llvm::Constant *value = llvm::ConstantFP::get(*context, llvm::APFloat(node->compileTimeValue.doubleValue));
    return value;
  }

  // Value is an integer constant
  if (node->type == PrimitiveValueNode::TYPE_INT) {
    currentSymbolType = SymbolType(TY_INT);
    llvm::Type *intTy = builder->getInt32Ty();
    llvm::Constant *constant = currentConstSigned ? llvm::ConstantInt::getSigned(intTy, node->compileTimeValue.intValue)
                                                  : llvm::ConstantInt::get(intTy, node->compileTimeValue.intValue);
    return constant;
  }

  // Value is a short constant
  if (node->type == PrimitiveValueNode::TYPE_SHORT) {
    currentSymbolType = SymbolType(TY_SHORT);
    llvm::Type *shortTy = builder->getInt16Ty();
    llvm::Constant *constant = currentConstSigned ? llvm::ConstantInt::getSigned(shortTy, node->compileTimeValue.shortValue)
                                                  : llvm::ConstantInt::get(shortTy, node->compileTimeValue.shortValue);
    return constant;
  }

  // Value is a long constant
  if (node->type == PrimitiveValueNode::TYPE_LONG) {
    currentSymbolType = SymbolType(TY_LONG);
    llvm::Type *longTy = builder->getInt64Ty();
    llvm::Constant *constant = currentConstSigned ? llvm::ConstantInt::getSigned(longTy, node->compileTimeValue.longValue)
                                                  : llvm::ConstantInt::get(longTy, node->compileTimeValue.longValue);
    return constant;
  }

  // Value is a char constant
  if (node->type == PrimitiveValueNode::TYPE_CHAR) {
    currentSymbolType = SymbolType(TY_CHAR);
    llvm::Type *charTy = builder->getInt8Ty();
    llvm::Constant *constant = currentConstSigned ? llvm::ConstantInt::getSigned(charTy, node->compileTimeValue.charValue)
                                                  : llvm::ConstantInt::get(charTy, node->compileTimeValue.charValue);
    return constant;
  }

  // Value is a string constant
  if (node->type == PrimitiveValueNode::TYPE_STRING) {
    currentSymbolType = SymbolType(TY_STRING);
    std::string stringValue(node->compileTimeValue.stringValue);
    llvm::Constant *value = builder->CreateGlobalStringPtr(stringValue, "", 0, module.get());
    return value;
  }

  // Value is a boolean constant
  if (node->type == PrimitiveValueNode::TYPE_BOOL) {
    currentSymbolType = SymbolType(TY_BOOL);
    llvm::Constant *value = node->compileTimeValue.boolValue ? builder->getTrue() : builder->getFalse();
    return value;
  }

  throw std::runtime_error("Internal compiler error: Primitive data type generator fall-through"); // GCOV_EXCL_LINE
}

std::any GeneratorVisitor::visitFunctionCall(FunctionCallNode *node) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Get function access pointer
  Function *spiceFunc = currentScope->getFunctionAccessPointer(node->codeLoc);
  assert(spiceFunc != nullptr);
  bool isMethod = spiceFunc->isMethodFunction() || spiceFunc->isMethodProcedure();
  bool constructorCall = false;

  // Load the 'this' value if it is a pointer
  llvm::Value *thisValuePtr = currentThisValuePtr;
  SymbolType thisSymbolType = currentSymbolType;
  for (unsigned int i = 0; i < node->functionNameFragments.size(); i++) {
    std::string identifier = node->functionNameFragments[i];
    SymbolTableEntry *symbolEntry = accessScope->lookup(identifier);

    SymbolType symbolBaseType;
    if (symbolEntry != nullptr) {
      if (symbolEntry->type.getBaseType().is(TY_STRING)) {
        // Load string value
        llvm::Value *stringPtr = symbolEntry->getAddress();
        llvm::Value *string = builder->CreateLoad(symbolEntry->type.toLLVMType(*context, currentScope), stringPtr);
        // Propagate the string to a string object
        conversionsManager->propagateValueToStringObject(currentScope, symbolEntry->type, stringPtr, string, node->codeLoc);
        // Replace symbolEntry with anonymous entry
        symbolEntry = currentScope->lookupAnonymous(node->codeLoc);
        assert(symbolEntry != nullptr);
        symbolBaseType = SymbolType(TY_STRUCT, std::string("String"));
        accessScope = sourceFile.getRuntimeModuleScope(STRING_RT);
        assert(accessScope != nullptr);
      } else {
        symbolBaseType = symbolEntry->type.getBaseType();
      }
    }

    if (i < node->functionNameFragments.size() - 1) {
      if (!symbolEntry)
        throw IRError(node->codeLoc, REFERENCED_UNDEFINED_FUNCTION_IR,
                      "Symbol '" + scopePath.getScopePrefix() + identifier + "' was used before defined");
      thisValuePtr = symbolEntry->getAddress();
    } else if (symbolEntry != nullptr && symbolBaseType.is(TY_STRUCT)) {
      Struct *spiceStruct = currentScope->getStructAccessPointer(node->codeLoc);
      assert(spiceStruct != nullptr);

      // Check if the struct is defined
      symbolEntry = accessScope->lookup(spiceStruct->getSignature());
      assert(symbolEntry != nullptr);
      symbolBaseType = symbolEntry->type.getBaseType();

      // Get address of variable in memory
      if (lhsVarName.empty()) {
        llvm::Type *thisType = spiceFunc->getThisType().toLLVMType(*context, accessScope);
        thisValuePtr = insertAlloca(thisType);
      } else {
        SymbolTableEntry *assignVarEntry = currentScope->lookup(lhsVarName);
        assert(assignVarEntry != nullptr);
        if (assignVarEntry->getAddress() != nullptr) {
          thisValuePtr = assignVarEntry->getAddress();
        } else {
          llvm::Type *llvmType = assignVarEntry->type.toLLVMType(*context, currentScope);
          thisValuePtr = insertAlloca(llvmType, lhsVarName);
        }
      }

      constructorCall = true;
    } else {
      continue;
    }
    thisSymbolType = symbolEntry->type;

    std::string tableName = symbolEntry->type.is(TY_IMPORT) ? identifier : STRUCT_SCOPE_PREFIX + symbolBaseType.getName();
    accessScope = accessScope->lookupTable(tableName);
    assert(accessScope != nullptr);
    scopePath.pushFragment(identifier, accessScope);
  }

  // Load this value ptr if necessary
  while (isMethod && thisSymbolType.isPointer()) {
    thisValuePtr = builder->CreateLoad(thisSymbolType.toLLVMType(*context, accessScope), thisValuePtr);
    thisSymbolType = thisSymbolType.getContainedTy();
  }

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
    std::vector<SymbolType> argSymbolTypes = spiceFunc->getParamTypes();

    llvm::Type *returnType =
        returnSymbolType.is(TY_DYN) ? llvm::Type::getVoidTy(*context) : returnSymbolType.toLLVMType(*context, accessScope);

    std::vector<llvm::Type *> argTypes;
    if (isMethod)
      argTypes.push_back(thisSymbolType.toLLVMType(*context, accessScope)->getPointerTo());
    for (const auto &argSymbolType : argSymbolTypes)
      argTypes.push_back(argSymbolType.toLLVMType(*context, accessScope));

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
  if (node->argLst()) {
    for (const auto &arg : node->argLst()->args()) {
      // Get expected arg type
      SymbolType expectedArgSymbolType = spiceFunc->getParamTypes()[isMethod ? argIndex - 1 : argIndex];
      llvm::Type *expectedArgType = fctType->getParamType(argIndex);
      // Get the actual arg value
      SymbolType actualArgSymbolType = arg->getEvaluatedSymbolType();
      llvm::Value *actualArgPtr = resolveAddress(arg);

      // If the arrays are both of size -1 or 0, they are both pointers and do not need to be cast implicitly
      bool isSameArray = actualArgSymbolType.isArray() && expectedArgSymbolType.isArray() &&
                         actualArgSymbolType.getArraySize() <= 0 && expectedArgSymbolType.getArraySize() <= 0;

      if (actualArgSymbolType == expectedArgSymbolType || isSameArray) {
        actualArgPtr = builder->CreateLoad(actualArgSymbolType.toLLVMType(*context, accessScope), actualArgPtr);
      } else {
        actualArgPtr = doImplicitCast(actualArgPtr, expectedArgType, actualArgSymbolType);
      }
      argValues.push_back(actualArgPtr);
      argIndex++;
    }
  }

  // Create the function call
  llvm::Value *resultValue = builder->CreateCall(fct, argValues);

  SymbolType returnSymbolType = spiceFunc->getReturnType();
  if (returnSymbolType.isBaseType(TY_STRUCT)) {
    // Add struct scope to scope path
    std::string structSignature = Struct::getSignature(returnSymbolType.getSubType(), returnSymbolType.getTemplateTypes());
    SymbolTable *newAccessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
    scopePath.pushFragment(returnSymbolType.getSubType(), newAccessScope);
  }

  // Consider constructor calls
  if (constructorCall) {
    // Update mem-address of anonymous symbol
    SymbolTableEntry *anonEntry = currentScope->lookupAnonymous(node->codeLoc);
    assert(anonEntry != nullptr);
    anonEntry->updateAddress(thisValuePtr);

    // Return pointer to this value
    currentSymbolType = anonEntry->type;
    structAccessType = currentSymbolType.toLLVMType(*context, accessScope);
    return thisValuePtr;
  } else if (!resultValue->getType()->isSized()) {
    // Set return type bool for procedures
    resultValue = builder->getTrue();
  }

  llvm::Value *resultPtr = insertAlloca(resultValue->getType());
  builder->CreateStore(resultValue, resultPtr);
  return resultPtr;
}

std::any GeneratorVisitor::visitArrayInitialization(ArrayInitializationNode *node) {
  // Get data type
  size_t actualItemCount = node->itemLst() ? node->itemLst()->args().size() : 0;
  size_t arraySize = lhsType != nullptr && lhsType->isArrayTy() ? lhsType->getArrayNumElements() : actualItemCount;

  bool dynamicallySized = false;
  bool outermostArray = true;
  if (!arraySymbolType.is(TY_INVALID)) {
    dynamicallySized = arraySymbolType.is(TY_PTR) && arraySymbolType.getDynamicArraySize() != nullptr;
    outermostArray = false;
  } else if (!lhsVarName.empty()) {
    SymbolTableEntry *arrayEntry = currentScope->lookupStrict(lhsVarName);
    assert(arrayEntry != nullptr);
    arraySymbolType = arrayEntry->type;
    dynamicallySized = arraySymbolType.isPointer() && arraySymbolType.getDynamicArraySize() != nullptr;
    if (dynamicallySized)
      dynamicArraySize = arraySymbolType.getDynamicArraySize();
  } else {
    arraySymbolType = node->getEvaluatedSymbolType();
  }

  SymbolType itemSymbolType = arraySymbolType.getContainedTy();
  llvm::Type *arrayType = arraySymbolType.toLLVMType(*context, currentScope);
  llvm::Type *itemType = itemSymbolType.toLLVMType(*context, currentScope);

  std::vector<llvm::Value *> itemValues;
  std::vector<llvm::Constant *> itemConstants;
  itemValues.reserve(arraySize);
  itemConstants.reserve(arraySize);

  bool toggledConstantArray = false;
  if (!withinConstantArray)
    withinConstantArray = toggledConstantArray = true;

  if (node->itemLst()) { // The array is initialized with values
    // Set the lhs type to the item type
    std::string lhsVarNameBackup = lhsVarName;
    llvm::Type *lhsTypeBackup = lhsType;
    lhsType = itemSymbolType.toLLVMType(*context, currentScope);
    SymbolType arraySymbolTypeBackup = arraySymbolType;
    arraySymbolType = arraySymbolType.getContainedTy();
    llvm::Value *dynamicArraySizeBackup = dynamicArraySize;
    dynamicArraySize = nullptr;

    // Visit all args to check if they are hardcoded or not
    allArgsHardcoded = true;
    for (size_t i = 0; i < std::min(actualItemCount, arraySize); i++) {
      currentConstValue = nullptr;
      lhsVarName = lhsVarNameBackup + "." + std::to_string(i);

      if (arraySymbolType.isPointer())
        dynamicArraySize = arraySymbolType.getDynamicArraySize();

      AssignExprNode *arg = node->itemLst()->args()[i];
      llvm::Value *itemValue = resolveValue(arg);

      itemValues.push_back(itemValue);
      itemConstants.push_back(currentConstValue);
    }

    // Restore lhs type
    lhsVarName = lhsVarNameBackup;
    lhsType = lhsTypeBackup;
    arraySymbolType = arraySymbolTypeBackup;
    dynamicArraySize = dynamicArraySizeBackup;
  }

  if (toggledConstantArray)
    withinConstantArray = false;
  if (outermostArray)
    arraySymbolType = SymbolType(TY_INVALID);
  currentConstValue = nullptr;

  // Decide if the array can be defined globally
  if (allArgsHardcoded && !dynamicallySized && !itemType->isStructTy()) { // Global array is possible
    // Fill up the rest of the items with default values
    if (itemConstants.size() < arraySize) {
      llvm::Constant *constantValue = getDefaultValueForSymbolType(itemSymbolType);
      for (size_t i = itemConstants.size(); i < arraySize; i++)
        itemConstants.push_back(constantValue);
    }

    // Create hardcoded array
    assert(arrayType != nullptr);
    auto type = reinterpret_cast<llvm::ArrayType *>(arrayType);
    currentConstValue = llvm::ConstantArray::get(type, itemConstants);

    if (withinConstantArray)
      return {};

    return createGlobalArray(currentConstValue);
  } else { // Global array is not possible => fallback to individual indexing
    allArgsHardcoded = false;

    // Allocate array
    llvm::Value *arrayAddress;
    if (dynamicallySized) {
      arrayAddress = allocateDynamicallySizedArray(itemType);
      arrayType = itemSymbolType.toLLVMType(*context, currentScope);
    } else {
      arrayAddress = insertAlloca(arrayType);
    }

    // Insert all given values
    for (size_t valueIndex = 0; valueIndex < arraySize; valueIndex++) {
      // Calculate item address
      llvm::Value *itemAddress;
      if (arrayType->isArrayTy()) {
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(valueIndex)};
        itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, indices);
      } else {
        itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, builder->getInt32(valueIndex));
      }

      // Store item value to item address
      llvm::Value *itemValue;
      if (node->itemLst() && valueIndex < node->itemLst()->args().size()) {
        itemValue = itemValues[valueIndex];
      } else {
        itemValue = getDefaultValueForSymbolType(itemSymbolType);
      }
      builder->CreateStore(itemValue, itemAddress);
    }

    if (!dynamicallySized)
      return arrayAddress;

    // Save value to address
    llvm::Value *newArrayAddress = insertAlloca(arrayAddress->getType(), lhsVarName);
    builder->CreateStore(arrayAddress, newArrayAddress);
    return newArrayAddress;
  }
}

std::any GeneratorVisitor::visitStructInstantiation(StructInstantiationNode *node) {
  // Get struct name in format a.b.c and struct scope
  std::string structName;
  SymbolTable *structScope = currentScope;
  for (unsigned int i = 0; i < node->structNameFragments.size(); i++) {
    std::string identifier = node->structNameFragments[i];
    structName += structName.empty() ? identifier : "." + identifier;
    if (i < node->structNameFragments.size() - 1) {
      SymbolTableEntry *entry = structScope->lookup(identifier);
      assert(entry != nullptr);
      if (entry->type.is(TY_IMPORT)) {
        structScope = structScope->lookupTable(identifier);
      } else if (entry->type.is(TY_STRUCT)) {
        structScope = structScope->lookupTable(STRUCT_SCOPE_PREFIX + identifier);
      }
    }
  }

  // Get struct from struct access pointer
  Struct *spiceStruct = currentScope->getStructAccessPointer(node->codeLoc);
  assert(spiceStruct);

  // Check if the struct is defined
  SymbolTableEntry *structSymbol = structScope->lookup(spiceStruct->getSignature());
  assert(structSymbol);
  llvm::Type *structType = structSymbol->type.toLLVMType(*context, structScope);

  // Allocate space for the struct in memory
  llvm::Value *structAddress = insertAlloca(structType);

  // Get struct table
  SymbolTable *structTable = structScope->lookupTable(STRUCT_SCOPE_PREFIX + spiceStruct->getSignature());
  assert(structTable);

  // Fill the struct with the stated values
  if (node->fieldLst()) {
    for (unsigned int i = 0; i < node->fieldLst()->args().size(); i++) {
      // Set address to the struct instance field
      SymbolTableEntry *fieldEntry = structTable->lookupByIndex(i);
      assert(fieldEntry);
      // Visit assignment
      llvm::Value *assignment = resolveValue(node->fieldLst()->args()[i]);
      // Get pointer to struct element
      llvm::Value *fieldAddress = builder->CreateStructGEP(structType, structAddress, i);
      fieldEntry->updateAddress(fieldAddress);
      // Store value to address
      builder->CreateStore(assignment, fieldAddress);
    }
  }

  // Update the address of the anonymous symbol
  SymbolTableEntry *anonSymbol = currentScope->lookupAnonymous(node->codeLoc);
  assert(anonSymbol != nullptr);
  anonSymbol->updateAddress(structAddress);

  return structAddress;
}

std::any GeneratorVisitor::visitDataType(DataTypeNode *node) {
  setSourceLocation(node);

  SymbolType symbolType = node->getEvaluatedSymbolType();
  if (symbolType.is(TY_DYN)) {
    SymbolTableEntry *lhsVarEntry = currentScope->lookup(lhsVarName);
    assert(lhsVarEntry != nullptr);
    currentSymbolType = lhsVarEntry->type;
    return currentSymbolType.toLLVMType(*context, currentScope);
  }

  if (!node->arraySizeExpr().empty()) {
    // Reset to base type because it will be rebuilt in the next step
    symbolType = symbolType.getBaseType();

    size_t assignExprCounter = 0;
    std::vector<AssignExprNode *> arraySizeExpr = node->arraySizeExpr();
    std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
    while (!tmQueue.empty()) {
      DataTypeNode::TypeModifier typeModifier = tmQueue.front();
      switch (typeModifier.modifierType) {
      case DataTypeNode::TYPE_PTR: {
        symbolType = symbolType.toPointer(node->codeLoc);
        break;
      }
      case DataTypeNode::TYPE_ARRAY: {
        if (!typeModifier.hasSize) {
          symbolType = symbolType.toPointer(node->codeLoc);
        } else if (typeModifier.isSizeHardcoded) {
          symbolType = symbolType.toArray(node->codeLoc, typeModifier.hardcodedSize);
        } else {
          AssignExprNode *indexExpr = arraySizeExpr[assignExprCounter++];
          assert(indexExpr != nullptr);
          auto sizeValuePtr = any_cast<llvm::Value *>(visit(indexExpr));
          llvm::Type *sizeType = indexExpr->getEvaluatedSymbolType().toLLVMType(*context, currentScope);
          dynamicArraySize = builder->CreateLoad(sizeType, sizeValuePtr);
          symbolType = symbolType.toPointer(node->codeLoc, dynamicArraySize);
        }
        break;
      }
      default:
        throw std::runtime_error("Modifier type fall-through");
      }
      tmQueue.pop();
    }
  }
  currentSymbolType = symbolType;
  return currentSymbolType.toLLVMType(*context, currentScope);
}

llvm::Value *GeneratorVisitor::resolveValue(AstNode *node, SymbolTable *accessScope) {
  if (!accessScope)
    accessScope = currentScope;

  std::any valueAny = visit(node);

  if (!valueAny.has_value() && currentConstValue)
    return currentConstValue;

  auto valueAddr = any_cast<llvm::Value *>(valueAny);
  llvm::Type *dstType = node->getEvaluatedSymbolType().toLLVMType(*context, accessScope);
  return builder->CreateLoad(dstType, valueAddr);
}

llvm::Value *GeneratorVisitor::resolveAddress(AstNode *node, bool storeVolatile) {
  std::any valueAny = visit(node);

  if (!valueAny.has_value() && currentConstValue) {
    llvm::Value *valueAddr = insertAlloca(currentConstValue->getType(), lhsVarName);
    builder->CreateStore(currentConstValue, valueAddr, storeVolatile);
    return valueAddr;
  }

  return any_cast<llvm::Value *>(valueAny);
}

void GeneratorVisitor::moveInsertPointToBlock(llvm::BasicBlock *block) {
  builder->SetInsertPoint(block);
  blockAlreadyTerminated = false;
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

llvm::Value *GeneratorVisitor::insertAlloca(llvm::Type *llvmType, const std::string &varName) {
  if (allocaInsertInst != nullptr) {
    llvm::AllocaInst *allocaInst = builder->CreateAlloca(llvmType, nullptr, varName);
    allocaInst->setDebugLoc(llvm::DebugLoc());
    allocaInst->moveAfter(allocaInsertInst);
    allocaInsertInst = allocaInst;
  } else {
    // Save current basic block and move insert cursor to entry block of the current function
    llvm::BasicBlock *currentBlock = builder->GetInsertBlock();
    builder->SetInsertPoint(allocaInsertBlock);

    allocaInsertInst = builder->CreateAlloca(llvmType, nullptr, varName);
    allocaInsertInst->setDebugLoc(llvm::DebugLoc());

    // Restore old basic block
    builder->SetInsertPoint(currentBlock);
  }
  return static_cast<llvm::Value *>(allocaInsertInst);
}

llvm::Value *GeneratorVisitor::allocateDynamicallySizedArray(llvm::Type *itemType) {
  // Call llvm.stacksave intrinsic
  llvm::Function *stackSaveFct = stdFunctionManager->getStackSaveIntrinsic();
  if (stackState == nullptr)
    stackState = builder->CreateCall(stackSaveFct);
  // Allocate array
  llvm::Value *memAddress = builder->CreateAlloca(itemType, dynamicArraySize); // Intentionally not via insertAlloca
  dynamicArraySize = nullptr;
  return memAddress;
}

llvm::Value *GeneratorVisitor::createGlobalArray(llvm::Constant *constArray) {
  // Find an unused global name
  std::string globalVarName = lhsVarName;
  if (globalVarName.empty() || module->getNamedGlobal(globalVarName) != nullptr) {
    unsigned int suffixNumber = 0;
    do {
      globalVarName = "anonymous." + std::to_string(suffixNumber);
      suffixNumber++;
    } while (module->getNamedGlobal(globalVarName) != nullptr);
  }

  // Create global variable
  llvm::Value *memAddress = module->getOrInsertGlobal(globalVarName, constArray->getType());
  llvm::GlobalVariable *global = module->getNamedGlobal(globalVarName);
  global->setInitializer(constArray);

  return memAddress;
}

bool GeneratorVisitor::insertDestructorCall(const CodeLoc &codeLoc, SymbolTableEntry *varEntry) {
  if (varEntry->type.isStringStruct()) {
    // Get dtor function
    llvm::Function *fct = stdFunctionManager->getStringDtorFct();

    // Get this value pointer
    llvm::Value *thisValuePtr = varEntry->getAddress();
    assert(thisValuePtr != nullptr);

    // Insert call
    builder->CreateCall(fct, thisValuePtr);
  } else {
    Function *spiceDtor = currentScope->getFunctionAccessPointer(codeLoc, DTOR_VARIABLE_NAME);

    // Cancel if no destructor was found
    if (spiceDtor == nullptr)
      return false;

    // Get this value pointer
    llvm::Value *thisValuePtr = varEntry->getAddress();

    // Check if function exists in the current module
    bool functionFound = false;
    std::string mangledName = spiceDtor->getMangledName();
    for (const auto &function : module->getFunctionList()) { // Search for function definition
      llvm::StringRef functionName = function.getName();
      if (functionName == mangledName) {
        functionFound = true;
        break;
      } else if (functionName == spiceDtor->getName()) {
        functionFound = true;
        mangledName = spiceDtor->getName();
        break;
      }
    }

    if (!functionFound) { // Not found => Declare function, which will be linked in
      llvm::FunctionType *fctType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), thisValuePtr->getType(), false);
      module->getOrInsertFunction(mangledName, fctType);
    }

    // Get the declared function and its type
    llvm::Function *fct = module->getFunction(mangledName);
    assert(fct != nullptr);

    // Create the function call
    builder->CreateCall(fct, thisValuePtr);
  }
  return true;
}

llvm::Value *GeneratorVisitor::materializeString(llvm::Value *stringStructPtr) {
  llvm::Value *rawStringValue = builder->CreateCall(stdFunctionManager->getStringGetRawFct(), stringStructPtr);
  return rawStringValue;
}

llvm::Constant *GeneratorVisitor::getDefaultValueForSymbolType(const SymbolType &symbolType) {
  // Double
  if (symbolType.is(TY_DOUBLE))
    return currentConstValue = llvm::ConstantFP::get(*context, llvm::APFloat(0.0));

  // Int
  if (symbolType.is(TY_INT))
    return currentConstValue = builder->getInt32(0);

  // Short
  if (symbolType.is(TY_SHORT))
    return currentConstValue = builder->getInt16(0);

  // Long
  if (symbolType.is(TY_LONG))
    return currentConstValue = builder->getInt64(0);

  // Byte or char
  if (symbolType.isOneOf({TY_BYTE, TY_CHAR}))
    return currentConstValue = builder->getInt8(0);

  // String
  if (symbolType.is(TY_STRING))
    return currentConstValue = builder->CreateGlobalStringPtr("", "", 0, module.get());

  // Bool
  if (symbolType.is(TY_BOOL))
    return currentConstValue = builder->getFalse();

  // Pointer
  if (symbolType.is(TY_PTR)) {
    llvm::Type *baseType = symbolType.getContainedTy().toLLVMType(*context, currentScope);
    return currentConstValue = llvm::Constant::getNullValue(baseType->getPointerTo());
  }

  // Array
  if (symbolType.is(TY_ARRAY)) {
    size_t arraySize = symbolType.getArraySize();

    llvm::Type *itemType = symbolType.getContainedTy().toLLVMType(*context, currentScope);
    llvm::ArrayType *arrayType = llvm::ArrayType::get(itemType, arraySize);
    llvm::Constant *zeroItem = getDefaultValueForSymbolType(symbolType.getContainedTy());
    std::vector<llvm::Constant *> itemConstants(arraySize, zeroItem);

    return llvm::ConstantArray::get(arrayType, itemConstants);
  }

  // Struct
  if (symbolType.is(TY_STRUCT)) {
    // Get struct entry
    std::string structName = symbolType.getSubType();
    SymbolTableEntry *structEntry = currentScope->lookup(structName);
    assert(structEntry != nullptr);

    // Get struct table
    SymbolTable *structScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structName);
    assert(structScope != nullptr);

    size_t fieldCount = structScope->getFieldCount();
    auto structType = reinterpret_cast<llvm::StructType *>(structEntry->type.toLLVMType(*context, structScope));

    // Allocate space for the struct in memory
    llvm::Value *structAddress = insertAlloca(structType);

    std::vector<llvm::Constant *> fieldConstants;
    fieldConstants.reserve(fieldCount);
    for (int i = 0; i < fieldCount; i++) {
      SymbolTableEntry *fieldEntry = structScope->lookupByIndex(i);
      assert(fieldEntry != nullptr);

      // Retrieve default field value
      llvm::Constant *defaultFieldValue = getDefaultValueForSymbolType(fieldEntry->type);

      // Get pointer to struct element
      llvm::Value *fieldAddress = builder->CreateStructGEP(structType, structAddress, i);
      fieldEntry->updateAddress(fieldAddress);
      // Store value to address
      builder->CreateStore(defaultFieldValue, fieldAddress);

      fieldConstants.push_back(defaultFieldValue);
    }

    return llvm::ConstantStruct::get(structType, fieldConstants);
  }

  throw std::runtime_error("Internal compiler error: Cannot determine default value for type"); // GCOV_EXCL_LINE
}

SymbolTableEntry *GeneratorVisitor::initExtGlobal(const std::string &globalName, const std::string &fqGlobalName) {
  Capture *capture = currentScope->lookupCapture(fqGlobalName);
  SymbolTableEntry *global = capture->capturedEntry;

  // Check if the entry is already initialized
  if (capture->getState() == INITIALIZED)
    return global;

  // Declare the global also in the current module and update the address of the symbol accordingly
  llvm::Value *memAddress = module->getOrInsertGlobal(globalName, global->type.toLLVMType(*context, currentScope));
  global->updateAddress(memAddress);

  return global;
}

llvm::Value *GeneratorVisitor::doImplicitCast(llvm::Value *src, llvm::Type *dstTy, SymbolType srcType) {
  // Unpack the pointers until a pointer of another type is met
  unsigned int loadCounter = 0;
  while (srcType.isPointer()) {
    src = builder->CreateLoad(srcType.toLLVMType(*context, currentScope), src);
    srcType = srcType.getContainedTy();
    loadCounter++;
  }
  // GEP or bit-cast
  if (dstTy->isPointerTy() && srcType.isArray()) {
    llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(0)};
    src = builder->CreateInBoundsGEP(srcType.toLLVMType(*context, currentScope), src, indices);
  } else {
    src = builder->CreateLoad(srcType.toLLVMType(*context, currentScope), src);
    src = builder->CreateBitCast(src, dstTy);
  }
  // Pack the pointers together again
  for (; loadCounter > 0; loadCounter--) {
    llvm::Value *newActualArg = insertAlloca(src->getType());
    builder->CreateStore(src, newActualArg);
    src = newActualArg;
  }
  return src;
}

void GeneratorVisitor::initializeDIBuilder(const std::string &sourceFileName, const std::string &sourceFileDir) {
  std::string producerString = "spice version " + std::string(SPICE_VERSION);
  // Create DIBuilder
  diBuilder = std::make_unique<llvm::DIBuilder>(*module);
  // Create compilation unit
  debugInfo.diFile = diBuilder->createFile(sourceFileName, sourceFileDir);
  debugInfo.compileUnit = diBuilder->createCompileUnit(llvm::dwarf::DW_LANG_C, debugInfo.diFile, producerString,
                                                       cliOptions.optLevel > 0, "", 0, "", llvm::DICompileUnit::FullDebug, 0,
                                                       false, false, llvm::DICompileUnit::DebugNameTableKind::None);
  // Initialize primitive types
  debugInfo.doubleTy = diBuilder->createBasicType("double", 64, llvm::dwarf::DW_ATE_float);
  debugInfo.intTy = diBuilder->createBasicType("int", 32, llvm::dwarf::DW_ATE_signed);
  debugInfo.uIntTy = diBuilder->createBasicType("unsigned int", 32, llvm::dwarf::DW_ATE_unsigned);
  debugInfo.shortTy = diBuilder->createBasicType("short", 16, llvm::dwarf::DW_ATE_signed);
  debugInfo.uShortTy = diBuilder->createBasicType("unsigned short", 16, llvm::dwarf::DW_ATE_unsigned);
  debugInfo.longTy = diBuilder->createBasicType("long", 64, llvm::dwarf::DW_ATE_signed);
  debugInfo.uLongTy = diBuilder->createBasicType("unsigned long", 64, llvm::dwarf::DW_ATE_unsigned);
  debugInfo.byteTy = diBuilder->createBasicType("byte", 8, llvm::dwarf::DW_ATE_signed);
  debugInfo.uByteTy = diBuilder->createBasicType("unsigned byte", 8, llvm::dwarf::DW_ATE_unsigned);
  debugInfo.charTy = diBuilder->createBasicType("char", 8, llvm::dwarf::DW_ATE_signed_char);
  debugInfo.uCharTy = diBuilder->createBasicType("unsigned char", 8, llvm::dwarf::DW_ATE_unsigned_char);
  debugInfo.stringTy = diBuilder->createBasicType("string", 8, llvm::dwarf::DW_ATE_ASCII);
  debugInfo.boolTy = diBuilder->createBasicType("bool", 1, llvm::dwarf::DW_ATE_boolean);
}

llvm::DIType *GeneratorVisitor::getDITypeForSymbolType(const SymbolType &symbolType) const {
  if (symbolType.isPointer()) { // Pointer type
    llvm::DIType *pointeeTy = getDITypeForSymbolType(symbolType.getContainedTy());
    unsigned int pointerWidth = module->getDataLayout().getPointerSizeInBits();
    return diBuilder->createPointerType(pointeeTy, pointerWidth);
  }

  if (symbolType.isArray()) { // Array type
    llvm::DIType *itemTy = getDITypeForSymbolType(symbolType.getContainedTy());
    size_t size = symbolType.getArraySize();
    llvm::DINodeArray subscripts = diBuilder->getOrCreateArray({});
    return diBuilder->createArrayType(size, 0, itemTy, subscripts);
  }

  // Primitive types
  if (symbolType.is(TY_DOUBLE))
    return debugInfo.doubleTy;
  if (symbolType.is(TY_INT))
    return symbolType.isBaseTypeSigned ? debugInfo.intTy : debugInfo.uIntTy;
  if (symbolType.is(TY_SHORT))
    return symbolType.isBaseTypeSigned ? debugInfo.shortTy : debugInfo.uShortTy;
  if (symbolType.is(TY_LONG))
    return symbolType.isBaseTypeSigned ? debugInfo.longTy : debugInfo.uLongTy;
  if (symbolType.is(TY_BYTE))
    return symbolType.isBaseTypeSigned ? debugInfo.byteTy : debugInfo.uByteTy;
  if (symbolType.is(TY_CHAR))
    return symbolType.isBaseTypeSigned ? debugInfo.charTy : debugInfo.uCharTy;
  if (symbolType.is(TY_STRING))
    return debugInfo.stringTy;
  if (symbolType.is(TY_BOOL))
    return debugInfo.boolTy;

  return nullptr;
}

void GeneratorVisitor::generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc) {
  // Create function type
  std::vector<llvm::Metadata *> argTypes;
  argTypes.push_back(getDITypeForSymbolType(spiceFunc->getReturnType())); // Add result type
  for (const auto &argType : spiceFunc->getParamTypes())                  // Add arg types
    argTypes.push_back(getDITypeForSymbolType(argType));
  llvm::DISubroutineType *functionTy = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(argTypes));

  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  size_t lineNumber = spiceFunc->getDeclCodeLoc().line;

  llvm::DISubprogram *subprogram =
      diBuilder->createFunction(unit, spiceFunc->getName(), spiceFunc->getMangledName(), unit, lineNumber, functionTy, lineNumber,
                                llvm::DINode::FlagZero, llvm::DISubprogram::SPFlagDefinition);

  // Add debug info to LLVM function
  llvmFunction->setSubprogram(subprogram);
  // Add scope to lexicalBlocks
  debugInfo.lexicalBlocks.push(subprogram);
}

/*llvm::DIType *GeneratorVisitor::generateStructDebugInfo(llvm::StructType *llvmStructTy, const Struct *spiceStruct) const {
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  size_t lineNumber = spiceStruct->getDeclCodeLoc().line;
  size_t sizeInBits = module->getDataLayout().getTypeSizeInBits(llvmStructTy);
  llvm::DINode::DIFlags flags = spiceStruct->getSpecifiers().isPublic() ? llvm::DINode::FlagPublic : llvm::DINode::FlagPrivate;
  llvm::DINodeArray elements = diBuilder->getOrCreateArray({}); // ToDo: fill
  return diBuilder->createStructType(unit, spiceStruct->getName(), unit, lineNumber, sizeInBits, 0, flags, nullptr, elements);
}*/

void GeneratorVisitor::generateGlobalVarDebugInfo(llvm::GlobalVariable *global, const SymbolTableEntry *globalEntry) {
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  size_t lineNumber = globalEntry->getDeclCodeLoc().line;
  llvm::StringRef name = global->getName();
  llvm::DIType *type = getDITypeForSymbolType(globalEntry->type);
  bool isLocal = globalEntry->specifiers.isPublic();

  global->addDebugInfo(diBuilder->createGlobalVariableExpression(unit, name, name, unit, lineNumber, type, isLocal));
}

void GeneratorVisitor::generateDeclDebugInfo(const CodeLoc &codeLoc, const std::string &varName, llvm::Value *address,
                                             bool moveToPrev) {
  if (!cliOptions.generateDebugInfo)
    return;
  // Get symbol table entry
  SymbolTableEntry *variableEntry = currentScope->lookup(varName);
  assert(variableEntry != nullptr);
  // Build debug info
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  llvm::DIScope *scope = debugInfo.lexicalBlocks.top();
  llvm::DIType *diType = getDITypeForSymbolType(variableEntry->type);
  llvm::DILocalVariable *varInfo = diBuilder->createAutoVariable(scope, varName, unit, codeLoc.line, diType);
  llvm::DIExpression *expr = diBuilder->createExpression();
  auto inst = diBuilder->insertDeclare(address, varInfo, expr, builder->getCurrentDebugLocation(), allocaInsertBlock);
  if (moveToPrev)
    inst->moveBefore(builder->GetInsertPoint()->getPrevNonDebugInstruction());
}

void GeneratorVisitor::setSourceLocation(AstNode *node) {
  if (!cliOptions.generateDebugInfo)
    return;

  if (debugInfo.lexicalBlocks.empty()) {
    builder->SetCurrentDebugLocation(llvm::DebugLoc());
    return;
  }

  llvm::DIScope *scope = debugInfo.lexicalBlocks.top();
  auto codeLoc = llvm::DILocation::get(scope->getContext(), node->codeLoc.line, node->codeLoc.col, scope);
  builder->SetCurrentDebugLocation(codeLoc);
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
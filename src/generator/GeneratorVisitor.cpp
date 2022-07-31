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
#include <util/CommonUtil.h>
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
  this->err = std::make_unique<ErrorFactory>();

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

  // Initialize debug info generator
  if (cliOptions.generateDebugInfo) {
    module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", llvm::dwarf::DWARF_VERSION);
    module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);
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

void GeneratorVisitor::dumpAsm() {
  llvm::legacy::PassManager passManager;
  if (targetMachine->addPassesToEmitFile(passManager, llvm::outs(), nullptr, llvm::CGFT_AssemblyFile))
    throw err->get(WRONG_TYPE, "Target machine can't emit a file of this type"); // GCOV_EXCL_LINE

  // Emit object file
  passManager.run(*module);
  llvm::outs().flush();
}

std::any GeneratorVisitor::visitEntry(EntryNode *node) {
  AstVisitor::visitEntry(node);

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
      throw err->get(node->codeLoc, INVALID_MODULE, oss.str());
  }

  return false;
}

std::any GeneratorVisitor::visitMainFctDef(MainFctDefNode *node) {
  if (!secondRun)
    return nullptr;

  if (requiresMainFct) { // Only create main function when it is required
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
        SymbolTableEntry *argSymbol = currentScope->lookup(currentVarName);
        assert(argSymbol != nullptr);
        currentConstSigned = argSymbol->getSpecifiers().isSigned();
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
      std::vector<std::pair<SymbolType, bool>> argTypes;
      for (auto &argName : argNames) {
        SymbolTableEntry *argEntry = currentScope->lookup(argName);
        assert(argEntry != nullptr);
        argTypes.emplace_back(argEntry->getType(), true);
      }
      // Build spice function
      SymbolSpecifiers specifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
      Function spiceFunc("main", specifiers, SymbolType(TY_DYN), SymbolType(TY_INT), argTypes, {}, node->codeLoc);
      // Add debug info
      generateFunctionDebugInfo(fct, &spiceFunc);
    }

    // Create entry block
    llvm::BasicBlock *bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
    fct->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);
    allocaInsertInst = nullptr;

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
        llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup");
        moveInsertPointToBlock(bCleanup);

        // Generate cleanup instructions (e.g. dtor calls)
        bool destructorCalled = false;
        for (SymbolTableEntry *varEntry : varsToDestruct)
          destructorCalled |= insertDestructorCall(node->codeLoc, varEntry);

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
      if (stackState != nullptr)
        builder->CreateCall(retrieveStackRestoreFct(), {stackState});

      // Create return stmt
      llvm::Value *result = returnSymbol->getAddress();
      builder->CreateRet(builder->CreateLoad(result->getType()->getPointerElementType(), result));
    }

    // Conclude debug information
    if (cliOptions.generateDebugInfo)
      debugInfo.lexicalBlocks.pop_back();

    // Verify function
    if (!cliOptions.disableVerifier && !cliOptions.generateDebugInfo) { // Verifying while generating debug info throws errors
      std::string output;
      llvm::raw_string_ostream oss(output);
      if (llvm::verifyFunction(*fct, &oss))
        throw err->get(node->codeLoc, INVALID_FUNCTION, oss.str());
    }

    // Change scope back
    currentScope = currentScope->getParent();
    assert(currentScope != nullptr);
  }

  return nullptr;
}

std::any GeneratorVisitor::visitFctDef(FctDefNode *node) {
  if (!secondRun)
    return nullptr;

  // Change to the (potentially generic) struct scope
  SymbolTable *accessScope = currentScope;
  if (node->isMethod)
    accessScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + node->structName);
  assert(accessScope != nullptr);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Function> *manifestations = accessScope->getFunctionManifestations(node->codeLoc);

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

      if (node->isMethod) { // Change to the struct scope
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
      llvm::Type *returnType = spiceFunc.getReturnType().toLLVMType(*context, currentScope);

      // Arguments
      unsigned int currentArgIndex = 0;
      if (node->hasParams) {
        std::vector<SymbolType> argSymbolTypes = spiceFunc.getArgTypes();
        for (; currentArgIndex < argSymbolTypes.size(); currentArgIndex++) {
          currentVarName = node->paramLst()->params()[currentArgIndex]->varName;
          argNames.push_back(currentVarName);
          SymbolTableEntry *argSymbol = currentScope->lookup(currentVarName);
          assert(argSymbol != nullptr);
          currentConstSigned = argSymbol->getSpecifiers().isSigned();
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
      if (cliOptions.generateDebugInfo)
        generateFunctionDebugInfo(fct, &spiceFunc);

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
      if (node->paramLst()) {
        for (; currentArgIndex < node->paramLst()->params().size(); currentArgIndex++)
          visit(node->paramLst()->params()[currentArgIndex]);
      }

      // Declare result variable
      llvm::Value *returnMemAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
      SymbolTableEntry *returnSymbol = currentScope->lookup(RETURN_VARIABLE_NAME);
      assert(returnSymbol != nullptr);
      returnSymbol->updateAddress(returnMemAddress);
      returnSymbol->updateLLVMType(returnType);

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
          llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup");
          moveInsertPointToBlock(bCleanup);

          // Generate cleanup instructions (e.g. dtor calls)
          bool destructorCalled = false;
          for (SymbolTableEntry *varEntry : varsToDestruct)
            destructorCalled |= insertDestructorCall(node->codeLoc, varEntry);

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
        if (stackState != nullptr)
          builder->CreateCall(retrieveStackRestoreFct(), {stackState});
        // Create return stmt
        llvm::Value *result = returnSymbol->getAddress();
        builder->CreateRet(builder->CreateLoad(result->getType()->getPointerElementType(), result));
      }

      // Conclude debug information
      if (cliOptions.generateDebugInfo)
        debugInfo.lexicalBlocks.pop_back();

      // Verify function
      if (!cliOptions.disableVerifier && !cliOptions.generateDebugInfo) { // Verifying while generating debug info throws errors
        std::string output;
        llvm::raw_string_ostream oss(output);
        if (llvm::verifyFunction(*fct, &oss))
          throw err->get(node->codeLoc, INVALID_FUNCTION, oss.str());
      }

      // Change scope back
      currentScope = currentScope->getParent();
      assert(currentScope);

      // Leave the struct scope
      if (node->isMethod) {
        currentScope = currentScope->getParent();
        assert(currentScope);
      }
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitProcDef(ProcDefNode *node) {
  if (!secondRun)
    return nullptr;

  // Change to the (potentially generic) struct scope
  SymbolTable *accessScope = currentScope;
  if (node->isMethod)
    accessScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + node->structName);
  assert(accessScope != nullptr);

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Function> *manifestations = accessScope->getFunctionManifestations(node->codeLoc);

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

      if (node->isMethod) { // Change to the struct scope
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
      if (node->paramLst()) {
        std::vector<SymbolType> argSymbolTypes = spiceProc.getArgTypes();
        for (; currentArgIndex < argSymbolTypes.size(); currentArgIndex++) {
          currentVarName = node->paramLst()->params()[currentArgIndex]->varName;
          argNames.push_back(currentVarName);
          SymbolTableEntry *argSymbol = currentScope->lookup(currentVarName);
          assert(argSymbol != nullptr);
          currentConstSigned = argSymbol->getSpecifiers().isSigned();
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
      if (cliOptions.generateDebugInfo)
        generateFunctionDebugInfo(proc, &spiceProc);

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
          llvm::BasicBlock *bCleanup = llvm::BasicBlock::Create(*context, "cleanup");
          moveInsertPointToBlock(bCleanup);

          // Generate cleanup instructions (e.g. dtor calls)
          bool destructorCalled = false;
          for (SymbolTableEntry *varEntry : varsToDestruct)
            destructorCalled |= insertDestructorCall(node->codeLoc, varEntry);

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
        if (stackState != nullptr)
          builder->CreateCall(retrieveStackRestoreFct(), {stackState});
        // Create return stmt
        builder->CreateRetVoid();
      }

      // Conclude debug information
      if (cliOptions.generateDebugInfo)
        debugInfo.lexicalBlocks.pop_back();

      // Verify procedure
      if (!cliOptions.disableVerifier && !cliOptions.generateDebugInfo) { // Verifying while generating debug info throws errors
        std::string output;
        llvm::raw_string_ostream oss(output);
        if (llvm::verifyFunction(*proc, &oss))
          throw err->get(node->codeLoc, INVALID_FUNCTION, oss.str());
      }

      // Change scope back
      currentScope = currentScope->getParent();
      assert(currentScope);

      // Leave the struct scope
      if (node->isMethod) {
        currentScope = currentScope->getParent();
        assert(currentScope);
      }
    }
  }
  return nullptr;
}

std::any GeneratorVisitor::visitStructDef(StructDefNode *node) {
  if (secondRun)
    return nullptr;

  // Get all substantiated function which result from this function declaration
  std::map<std::string, Struct> *manifestations = currentScope->getStructManifestations(node->codeLoc);
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

      // Create global struct
      llvm::StructType *structType = llvm::StructType::create(*context, mangledName);
      SymbolTableEntry *structSymbol = currentScope->lookup(spiceStruct.getSignature());
      assert(structSymbol);
      structSymbol->updateLLVMType(structType);

      // Collect concrete field types
      std::vector<llvm::Type *> fieldTypes;
      for (const auto &field : node->fields()) {
        SymbolTableEntry *fieldEntry = currentScope->lookup(field->name);
        assert(fieldEntry && !fieldEntry->getType().is(TY_GENERIC));
        currentConstSigned = fieldEntry->getSpecifiers().isSigned();
        fieldTypes.push_back(fieldEntry->getType().toLLVMType(*context, currentScope));
      }

      // Set field types to struct type
      structType->setBody(fieldTypes);

      // Return to old scope
      currentScope = currentScope->getParent();
      assert(currentScope);
    }
  }

  return nullptr;
}

std::any GeneratorVisitor::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (secondRun)
    return nullptr;

  currentVarName = node->varName;

  // Get symbol table entry and the symbol specifiers
  SymbolTableEntry *globalVarEntry = currentScope->lookup(node->varName);
  assert(globalVarEntry != nullptr);
  SymbolSpecifiers specifiers = globalVarEntry->getSpecifiers();
  llvm::GlobalValue::LinkageTypes linkage =
      specifiers.isPublic() ? llvm::GlobalValue::LinkageTypes::ExternalLinkage : llvm::GlobalValue::LinkageTypes::InternalLinkage;

  // Create correctly signed LLVM type from the data type
  currentConstSigned = specifiers.isSigned();
  auto varType = any_cast<llvm::Type *>(visit(node->dataType()));
  globalVarEntry->updateLLVMType(varType);

  // Create global variable
  llvm::Value *memAddress = module->getOrInsertGlobal(node->varName, varType);
  globalVarEntry->updateAddress(memAddress);
  // Set some attributes to it
  llvm::GlobalVariable *global = module->getNamedGlobal(node->varName);
  global->setLinkage(linkage);
  global->setConstant(specifiers.isConst());

  if (node->value()) { // Variable is initialized here
    visit(node->value());
    constNegate = node->negative;
    global->setInitializer(currentConstValue);
    constNegate = false;
  }

  return nullptr;
}

std::any GeneratorVisitor::visitExtDecl(ExtDeclNode *node) {
  if (secondRun)
    return nullptr;

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
    symbolTypes.push_back(returnEntry->getType());
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
  std::vector<SymbolType> argSymbolTypes = spiceFunc.getArgTypes();
  symbolTypes.insert(std::end(symbolTypes), std::begin(argSymbolTypes), std::end(argSymbolTypes));

  // Declare function
  llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, argTypes, node->isVarArg);
  module->getOrInsertFunction(node->extFunctionName, functionType);
  if (node->isDll)
    module->getFunction(node->extFunctionName)->setDLLStorageClass(llvm::GlobalValue::DLLImportStorageClass);

  return nullptr;
}

std::any GeneratorVisitor::visitThreadDef(ThreadDefNode *node) {
  emitSourceLocation(node);

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
  visit(node->stmtLst());

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
    throw err->get(node->codeLoc, INVALID_FUNCTION, oss.str());

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
  emitSourceLocation(node);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);

  // Visit instructions in the block
  visit(node->stmtLst());

  // Change scope back
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  return nullptr;
}

std::any GeneratorVisitor::visitForLoop(ForLoopNode *node) {
  emitSourceLocation(node);

  // Create blocks
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "for.cond");
  llvm::BasicBlock *bInc = llvm::BasicBlock::Create(*context, "for.inc");
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "for");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "for.end");

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
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitForeachLoop(ForeachLoopNode *node) {
  emitSourceLocation(node);

  // Create blocks
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "foreach.cond");
  llvm::BasicBlock *bInc = llvm::BasicBlock::Create(*context, "foreach.inc");
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "foreach.loop");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "foreach.end");

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);
  breakBlocks.push(bEnd);
  continueBlocks.push(bInc);

  // Get array variable entry
  visit(node->arrayAssign());
  SymbolTableEntry *arrayVarEntry = currentScope->lookup(currentVarName);
  bool dynamicallySized =
      arrayVarEntry && arrayVarEntry->getType().is(TY_PTR) && arrayVarEntry->getType().getDynamicArraySize() != nullptr;

  // Initialize loop variables
  llvm::Value *idxVarPtr;
  if (node->idxVarDecl()) {
    auto idxVarName = any_cast<std::string>(visit(node->idxVarDecl()));
    SymbolTableEntry *idxVarEntry = currentScope->lookup(idxVarName);
    assert(idxVarEntry != nullptr);
    idxVarPtr = idxVarEntry->getAddress();
  } else {
    std::string indexVariableName = FOREACH_DEFAULT_IDX_VARIABLE_NAME;
    // Create local variable for
    llvm::Type *indexVariableType = llvm::Type::getInt32Ty(*context);
    idxVarPtr = insertAlloca(indexVariableType, indexVariableName);
    SymbolTableEntry *idxVarEntry = currentScope->lookup(indexVariableName);
    assert(idxVarEntry != nullptr);
    idxVarEntry->updateAddress(idxVarPtr);
    idxVarEntry->updateLLVMType(indexVariableType);
    idxVarEntry->setUsed();
    // Initialize variable with 0
    builder->CreateStore(builder->getInt32(0), idxVarPtr);
  }
  auto itemVarName = any_cast<std::string>(visit(node->itemDecl()));
  SymbolTableEntry *itemVarEntry = currentScope->lookup(itemVarName);
  assert(itemVarEntry != nullptr);
  llvm::Value *itemVarPtr = itemVarEntry->getAddress();

  // Do loop variable initialization
  llvm::Value *arrayValuePtr = resolveAddress(node->arrayAssign());
  llvm::Value *arrayValue = builder->CreateLoad(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr);
  llvm::Value *arraySizeValue = dynamicallySized ? arrayVarEntry->getType().getDynamicArraySize()
                                                 : builder->getInt32(arrayValue->getType()->getArrayNumElements());

  // Cast to i32 type if necessary
  if (arraySizeValue->getType()->getIntegerBitWidth() != 32)
    arraySizeValue = builder->CreateSExtOrTrunc(arraySizeValue, builder->getInt32Ty());

  // Load the first item into item variable
  llvm::Value *index = builder->CreateLoad(idxVarPtr->getType()->getPointerElementType(), idxVarPtr);

  llvm::Value *itemPtr;
  llvm::Value *indices[2] = {builder->getInt32(0), index};
  if (dynamicallySized) {
    arrayValuePtr = builder->CreateLoad(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr);
    itemPtr = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr, index);
  } else {
    itemPtr = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr, indices);
  }
  llvm::Value *newItemValue = builder->CreateLoad(itemPtr->getType()->getPointerElementType(), itemPtr);
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
  index = builder->CreateLoad(idxVarPtr->getType()->getPointerElementType(), idxVarPtr, "idx");
  index = builder->CreateAdd(index, builder->getInt32(1), "idx.inc");
  builder->CreateStore(index, idxVarPtr);
  // Load new item into item variable
  indices[1] = index;
  if (dynamicallySized) {
    itemPtr = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr, index);
  } else {
    itemPtr = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr, indices);
  }
  newItemValue = builder->CreateLoad(itemPtr->getType()->getPointerElementType(), itemPtr);
  builder->CreateStore(newItemValue, itemVarPtr);
  createBr(bCond);

  // Fill cond block
  parentFct->getBasicBlockList().push_back(bCond);
  moveInsertPointToBlock(bCond);
  // Check condition
  index = builder->CreateLoad(idxVarPtr->getType()->getPointerElementType(), idxVarPtr);
  llvm::Value *cond = builder->CreateICmpULT(index, arraySizeValue);
  createCondBr(cond, bLoop, bEnd);

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

std::any GeneratorVisitor::visitWhileLoop(WhileLoopNode *node) {
  emitSourceLocation(node);

  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *bCond = llvm::BasicBlock::Create(*context, "while.cond");
  llvm::BasicBlock *bLoop = llvm::BasicBlock::Create(*context, "while");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "while.end");

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
  currentScope = currentScope->getParent();
  assert(currentScope != nullptr);

  // Return true as result for the loop
  return static_cast<llvm::Value *>(builder->getTrue());
}

std::any GeneratorVisitor::visitStmtLst(StmtLstNode *node) {
  emitSourceLocation(node);

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
  emitSourceLocation(node);

  // Change scope
  currentScope = currentScope->getChild(node->getScopeId());
  assert(currentScope != nullptr);

  llvm::Value *condValue = resolveValue(node->condition());
  llvm::Function *parentFct = builder->GetInsertBlock()->getParent();

  // Create blocks
  llvm::BasicBlock *bThen = llvm::BasicBlock::Create(*context, "if.then");
  llvm::BasicBlock *bElse = llvm::BasicBlock::Create(*context, "if.else");
  llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "if.end");

  // Check if condition is fulfilled
  createCondBr(condValue, bThen, node->elseStmt() ? bElse : bEnd);

  // Fill then block
  parentFct->getBasicBlockList().push_back(bThen);
  moveInsertPointToBlock(bThen);
  // Generate IR for nested statements
  visit(node->stmtLst());
  createBr(bEnd);

  // Change scope back
  currentScope = currentScope->getParent();
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
  emitSourceLocation(node);

  if (node->ifStmt()) { // It is an else if branch
    visit(node->ifStmt());
  } else { // It is an else branch
    // Change scope
    currentScope = currentScope->getChild(node->getScopeId());
    assert(currentScope != nullptr);

    // Generate IR for nested statements
    visit(node->stmtLst());

    // Change scope back
    currentScope = currentScope->getParent();
    assert(currentScope != nullptr);
  }
  return nullptr;
}

std::any GeneratorVisitor::visitAssertStmt(AssertStmtNode *node) {
  emitSourceLocation(node);

  // Only generate assertions with -O0
  if (cliOptions.optLevel == 0) {
    // Visit the assignExpr
    llvm::Value *condValue = resolveValue(node->assignExpr());
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
    std::string errorMsg = "Assertion failed: Condition '" + node->expressionString + "' evaluated to false.";
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

std::any GeneratorVisitor::visitDeclStmt(DeclStmtNode *node) {
  emitSourceLocation(node);

  // Get var name
  currentVarName = lhsVarName = node->varName;

  // Get variable entry
  SymbolTableEntry *entry = currentScope->lookup(lhsVarName);
  assert(entry != nullptr);
  currentConstSigned = entry->getSpecifiers().isSigned();

  // Get data type
  llvm::Type *varType = lhsType = any_cast<llvm::Type *>(visit(node->dataType()));
  entry->updateLLVMType(varType);
  entry->updateType(currentSymbolType, true);

  // Restore var name
  currentVarName = lhsVarName = node->varName;

  llvm::Value *memAddress = nullptr;
  if (node->assignExpr()) { // Declaration with assignment
    memAddress = resolveAddress(node->assignExpr());
    assert(memAddress->getType()->getPointerElementType() == varType);

    // Generate debug info for local variable
    generateDeclDebugInfo(node->codeLoc, lhsVarName, memAddress);
  } else { // Declaration with default value
    if (entry->getType().is(TY_PTR) && entry->getType().getDynamicArraySize() != nullptr) {
      llvm::Type *itemType = entry->getType().getContainedTy().toLLVMType(*context, nullptr);
      dynamicArraySize = entry->getType().getDynamicArraySize();
      llvm::Value *arrayValue = allocateDynamicallySizedArray(itemType);
      memAddress = insertAlloca(arrayValue->getType(), lhsVarName);
      builder->CreateStore(arrayValue, memAddress, entry->isVolatile());
    } else {
      llvm::Value *defaultValue = getDefaultValueForType(varType, entry->getType().getBaseType().getSubType());
      memAddress = insertAlloca(varType, lhsVarName);

      // Generate debug info for local variable
      generateDeclDebugInfo(node->codeLoc, lhsVarName, memAddress);

      // Save default value to address
      builder->CreateStore(defaultValue, memAddress, entry->isVolatile());
    }
  }

  // Update address in symbol table
  entry->updateAddress(memAddress);

  lhsType = nullptr; // Reset nullptr

  // Return the variable name
  return lhsVarName;
}

std::any GeneratorVisitor::visitImportStmt(ImportStmtNode * /*node*/) {
  // Noop
  return nullptr;
}

std::any GeneratorVisitor::visitReturnStmt(ReturnStmtNode *node) {
  emitSourceLocation(node);

  SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);

  // Check if a value is attached to the return statement
  llvm::Value *returnValuePtr = nullptr;
  if (node->assignExpr()) {
    assert(returnVarEntry != nullptr);
    // Set the expected type of the value
    lhsType = returnVarEntry->getType().toLLVMType(*context, currentScope);
    // Visit return value
    returnValuePtr = resolveAddress(node->assignExpr());
  } else if (returnVarEntry != nullptr) { // Function. Procedures do not have a return variable
    returnValuePtr = returnVarEntry->getAddress();
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
      destructorCalled |= insertDestructorCall(node->codeLoc, varEntry);

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
  llvm::Value *returnValue = builder->CreateLoad(returnValuePtr->getType()->getPointerElementType(), returnValuePtr);
  builder->CreateRet(returnValue);
  return returnValuePtr;
}

std::any GeneratorVisitor::visitBreakStmt(BreakStmtNode *node) {
  emitSourceLocation(node);

  // Get destination block
  for (int i = 1; i < node->breakTimes; i++)
    breakBlocks.pop();

  // Jump to destination block
  createBr(breakBlocks.top());
  return nullptr;
}

std::any GeneratorVisitor::visitContinueStmt(ContinueStmtNode *node) {
  emitSourceLocation(node);

  // Get destination block
  for (int i = 1; i < node->continueTimes; i++)
    continueBlocks.pop();

  // Jump to destination block
  createBr(continueBlocks.top());
  return nullptr;
}

std::any GeneratorVisitor::visitPrintfCall(PrintfCallNode *node) {
  // Declare if not declared already
  llvm::Function *printfFct = retrievePrintfFct();

  std::vector<llvm::Value *> printfArgs;
  printfArgs.push_back(builder->CreateGlobalStringPtr(node->templatedString));
  for (const auto &arg : node->assignExpr()) {
    // Visit argument
    auto argValPtr = resolveAddress(arg);

    llvm::Value *argVal;
    if (argValPtr->getType()->getPointerElementType()->isArrayTy()) { // Convert array type to pointer type
      llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(0)};
      llvm::Type *targetType = argValPtr->getType()->getPointerElementType();
      argVal = builder->CreateInBoundsGEP(targetType, argValPtr, indices);
    } else {
      argVal = builder->CreateLoad(argValPtr->getType()->getPointerElementType(), argValPtr);
    }

    // Cast all integer types to 32 bit
    if (argVal->getType()->isIntegerTy(8) || argVal->getType()->isIntegerTy(16))
      argVal = builder->CreateSExt(argVal, llvm::Type::getInt32Ty(*context));
    if (argVal->getType()->isIntegerTy(1))
      argVal = builder->CreateZExt(argVal, llvm::Type::getInt32Ty(*context));

    printfArgs.push_back(argVal);
  }
  return static_cast<llvm::Value *>(builder->CreateCall(printfFct, printfArgs));
}

std::any GeneratorVisitor::visitSizeofCall(SizeofCallNode *node) {
  llvm::Type *type;
  if (node->assignExpr()) { // Assign expression
    // Visit the argument
    llvm::Value *value = resolveValue(node->assignExpr());
    type = value->getType();
  } else { // Type
    type = any_cast<llvm::Type *>(visit(node->dataType()));
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

std::any GeneratorVisitor::visitAssignExpr(AssignExprNode *node) {
  emitSourceLocation(node);

  // Check if there is an assign operator applied
  if (node->hasOperator) { // This is an assignment or compound assignment
    lhsType = nullptr;     // Reset lhs type

    // Retrieve the lhs variable name
    if (!node->lhs()->postfixUnaryExpr()->atomicExpr()->identifier.empty())
      lhsVarName = node->lhs()->postfixUnaryExpr()->atomicExpr()->identifier;

    // Get value of right side
    llvm::Value *rhs = resolveValue(node->rhs());

    // Visit the left side
    auto lhsPtr = resolveAddress(node->lhs());
    lhsVarName = currentVarName;

    // Take a look at the operator
    if (node->op == AssignExprNode::OP_ASSIGN) { // Simple assign
      assert(rhs->getType() == lhsPtr->getType()->getPointerElementType());
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
      switch (node->op) {
      case AssignExprNode::OP_PLUS_EQUAL:
        rhs = conversionsManager->getPlusEqualInst(lhs, rhs, node->codeLoc);
        break;
      case AssignExprNode::OP_MINUS_EQUAL:
        rhs = conversionsManager->getMinusEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_MUL_EQUAL:
        rhs = conversionsManager->getMulEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_DIV_EQUAL:
        rhs = conversionsManager->getDivEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_REM_EQUAL:
        rhs = conversionsManager->getRemEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_SHL_EQUAL:
        rhs = conversionsManager->getSHLEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_SHR_EQUAL:
        rhs = conversionsManager->getSHREqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_AND_EQUAL:
        rhs = conversionsManager->getAndEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_OR_EQUAL:
        rhs = conversionsManager->getOrEqualInst(lhs, rhs);
        break;
      case AssignExprNode::OP_XOR_EQUAL:
        rhs = conversionsManager->getXorEqualInst(lhs, rhs);
        break;
      default:
        throw std::runtime_error("Assign op fall-through");
      }
      builder->CreateStore(rhs, lhsPtr, variableEntry->isVolatile());
    }

    // Add debug info for value change
    generateAssignDebugInfo(node->codeLoc, lhsVarName, rhs);

    return lhsPtr;
  } else if (node->ternaryExpr()) {
    std::any rhs = visit(node->ternaryExpr());
    lhsType = nullptr; // Reset lhs type
    return rhs;
  } else if (node->threadDef()) {
    std::any rhs = visit(node->threadDef());
    lhsType = nullptr; // Reset lhs type
    return rhs;
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any GeneratorVisitor::visitTernaryExpr(TernaryExprNode *node) {
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    auto conditionPtr = resolveAddress(node->operands()[0]);
    auto trueValuePtr = resolveAddress(node->operands()[1]);
    auto falseValuePtr = resolveAddress(node->operands()[2]);

    llvm::Value *condition = builder->CreateLoad(conditionPtr->getType()->getPointerElementType(), conditionPtr);
    return builder->CreateSelect(condition, trueValuePtr, falseValuePtr);
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitLogicalOrExpr(LogicalOrExprNode *node) {
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    // Prepare for short-circuiting
    std::pair<llvm::Value *, llvm::BasicBlock *> incomingBlocks[node->operands().size()];
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "lor.end");
    llvm::Function *parentFunction = builder->GetInsertBlock()->getParent();

    // Visit the first condition
    llvm::Value *lhs = resolveValue(node->operands().front());

    // Prepare the blocks
    incomingBlocks[0] = {lhs, builder->GetInsertBlock()};
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "lor." + std::to_string(i));
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
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    // Prepare for short-circuiting
    std::pair<llvm::Value *, llvm::BasicBlock *> incomingBlocks[node->operands().size()];
    llvm::BasicBlock *bEnd = llvm::BasicBlock::Create(*context, "land.end");
    llvm::Function *parentFunction = builder->GetInsertBlock()->getParent();

    // Visit the first condition
    llvm::Value *lhs = resolveValue(node->operands().front());

    // Prepare the blocks
    incomingBlocks[0] = {lhs, builder->GetInsertBlock()};
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "land." + std::to_string(i));
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
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    llvm::Value *lhs = resolveValue(node->operands().front());
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::Value *rhs = resolveValue(node->operands()[i]);
      lhs = conversionsManager->getBitwiseOrInst(lhs, rhs);
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    llvm::Value *lhs = resolveValue(node->operands().front());
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::Value *rhs = resolveValue(node->operands()[i]);
      lhs = conversionsManager->getBitwiseXorInst(lhs, rhs);
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    llvm::Value *lhs = resolveValue(node->operands().front());
    for (int i = 1; i < node->operands().size(); i++) {
      llvm::Value *rhs = resolveValue(node->operands()[i]);
      lhs = conversionsManager->getBitwiseAndInst(lhs, rhs);
    }
    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitEqualityExpr(EqualityExprNode *node) {
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    llvm::Value *lhs = resolveValue(node->operands()[0]);
    llvm::Value *rhs = resolveValue(node->operands()[1]);

    llvm::Value *result;
    switch (node->op) {
    case EqualityExprNode::OP_EQUAL:
      result = conversionsManager->getEqualInst(lhs, rhs, node->codeLoc);
      break;
    case EqualityExprNode::OP_NOT_EQUAL:
      result = conversionsManager->getNotEqualInst(lhs, rhs, node->codeLoc);
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
  emitSourceLocation(node);

  if (node->operands().size() > 1) {
    llvm::Value *lhs = resolveValue(node->operands()[0]);
    llvm::Value *rhs = resolveValue(node->operands()[1]);

    llvm::Value *result;
    switch (node->op) {
    case RelationalExprNode::OP_LESS:
      result = conversionsManager->getLessInst(lhs, rhs);
      break;
    case RelationalExprNode::OP_GREATER:
      result = conversionsManager->getGreaterInst(lhs, rhs);
      break;
    case RelationalExprNode::OP_LESS_EQUAL:
      result = conversionsManager->getLessEqualInst(lhs, rhs);
      break;
    case RelationalExprNode::OP_GREATER_EQUAL:
      result = conversionsManager->getGreaterEqualInst(lhs, rhs);
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
  emitSourceLocation(node);

  // Check if there is a shift operation attached
  if (node->operands().size() > 1) {
    llvm::Value *lhs = resolveValue(node->operands()[0]);
    llvm::Value *rhs = resolveValue(node->operands()[1]);

    llvm::Value *result;
    switch (node->op) {
    case ShiftExprNode::OP_SHIFT_LEFT:
      result = conversionsManager->getShiftLeftInst(lhs, rhs);
      break;
    case ShiftExprNode::OP_SHIFT_RIGHT:
      result = conversionsManager->getShiftRightInst(lhs, rhs);
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
  emitSourceLocation(node);

  // Check if at least one additive operator is applied
  if (!node->opQueue.empty()) {
    llvm::Value *lhs = resolveValue(node->operands().front());

    std::queue<AdditiveExprNode::AdditiveOp> opQueue = node->opQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      MultiplicativeExprNode *operand = node->operands()[operandIndex++];
      assert(operand != nullptr);
      llvm::Value *rhs = resolveValue(operand);

      switch (opQueue.front()) {
      case AdditiveExprNode::OP_PLUS:
        lhs = conversionsManager->getPlusInst(lhs, rhs, node->codeLoc);
        break;
      case AdditiveExprNode::OP_MINUS:
        lhs = conversionsManager->getMinusInst(lhs, rhs);
        break;
      default:
        throw std::runtime_error("Additive expr fall-through");
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
  emitSourceLocation(node);

  // Check if at least one multiplicative operator is applied
  if (!node->opQueue.empty()) {
    llvm::Value *lhs = resolveValue(node->operands().front());

    std::queue<MultiplicativeExprNode::MultiplicativeOp> opQueue = node->opQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      CastExprNode *operand = node->operands()[operandIndex++];
      assert(operand != nullptr);
      llvm::Value *rhs = resolveValue(operand);

      switch (opQueue.front()) {
      case MultiplicativeExprNode::OP_MUL:
        lhs = conversionsManager->getMulInst(lhs, rhs, node->codeLoc);
        break;
      case MultiplicativeExprNode::OP_DIV:
        lhs = conversionsManager->getDivInst(lhs, rhs);
        break;
      case MultiplicativeExprNode::OP_REM:
        lhs = conversionsManager->getRemInst(lhs, rhs);
        break;
      default:
        throw std::runtime_error("Multiplicative expr fall-through");
      }

      opQueue.pop();
    }

    llvm::Value *resultPtr = insertAlloca(lhs->getType());
    builder->CreateStore(lhs, resultPtr);
    return resultPtr;
  }
  return visit(node->operands().front());
}

std::any GeneratorVisitor::visitCastExpr(CastExprNode *node) {
  emitSourceLocation(node);

  if (node->isCasted) { // Cast operator is applied
    auto dstTy = any_cast<llvm::Type *>(visit(node->dataType()));
    llvm::Value *rhs = resolveValue(node->prefixUnaryExpr());
    llvm::Value *result = conversionsManager->getCastInst(dstTy, rhs);
    llvm::Value *resultPtr = insertAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
  }

  return visit(node->prefixUnaryExpr());
}

std::any GeneratorVisitor::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  emitSourceLocation(node);

  currentVarName = "";           // Reset the current variable name
  scopePath.clear();             // Clear the scope path
  structAccessAddress = nullptr; // Clear struct access address

  if (!node->opStack.empty()) {
    // Load the value
    llvm::Value *lhsPtr = resolveAddress(node->postfixUnaryExpr());
    llvm::Value *lhs = nullptr;

    bool isVolatile = false;
    bool storeValue = true;

    std::stack<PrefixUnaryExprNode::PrefixUnaryOp> opStack = node->opStack;
    while (!opStack.empty()) {
      switch (opStack.top()) {
      case PrefixUnaryExprNode::OP_MINUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        lhs = conversionsManager->getPrefixMinusInst(lhs);
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PrefixUnaryExprNode::OP_PLUS_PLUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        lhs = conversionsManager->getPrefixPlusPlusInst(lhs);

        // Store the new value volatile
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        if (!isVolatile && lhsVarEntry && lhsVarEntry->isVolatile())
          isVolatile = true;
        break;
      }
      case PrefixUnaryExprNode::OP_MINUS_MINUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        lhs = conversionsManager->getPrefixMinusMinusInst(lhs);

        // Store the new value volatile
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        if (!isVolatile && lhsVarEntry && lhsVarEntry->isVolatile())
          isVolatile = true;
        break;
      }
      case PrefixUnaryExprNode::OP_NOT: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        lhs = conversionsManager->getPrefixNotInst(lhs);
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PrefixUnaryExprNode::OP_BITWISE_NOT: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        lhs = conversionsManager->getPrefixBitwiseNotInst(lhs);
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PrefixUnaryExprNode::OP_INDIRECTION: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        // Indirect pointer
        lhsPtr = lhs;
        lhs = builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
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

      opStack.pop();
    }

    if (storeValue) {
      // Store the value back again
      builder->CreateStore(lhs, lhsPtr, isVolatile);
      // Create debug info for assignment
      generateAssignDebugInfo(node->codeLoc, currentVarName, lhs);
    }

    return lhsPtr;
  }

  return visit(node->postfixUnaryExpr());
}

std::any GeneratorVisitor::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  emitSourceLocation(node);

  if (!node->opQueue.empty()) {
    // Retrieve the address and the value if required
    llvm::Value *lhsPtr = resolveAddress(node->atomicExpr());
    llvm::Value *lhs = lhsPtr != nullptr ? builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr) : nullptr;

    size_t subscriptCounter = 0;
    size_t memberAccessCounter = 0;

    std::queue<PostfixUnaryExprNode::PostfixUnaryOp> opQueue = node->opQueue; // Copy to not modify the queue in the AST node
    while (!opQueue.empty()) {
      switch (opQueue.front()) {
      case PostfixUnaryExprNode::OP_SUBSCRIPT: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

        assert(lhs->getType()->isArrayTy() || lhs->getType()->isPointerTy());

        // Save variables to restore later
        std::string currentVarNameBackup = currentVarName;
        ScopePath scopePathBackup = scopePath;
        llvm::Value *structAccessAddressBackup = structAccessAddress;

        // Get the index value
        AssignExprNode *indexExpr = node->assignExpr()[subscriptCounter++];
        llvm::Value *indexValue = resolveValue(indexExpr);

        // Restore variables
        currentVarName = currentVarNameBackup;
        scopePath = scopePathBackup;
        structAccessAddress = structAccessAddressBackup;

        if (lhs->getType()->isArrayTy()) {
          // Calculate address of array item
          llvm::Value *indices[2] = {builder->getInt32(0), indexValue};
          lhsPtr = builder->CreateInBoundsGEP(lhsPtr->getType()->getPointerElementType(), lhsPtr, indices);
        } else {
          // Calculate address of pointer offset
          lhsPtr = builder->CreateInBoundsGEP(lhs->getType()->getPointerElementType(), lhs, indexValue);
        }

        lhs = nullptr;
        break;
      }
      case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
        // Auto de-reference pointer
        while (lhsPtr && lhsPtr->getType()->getPointerElementType()->isPointerTy())
          lhsPtr = structAccessAddress = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

        // Visit identifier after the dot
        PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];
        lhsPtr = resolveAddress(rhs);

        lhs = nullptr;
        break;
      }
      case PostfixUnaryExprNode::OP_PLUS_PLUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        // Get the lhs variable entry
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        // Save the new value to the old pointer
        llvm::Value *newLhsValue = conversionsManager->getPostfixPlusPlusInst(lhs);
        builder->CreateStore(newLhsValue, lhsPtr, lhsVarEntry && lhsVarEntry->isVolatile());
        // Allocate new space and continue working with the new memory slot
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      case PostfixUnaryExprNode::OP_MINUS_MINUS: {
        if (!lhs)
          lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        // Get the lhs variable entry
        SymbolTableEntry *lhsVarEntry = currentScope->lookup(currentVarName);
        // Save the new value to the old pointer
        llvm::Value *newLhsValue = conversionsManager->getPostfixMinusMinusInst(lhs);
        builder->CreateStore(newLhsValue, lhsPtr, lhsVarEntry && lhsVarEntry->isVolatile());
        // Allocate new space and continue working with the new memory slot
        lhsPtr = insertAlloca(lhs->getType());
        break;
      }
      default:
        throw std::runtime_error("PostfixUnary fall-through");
      }
      opQueue.pop();
    }

    if (lhs != nullptr) {
      // Store the value back again
      builder->CreateStore(lhs, lhsPtr);
      // Create debug info for assignment
      generateAssignDebugInfo(node->codeLoc, currentVarName, lhs);
    }

    return lhsPtr;
  }

  return visit(node->atomicExpr());
}

std::any GeneratorVisitor::visitAtomicExpr(AtomicExprNode *node) {
  emitSourceLocation(node);

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

    // Import
    if (entry->getType().is(TY_IMPORT)) {
      SymbolTable *newScope = accessScope->lookupTable(node->identifier);
      assert(newScope != nullptr);
      scopePath.pushFragment(node->identifier, newScope);
      return static_cast<llvm::Value *>(nullptr);
    }

    // Global variables
    if (entry->isGlobal()) {
      // Initialize if it is an external global var
      if (importedScope)
        entry = initExtGlobal(node->identifier, scopePath.getScopePrefix(true) + node->identifier);
      return entry->getAddress();
    }

    // Struct without pointer
    if (entry->getType().is(TY_STRUCT)) {
      // Retrieve struct scope
      SymbolType entryType = entry->getType();
      std::string structSignature = Struct::getSignature(entryType.getSubType(), entryType.getTemplateTypes());
      SymbolTable *structScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
      assert(structScope != nullptr);
      scopePath.pushFragment(node->identifier, structScope);

      if (structAccessAddress == nullptr) {
        // Initialize struct resolution
        return structAccessAddress = entry->getAddress();
      } else {
        // Add field index to indices
        unsigned int fieldIndex = entry->getOrderIndex();
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(fieldIndex)};
        return structAccessAddress = builder->CreateInBoundsGEP(structAccessAddress->getType()->getPointerElementType(),
                                                                structAccessAddress, indices);
      }
    }

    // Struct* or Struct** or ...
    if (entry->getType().isBaseType(TY_STRUCT)) {
      // Retrieve struct scope
      SymbolType entryBaseType = entry->getType().getBaseType();
      std::string structSignature = Struct::getSignature(entryBaseType.getSubType(), entryBaseType.getTemplateTypes());
      SymbolTable *structScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
      assert(structScope != nullptr);
      scopePath.pushFragment(node->identifier, structScope);

      if (structAccessAddress == nullptr) {
        // Auto de-referencing is done by the dot operator
        return structAccessAddress = entry->getAddress();
      } else {
        unsigned int fieldIndex = entry->getOrderIndex();
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(fieldIndex)};
        // Auto de-referencing is done by the dot operator
        return structAccessAddress = builder->CreateInBoundsGEP(structAccessAddress->getType()->getPointerElementType(),
                                                                structAccessAddress, indices);
      }
    }

    // Other types
    if (accessScope->getScopeType() == SCOPE_STRUCT) { // Struct field
      // Retrieve field index
      unsigned int fieldIndex = entry->getOrderIndex();
      llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(fieldIndex)};

      // Calculate field address
      llvm::Value *fieldAddress =
          builder->CreateInBoundsGEP(structAccessAddress->getType()->getPointerElementType(), structAccessAddress, indices);

      structAccessAddress = nullptr;
      return fieldAddress;
    }

    // If the address is known, return it
    if (entry->getAddress())
      return entry->getAddress();

    // For the case that in the current scope there is a variable with the same name, but it is initialized later, so the
    // symbol above in the hierarchy is meant to be used.
    SymbolTableEntry *entryAbove = entry;
    while (entryAbove && entryAbove->getAddress() == nullptr && accessScope->getParent()) {
      accessScope = accessScope->getParent();
      entryAbove = accessScope->lookup(currentVarName);
      assert(entryAbove != nullptr);
      if (entryAbove->getAddress())
        return entryAbove->getAddress();
    }

    // If no variable was found, that has a valid address => allocate space for the original entry
    llvm::Type *llvmType = entry->getType().toLLVMType(*context, currentScope);
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
  emitSourceLocation(node);

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
  if (node->type == PrimitiveValueNode::TY_DOUBLE) {
    currentSymbolType = SymbolType(TY_DOUBLE);
    double value = constNegate ? -node->data.doubleValue : node->data.doubleValue;
    return static_cast<llvm::Constant *>(llvm::ConstantFP::get(*context, llvm::APFloat(value)));
  }

  // Value is an integer constant
  if (node->type == PrimitiveValueNode::TY_INT) {
    currentSymbolType = SymbolType(TY_INT);
    int value = constNegate ? -node->data.intValue : node->data.intValue;
    llvm::Type *intTy = builder->getInt32Ty();
    llvm::Constant *constant =
        currentConstSigned ? llvm::ConstantInt::getSigned(intTy, value) : llvm::ConstantInt::get(intTy, value);
    return static_cast<llvm::Constant *>(constant);
  }

  // Value is a short constant
  if (node->type == PrimitiveValueNode::TY_SHORT) {
    currentSymbolType = SymbolType(TY_SHORT);
    int value = constNegate ? -node->data.shortValue : node->data.shortValue;
    llvm::Type *shortTy = builder->getInt16Ty();
    llvm::Constant *constant =
        currentConstSigned ? llvm::ConstantInt::getSigned(shortTy, value) : llvm::ConstantInt::get(shortTy, value);
    return static_cast<llvm::Constant *>(constant);
  }

  // Value is a long constant
  if (node->type == PrimitiveValueNode::TY_LONG) {
    currentSymbolType = SymbolType(TY_LONG);
    long long value = constNegate ? -node->data.longValue : node->data.longValue;
    llvm::Type *longTy = builder->getInt64Ty();
    llvm::Constant *constant =
        currentConstSigned ? llvm::ConstantInt::getSigned(longTy, value) : llvm::ConstantInt::get(longTy, value);
    return static_cast<llvm::Constant *>(constant);
  }

  // Value is a char constant
  if (node->type == PrimitiveValueNode::TY_CHAR) {
    currentSymbolType = SymbolType(TY_CHAR);
    char value = node->data.charValue;
    llvm::Type *charTy = builder->getInt8Ty();
    llvm::Constant *constant =
        currentConstSigned ? llvm::ConstantInt::getSigned(charTy, value) : llvm::ConstantInt::get(charTy, value);
    return static_cast<llvm::Constant *>(constant);
  }

  // Value is a string constant
  if (node->type == PrimitiveValueNode::TY_STRING) {
    currentSymbolType = SymbolType(TY_STRING);
    std::string value = node->data.stringValue;
    return static_cast<llvm::Constant *>(builder->CreateGlobalStringPtr(value, "", 0, module.get()));
  }

  // Value is a boolean constant
  if (node->type == PrimitiveValueNode::TY_BOOL) {
    currentSymbolType = SymbolType(TY_BOOL);
    return static_cast<llvm::Constant *>(node->data.boolValue ? builder->getTrue() : builder->getFalse());
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
  llvm::Value *thisValuePtr = nullptr;
  for (unsigned int i = 0; i < node->functionNameFragments.size(); i++) {
    std::string identifier = node->functionNameFragments[i];
    SymbolTableEntry *symbolEntry = accessScope->lookup(identifier);

    if (i < node->functionNameFragments.size() - 1) {
      if (!symbolEntry)
        throw err->get(node->codeLoc, REFERENCED_UNDEFINED_FUNCTION,
                       "Symbol '" + scopePath.getScopePrefix() + identifier + "' was used before defined");
      thisValuePtr = symbolEntry->getAddress();
    } else if (symbolEntry != nullptr && symbolEntry->getType().getBaseType().is(TY_STRUCT)) {
      Struct *spiceStruct = currentScope->getStructAccessPointer(node->codeLoc);
      assert(spiceStruct != nullptr);

      // Check if the struct is defined
      symbolEntry = accessScope->lookup(spiceStruct->getSignature());
      assert(symbolEntry != nullptr);

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
          llvm::Type *llvmType = assignVarEntry->getType().toLLVMType(*context, currentScope);
          thisValuePtr = insertAlloca(llvmType, lhsVarName);
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
        returnSymbolType.is(TY_DYN) ? llvm::Type::getVoidTy(*context) : returnSymbolType.toLLVMType(*context, accessScope);

    std::vector<llvm::Type *> argTypes;
    if (isMethod)
      argTypes.push_back(thisValuePtr->getType());
    for (auto &argSymbolType : argSymbolTypes)
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
      llvm::Type *expectedArgType = fctType->getParamType(argIndex);
      // Get the actual arg value
      auto actualArgPtr = resolveAddress(arg);
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

std::any GeneratorVisitor::visitArrayInitialization(ArrayInitializationNode *node) {
  // Get data type
  size_t actualItemCount = node->itemLst() ? node->itemLst()->args().size() : 0;
  size_t arraySize = lhsType != nullptr && lhsType->isArrayTy() ? lhsType->getArrayNumElements() : actualItemCount;
  auto arrayType = lhsType;

  bool dynamicallySized = false;
  if (!lhsVarName.empty()) {
    SymbolTableEntry *arrayEntry = currentScope->lookupStrict(lhsVarName);
    assert(arrayEntry != nullptr);
    dynamicallySized = arrayEntry->getType().is(TY_PTR) && arrayEntry->getType().getDynamicArraySize() != nullptr;
  }

  std::vector<llvm::Value *> itemValues;
  std::vector<llvm::Constant *> itemConstants;
  itemValues.reserve(arraySize);
  itemConstants.reserve(arraySize);

  if (node->itemLst()) { // The array is initialized with values
    // Visit all args to check if they are hardcoded or not
    allArgsHardcoded = true;
    for (size_t i = 0; i < std::min(node->itemLst()->args().size(), arraySize); i++) {
      currentConstValue = nullptr;
      llvm::Value *itemValue = resolveValue(node->itemLst()->args()[i]);
      itemValues.push_back(itemValue);
      itemConstants.push_back(currentConstValue);
    }
  }

  llvm::Type *itemType = nullptr;
  if (arrayType) {
    // Check if array type matches the item type
    if (arrayType->isArrayTy()) { // Array
      assert(itemConstants.empty() || !itemConstants.front() ||
             arrayType->getArrayElementType() == itemConstants.front()->getType());
      itemType = arrayType->getArrayElementType();
    } else { // Pointer
      assert(itemConstants.empty() || !itemConstants.front() ||
             arrayType->getPointerElementType() == itemConstants.front()->getType());
      itemType = arrayType->getPointerElementType();
    }
  } else {
    // Infer type of array from the item type
    assert(!itemConstants.empty());
    itemType = itemConstants.front()->getType();
    arrayType = llvm::ArrayType::get(itemType, arraySize);
  }

  // Decide if the array can be defined globally
  if (allArgsHardcoded && !dynamicallySized && !itemType->isStructTy()) { // Global array is possible
    // Fill up the rest of the items with default values
    if (itemConstants.size() < arraySize) {
      llvm::Constant *constantValue = getDefaultValueForType(itemType, ""); // ToDo: Fill empty string
      for (size_t i = itemConstants.size(); i < arraySize; i++)
        itemConstants.push_back(constantValue);
    }

    // Create global array from constant values
    assert(arrayType != nullptr);
    return createGlobalArray(arrayType, itemConstants);
  } else { // Global array is not possible => fallback to individual indexing
    // Allocate array
    llvm::Value *arrayAddress = nullptr;
    if (dynamicallySized) {
      arrayAddress = allocateDynamicallySizedArray(itemType);
      arrayType = arrayAddress->getType()->getPointerElementType();
    } else {
      arrayAddress = insertAlloca(arrayType, lhsVarName);
    }

    // Insert all given values
    llvm::Value *itemDefaultValue = getDefaultValueForType(itemType, "");
    for (size_t valueIndex = 0; valueIndex < arraySize; valueIndex++) {
      // Calculate item address
      llvm::Value *itemAddress = nullptr;
      if (arrayType->isArrayTy()) {
        llvm::Value *indices[2] = {builder->getInt32(0), builder->getInt32(valueIndex)};
        itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, indices);
      } else {
        itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, builder->getInt32(valueIndex));
      }

      // Store item value to item address
      llvm::Value *itemValue =
          node->itemLst() && valueIndex < node->itemLst()->args().size() ? itemValues[valueIndex] : itemDefaultValue;
      builder->CreateStore(itemValue, itemAddress);
    }

    // Save value to address
    if (dynamicallySized) {
      llvm::Value *newArrayAddress = insertAlloca(arrayAddress->getType(), lhsVarName);
      builder->CreateStore(arrayAddress, newArrayAddress);
      return newArrayAddress;
    }
    return arrayAddress;
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
      if (entry->getType().is(TY_IMPORT)) {
        structScope = structScope->lookupTable(identifier);
      } else if (entry->getType().is(TY_STRUCT)) {
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
  llvm::Type *structType = structSymbol->getLLVMType();

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

  return structAddress;
}

std::any GeneratorVisitor::visitDataType(DataTypeNode *node) {
  emitSourceLocation(node);

  if (node->symbolType.is(TY_DYN)) {
    SymbolTableEntry *lhsVarEntry = currentScope->lookup(lhsVarName);
    assert(lhsVarEntry != nullptr);
    currentSymbolType = lhsVarEntry->getType();
    return currentSymbolType.toLLVMType(*context, currentScope);
  }

  if (!node->arraySizeExpr().empty()) {
    // Reset to base type because it will be rebuilt in the next step
    node->symbolType = node->symbolType.getBaseType();

    size_t assignExprCounter = 0;
    std::vector<AssignExprNode *> arraySizeExpr = node->arraySizeExpr();
    std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
    while (!tmQueue.empty()) {
      DataTypeNode::TypeModifier typeModifier = tmQueue.front();
      switch (typeModifier.modifierType) {
      case DataTypeNode::TY_POINTER: {
        node->symbolType = node->symbolType.toPointer(err.get(), node->codeLoc);
        break;
      }
      case DataTypeNode::TY_ARRAY: {
        if (!typeModifier.hasSize) {
          node->symbolType = node->symbolType.toPointer(err.get(), node->codeLoc);
        } else if (typeModifier.isSizeHardcoded) {
          node->symbolType = node->symbolType.toArray(err.get(), node->codeLoc, typeModifier.hardcodedSize);
        } else {
          AssignExprNode *indexExpr = arraySizeExpr[assignExprCounter++];
          assert(indexExpr != nullptr);
          auto sizeValuePtr = std::any_cast<llvm::Value *>(visit(indexExpr));
          dynamicArraySize = builder->CreateLoad(sizeValuePtr->getType()->getPointerElementType(), sizeValuePtr);
          node->symbolType = node->symbolType.toPointer(err.get(), node->codeLoc, dynamicArraySize);
        }
        break;
      }
      default:
        throw std::runtime_error("Modifier type fall-through");
      }
      tmQueue.pop();
    }
  }
  currentSymbolType = node->symbolType;
  return currentSymbolType.toLLVMType(*context, currentScope);
}

llvm::Value *GeneratorVisitor::resolveValue(AstNode *node) {
  std::any valueAny = visit(node);
  if (!valueAny.has_value() && currentConstValue)
    return currentConstValue;
  auto valueAddr = any_cast<llvm::Value *>(valueAny);
  return builder->CreateLoad(valueAddr->getType()->getPointerElementType(), valueAddr);
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
    allocaInst->moveAfter(allocaInsertInst);
    allocaInsertInst = allocaInst;
  } else {
    // Save current basic block and move insert cursor to entry block of the current function
    llvm::BasicBlock *currentBlock = builder->GetInsertBlock();
    builder->SetInsertPoint(allocaInsertBlock);

    allocaInsertInst = builder->CreateAlloca(llvmType, nullptr, varName);

    // Restore old basic block
    builder->SetInsertPoint(currentBlock);
  }
  return static_cast<llvm::Value *>(allocaInsertInst);
}

llvm::Value *GeneratorVisitor::allocateDynamicallySizedArray(llvm::Type *itemType) {
  // Call llvm.stacksave intrinsic
  llvm::Function *stackSaveFct = retrieveStackSaveFct();
  stackState = builder->CreateCall(stackSaveFct);
  // Allocate array
  llvm::Value *memAddress = builder->CreateAlloca(itemType, dynamicArraySize); // Intentionally not via insertAlloca
  dynamicArraySize = nullptr;
  return memAddress;
}

llvm::Value *GeneratorVisitor::createGlobalArray(llvm::Type *arrayType, const std::vector<llvm::Constant *> &itemConstants) {
  // Create hardcoded array
  auto type = static_cast<llvm::ArrayType *>(arrayType);
  llvm::Constant *constArray = llvm::ConstantArray::get(type, itemConstants);

  // Find an unused global name
  std::string globalVarName = lhsVarName;
  if (globalVarName.empty()) {
    unsigned int suffixNumber = 0;
    do {
      globalVarName = "anonymous." + std::to_string(suffixNumber);
      suffixNumber++;
    } while (module->getNamedGlobal(globalVarName) != nullptr);
  }

  // Create global variable
  module->getOrInsertGlobal(globalVarName, arrayType);
  llvm::Value *arrayAddress = insertAlloca(arrayType, lhsVarName);
  builder->CreateStore(constArray, arrayAddress);

  // Set some attributes to it
  llvm::GlobalVariable *global = module->getNamedGlobal(globalVarName);
  global->setConstant(true);
  global->setInitializer(constArray);
  return arrayAddress;
}

bool GeneratorVisitor::insertDestructorCall(const CodeLoc &codeLoc, SymbolTableEntry *varEntry) {
  Function *spiceDtor = currentScope->getFunctionAccessPointer(codeLoc);

  // Cancel if no destructor was found
  if (spiceDtor == nullptr)
    return false;

  // Get this value pointer
  llvm::Value *thisValuePtr = varEntry->getAddress();

  // Check if function exists in the current module
  bool functionFound = false;
  std::string fctIdentifier = spiceDtor->getMangledName();
  for (const auto &function : module->getFunctionList()) { // Search for function definition
    if (function.getName() == fctIdentifier) {
      functionFound = true;
      break;
    } else if (function.getName() == spiceDtor->getName()) {
      functionFound = true;
      fctIdentifier = spiceDtor->getName();
      break;
    }
  }

  if (!functionFound) { // Not found => Declare function, which will be linked in
    llvm::FunctionType *fctType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), thisValuePtr->getType(), false);
    module->getOrInsertFunction(fctIdentifier, fctType);
  }

  // Get the declared function and its type
  llvm::Function *fct = module->getFunction(fctIdentifier);
  assert(fct != nullptr);

  // Create the function call
  builder->CreateCall(fct, thisValuePtr);

  return true;
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

llvm::Function *GeneratorVisitor::retrieveStackSaveFct() {
  std::string stackSaveFctName = "llvm.stacksave";
  llvm::Function *stackSaveFct = module->getFunction(stackSaveFctName);
  if (stackSaveFct)
    return stackSaveFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *stackSaveFctTy = llvm::FunctionType::get(builder->getInt8PtrTy(), {}, false);
  module->getOrInsertFunction(stackSaveFctName, stackSaveFctTy);
  return module->getFunction(stackSaveFctName);
}

llvm::Function *GeneratorVisitor::retrieveStackRestoreFct() {
  std::string stackRestoreFctName = "llvm.stackrestore";
  llvm::Function *stackRestoreFct = module->getFunction(stackRestoreFctName);
  if (stackRestoreFct)
    return stackRestoreFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *stackRestoreFctTy = llvm::FunctionType::get(builder->getVoidTy(), builder->getInt8PtrTy(), false);
  module->getOrInsertFunction(stackRestoreFctName, stackRestoreFctTy);
  return module->getFunction(stackRestoreFctName);
}

llvm::Constant *GeneratorVisitor::getDefaultValueForType(llvm::Type *type, const std::string &subTypeName) {
  // Double
  if (OpRuleConversionsManager::isDouble(type))
    return currentConstValue = llvm::ConstantFP::get(*context, llvm::APFloat(0.0));

  // Int
  if (OpRuleConversionsManager::isInt(type))
    return currentConstValue = builder->getInt32(0);

  // Short
  if (OpRuleConversionsManager::isShort(type))
    return currentConstValue = builder->getInt16(0);

  // Long
  if (OpRuleConversionsManager::isLong(type))
    return currentConstValue = builder->getInt64(0);

  // Byte or char
  if (OpRuleConversionsManager::isByteOrChar(type))
    return currentConstValue = builder->getInt8(0);

  // String
  if (OpRuleConversionsManager::isString(type))
    return currentConstValue = builder->CreateGlobalStringPtr("", "", 0, module.get());

  // Bool
  if (OpRuleConversionsManager::isBool(type))
    return currentConstValue = builder->getFalse();

  // Pointer
  if (type->isPointerTy())
    return currentConstValue = llvm::Constant::getNullValue(type);

  // Array
  if (type->isArrayTy()) {
    size_t arraySize = type->getArrayNumElements();

    llvm::Type *itemType = type->getArrayElementType();
    llvm::ArrayType *arrayType = llvm::ArrayType::get(itemType, arraySize);
    llvm::Constant *zeroItem = getDefaultValueForType(itemType, subTypeName);
    std::vector<llvm::Constant *> itemConstants(arraySize, zeroItem);

    return llvm::ConstantArray::get(arrayType, itemConstants);
  }

  // Struct
  if (type->isStructTy()) {
    assert(!subTypeName.empty());
    SymbolTable *childTable = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + subTypeName);
    assert(childTable != nullptr);

    size_t fieldNumber = type->getStructNumElements();
    auto structType = static_cast<llvm::StructType *>(type);

    // Allocate space for the struct in memory
    llvm::Value *structAddress = insertAlloca(structType);

    std::vector<llvm::Type *> fieldTypes;
    std::vector<llvm::Constant *> fieldConstants;
    fieldTypes.reserve(fieldNumber);
    fieldConstants.reserve(fieldNumber);
    for (int i = 0; i < fieldNumber; i++) {
      SymbolTableEntry *fieldEntry = childTable->lookupByIndex(i);
      assert(fieldEntry != nullptr);
      llvm::Type *fieldType = type->getContainedType(i);
      fieldEntry->updateLLVMType(fieldType);
      fieldTypes.push_back(fieldType);
      llvm::Constant *defaultFieldValue = getDefaultValueForType(fieldType, fieldEntry->getType().getBaseType().getSubType());

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

void GeneratorVisitor::initializeDIBuilder(const std::string &sourceFileName, const std::string &sourceFileDir) {
  // Create DIBuilder
  diBuilder = std::make_unique<llvm::DIBuilder>(*module);
  // Create compilation unit
  debugInfo.diFile = diBuilder->createFile(sourceFileName, sourceFileDir);
  debugInfo.compileUnit =
      diBuilder->createCompileUnit(llvm::dwarf::DW_LANG_C, debugInfo.diFile, "Spice Compiler", cliOptions.optLevel > 0, "", 0);
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
    return symbolType.isSigned() ? debugInfo.intTy : debugInfo.uIntTy;
  if (symbolType.is(TY_SHORT))
    return symbolType.isSigned() ? debugInfo.shortTy : debugInfo.uShortTy;
  if (symbolType.is(TY_LONG))
    return symbolType.isSigned() ? debugInfo.longTy : debugInfo.uLongTy;
  if (symbolType.is(TY_BYTE))
    return symbolType.isSigned() ? debugInfo.byteTy : debugInfo.uByteTy;
  if (symbolType.is(TY_CHAR))
    return symbolType.isSigned() ? debugInfo.charTy : debugInfo.uCharTy;
  if (symbolType.is(TY_STRING))
    return debugInfo.stringTy;
  if (symbolType.is(TY_BOOL))
    return debugInfo.boolTy;

  return nullptr;
}

void GeneratorVisitor::generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc) {
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  size_t lineNumber = spiceFunc->getDeclCodeLoc().line;

  // Create function type
  std::vector<llvm::Metadata *> argTypes;
  argTypes.push_back(getDITypeForSymbolType(spiceFunc->getReturnType())); // Add result type
  for (auto &argType : spiceFunc->getArgTypes())                          // Add arg types
    argTypes.push_back(getDITypeForSymbolType(argType));
  llvm::DISubroutineType *functionTy = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(argTypes));

  llvm::DISubprogram *subprogram =
      diBuilder->createFunction(unit, spiceFunc->getName(), spiceFunc->getMangledName(), unit, lineNumber, functionTy, lineNumber,
                                llvm::DINode::FlagZero, llvm::DISubprogram::SPFlagDefinition);

  // Add debug info to LLVM function
  llvmFunction->setSubprogram(subprogram);
  // Add scope to lexicalBlocks
  debugInfo.lexicalBlocks.push_back(subprogram);
}

llvm::DIType *GeneratorVisitor::generateStructDebugInfo(llvm::StructType *llvmStructTy, const Struct *spiceStruct) const {
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  size_t lineNumber = spiceStruct->getDeclCodeLoc().line;
  size_t sizeInBits = module->getDataLayout().getTypeSizeInBits(llvmStructTy);
  llvm::DINode::DIFlags flags = spiceStruct->getSpecifiers().isPublic() ? llvm::DINode::FlagPublic : llvm::DINode::FlagPrivate;
  llvm::DINodeArray elements = diBuilder->getOrCreateArray({}); // ToDo: fill
  return diBuilder->createStructType(unit, spiceStruct->getName(), unit, lineNumber, sizeInBits, 0, flags, nullptr, elements);
}

void GeneratorVisitor::generateDeclDebugInfo(const CodeLoc &codeLoc, const std::string &varName, llvm::Value *address) {
  if (!cliOptions.generateDebugInfo)
    return;
  // Get symbol table entry
  SymbolTableEntry *variableEntry = currentScope->lookup(lhsVarName);
  assert(variableEntry != nullptr);
  // Build debug info
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  llvm::DIScope *scope = debugInfo.lexicalBlocks.back();
  llvm::DIType *diType = getDITypeForSymbolType(variableEntry->getType());
  llvm::DILocalVariable *varInfo = diBuilder->createAutoVariable(scope, currentVarName, unit, codeLoc.line, diType);
  llvm::DIExpression *expr = diBuilder->createExpression();
  diBuilder->insertDbgAddrIntrinsic(address, varInfo, expr, builder->getCurrentDebugLocation(), allocaInsertBlock);
}

void GeneratorVisitor::generateAssignDebugInfo(const CodeLoc &codeLoc, const std::string &varName, llvm::Value *value) {
  if (!cliOptions.generateDebugInfo)
    return;
  // Get symbol table entry
  SymbolTableEntry *variableEntry = currentScope->lookup(lhsVarName);
  assert(variableEntry != nullptr);
  // Build debug info
  llvm::DIFile *unit = diBuilder->createFile(debugInfo.compileUnit->getFilename(), debugInfo.compileUnit->getDirectory());
  llvm::DIScope *scope = debugInfo.lexicalBlocks.back();
  llvm::DIType *diType = getDITypeForSymbolType(variableEntry->getType());
  llvm::DILocalVariable *varInfo = diBuilder->createAutoVariable(scope, currentVarName, unit, codeLoc.line, diType);
  llvm::DIExpression *expr = diBuilder->createExpression();
  // Insert intrinsic call
  diBuilder->insertDbgValueIntrinsic(value, varInfo, expr, builder->getCurrentDebugLocation(), builder->GetInsertBlock());
}

void GeneratorVisitor::emitSourceLocation(AstNode *node) {
  if (!cliOptions.generateDebugInfo)
    return;
  if (debugInfo.lexicalBlocks.empty())
    return;
  llvm::DIScope *scope = debugInfo.lexicalBlocks.back();
  builder->SetCurrentDebugLocation(llvm::DILocation::get(scope->getContext(), node->codeLoc.line, node->codeLoc.col, scope));
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
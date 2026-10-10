// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include <algorithm>
#include <queue>
#include <unordered_set>

#include <ast/ASTBuilder.h>
#include <driver/Driver.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <exception/CompilerError.h>
#include <global/CacheManager.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <importcollector/ImportCollector.h>
#include <irgenerator/IRGenerator.h>
#include <iroptimizer/IROptimizer.h>
#include <linker/BitcodeLinker.h>
#include <objectemitter/LLVMObjectEmitter.h>
#ifdef SPICE_ENABLE_TPDE
#include <objectemitter/TPDEObjectEmitter.h>
#endif
#include <symboltablebuilder/SymbolTable.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/InterfaceManager.h>
#include <typechecker/MacroDefs.h>
#include <typechecker/PostTypeCheckingVerifier.h>
#include <typechecker/StructManager.h>
#include <typechecker/TypeChecker.h>
#include <util/CompilerWarning.h>
#include <util/Concurrency.h>
#include <util/FileUtil.h>
#include <util/SystemUtil.h>
#include <util/ThreadPool.h>
#include <util/Timer.h>
#include <visualizer/ASTVisualizer.h>
#include <visualizer/DependencyGraphVisualizer.h>

#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>

namespace spice::compiler {

/**
 * Map the Spice optimization level to the LLVM code generation optimization level, like Clang does.
 *
 * The code generation level must match the 'optnone' marking of the functions. At O0, all functions except the
 * 'alwaysinline' ones are marked as 'optnone', which forces the O0 instruction selector. The 'alwaysinline' ones would be
 * compiled with the instruction selector of the target machine's level. On arm64-apple-darwin, LLVM's two instruction
 * selectors disagree on the stack layout of by-value aggregate arguments, so calls between the two kinds of functions
 * (e.g. to an inline function in another module) would read their arguments from the wrong stack slots.
 *
 * @param optLevel Spice optimization level
 * @return LLVM code generation optimization level
 */
static llvm::CodeGenOptLevel getCodeGenOptLevel(OptLevel optLevel) {
  switch (optLevel) {
  case OptLevel::O0:
    return llvm::CodeGenOptLevel::None;
  case OptLevel::O1:
    return llvm::CodeGenOptLevel::Less;
  case OptLevel::O3:
    return llvm::CodeGenOptLevel::Aggressive;
  default: // O2, Os, Oz
    return llvm::CodeGenOptLevel::Default;
  }
}

SourceFile::SourceFile(GlobalResourceManager &resourceManager, SourceFile *parent, std::string name,
                       const std::filesystem::path &filePath, bool stdFile)
    : name(std::move(name)), filePath(filePath), isStdFile(stdFile), parent(parent),
      builder(resourceManager.cliOptions.useLTO ? resourceManager.ltoContext : context), resourceManager(resourceManager),
      cliOptions(resourceManager.cliOptions) {
  // Deduce fileName and fileDir
  fileName = std::filesystem::path(filePath).filename().string();
  fileDir = std::filesystem::path(filePath).parent_path().string();

  // Discard value names if not required
  context.setDiscardValueNames(!cliOptions.namesForIRValues);

  // Search after the selected target
  std::string error;
  const llvm::Target *target = llvm::TargetRegistry::lookupTarget(cliOptions.targetTriple, error);
  if (!target)
    throw CompilerError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // LCOV_EXCL_LINE

  // Create the target machine
  llvm::TargetOptions opt;
  opt.MCOptions.AsmVerbose = true;
  opt.MCOptions.PreserveAsmComments = true;
  const std::string &cpuName = resourceManager.cpuName;
  const std::string &features = resourceManager.cpuFeatures;
  const llvm::Triple &targetTriple = cliOptions.targetTriple;
  constexpr llvm::Reloc::Model relocModel = llvm::Reloc::PIC_;
  llvm::TargetMachine *targetMachineRaw = target->createTargetMachine(targetTriple, cpuName, features, opt, relocModel);
  targetMachine = std::unique_ptr<llvm::TargetMachine>(targetMachineRaw);
}

void SourceFile::runLexer() {
  // Check if this stage has already been done
  if (previousStage >= LEXER)
    return;

  Timer timer(&compilerOutput.times.lexer);
  timer.start();

  // Read from the input source file
  std::ifstream fileInputStream(filePath);
  if (!fileInputStream)
    throw CompilerError(SOURCE_FILE_NOT_FOUND, "Source file at path '" + filePath.string() + "' does not exist.");

  // Tokenize input
  antlrCtx.inputStream = std::make_unique<antlr4::ANTLRInputStream>(fileInputStream);
  antlrCtx.lexer = std::make_unique<SpiceLexer>(antlrCtx.inputStream.get());
  antlrCtx.lexer->removeErrorListeners();
  antlrCtx.lexerErrorHandler = std::make_unique<AntlrThrowingErrorListener>(ThrowingErrorListenerMode::LEXER, this);
  antlrCtx.lexer->addErrorListener(antlrCtx.lexerErrorHandler.get());
  antlrCtx.tokenStream = std::make_unique<antlr4::CommonTokenStream>(antlrCtx.lexer.get());

  // Pre-compute a local cache key so the field is populated for cycle-aware fallbacks.
  // The source key (which folds in transitive dependency cache keys) is computed at the end
  // of runImportCollector, once every dependency's cache key has been finalized.
  cacheKey = resourceManager.cacheManager.computeCacheKey(antlrCtx.tokenStream->getText());

  previousStage = LEXER;
  timer.stop();
  printStatusMessage("Lexer", IO_CODE, IO_TOKENS, compilerOutput.times.lexer);
}

void SourceFile::runParser() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= PARSER)
    return;

  Timer timer(&compilerOutput.times.parser);
  timer.start();

  // Parse input
  antlrCtx.parser = std::make_unique<SpiceParser>(antlrCtx.tokenStream.get()); // Check for syntax errors
  antlrCtx.parser->removeErrorListeners();
  antlrCtx.parserErrorHandler = std::make_unique<AntlrThrowingErrorListener>(ThrowingErrorListenerMode::PARSER, this);
  antlrCtx.parser->addErrorListener(antlrCtx.parserErrorHandler.get());
  antlrCtx.parser->removeParseListeners();

  previousStage = PARSER;
  timer.stop();
  printStatusMessage("Parser", IO_TOKENS, IO_CST, compilerOutput.times.parser);
}

void SourceFile::runASTBuilder() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= AST_BUILDER)
    return;

  Timer timer(&compilerOutput.times.astBuilder);
  timer.start();

  // Build AST for this source file
  ASTBuilder astBuilder(resourceManager, this, antlrCtx.inputStream.get());
  ast = std::any_cast<EntryNode *>(astBuilder.visit(parseEntry()));
  antlrCtx.parser->reset();

  // Create global scope
  globalScope = std::make_unique<Scope>(nullptr, this, ScopeType::GLOBAL, &ast->codeLoc);

  previousStage = AST_BUILDER;
  timer.stop();
  printStatusMessage("AST Builder", IO_CST, IO_AST, compilerOutput.times.astBuilder);
}

/**
 * Parse the token stream of this source file in two stages. The first stage uses the SLL prediction mode, which is much
 * faster than the full LL prediction mode, but cannot handle all inputs. It does not report syntax errors, but bails out
 * at the first one. Only then the second stage parses the whole token stream again with the LL prediction mode, which
 * reports the syntax error exactly like a single LL stage would.
 *
 * @return Parse tree of the source file
 */
SpiceParser::EntryContext *SourceFile::parseEntry() const {
  SpiceParser &parser = *antlrCtx.parser;
  auto *interpreter = parser.getInterpreter<antlr4::atn::ParserATNSimulator>();

  // Try SLL prediction mode
  interpreter->setPredictionMode(antlr4::atn::PredictionMode::SLL);
  parser.removeErrorListeners();
  parser.setErrorHandler(std::make_shared<antlr4::BailErrorStrategy>());
  try {
    return parser.entry();
  } catch (const antlr4::ParseCancellationException &) {
    // Fall back to LL prediction mode to report syntax errors correctly
    antlrCtx.tokenStream->seek(0);
    parser.reset();
    interpreter->setPredictionMode(antlr4::atn::PredictionMode::LL);
    parser.addErrorListener(antlrCtx.parserErrorHandler.get());
    parser.setErrorHandler(std::make_shared<antlr4::DefaultErrorStrategy>());
    return parser.entry();
  }
}

void SourceFile::runASTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache)
    return;
  if (!cliOptions.dump.dumpAST && !cliOptions.testMode)
    return;
  // Check if this stage has already been done
  if (previousStage >= AST_VISUALIZER)
    return;

  Timer timer(&compilerOutput.times.astVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  ASTVisualizer astVisualizer(resourceManager, this);
  dotCode << " " << std::any_cast<std::string>(astVisualizer.visit(ast)) << "}";

  // Dump the serialized AST string and the SVG file
  compilerOutput.astString = dotCode.str();

  if (cliOptions.dump.dumpAST)
    visualizerOutput("AST", compilerOutput.astString);

  previousStage = AST_VISUALIZER;
  timer.stop();
  printStatusMessage("AST Visualizer", IO_AST, IO_AST, compilerOutput.times.astVisualizer);
}

void SourceFile::runImportCollector() { // NOLINT(misc-no-recursion)
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IMPORT_COLLECTOR)
    return;

  Timer timer(&compilerOutput.times.importCollector);
  timer.start();

  // Collect the imports for this source file
  ImportCollector importCollector(resourceManager, this);
  importCollector.visit(ast);

  previousStage = IMPORT_COLLECTOR;

  // Run first part of pipeline for the imported source file
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->runFrontEnd();

  // Now that every transitive dependency has its final cache key, fold them into our own
  // cache key. This way any change to a dependency invalidates the cache entry of every
  // dependent (and transitively of the dependents' dependents), avoiding stale object files.
  std::vector<std::string> transitiveDepCacheKeys;
  std::unordered_set<std::string> visited;
  std::queue<const SourceFile *> worklist;
  for (const SourceFile *dep : dependencies | std::views::values)
    worklist.push(dep);
  while (!worklist.empty()) {
    const SourceFile *dep = worklist.front();
    worklist.pop();
    if (!visited.insert(dep->cacheKey).second)
      continue;
    transitiveDepCacheKeys.push_back(dep->cacheKey);
    for (const SourceFile *transitive : dep->dependencies | std::views::values)
      worklist.push(transitive);
  }
  cacheKey = resourceManager.cacheManager.computeCacheKey(antlrCtx.tokenStream->getText(), transitiveDepCacheKeys);

  timer.stop();
  printStatusMessage("Import Collector", IO_AST, IO_AST, compilerOutput.times.importCollector);
}

void SourceFile::runSymbolTableBuilder() {
  // Skip if this stage has already been done. Unlike the later stages, this one must still run even if the file was
  // restored from the cache: it's the only pass that populates exportedNameRegistry, and a dependant that isn't itself
  // a cache hit needs that registry to resolve the symbols it imports from this file.
  if (previousStage >= SYMBOL_TABLE_BUILDER)
    return;

  Timer timer(&compilerOutput.times.symbolTableBuilder);
  timer.start();

  // Build symbol table of the current file. The dependencies' exported name registries are merged in afterwards, in a
  // separate pass (mergeNameRegistriesRecursive), once every reachable file has built its own registry. This deferral
  // is what makes circular imports work: with a cycle, a dependency's registry is not fully populated yet at this point.
  SymbolTableBuilder symbolTableBuilder(resourceManager, this);
  symbolTableBuilder.visit(ast);

  previousStage = SYMBOL_TABLE_BUILDER;
  timer.stop();
  printStatusMessage("Symbol Table Builder", IO_AST, IO_AST, compilerOutput.times.symbolTableBuilder);
}

void SourceFile::runTypeCheckerPre() { // NOLINT(misc-no-recursion)
  // Skip if this stage has already been done. Unlike the later (codegen) stages, this one must still run even if the
  // file was restored from the cache: it's what populates the FunctionManager/StructManager manifestations that a
  // dependant which isn't itself a cache hit needs for overload resolution and generic substantiation.
  // The typeCheckerPreRunning guard breaks the recursion on a circular import: a cyclic back-edge returns immediately
  // instead of recursing forever. The file is still pre-checked once the in-progress invocation reaches it, and any
  // cross-file references left unresolved (because a cycle peer was not pre-checked yet) are fixed up by the post run.
  if (previousStage >= TYPE_CHECKER_PRE || typeCheckerPreRunning)
    return;
  typeCheckerPreRunning = true;

  // Type-check all dependencies first
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->runTypeCheckerPre();

  Timer timer(&compilerOutput.times.typeCheckerPre);
  timer.start();

  // Then type-check the current file
  TypeChecker typeChecker(resourceManager, this, TC_MODE_PRE);
  typeChecker.visit(ast);

  previousStage = TYPE_CHECKER_PRE;
  typeCheckerPreRunning = false;
  timer.stop();
  printStatusMessage("Type Checker Pre", IO_AST, IO_AST, compilerOutput.times.typeCheckerPre);
}

void SourceFile::runTypeCheckerPost() { // NOLINT(misc-no-recursion)
  // Re-entrancy guard: within an import cycle, a dependency's post-run recurses back into this file's post-run. The
  // in-flight fixpoint loop below already revisits this file, so the nested call must be a no-op to avoid unbounded
  // mutual recursion. Convergence is driven by the reVisitRequested flags propagating across the cycle.
  if (typeCheckerPostRunning)
    return;

  // Skip if not all dependants finished type checking yet. This still has to run for files restored from the cache,
  // for the same reason as runTypeCheckerPre (see comment there).
  if (!haveAllDependantsBeenTypeChecked())
    return;

  typeCheckerPostRunning = true;

  Timer timer(&compilerOutput.times.typeCheckerPost);
  timer.start();

  // Start type-checking loop. The type-checker can request a re-execution. The max number of type-checker runs is limited
  TypeChecker typeChecker(resourceManager, this, TC_MODE_POST);
  unsigned short typeCheckerRuns = 0;
  while (reVisitRequested) {
    typeCheckerRuns++;
    totalTypeCheckerRuns++;
    reVisitRequested = false;

    // Type-check the current file first. Multiple times, if requested
    timer.resume();
    typeChecker.visit(ast);
    timer.pause();

    // Then type-check all dependencies
    for (SourceFile *sourceFile : dependencies | std::views::values)
      sourceFile->runTypeCheckerPost();
  }

  typeCheckerPostRunning = false;

  checkForSoftErrors();

  // Check if all dyn variables were type-inferred successfully
  globalScope->ensureSuccessfulTypeInference();

#ifndef NDEBUG
  // In debug builds, verify that the TypeChecker fully annotated the AST
  runPostTypeCheckingVerifier();
#endif

  previousStage = TYPE_CHECKER_POST;
  timer.stop();
  printStatusMessage("Type Checker Post", IO_AST, IO_AST, compilerOutput.times.typeCheckerPost, typeCheckerRuns);

  // Save the JSON version in the compiler output
  if (cliOptions.dump.dumpSymbolTable || cliOptions.testMode)
    compilerOutput.symbolTableString = globalScope->getSymbolTableJSON().dump(/*indent=*/2);

  // Dump symbol table
  if (cliOptions.dump.dumpSymbolTable)
    dumpOutput(compilerOutput.symbolTableString, "Symbol Table", "symbol-table.json");
}

void SourceFile::runPostTypeCheckingVerifier() {
  PostTypeCheckingVerifier verifier(resourceManager, this);
  verifier.verify(ast);
}

void SourceFile::runDependencyGraphVisualizer() {
  // Only execute if enabled
  if (restoredFromCache)
    return;
  if (!cliOptions.dump.dumpDependencyGraph && !cliOptions.testMode)
    return;
  // Check if this stage has already been done
  if (previousStage >= DEP_GRAPH_VISUALIZER)
    return;

  Timer timer(&compilerOutput.times.depGraphVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  DependencyGraphVisualizer depGraphVisualizer(resourceManager, this);
  depGraphVisualizer.getDependencyGraph(dotCode);
  dotCode << "}";

  // Dump the serialized AST string and the SVG file
  compilerOutput.depGraphString = dotCode.str();

  if (cliOptions.dump.dumpDependencyGraph)
    visualizerOutput("Dependency Graph", compilerOutput.depGraphString);

  previousStage = DEP_GRAPH_VISUALIZER;
  timer.stop();
  printStatusMessage("AST Visualizer", IO_AST, IO_AST, compilerOutput.times.depGraphVisualizer);
}

void SourceFile::runIRGenerator() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_GENERATOR)
    return;

  Timer timer(&compilerOutput.times.irGenerator);
  timer.start();

  // Create the LLVM module for this source file
  llvm::LLVMContext &llvmContext = cliOptions.useLTO ? resourceManager.ltoContext : context;
  llvmModule = std::make_unique<llvm::Module>(fileName, llvmContext);

  // Generate this source file
  IRGenerator irGenerator(resourceManager, this);
  irGenerator.visit(ast);

  // Save the ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode)
    compilerOutput.irString = IRGenerator::getIRString(llvmModule.get(), cliOptions);

  // Dump unoptimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irString, "Unoptimized IR Code", "ir-code.ll");

  previousStage = IR_GENERATOR;
  timer.stop();
  printStatusMessage("IR Generator", IO_AST, IO_IR, compilerOutput.times.irGenerator);
}

void SourceFile::runDefaultIROptimizer() {
  assert(!cliOptions.useLTO);

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage > IR_OPTIMIZER || (previousStage == IR_OPTIMIZER && !cliOptions.testMode))
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.start();

  // Optimize this source file
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.prepare();
  irOptimizer.optimizeDefault();

  // Save the optimized ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode)
    compilerOutput.irOptString = IRGenerator::getIRString(llvmModule.get(), cliOptions);

  // Dump optimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irOptString, "Optimized IR Code",
               "ir-code-O" + std::to_string(static_cast<uint8_t>(cliOptions.optLevel)) + ".ll");

  previousStage = IR_OPTIMIZER;
  timer.stop();
  printStatusMessage("IR Optimizer", IO_IR, IO_IR, compilerOutput.times.irOptimizer);
}

void SourceFile::runPreLinkIROptimizer() {
  assert(cliOptions.useLTO);

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_OPTIMIZER)
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.start();

  // Optimize this source file
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.prepare();
  irOptimizer.optimizePreLink();

  // Save the optimized ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode)
    compilerOutput.irOptString = IRGenerator::getIRString(llvmModule.get(), cliOptions);

  // Dump optimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irOptString, "Optimized IR Code (pre-link)", "ir-code-lto-pre-link.ll");

  timer.pause();
}

void SourceFile::runBitcodeLinker() {
  assert(cliOptions.useLTO);

  // Skip if this is not the main source file
  if (!isMainFile)
    return;

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_OPTIMIZER)
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.resume();

  // Link all source files together
  BitcodeLinker linker(resourceManager);
  linker.link();

  timer.pause();
}

void SourceFile::runPostLinkIROptimizer() {
  assert(cliOptions.useLTO);

  // Skip if this is not the main source file
  if (!isMainFile)
    return;

  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= IR_OPTIMIZER)
    return;

  Timer timer(&compilerOutput.times.irOptimizer);
  timer.resume();

  // Optimize LTO module
  IROptimizer irOptimizer(resourceManager, this);
  irOptimizer.prepare();
  irOptimizer.optimizePostLink();

  // Save the optimized ir string in the compiler output
  if (cliOptions.dump.dumpIR || cliOptions.testMode) {
    llvm::Module *module = resourceManager.ltoModule.get();
    compilerOutput.irOptString = IRGenerator::getIRString(module, cliOptions);
  }

  // Dump optimized IR code
  if (cliOptions.dump.dumpIR)
    dumpOutput(compilerOutput.irOptString, "Optimized IR Code (post-Link)", "ir-code-lto-post-link.ll");

  previousStage = IR_OPTIMIZER;
  timer.stop();
  printStatusMessage("IR Optimizer", IO_IR, IO_IR, compilerOutput.times.irOptimizer);
}

void SourceFile::runObjectEmitter() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= OBJECT_EMITTER)
    return;

  // Skip if LTO is enabled and this is not the main source file
  if (cliOptions.useLTO && !isMainFile)
    return;

  Timer timer(&compilerOutput.times.objectEmitter);
  timer.start();

  // Let the code generation opt level match the opt level, the IR was generated and optimized with
  targetMachine->setOptLevel(getCodeGenOptLevel(cliOptions.optLevel));

  // Deduce an object file path
  objectFilePath = cliOptions.outputDir / filePath.filename();
  objectFilePath.replace_extension("o");

  // Pick a concrete emitter based on the selected backend. The TPDE emitter is compiled into a
  // sibling library (spice_tpde) that keeps its -fno-rtti requirement out of spicecore; the
  // AbstractObjectEmitter base gives us a single interface both branches produce.
  std::unique_ptr<AbstractObjectEmitter> objectEmitter;
#ifdef SPICE_ENABLE_TPDE
  if (cliOptions.backend == Backend::TPDE) {
    llvm::Module &module = cliOptions.useLTO ? *resourceManager.ltoModule : *llvmModule;
    objectEmitter = std::make_unique<TPDEObjectEmitter>(module);
  } else {
    objectEmitter = std::make_unique<LLVMObjectEmitter>(resourceManager, this);
  }
#else
  objectEmitter = std::make_unique<LLVMObjectEmitter>(resourceManager, this);
#endif

  // Emit object for this source file
  objectEmitter->emit(objectFilePath);

  // Save assembly string in the compiler output (TPDE emits a placeholder note)
  if (cliOptions.isNativeTarget && (cliOptions.dump.dumpAssembly || cliOptions.testMode))
    objectEmitter->getASMString(compilerOutput.asmString);

  // Dump assembly code
  if (cliOptions.dump.dumpAssembly)
    dumpOutput(compilerOutput.asmString, "Assembly code", "assembly-code.s");

  // The object file is registered with the linker in concludeCompilation and not here, because this stage may run on a
  // worker thread of the parallel back end and the linker input order has to stay deterministic.

  previousStage = OBJECT_EMITTER;
  timer.stop();
  printStatusMessage("Object Emitter", IO_IR, IO_OBJECT_FILE, compilerOutput.times.objectEmitter);
}

void SourceFile::concludeCompilation() {
  // Handle cache-restored files: register all cached objects with linker
  if (restoredFromCache) {
    for (const auto &cachedObjectFilePath : cachedObjectFilePaths)
      resourceManager.linker.addFileToLinkage(cachedObjectFilePath);
    for (const auto &flag : sourceLinkerFlags)
      resourceManager.linker.addLinkerFlag(flag);
    for (const auto &path : sourceAdditionalSourcePaths)
      resourceManager.linker.addAdditionalSourcePath(path);
    return;
  }

  if (previousStage >= FINISHED)
    return;

  // Add the emitted object file to the linker objects. This happens here and not in runObjectEmitter, because
  // concludeCompilation is always driven serially and in dependency order, while the object emitter may run on a worker
  // thread of the parallel back end.
  if (!objectFilePath.empty())
    resourceManager.linker.addFileToLinkage(objectFilePath);

  // Cache the source file
  if (!cliOptions.ignoreCache)
    resourceManager.cacheManager.cacheSourceFile(this);

  // Save type registry as string in the compiler output
  if (isMainFile && (cliOptions.dump.dumpTypes || cliOptions.testMode))
    compilerOutput.typesString = TypeRegistry::dump();

  // Dump type registry
  if (isMainFile && cliOptions.dump.dumpTypes)
    dumpOutput(compilerOutput.typesString, "Type Registry", "type-registry.out");

  // Save cache statistics as string in the compiler output
  if (isMainFile && (cliOptions.dump.dumpCacheStats || cliOptions.testMode))
    dumpCacheStats();

  // Dump lookup cache statistics
  if (isMainFile && cliOptions.dump.dumpCacheStats)
    dumpOutput(compilerOutput.cacheStats, "Cache Statistics", "cache-stats.out");

  if (cliOptions.printDebugOutput)
    std::cout << "Finished compiling " << fileName << std::endl;

  previousStage = FINISHED;
}

void SourceFile::runFrontEnd() { // NOLINT(misc-no-recursion)
  // The front end of the main source file covers the front ends of all its (transitive) dependencies. A circular import
  // re-enters the front end of the main source file, so only measure the outermost call, which starts with its lexer
  const bool measureWallTime = isMainFile && previousStage == NONE;
  if (measureWallTime)
    resourceManager.frontEndTimer.start();

  runLexer();
  CHECK_ABORT_FLAG_V()
  runParser();
  CHECK_ABORT_FLAG_V()
  runASTBuilder();
  CHECK_ABORT_FLAG_V()
  runASTVisualizer();
  CHECK_ABORT_FLAG_V()
  runImportCollector();
  CHECK_ABORT_FLAG_V()
  runSymbolTableBuilder();
  CHECK_ABORT_FLAG_V()

  if (measureWallTime)
    resourceManager.frontEndTimer.stop();
}

void SourceFile::runMiddleEnd() {
  if (isMainFile)
    resourceManager.middleEndTimer.start();

  // Merge the exported name registries of all (transitive) dependencies into the respective importing files. This is
  // the deferred tail of the front-end: it must run after every reachable file has built its own registry, which is
  // why it cannot live inside the per-file front-end recursion (a circular import would otherwise merge a dependency
  // whose registry is not populated yet). runMiddleEnd is the first stage that is only ever invoked at the top level.
  mergeNameRegistriesRecursive();
  CHECK_ABORT_FLAG_V()
  // From here on, struct manifestations may be substantiated, and each of them gets its compiler-generated default
  // members decided right at that point (see TypeChecker::createImplicitDefaultMembers). Once the middle end is done,
  // every manifestation exists and was decided on, so the back end must not create any more of them.
  const DefaultMemberCreationSection defaultMemberCreationSection;
  // We need two runs here due to generics.
  // The first run to determine all concrete function/struct/interface substantiations
  runTypeCheckerPre(); // Visit the dependency tree from bottom to top
  CHECK_ABORT_FLAG_V()
  // The second run to ensure, also generic scopes are type-checked properly
  runTypeCheckerPost(); // Visit the dependency tree from top to bottom in topological order
  CHECK_ABORT_FLAG_V()
  // The per-file convergence loop inside runTypeCheckerPost is scoped to its own call stack: a cross-file revisit
  // request that lands on a file whose loop already unwound (e.g. a recursive generic dtor chain that closes back
  // through a runtime module which was itself gated behind another, not-yet-checked importer) is otherwise dropped,
  // leaving a fully-substantiated manifestation whose body was never type-checked. Sweep every source file in the
  // program for a straggling revisit request and drive it to convergence directly, repeating until none are left.
  bool anySourceFileRevisitPending;
  do {
    anySourceFileRevisitPending = false;
    // Snapshot the current files before driving any of them: runTypeCheckerPost() below can itself trigger a
    // freshly-discovered runtime import (SourceFile::requestRuntimeModule -> GlobalResourceManager::createSourceFile),
    // which inserts into resourceManager.sourceFiles - iterating that map while it is being mutated is undefined
    // behavior, so a stable list of raw pointers is collected first. It is taken in creation order, so that the revisits
    // happen in a deterministic order, independent of the hash function of the source file map.
    const std::vector<SourceFile *> sourceFilesSnapshot = resourceManager.sourceFilesInCreationOrder;
    for (SourceFile *sourceFile : sourceFilesSnapshot) {
      if (sourceFile->reVisitRequested) {
        sourceFile->runTypeCheckerPost();
        anySourceFileRevisitPending = true;
      }
    }
    // A source file created mid-sweep (see above) still needs its own pass; it starts out with reVisitRequested
    // true, so re-looping picks it up via the snapshot taken on the next iteration.
    if (resourceManager.sourceFiles.size() > sourceFilesSnapshot.size())
      anySourceFileRevisitPending = true;
  } while (anySourceFileRevisitPending);
  CHECK_ABORT_FLAG_V()
  // Check that no two definitions of the program are exported under the same linker symbol name
  checkForExportedSymbolCollisions();
  CHECK_ABORT_FLAG_V()
  // Visualize dependency graph
  runDependencyGraphVisualizer();
  CHECK_ABORT_FLAG_V()

  if (isMainFile)
    resourceManager.middleEndTimer.stop();
}

void SourceFile::lookupCache() {
  // Generic instantiations end up in the object of the defining module but are requested by its importers, so the key has
  // to cover them. They are only final after the middle end, which is why the lookup is not done in runImportCollector.
  assert(previousStage >= TYPE_CHECKER_POST);
  std::stringstream manifestations;
  globalScope->collectManifestationFingerprint(manifestations);
  cacheKey = CacheManager::foldManifestations(cacheKey, manifestations);
  restoredFromCache = resourceManager.cacheManager.lookupSourceFile(this);
}

void SourceFile::collectBackEndSourceFiles(std::vector<SourceFile *> &backEndSourceFiles) { // NOLINT(misc-no-recursion)
  // Guard against collecting a file that already went through the back end. Circular imports form a cycle in the
  // dependency graph, so the deps-first recursion below would otherwise loop forever.
  if (backEndStarted)
    return;
  backEndStarted = true;

  // Collect all dependencies first, so that they end up in front of this file in the resulting list
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->collectBackEndSourceFiles(backEndSourceFiles);

  backEndSourceFiles.push_back(this);
}

void SourceFile::runBackEndForThisFile() {
  runIRGenerator();
  CHECK_ABORT_FLAG_V()
  if (cliOptions.useLTO) {
    runPreLinkIROptimizer();
    CHECK_ABORT_FLAG_V()
    runBitcodeLinker();
    CHECK_ABORT_FLAG_V()
    runPostLinkIROptimizer();
    CHECK_ABORT_FLAG_V()
  } else {
    runDefaultIROptimizer();
    CHECK_ABORT_FLAG_V()
  }
  runObjectEmitter();
}

void SourceFile::runBackEnd() {
  if (isMainFile)
    resourceManager.backEndTimer.start();

  // Flatten the dependency graph into the order the back end used to recurse in: every file comes after all of its
  // dependencies, and files that already ran their back end are skipped.
  std::vector<SourceFile *> backEndSourceFiles;
  collectBackEndSourceFiles(backEndSourceFiles);

  // Nothing to do if this file and all of its dependencies already went through the back end
  if (backEndSourceFiles.empty())
    return;

  // Look up all files before compiling any: a key is only final after its lookup and cache entries of dependants refer to it
  if (!cliOptions.ignoreCache)
    for (SourceFile *sourceFile : backEndSourceFiles)
      sourceFile->lookupCache();

  // Unlike the front end and the middle end, the back end has no cross-file data dependencies: every source file owns
  // its own LLVMContext, IRBuilder, TargetMachine and llvm::Module, and references to symbols of other files are emitted
  // as declarations into the local module. So the per-file pipelines can simply be spread over a worker pool.
  // Exceptions, in which the back end stays serial:
  // - LTO, because all source files share the LTO context and module of the GlobalResourceManager
  // - dump modes, because they write to the console/output dir and their ordering is part of the user-visible output
  const bool dumpRequested = cliOptions.dump.dumpIR || cliOptions.dump.dumpAssembly || cliOptions.dump.dumpObjectFiles;
  const size_t jobCount = std::min(resourceManager.getCompileJobCount(), backEndSourceFiles.size());
  const bool runParallel = jobCount > 1 && !cliOptions.useLTO && !dumpRequested;

  if (runParallel) {
    ThreadPool &threadPool = resourceManager.getThreadPool(jobCount);
    const ParallelSection parallelSection;
    for (SourceFile *sourceFile : backEndSourceFiles)
      threadPool.submit([sourceFile] { sourceFile->runBackEndForThisFile(); });
    threadPool.waitForAll(); // Re-throws the exception of the first failing source file, if there was one
  } else {
    for (SourceFile *sourceFile : backEndSourceFiles) {
      sourceFile->runBackEndForThisFile();
      CHECK_ABORT_FLAG_V()
    }
  }
  CHECK_ABORT_FLAG_V()

  // Conclude the compilation of all source files. This registers the emitted object files with the linker and writes the
  // compilation cache, both of which have to happen serially and in a fixed order to stay deterministic.
  for (SourceFile *sourceFile : backEndSourceFiles)
    sourceFile->concludeCompilation();

  if (isMainFile) {
    resourceManager.backEndTimer.stop();
    if (cliOptions.printDebugOutput)
      dumpCompilationStats();
  }
}

void SourceFile::addDependency(SourceFile *sourceFile, const std::string &dependencyName) {
  // Circular imports are explicitly supported, so cycles are not rejected here. Source files are deduplicated by path
  // in GlobalResourceManager::createSourceFile, so a cyclic import resolves to the same SourceFile instance and the
  // pipeline drivers guard against re-entering a file that is already in progress.

  // Add the dependency. Do not demote the compilation root (parent == nullptr) to a non-main file: with a circular
  // import the root can be imported by one of its own transitive dependencies, yet it must remain the main file (the
  // isMainFile flag drives getRootSourceFile, object emission, timing, etc.).
  if (sourceFile->parent != nullptr)
    sourceFile->isMainFile = false;
  dependencies.emplace(dependencyName, sourceFile);

  // Add the dependant
  sourceFile->dependants.push_back(this);
}

bool SourceFile::imports(const SourceFile *sourceFile) const {
  return std::ranges::any_of(dependencies, [=](const auto &dependency) { return dependency.second == sourceFile; });
}

SourceFile *SourceFile::requestRuntimeModule(RuntimeModule runtimeModule) {
  // Check if the module was already imported
  if (isRuntimeModuleAvailable(runtimeModule))
    return resourceManager.runtimeModuleManager.getModule(runtimeModule);
  return resourceManager.runtimeModuleManager.requestModule(this, runtimeModule);
}

bool SourceFile::isRuntimeModuleAvailable(RuntimeModule runtimeModule) const { return importedRuntimeModules & runtimeModule; }

void SourceFile::addNameRegistryEntry(const std::string &symbolName, uint64_t typeId, SymbolTableEntry *entry, Scope *scope,
                                      bool keepNewOnCollision, SymbolTableEntry *importEntry) {
  const auto it = exportedNameRegistry.find(symbolName);
  if (keepNewOnCollision || it == exportedNameRegistry.end()) { // Overwrite potential existing entry
    if (it != exportedNameRegistry.end())
      untrackNameRegistryTargetEntry(it->second.targetEntry);
    exportedNameRegistry[symbolName] = {symbolName, typeId, entry, scope, importEntry};
    trackNameRegistryTargetEntry(entry);
    if (keepNewOnCollision)
      ambiguousNameRegistry.erase(symbolName);
    return;
  }

  // Name collision => we must remove the existing entry. Remember the colliding imports to report the ambiguity later
  std::vector<const SymbolTableEntry *> &collidingImports = ambiguousNameRegistry[symbolName];
  collidingImports.push_back(it->second.importEntry);
  collidingImports.push_back(importEntry);
  untrackNameRegistryTargetEntry(it->second.targetEntry);
  exportedNameRegistry.erase(it);
}

const NameRegistryEntry *SourceFile::getNameRegistryEntry(const std::string &symbolName) const {
  const auto it = exportedNameRegistry.find(symbolName);
  if (it == exportedNameRegistry.end())
    return nullptr;

  // Resolve registry entry for the given name
  const NameRegistryEntry *entry = &it->second;

  // Mark the import entry as used
  if (entry->importEntry != nullptr)
    entry->importEntry->used = true;

  return entry;
}

bool SourceFile::isAmbiguousName(const std::string &symbolName) const { return ambiguousNameRegistry.contains(symbolName); }

/**
 * Check if the name registry of this source file knows the struct manifestation with the given body scope
 *
 * @param bodyScope Body scope of the struct manifestation
 * @param structName Name of the struct
 * @return Known or not
 */
bool SourceFile::isStructKnownByNameRegistry(Scope *bodyScope, const std::string &structName) const {
  if (bodyScope == nullptr || bodyScope->parent == nullptr)
    return false;

  // All manifestations of a struct live in the scope the struct is defined in, next to the struct's symbol table entry.
  // The name registry entry of a struct points to this symbol table entry, so we can look it up instead of scanning the
  // whole name registry
  const SymbolTableEntry *structEntry = bodyScope->parent->lookupStrict(structName);
  if (structEntry == nullptr || !nameRegistryTargetEntryCounts.contains(structEntry))
    return false;
  if (!structEntry->getQualType().isBase(TY_STRUCT))
    return false;

  // Check if the body scope belongs to one of the manifestations of the struct
  const std::vector<Struct *> *manifestations = structEntry->declNode->getStructManifestations();
  return std::ranges::any_of(*manifestations, [&](const Struct *manifestation) { return manifestation->scope == bodyScope; });
}

/**
 * Build an error message for a name that is not available, because multiple imports expose it. The message lists the
 * colliding imports and suggests how to qualify the name.
 *
 * @param symbolName Ambiguous name
 * @return Error message
 */
std::string SourceFile::getAmbiguousNameMessage(const std::string &symbolName) const {
  assert(isAmbiguousName(symbolName));

  // Collect the distinct imports that expose the name. Runtime imports have no import entry and are skipped
  std::vector<const ImportDefNode *> importNodes;
  for (const SymbolTableEntry *importEntry : ambiguousNameRegistry.at(symbolName)) {
    if (importEntry == nullptr)
      continue;
    const auto importNode = spice_pointer_cast<const ImportDefNode *>(importEntry->declNode);
    if (std::ranges::find(importNodes, importNode) == importNodes.end())
      importNodes.push_back(importNode);
  }

  std::stringstream msg;
  msg << "'" << symbolName << "' is exposed by ";
  if (importNodes.size() < 2) {
    msg << "multiple imports";
  } else {
    if (importNodes.size() == 2)
      msg << "both ";
    for (size_t i = 0; i < importNodes.size(); i++) {
      if (i > 0)
        msg << (i == importNodes.size() - 1 ? " and " : ", ");
      msg << "'" << importNodes.at(i)->importPath << "'";
    }
  }

  // Suggest qualifying the name with the first import name, that can be written in source code
  const auto isIdentifier = [](const std::string &name) {
    const auto isIdentifierChar = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };
    return !name.empty() && (std::islower(static_cast<unsigned char>(name.front())) || name.front() == '_') &&
           std::ranges::all_of(name, isIdentifierChar);
  };
  const auto qualifiableImport =
      std::ranges::find_if(importNodes, [&](const ImportDefNode *importNode) { return isIdentifier(importNode->importName); });
  if (qualifiableImport != importNodes.end())
    msg << ". Please qualify it, e.g. '" << (*qualifiableImport)->importName << SCOPE_ACCESS_TOKEN << symbolName << "'";
  else
    msg << ". Please import one of them with an alias and qualify it, e.g. 'alias" << SCOPE_ACCESS_TOKEN << symbolName << "'";
  return msg.str();
}

llvm::Type *SourceFile::getLLVMType(const Type *type) {
  // Check if the type is already in the mapping
  const auto it = typeToLLVMTypeMapping.find(type);
  if (it != typeToLLVMTypeMapping.end())
    return it->second;

  // If not, generate the LLVM type
  llvm::Type *llvmType = type->toLLVMType(this);
  typeToLLVMTypeMapping[type] = llvmType;
  return llvmType;
}

void SourceFile::checkForSoftErrors() const {
  // Check if there are any soft errors and if so, print them
  if (!resourceManager.errorManager.softErrors.empty()) {
    std::stringstream errorStream;
    errorStream << "There are unresolved errors. Please fix them and recompile.";
    for (const auto &[codeLoc, message] : resourceManager.errorManager.softErrors)
      errorStream << "\n\n" << message;
    throw CompilerError(UNRESOLVED_SOFT_ERRORS, errorStream.str());
  }
}

/**
 * Check that no two definitions of the program are exported under the same linker symbol name. Non-public symbols get
 * internal linkage and therefore can never collide. Public ones get external linkage under a name that does not contain
 * the source file, so e.g. two public functions with the same signature in different source files would break the linking.
 * Every collision is reported as soft error.
 */
void SourceFile::checkForExportedSymbolCollisions() const {
  std::unordered_map<std::string, const ASTNode *> exportedSymbols;
  std::unordered_set<const SourceFile *> visitedSourceFiles;
  registerExportedSymbols(exportedSymbols, visitedSourceFiles);
  checkForSoftErrors();
}

/**
 * Register the exported symbols of this source file and all its (transitive) dependencies in the given registry. This
 * covers the same source files as collectBackEndSourceFiles, so exactly the ones that end up in the linked program.
 *
 * @param exportedSymbols Registry, that maps each linker symbol name to the AST node of its definition
 * @param visitedSourceFiles Source files, that were already registered
 */
void SourceFile::registerExportedSymbols(
    std::unordered_map<std::string, const ASTNode *> &exportedSymbols,
    std::unordered_set<const SourceFile *> &visitedSourceFiles) const { // NOLINT(misc-no-recursion)
  // Guard against circular imports
  if (!visitedSourceFiles.insert(this).second)
    return;

  // Register the symbols of all dependencies first, so that a collision is reported at the definition in the importing file
  for (const SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->registerExportedSymbols(exportedSymbols, visitedSourceFiles);

  const auto registerSymbol = [&](const std::string &linkerName, const std::string &displayName, const ASTNode *declNode) {
    const auto [it, inserted] = exportedSymbols.emplace(linkerName, declNode);
    if (inserted || it->second == declNode)
      return;
    const std::string message = "Exported symbol '" + displayName + "' collides with the exported symbol defined at " +
                                it->second->codeLoc.toPrettyString() + ", because both have the linker name '" + linkerName + "'";
    resourceManager.errorManager.addSoftError(declNode, EXPORTED_SYMBOL_COLLISION, message);
  };

  for (const TopLevelDefNode *topLevelDef : ast->topLevelDefs) {
    if (const auto globalVarDef = dynamic_cast<const GlobalVarDefNode *>(topLevelDef)) {
      // Public globals are exported under their plain name
      if (globalVarDef->entry->getQualType().isPublic())
        registerSymbol(globalVarDef->varName, globalVarDef->varName, globalVarDef);
    } else if (const auto fctDef = dynamic_cast<const FctDefBaseNode *>(topLevelDef)) {
      // Each emitted manifestation of an exported function is exported under its mangled name
      for (const Function *manifestation : fctDef->manifestations)
        if (manifestation->isFullySubstantiated() && IRGenerator::isExportedFunction(manifestation))
          registerSymbol(manifestation->getMangledName(), manifestation->getSignature(), fctDef);
    }
  }
}

bool SourceFile::isLibraryOutput() const {
  return cliOptions.outputContainer == OutputContainer::STATIC_LIBRARY ||
         cliOptions.outputContainer == OutputContainer::SHARED_LIBRARY;
}

void SourceFile::collectAndPrintWarnings() { // NOLINT(misc-no-recursion)
  // Skip if restored from cache (no scope tree available), or if already visited. The latter guard keeps circular
  // imports from recursing infinitely, since the dependency graph may contain cycles.
  if (restoredFromCache || warningsCollected)
    return;
  warningsCollected = true;
  // Print warnings for all dependencies
  for (SourceFile *sourceFile : dependencies | std::views::values)
    if (!sourceFile->isStdFile)
      sourceFile->collectAndPrintWarnings();
  // Collect warnings for this file
  if (!ignoreWarnings)
    globalScope->collectWarnings(compilerOutput.warnings);
  // Print warnings for this file
  for (const CompilerWarning &warning : compilerOutput.warnings)
    warning.print();
}

const SourceFile *SourceFile::getRootSourceFile() const { // NOLINT(misc-no-recursion)
  return isMainFile ? this : parent->getRootSourceFile();
}

bool SourceFile::isRT(RuntimeModule runtimeModule) const {
  assert(IDENTIFYING_TOP_LEVEL_NAMES.contains(runtimeModule));
  const char *topLevelName = IDENTIFYING_TOP_LEVEL_NAMES.at(runtimeModule);
  const auto it = exportedNameRegistry.find(topLevelName);
  if (it == exportedNameRegistry.end())
    return false;
  return exportedNameRegistry.at(topLevelName).targetEntry->scope == globalScope.get();
}

bool SourceFile::haveAllDependantsBeenTypeChecked() const {
  return std::ranges::all_of(dependants, [this](const SourceFile *dependant) {
    // Ignore dependants that are part of the same import cycle (i.e. this file transitively depends on them). They
    // cannot be type-checked before us either, so waiting on them would deadlock the whole cycle. Such a strongly
    // connected component is type-checked as a unit and converges through the reVisitRequested fixpoint loop instead.
    if (dependsOn(dependant))
      return true;
    return dependant->totalTypeCheckerRuns >= 1;
  });
}

/**
 * Check whether this source file transitively depends on (imports) the given other source file.
 * Used to detect strongly connected components (import cycles) in the dependency graph.
 *
 * @param other Potential (transitive) dependency
 * @return true if this file reaches the other file by following dependency edges
 */
bool SourceFile::dependsOn(const SourceFile *other) const {
  std::unordered_set<const SourceFile *> visited;
  std::queue<const SourceFile *> worklist;
  worklist.push(this);
  visited.insert(this);
  while (!worklist.empty()) {
    const SourceFile *current = worklist.front();
    worklist.pop();
    for (const SourceFile *dependency : current->dependencies | std::views::values) {
      if (dependency == other)
        return true;
      if (visited.insert(dependency).second)
        worklist.push(dependency);
    }
  }
  return false;
}

/**
 * Acquire all publicly visible symbols from the imported source file and put them in the name registry of the current one.
 * But only do that for the symbols that are actually defined in the imported source file. Do not allow transitive dependencies.
 * Here, we also register privately visible symbols to know that the symbol exist. The error handling regarding the visibility
 * is issued later in the pipeline.
 *
 * @param importedSourceFile Imported source file
 * @param importName First fragment of all fully qualified symbol names from that import
 */
void SourceFile::mergeNameRegistries(const SourceFile &importedSourceFile, const std::string &importName) {
  // Retrieve import entry
  SymbolTableEntry *importEntry = globalScope->lookupStrict(importName);
  assert(importEntry != nullptr || importName.starts_with("__")); // Runtime imports start with two underscores

  for (const auto &[originalName, entry] : importedSourceFile.exportedNameRegistry) {
    // Skip if we introduce a transitive dependency
    if (entry.targetScope->sourceFile->globalScope != importedSourceFile.globalScope)
      continue;
    // Add the fully qualified name
    std::string newName = importName;
    newName += SCOPE_ACCESS_TOKEN;
    newName += originalName;
    const NameRegistryEntry newEntry{newName, entry.typeId, entry.targetEntry, entry.targetScope, importEntry};
    if (exportedNameRegistry.emplace(newName, newEntry).second)
      trackNameRegistryTargetEntry(entry.targetEntry);
    // Add the shortened name, considering the name collision. A symbol defined in the importing file itself always
    // shadows imported symbols of the same name. Since this merge runs after the file built its own registry (so that
    // circular imports work), we must explicitly avoid letting an import overwrite or erase such an own symbol - the old
    // ordering achieved this implicitly by registering own symbols last with keep-on-collision.
    const auto existing = exportedNameRegistry.find(originalName);
    const bool existingIsOwn =
        existing != exportedNameRegistry.end() && existing->second.targetScope->sourceFile->globalScope == globalScope;
    if (!existingIsOwn) {
      const bool keepOnCollision = importedSourceFile.alwaysKeepSymbolsOnNameCollision;
      addNameRegistryEntry(originalName, entry.typeId, entry.targetEntry, entry.targetScope, keepOnCollision, importEntry);
    }
  }
}

/**
 * Count a new name registry entry pointing to the given target entry
 *
 * @param targetEntry Target entry of the name registry entry
 */
void SourceFile::trackNameRegistryTargetEntry(const SymbolTableEntry *targetEntry) {
  if (targetEntry != nullptr)
    nameRegistryTargetEntryCounts[targetEntry]++;
}

/**
 * Uncount a removed name registry entry pointing to the given target entry
 *
 * @param targetEntry Target entry of the name registry entry
 */
void SourceFile::untrackNameRegistryTargetEntry(const SymbolTableEntry *targetEntry) {
  if (targetEntry == nullptr)
    return;
  const auto it = nameRegistryTargetEntryCounts.find(targetEntry);
  assert(it != nameRegistryTargetEntryCounts.end());
  if (--it->second == 0)
    nameRegistryTargetEntryCounts.erase(it);
}

/**
 * Recursively merge the exported name registries of all (transitive) dependencies into the respective importing source
 * files. Each file merges its direct dependencies' registries exactly once (guarded by registriesMerged), so this is
 * safe to call on overlapping subgraphs and on graphs that contain cycles (circular imports).
 *
 * Must only be called once every reachable file has built its own exported name registry (i.e. after the front-end).
 */
void SourceFile::mergeNameRegistriesRecursive() { // NOLINT(misc-no-recursion)
  if (registriesMerged)
    return;
  registriesMerged = true;

  // Merge the direct dependencies' registries into this file. Their own registries are fully built by now, so the
  // order in which the reachable files are visited does not matter (even across import cycles).
  for (const auto &[importName, dependency] : dependencies)
    mergeNameRegistries(*dependency, importName);

  // Recurse into the dependencies to cover the rest of the reachable graph
  for (SourceFile *dependency : dependencies | std::views::values)
    dependency->mergeNameRegistriesRecursive();
}

void SourceFile::dumpCacheStats() {
  std::stringstream cacheStats;
  cacheStats << FunctionManager::dumpLookupCacheStatistics() << std::endl;
  cacheStats << StructManager::dumpLookupCacheStatistics() << std::endl;
  cacheStats << InterfaceManager::dumpLookupCacheStatistics() << std::endl;
  compilerOutput.cacheStats = cacheStats.str();
}

void SourceFile::dumpCompilationStats() const {
  const size_t sourceFileCount = resourceManager.sourceFiles.size();
  const size_t totalLineCount = resourceManager.getTotalLineCount();
  const size_t totalTypeCount = TypeRegistry::getTypeCount();
  const size_t allocatedBytes = resourceManager.astNodeAlloc.getTotalAllocatedSize();
  const size_t allocationCount = resourceManager.astNodeAlloc.getAllocationCount();
  const size_t frontEndDuration = resourceManager.frontEndTimer.getDurationMilliseconds();
  const size_t middleEndDuration = resourceManager.middleEndTimer.getDurationMilliseconds();
  const size_t backEndDuration = resourceManager.backEndTimer.getDurationMilliseconds();
  const size_t totalDuration = frontEndDuration + middleEndDuration + backEndDuration;
  std::cout << "\nSuccessfully compiled " << std::to_string(sourceFileCount) << " source file(s)";
  std::cout << " or " << std::to_string(totalLineCount) << " lines in total.\n";
  std::cout << "Total number of blocks allocated via BlockAllocator: " << CommonUtil::formatBytes(allocatedBytes);
  std::cout << " in " << std::to_string(allocationCount) << " allocations.\n";
#ifndef NDEBUG
  resourceManager.astNodeAlloc.printAllocatedClassStatistic();
#endif
  std::cout << "Total number of types: " << std::to_string(totalTypeCount) << "\n";
  std::cout << "Total compile time: " << std::to_string(totalDuration) << " ms (frontend: " << std::to_string(frontEndDuration);
  std::cout << " ms, middle end: " << std::to_string(middleEndDuration) << " ms, backend: " << std::to_string(backEndDuration);
  std::cout << " ms)\n";
}

void SourceFile::dumpOutput(const std::string &content, const std::string &caption, const std::string &fileSuffix) const {
  if (cliOptions.dump.dumpToFiles) {
    // Dump to file
    const std::string dumpFileName = filePath.stem().string() + "-" + fileSuffix;
    std::filesystem::path dumpFilePath = cliOptions.outputDir / dumpFileName;
    dumpFilePath.make_preferred();
    FileUtil::writeToFile(dumpFilePath, content);
  } else {
    // Dump to console
    std::cout << "\n" << caption << ":\n" << content;
  }

  // If the abort after dump is requested, set the abort compilation flag
  if (cliOptions.dump.abortAfterDump) {
    // If this is an IR dump whilst having optimization enabled, we may not abort when dumping unoptimized IR,
    // because we also have to dump the optimized IR
    if (cliOptions.dump.dumpIR && fileSuffix == "ir-code.ll") {
      resourceManager.abortCompilation = cliOptions.optLevel == OptLevel::O0;
    } else {
      resourceManager.abortCompilation = true;
    }
  }
}

void SourceFile::visualizerPreamble(std::stringstream &output) const {
  if (isMainFile)
    output << "digraph {\n rankdir=\"TB\";\n";
  else
    output << "subgraph {\n";
  output << " label=\"" << filePath.generic_string() << "\";\n";
}

void SourceFile::visualizerOutput(std::string outputName, const std::string &output) const {
  if (cliOptions.dump.dumpToFiles) {
    // Check if graphviz is installed
    // GCOV_EXCL_START
    if (!SystemUtil::isGraphvizInstalled())
      throw CompilerError(IO_ERROR, "Please check if you have installed Graphviz and added it to the PATH variable");
    // GCOV_EXCL_STOP

    // Write to a dot file
    std::ranges::transform(outputName, outputName.begin(), ::tolower);
    dumpOutput(output, outputName, outputName + ".dot");

    // Generate SVG. This only works if the dot code was dumped into a file
    std::cout << "\nGenerating SVG file ... ";
    const std::string dotFileName = filePath.stem().string() + "-" + outputName + ".dot";
    std::filesystem::path dotFilePath = cliOptions.outputDir / dotFileName;
    std::filesystem::path svgFilePath = dotFilePath;
    svgFilePath.replace_extension("svg");
    dotFilePath.make_preferred();
    svgFilePath.make_preferred();
    SystemUtil::exec("dot", {"-T", "svg", "-o", svgFilePath.string(), dotFilePath.string()});
    std::cout << "done.\nSVG file can be found at: " << svgFilePath << "\n";
  } else {
    // Dump to console
    std::cout << "\nSerialized " << outputName << ":\n\n" << output << "\n";
  }

  // If the abort after dump is requested, set the abort compilation flag
  if (cliOptions.dump.abortAfterDump)
    resourceManager.abortCompilation = true;
}

void SourceFile::printStatusMessage(const char *stage, const CompileStageIOType &in, const CompileStageIOType &out,
                                    uint64_t stageRuntime, unsigned short stageRuns) const {
  if (cliOptions.printDebugOutput) {
    static constexpr const char *const compilerStageIoTypeName[6] = {"Code", "Tokens", "CST", "AST", "IR", "Obj"};
    // Build output string
    std::stringstream outputStr;
    outputStr << "[" << stage << "] for " << fileName << ": ";
    outputStr << compilerStageIoTypeName[in] << " --> " << compilerStageIoTypeName[out];
    outputStr << " (" << std::to_string(stageRuntime) << " ms";
    if (stageRuns > 0)
      outputStr << "; " << std::to_string(stageRuns) << " run(s)";
    outputStr << ")\n";
    // Print
    std::cout << outputStr.str();
  }
}

} // namespace spice::compiler
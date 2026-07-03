// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "SourceFile.h"

#include <queue>
#include <unordered_set>

#include <ast/ASTBuilder.h>
#include <driver/Driver.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <exception/CompilerError.h>
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
#include <util/FileUtil.h>
#include <util/SystemUtil.h>
#include <util/Timer.h>
#include <visualizer/ASTVisualizer.h>
#include <visualizer/CSTVisualizer.h>
#include <visualizer/DependencyGraphVisualizer.h>

#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>

namespace spice::compiler {

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
    throw CompilerError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error); // GCOV_EXCL_LINE

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
  if (isMainFile)
    resourceManager.totalTimer.start();

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
  // The final key (which folds in transitive dependency cache keys) is computed at the end
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

void SourceFile::runCSTVisualizer() {
  // Only execute if enabled
  if (restoredFromCache || (!cliOptions.dump.dumpCST && !cliOptions.testMode))
    return;
  // Check if this stage has already been done
  if (previousStage >= CST_VISUALIZER)
    return;

  Timer timer(&compilerOutput.times.cstVisualizer);
  timer.start();

  // Generate dot code for this source file
  std::stringstream dotCode;
  visualizerPreamble(dotCode);
  CSTVisualizer cstVisualizer(resourceManager, this, antlrCtx.lexer.get(), antlrCtx.parser.get());
  dotCode << " " << std::any_cast<std::string>(cstVisualizer.visit(antlrCtx.parser->entry())) << "}";
  antlrCtx.parser->reset();

  // Dump the serialized CST string and the SVG file
  if (cliOptions.dump.dumpCST || cliOptions.testMode)
    compilerOutput.cstString = dotCode.str();

  if (cliOptions.dump.dumpCST)
    visualizerOutput("CST", compilerOutput.cstString);

  previousStage = CST_VISUALIZER;
  timer.stop();
  printStatusMessage("CST Visualizer", IO_CST, IO_CST, compilerOutput.times.cstVisualizer);
}

void SourceFile::runASTBuilder() {
  // Skip if restored from the cache or this stage has already been done
  if (restoredFromCache || previousStage >= AST_BUILDER)
    return;

  Timer timer(&compilerOutput.times.astBuilder);
  timer.start();

  // Build AST for this source file
  ASTBuilder astBuilder(resourceManager, this, antlrCtx.inputStream.get());
  ast = std::any_cast<EntryNode *>(astBuilder.visit(antlrCtx.parser->entry()));
  antlrCtx.parser->reset();

  // Create global scope
  globalScope = std::make_unique<Scope>(nullptr, this, ScopeType::GLOBAL, &ast->codeLoc);

  previousStage = AST_BUILDER;
  timer.stop();
  printStatusMessage("AST Builder", IO_CST, IO_AST, compilerOutput.times.astBuilder);
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

  // Try to load from the cache. Deferred from runLexer so that dep cache keys can participate.
  if (!cliOptions.ignoreCache)
    restoredFromCache = resourceManager.cacheManager.lookupSourceFile(this);

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

  // Deduce an object file path
  std::filesystem::path objectFilePath = cliOptions.outputDir / filePath.filename();
  objectFilePath.replace_extension("o");

  // Pick a concrete emitter based on the selected backend. The TPDE emitter is compiled into a
  // sibling library (spice_tpde) that keeps its -fno-rtti requirement out of spicecore; the
  // AbstractObjectEmitter base gives us a single interface both branches produce.
  std::unique_ptr<AbstractObjectEmitter> objectEmitter;
#ifdef SPICE_ENABLE_TPDE
  if (cliOptions.backend == Backend::TPDE) {
    llvm::Module &module = cliOptions.useLTO ? *resourceManager.ltoModule : *llvmModule;
    objectEmitter = std::make_unique<TPDEObjectEmitter>(module);
  } else
#endif
  {
    objectEmitter = std::make_unique<LLVMObjectEmitter>(resourceManager, this);
  }

  // Emit object for this source file
  objectEmitter->emit(objectFilePath);

  // Save assembly string in the compiler output (TPDE emits a placeholder note)
  if (cliOptions.isNativeTarget && (cliOptions.dump.dumpAssembly || cliOptions.testMode))
    objectEmitter->getASMString(compilerOutput.asmString);

  // Dump assembly code
  if (cliOptions.dump.dumpAssembly)
    dumpOutput(compilerOutput.asmString, "Assembly code", "assembly-code.s");

  // Add the object file to the linker objects
  resourceManager.linker.addFileToLinkage(objectFilePath);

  previousStage = OBJECT_EMITTER;
  timer.stop();
  printStatusMessage("Object Emitter", IO_IR, IO_OBJECT_FILE, compilerOutput.times.objectEmitter);
}

void SourceFile::concludeCompilation() {
  // Handle cache-restored files: register all cached objects with linker
  if (restoredFromCache) {
    for (const auto &objectFilePath : cachedObjectFilePaths)
      resourceManager.linker.addFileToLinkage(objectFilePath);
    for (const auto &flag : sourceLinkerFlags)
      resourceManager.linker.addLinkerFlag(flag);
    for (const auto &path : sourceAdditionalSourcePaths)
      resourceManager.linker.addAdditionalSourcePath(path);
    return;
  }

  if (previousStage >= FINISHED)
    return;

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
  runLexer();
  CHECK_ABORT_FLAG_V()
  runParser();
  CHECK_ABORT_FLAG_V()
  runCSTVisualizer();
  CHECK_ABORT_FLAG_V()
  runASTBuilder();
  CHECK_ABORT_FLAG_V()
  runASTVisualizer();
  CHECK_ABORT_FLAG_V()
  runImportCollector();
  CHECK_ABORT_FLAG_V()
  runSymbolTableBuilder();
  CHECK_ABORT_FLAG_V()
}

void SourceFile::runMiddleEnd() {
  // Merge the exported name registries of all (transitive) dependencies into the respective importing files. This is
  // the deferred tail of the front-end: it must run after every reachable file has built its own registry, which is
  // why it cannot live inside the per-file front-end recursion (a circular import would otherwise merge a dependency
  // whose registry is not populated yet). runMiddleEnd is the first stage that is only ever invoked at the top level.
  mergeNameRegistriesRecursive();
  CHECK_ABORT_FLAG_V()
  // We need two runs here due to generics.
  // The first run to determine all concrete function/struct/interface substantiations
  runTypeCheckerPre(); // Visit the dependency tree from bottom to top
  CHECK_ABORT_FLAG_V()
  // The second run to ensure, also generic scopes are type-checked properly
  runTypeCheckerPost(); // Visit the dependency tree from top to bottom in topological order
  CHECK_ABORT_FLAG_V()
  // Visualize dependency graph
  runDependencyGraphVisualizer();
  CHECK_ABORT_FLAG_V()
}

void SourceFile::runBackEnd() { // NOLINT(misc-no-recursion)
  // Guard against re-entering a file that is already running its back end. Circular imports form a cycle in the
  // dependency graph, so the deps-first recursion below would otherwise loop forever.
  if (backEndStarted)
    return;
  backEndStarted = true;

  // Run backend for all dependencies first
  for (SourceFile *sourceFile : dependencies | std::views::values)
    sourceFile->runBackEnd();

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
  CHECK_ABORT_FLAG_V()
  concludeCompilation();

  if (isMainFile) {
    resourceManager.totalTimer.stop();
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
  if (keepNewOnCollision || !exportedNameRegistry.contains(symbolName)) // Overwrite potential existing entry
    exportedNameRegistry[symbolName] = {symbolName, typeId, entry, scope, importEntry};
  else // Name collision => we must remove the existing entry
    exportedNameRegistry.erase(symbolName);
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
    exportedNameRegistry.emplace(newName,
                                 NameRegistryEntry{newName, entry.typeId, entry.targetEntry, entry.targetScope, importEntry});
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
  const size_t totalDuration = resourceManager.totalTimer.getDurationMilliseconds();
  std::cout << "\nSuccessfully compiled " << std::to_string(sourceFileCount) << " source file(s)";
  std::cout << " or " << std::to_string(totalLineCount) << " lines in total.\n";
  std::cout << "Total number of blocks allocated via BlockAllocator: " << CommonUtil::formatBytes(allocatedBytes);
  std::cout << " in " << std::to_string(allocationCount) << " allocations.\n";
#ifndef NDEBUG
  resourceManager.astNodeAlloc.printAllocatedClassStatistic();
#endif
  std::cout << "Total number of types: " << std::to_string(totalTypeCount) << "\n";
  std::cout << "Total compile time: " << std::to_string(totalDuration) << " ms\n";
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
    SystemUtil::exec("dot -T svg -o" + svgFilePath.string() + " " + dotFilePath.string());
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
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/SemanticError.h>
#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>
#include <util/ScopeIdUtil.h>

#include <utility>

AnalyzerVisitor::AnalyzerVisitor(std::shared_ptr<llvm::LLVMContext> context, std::shared_ptr<llvm::IRBuilder<>> builder,
                                 const ThreadFactory &threadFactory, const SourceFile &sourceFile, CliOptions &options,
                                 bool requiresMainFct, bool isStdFile)
    : context(std::move(context)), builder(std::move(builder)), threadFactory(threadFactory), requiresMainFct(requiresMainFct),
      isStdFile(isStdFile) {
  // Retrieve symbol table
  this->currentScope = this->rootScope = sourceFile.symbolTable.get();

  // Use default target triple if empty
  if (options.targetTriple.empty()) {
    llvm::Triple targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    options.targetArch = targetTriple.getArchName();
    options.targetVendor = targetTriple.getVendorName();
    options.targetOs = targetTriple.getOSName();
  }

  // Create error factory for this specific file
  this->err = std::make_unique<ErrorFactory>(sourceFile.filePath);

  // Create OpRuleManager
  opRuleManager = std::make_unique<OpRuleManager>(err.get(), allowUnsafeOperations);
}

std::any AnalyzerVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
  // --- Pre-traversing actions
  needsReAnalyze = false;

  // --- Traverse AST
  visitChildren(ctx);

  // --- Post traversing actions
  // Remove non-substantiated functions and structs
  // Currently disabled because the generator relies on unsubstantiated structs in the main source file
  // if (requiresMainFct && secondRun)
  //  rootScope->purgeSubstantiationRemnants();

  // Check if the visitor got a main function
  if (requiresMainFct && !hasMainFunction)
    throw err->get(*ctx->start, MISSING_MAIN_FUNCTION, "No main function found");

  // Print compiler warnings once the whole ast is present, but not for std files
  if (requiresMainFct && !isStdFile)
    rootScope->printCompilerWarnings();

  return secondRun = needsReAnalyze;
}

std::any AnalyzerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  if (secondRun)
    return nullptr;

  std::string mainSignature = MAIN_FUNCTION_NAME + "()";

  // Check if the function is already defined
  if (currentScope->lookup(mainSignature))
    throw err->get(*ctx->start, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

  // Insert function name into the root symbol table
  SymbolType symbolType = SymbolType(TY_FUNCTION);
  currentScope->insert(mainSignature, symbolType, SymbolSpecifiers(symbolType), INITIALIZED, *ctx->start);

  // Create the function scope
  currentScope = currentScope->createChildBlock(mainSignature, SCOPE_FUNC_PROC_BODY);

  // Declare variable for the return value in the function scope
  SymbolType returnType = SymbolType(TY_INT);
  currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), INITIALIZED, *ctx->start);
  currentScope->lookup(RETURN_VARIABLE_NAME)->setUsed();

  // Visit arguments in new scope
  if (ctx->argLstDef())
    visit(ctx->argLstDef());

  // Visit statements in new scope
  visit(ctx->stmtLst());

  // Return to root scope
  currentScope = currentScope->getParent();

  // Confirm main function
  hasMainFunction = true;
  currentScope->lookup(mainSignature)->setUsed();

  return nullptr;
}

std::any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  std::string functionName = ctx->IDENTIFIER().back()->toString();

  // Check if this is a global function or a method
  bool isMethod = false;
  if (ctx->IDENTIFIER().size() > 1) { // Method
    isMethod = true;
    // Change to the struct scope
    std::string structName = ctx->IDENTIFIER().front()->toString();
    currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structName);
    if (!currentScope)
      throw err->get(*ctx->IDENTIFIER().front()->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                     "Struct '" + structName + "' could not be found");
  }

  if (!secondRun) { // First run
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId, SCOPE_FUNC_PROC_BODY);

    // Get 'this' type
    bool isGeneric = false;
    std::vector<GenericType> templateTypes;
    SymbolType thisType = SymbolType(TY_DYN);
    SymbolType thisTypePtr = thisType;
    if (isMethod) {
      std::string structName = ctx->IDENTIFIER().front()->toString();
      SymbolTableEntry *structEntry = currentScope->lookup(structName);
      thisType = structEntry->getType();
      thisTypePtr = thisType.toPointer(err.get(), *ctx->start);
      if (!thisType.getTemplateTypes().empty()) {
        isGeneric = true;
        for (const auto &templateType : thisType.getTemplateTypes())
          templateTypes.emplace_back(templateType);
      }
    }

    // Get template types
    if (ctx->typeLst()) {
      isGeneric = true;
      for (const auto &dataType : ctx->typeLst()->dataType()) {
        auto templateType = any_cast<SymbolType>(visit(dataType));
        if (!templateType.is(TY_GENERIC))
          throw err->get(*dataType->start, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
        assert(genericType != nullptr);
        templateTypes.push_back(*genericType);
      }
    }
    if (isGeneric)
      needsReAnalyze = true;

    // Visit arguments in new scope
    std::vector<std::string> argNames;
    ArgList argTypes;
    if (ctx->argLstDef()) {
      auto namedArgList = any_cast<NamedArgList>(visit(ctx->argLstDef()));
      for (const auto &namedArg : namedArgList) {
        std::string argName = std::get<0>(namedArg);
        SymbolType argType = std::get<1>(namedArg);
        bool argOptional = std::get<2>(namedArg);

        // Check if the type is present in the template for generic types
        if (argType.is(TY_GENERIC)) {
          if (std::none_of(templateTypes.begin(), templateTypes.end(), [&](const GenericType &t) { return t == argType; }))
            throw err->get(*ctx->argLstDef()->start, GENERIC_TYPE_NOT_IN_TEMPLATE,
                           "Generic arg type not included in function template");
        }

        argNames.push_back(argName);
        argTypes.push_back({argType, argOptional});
      }
    }

    // Declare 'this' variable in new scope
    if (isMethod) {
      SymbolSpecifiers thisTypeSpecifiers(thisTypePtr);
      thisTypeSpecifiers.setConst(true);
      currentScope->insert(THIS_VARIABLE_NAME, thisTypePtr, thisTypeSpecifiers, INITIALIZED, *ctx->start);
    }

    // Declare variable for the return value in the function scope
    auto returnType = any_cast<SymbolType>(visit(ctx->dataType()));
    if (returnType.is(TY_DYN))
      throw err->get(*ctx->start, UNEXPECTED_DYN_TYPE_SA, "Dyn return types are not allowed");
    if (returnType.isPointer())
      throw err->get(*ctx->start, COMING_SOON_SA,
                     "Spice currently not supports pointer return types due to not supporting heap allocations.");
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, *ctx->start);

    // Return to old scope
    currentScope = currentScope->getParent();

    // Build function specifiers
    auto functionSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    if (ctx->declSpecifiers()) {
      for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
        if (specifier->PUBLIC()) {
          functionSymbolSpecifiers.setPublic(true);
        } else if (specifier->INLINE()) {
          functionSymbolSpecifiers.setPublic(false);
        } else {
          throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Cannot use the " + specifier->getText() + " specifier on a function definition");
        }
      }
    }

    // Insert function into the symbol table
    std::string codeLoc = CommonUtil::tokenToCodeLoc(*ctx->start);
    Function spiceFunc = Function(functionName, functionSymbolSpecifiers, thisType, returnType, argTypes, templateTypes, codeLoc);
    currentScope->insertFunction(spiceFunc, err.get(), *ctx->start);

    // Rename / duplicate the original child block to reflect the substantiated versions of the function
    std::vector<Function> substantiatedFunctions = spiceFunc.substantiateOptionalArgs();
    currentScope->renameChildBlock(scopeId, substantiatedFunctions.front().getSignature());
    for (int i = 1; i < substantiatedFunctions.size(); i++)
      currentScope->copyChildBlock(substantiatedFunctions.front().getSignature(), substantiatedFunctions[i].getSignature());

    if (!isGeneric) { // Only visit body for non-generic functions. Otherwise, bodies will be visited with the second analyzer run
      // Go down again in scope
      currentScope = currentScope->getChild(substantiatedFunctions.front().getSignature());
      assert(currentScope != nullptr);

      // Visit statements in new scope
      visit(ctx->stmtLst());

      // Check if return variable is now initialized
      if (currentScope->lookup(RETURN_VARIABLE_NAME)->getState() == DECLARED)
        throw err->get(*ctx->start, FUNCTION_WITHOUT_RETURN_STMT, "Function without return statement");

      // Leave the function scope
      currentScope = currentScope->getParent();
    }

    // Leave the struct scope
    if (isMethod)
      currentScope = currentScope->getParent();
  } else { // Second run
    // Change to the struct scope
    if (isMethod)
      currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER().front()->toString());

    // Get manifestations of that function
    std::map<std::string, Function> *manifestations = currentScope->getFunctionManifestations(*ctx->start);

    // Leave the struct scope
    if (isMethod)
      currentScope = currentScope->getParent();

    if (manifestations) {
      for (const auto &[mangledName, spiceFunc] : *manifestations) {
        // Check if the function is substantiated
        if (!spiceFunc.isFullySubstantiated())
          continue;

        // Change scope to the struct specialization
        if (isMethod) {
          std::string structSignature =
              Struct::getSignature(ctx->IDENTIFIER()[0]->toString(), spiceFunc.getThisType().getTemplateTypes());
          currentScope = currentScope->getChild(STRUCT_SCOPE_PREFIX + structSignature);
          assert(currentScope);
        }

        // Go down again in scope
        currentScope = currentScope->getChild(spiceFunc.getSignature());
        assert(currentScope != nullptr);

        // Replace this type
        if (spiceFunc.isMethodFunction()) {
          SymbolTableEntry *thisVar = currentScope->lookup(THIS_VARIABLE_NAME);
          assert(thisVar != nullptr);
          thisVar->updateType(spiceFunc.getThisType(), true);
        }

        // Morph the generic return type
        SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
        if (returnVarEntry->getType().is(TY_GENERIC)) {
          SymbolType returnType = spiceFunc.getReturnType();
          if (returnType.isPointer())
            throw err->get(*ctx->start, COMING_SOON_SA,
                           "Spice currently not supports pointer return types due to not supporting heap allocations.");
          returnVarEntry->updateType(returnType, true);
        }

        // Get argument types
        std::vector<std::pair<std::string, SymbolType>> args;
        if (ctx->argLstDef()) {
          for (const auto argDecl : ctx->argLstDef()->declStmt()) {
            std::string argName = argDecl->IDENTIFIER()->toString();
            SymbolTableEntry *argEntry = currentScope->lookup(argName);
            assert(argEntry);
            args.emplace_back(argName, argEntry->getType());
          }
        }

        // Morph the generic types to the replacements
        std::vector<SymbolType> newArgTypes = spiceFunc.getArgTypes();
        for (int i = 0; i < newArgTypes.size(); i++) {
          SymbolTableEntry *argEntry = currentScope->lookup(args[i].first);
          argEntry->updateType(newArgTypes[i], true);
        }

        // Visit statements in new scope
        visit(ctx->stmtLst());

        // Reset generic types
        for (const auto &arg : args) {
          SymbolTableEntry *argEntry = currentScope->lookup(arg.first);
          assert(argEntry);
          argEntry->updateType(arg.second, true);
        }

        // Check if return variable is now initialized
        if (currentScope->lookup(RETURN_VARIABLE_NAME)->getState() == DECLARED)
          throw err->get(*ctx->start, FUNCTION_WITHOUT_RETURN_STMT, "Function without return statement");

        // Leave the function scope
        currentScope = currentScope->getParent();

        // Leave the struct scope
        if (isMethod)
          currentScope = currentScope->getParent();
      }
    }
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  std::string procedureName = ctx->IDENTIFIER().back()->toString();

  // Check if this is a global function or a method
  bool isMethod = ctx->IDENTIFIER().size() > 1;

  if (!secondRun) { // First run
    // Change to the struct scope
    if (isMethod) {
      std::string structName = ctx->IDENTIFIER().front()->toString();
      currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + structName);
      if (!currentScope)
        throw err->get(*ctx->IDENTIFIER().front()->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                       "Struct '" + structName + "' could not be found");
    }

    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId, SCOPE_FUNC_PROC_BODY);

    // Get 'this' type
    bool isGeneric = false;
    std::vector<GenericType> templateTypes;
    SymbolType thisType = SymbolType(TY_DYN);
    SymbolType thisTypePtr = thisType;
    if (isMethod) {
      std::string structName = ctx->IDENTIFIER().front()->toString();
      SymbolTableEntry *structEntry = currentScope->lookup(structName);
      thisType = structEntry->getType();
      thisTypePtr = thisType.toPointer(err.get(), *ctx->start);
      if (!thisType.getTemplateTypes().empty()) {
        isGeneric = true;
        for (const auto &templateType : thisType.getTemplateTypes())
          templateTypes.emplace_back(templateType);
      }
    }

    // Get template types
    if (ctx->typeLst()) {
      isGeneric = true;
      for (const auto &dataType : ctx->typeLst()->dataType()) {
        auto templateType = any_cast<SymbolType>(visit(dataType));
        if (!templateType.is(TY_GENERIC))
          throw err->get(*dataType->start, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
        assert(genericType != nullptr);
        templateTypes.push_back(*genericType);
      }
    }
    if (isGeneric)
      needsReAnalyze = true;

    // Visit arguments in new scope
    std::vector<std::string> argNames;
    ArgList argTypes;
    if (ctx->argLstDef()) {
      auto namedArgList = any_cast<NamedArgList>(visit(ctx->argLstDef()));
      for (const auto &namedArg : namedArgList) {
        std::string argName = std::get<0>(namedArg);
        SymbolType argType = std::get<1>(namedArg);
        bool argOptional = std::get<2>(namedArg);

        // Check if the type is present in the template for generic types
        if (argType.is(TY_GENERIC)) {
          if (std::none_of(templateTypes.begin(), templateTypes.end(), [&](const GenericType &t) { return t == argType; }))
            throw err->get(*ctx->argLstDef()->start, GENERIC_TYPE_NOT_IN_TEMPLATE,
                           "Generic arg type not included in procedure template");
        }

        argNames.push_back(argName);
        argTypes.push_back({argType, argOptional});
      }
    }

    // Declare 'this' variable in new scope
    if (isMethod) {
      auto thisSymbolSpecifiers = SymbolSpecifiers(thisTypePtr);
      thisSymbolSpecifiers.setConst(true);
      currentScope->insert(THIS_VARIABLE_NAME, thisTypePtr, thisSymbolSpecifiers, INITIALIZED, *ctx->start);
    }

    // Return to old scope
    currentScope = currentScope->getParent();

    // Build procedure specifiers
    auto procedureSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_PROCEDURE));
    if (ctx->declSpecifiers()) {
      for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
        if (specifier->PUBLIC()) {
          procedureSymbolSpecifiers.setPublic(true);
        } else if (specifier->INLINE()) {
          procedureSymbolSpecifiers.setPublic(false);
        } else {
          throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Cannot use the " + specifier->getText() + " specifier on a procedure definition");
        }
      }
    }

    // Insert function into the symbol table
    std::string codeLoc = CommonUtil::tokenToCodeLoc(*ctx->start);
    Function spiceProc =
        Function(procedureName, procedureSymbolSpecifiers, thisType, SymbolType(TY_DYN), argTypes, templateTypes, codeLoc);
    currentScope->insertFunction(spiceProc, err.get(), *ctx->start);

    // Rename / duplicate the original child block to reflect the substantiated versions of the function
    std::vector<Function> substantiatedProcedures = spiceProc.substantiateOptionalArgs();
    currentScope->renameChildBlock(scopeId, substantiatedProcedures.front().getSignature());
    for (int i = 0; i < substantiatedProcedures.size(); i++)
      currentScope->copyChildBlock(substantiatedProcedures.front().getSignature(), substantiatedProcedures[i].getSignature());

    if (!isGeneric) { // Only visit body for non-generic procs. Otherwise, bodies will be visited with the second analyzer run
      // Go down again in scope
      currentScope = currentScope->getChild(substantiatedProcedures.front().getSignature());
      assert(currentScope != nullptr);

      // Visit statement list in new scope
      visit(ctx->stmtLst());

      // Leave the function scope
      currentScope = currentScope->getParent();
    }

    // Leave the struct scope
    if (isMethod)
      currentScope = currentScope->getParent();
  } else { // Second run
    // Enter the struct scope
    if (isMethod)
      currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER().front()->toString());

    // Get manifestations of that procedure
    std::map<std::string, Function> *manifestations = currentScope->getFunctionManifestations(*ctx->start);

    // Leave the struct scope
    if (isMethod)
      currentScope = currentScope->getParent();

    if (manifestations) {
      for (const auto &[mangledName, spiceProc] : *manifestations) {
        // Check if the function is substantiated
        if (!spiceProc.isFullySubstantiated())
          continue;

        // Change scope to the struct specialization
        if (isMethod) {
          std::string structSignature =
              Struct::getSignature(ctx->IDENTIFIER()[0]->toString(), spiceProc.getThisType().getTemplateTypes());
          currentScope = currentScope->getChild(STRUCT_SCOPE_PREFIX + structSignature);
          assert(currentScope);
        }

        // Go down again in scope
        currentScope = currentScope->getChild(spiceProc.getSignature());
        assert(currentScope != nullptr);

        // Replace this type
        if (spiceProc.isMethodProcedure()) {
          SymbolTableEntry *thisVar = currentScope->lookup(THIS_VARIABLE_NAME);
          assert(thisVar != nullptr);
          thisVar->updateType(spiceProc.getThisType(), true);
        }

        // Get argument types
        std::vector<std::pair<std::string, SymbolType>> args;
        if (ctx->argLstDef()) {
          for (const auto argDecl : ctx->argLstDef()->declStmt()) {
            std::string argName = argDecl->IDENTIFIER()->toString();
            SymbolTableEntry *argEntry = currentScope->lookup(argName);
            assert(argEntry);
            args.emplace_back(argName, argEntry->getType());
          }
        }

        // Morph the generic types to the replacements
        std::vector<SymbolType> newArgTypes = spiceProc.getArgTypes();
        for (int i = 0; i < newArgTypes.size(); i++) {
          SymbolTableEntry *argEntry = currentScope->lookup(args[i].first);
          argEntry->updateType(newArgTypes[i], true);
        }

        // Visit statements in new scope
        visit(ctx->stmtLst());

        // Reset generic types
        for (const auto &arg : args) {
          SymbolTableEntry *argEntry = currentScope->lookup(arg.first);
          assert(argEntry);
          argEntry->updateType(arg.second, true);
        }

        // Leave the function scope
        currentScope = currentScope->getParent();

        // Leave the struct scope
        if (isMethod)
          currentScope = currentScope->getParent();
      }
    }
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  if (secondRun)
    return nullptr;

  std::string functionName = ctx->IDENTIFIER()->toString();
  std::string codeLoc = CommonUtil::tokenToCodeLoc(*ctx->start);

  ArgList argTypes;
  if (ctx->typeLstEllipsis()) {
    // Check if an argument is dyn
    for (const auto &arg : ctx->typeLstEllipsis()->typeLst()->dataType()) {
      auto argType = any_cast<SymbolType>(visit(arg));
      if (argType.is(TY_DYN))
        throw err->get(*arg->start, UNEXPECTED_DYN_TYPE_SA, "Dyn data type is not allowed as arg type for external functions");
      argTypes.emplace_back(argType, false);
    }
  }

  if (ctx->dataType()) { // Function
    // Check if return type is dyn
    auto returnType = any_cast<SymbolType>(visit(ctx->dataType()));
    if (returnType.is(TY_DYN))
      throw err->get(*ctx->dataType()->start, UNEXPECTED_DYN_TYPE_SA,
                     "Dyn data type is not allowed as return type for external functions");

    // Insert function into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    Function spiceFunc = Function(functionName, symbolSpecifiers, SymbolType(TY_DYN), returnType, argTypes, {}, codeLoc);
    currentScope->insertFunction(spiceFunc, err.get(), *ctx->start);

    // Add return symbol for function
    SymbolTable *functionTable = currentScope->createChildBlock(spiceFunc.getSignature(), SCOPE_FUNC_PROC_BODY);
    functionTable->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, *ctx->start);
    functionTable->lookup(RETURN_VARIABLE_NAME)->setUsed();
  } else { // Procedure
    // Insert procedure into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_PROCEDURE));
    Function spiceProc = Function(functionName, symbolSpecifiers, SymbolType(TY_DYN), SymbolType(TY_DYN), argTypes, {}, codeLoc);
    currentScope->insertFunction(spiceProc, err.get(), *ctx->start);

    // Add empty scope for function body
    currentScope->createChildBlock(spiceProc.getSignature(), SCOPE_FUNC_PROC_BODY);
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  if (secondRun)
    return nullptr;

  // Check if type already exists in this scope
  std::string typeName = ctx->IDENTIFIER()->toString();
  if (currentScope->lookup(typeName))
    throw err->get(*ctx->start, GENERIC_TYPE_DECLARED_TWICE, "Duplicate symbol name '" + typeName + "'");

  // Get type conditions
  std::vector<SymbolType> typeConditions;
  for (const auto &typeAlt : ctx->typeAlts()->dataType()) {
    auto typeCondition = any_cast<SymbolType>(visit(typeAlt));
    typeConditions.push_back(typeCondition);
  }

  // Build symbol specifiers
  GenericType genericType = GenericType(typeName, typeConditions);
  auto structSymbolSpecifiers = SymbolSpecifiers(genericType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->PUBLIC()) {
        structSymbolSpecifiers.setPublic(true);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a struct definition");
      }
    }
  }

  // Add it to the symbol table
  currentScope->insertGenericType(typeName, genericType);

  return nullptr;
}

std::any AnalyzerVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) {
  if (secondRun)
    return nullptr;

  // Get struct name
  std::string structName = ctx->IDENTIFIER()->toString();

  // Check if struct already exists in this scope
  if (currentScope->lookup(structName))
    throw err->get(*ctx->start, STRUCT_DECLARED_TWICE, "Duplicate struct '" + structName + "'");

  // Get template types
  std::vector<GenericType> genericTemplateTypes;
  std::vector<SymbolType> templateTypes;
  if (ctx->typeLst()) {
    needsReAnalyze = true;
    for (const auto &dataType : ctx->typeLst()->dataType()) {
      auto templateType = any_cast<SymbolType>(visit(dataType));
      if (!templateType.is(TY_GENERIC))
        throw err->get(*dataType->start, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      genericTemplateTypes.push_back(*genericType);
      templateTypes.push_back(*genericType);
    }
  }

  // Build symbol specifiers
  SymbolType symbolType = SymbolType(TY_STRUCT, structName, templateTypes);
  auto structSymbolSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->PUBLIC()) {
        structSymbolSpecifiers.setPublic(true);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a struct definition");
      }
    }
  }

  // Visit field list in a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  SymbolTable *structScope = currentScope = currentScope->createChildBlock(scopeId, SCOPE_STRUCT);

  // Insert a field for each field list entry
  std::vector<SymbolType> fieldTypes;
  for (const auto &field : ctx->field()) {
    std::string fieldName = field->IDENTIFIER()->toString();
    auto fieldType = any_cast<SymbolType>(visit(field->dataType()));

    if (fieldType.isBaseType(TY_GENERIC)) { // Check if the type is present in the template for generic types
      if (std::none_of(genericTemplateTypes.begin(), genericTemplateTypes.end(),
                       [&](const GenericType &t) { return t == fieldType.getBaseType(); }))
        throw err->get(*field->dataType()->start, GENERIC_TYPE_NOT_IN_TEMPLATE,
                       "Generic field type not included in struct template");
    }

    // Build symbol specifiers
    auto fieldTypeSpecifiers = SymbolSpecifiers(symbolType);
    if (field->declSpecifiers()) {
      for (const auto &specifier : field->declSpecifiers()->declSpecifier()) {
        if (specifier->CONST()) {
          // Struct fields cannot be const
          throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Struct fields cannot have the const specifier attached");
        } else if (specifier->SIGNED()) {
          fieldTypeSpecifiers.setSigned(true);
        } else if (specifier->UNSIGNED()) {
          fieldTypeSpecifiers.setSigned(false);
        } else if (specifier->PUBLIC()) {
          fieldTypeSpecifiers.setPublic(true);
        } else {
          throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Cannot use the " + specifier->getText() + " specifier on a struct field definition");
        }
      }
    }

    // Add the field to the symbol table
    currentScope->insert(fieldName, fieldType, fieldTypeSpecifiers, DECLARED, *field->start);

    fieldTypes.push_back(fieldType);
  }

  // Return to the old scope
  currentScope = currentScope->getParent();

  // Add the struct to the symbol table
  std::string codeLoc = CommonUtil::tokenToCodeLoc(*ctx->start);
  Struct s(structName, structSymbolSpecifiers, fieldTypes, genericTemplateTypes, codeLoc);
  s.setSymbolTable(structScope);
  currentScope->insertStruct(s, err.get(), *ctx->start);
  currentScope->insert(structName, symbolType, structSymbolSpecifiers, DECLARED, *ctx->start);

  return nullptr;
}

std::any AnalyzerVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  if (secondRun)
    return nullptr;

  std::string variableName = ctx->IDENTIFIER()->toString();

  // Check if symbol already exists in the symbol table
  if (currentScope->lookup(variableName))
    throw err->get(*ctx->start, VARIABLE_DECLARED_TWICE,
                   "The global variable '" + variableName + "' was declared more than once");

  // Check if symbol already exists in any imported module scope
  if (currentScope->lookupGlobal(variableName, true))
    throw err->get(*ctx->start, VARIABLE_DECLARED_TWICE,
                   "A global variable named '" + variableName +
                       "' is already declared in another module. Please use a different name.");

  // Insert variable name to symbol table
  auto symbolType = any_cast<SymbolType>(visit(ctx->dataType()));

  SymbolState state = DECLARED;
  if (ctx->value()) { // Variable is initialized here
    auto valueType = any_cast<SymbolType>(visit(ctx->value()));
    // Infer type
    if (symbolType.is(TY_DYN)) {
      symbolType = valueType;
    } else if (symbolType != valueType) {
      throw err->get(*ctx->value()->start, OPERATOR_WRONG_DATA_TYPE,
                     "Cannot apply the assign operator on different data types. You provided " + symbolType.getName(false) +
                         " and " + valueType.getName(false));
    }
    state = INITIALIZED;
  }

  // Check if the type is missing
  if (symbolType.is(TY_DYN))
    throw err->get(*ctx->dataType()->start, GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type");

  // Check if we would need to insert instructions in the global scope
  if (!symbolType.isPrimitive())
    throw err->get(*ctx->dataType()->start, GLOBAL_OF_INVALID_TYPE, "Spice does not allow global variables of this type");

  // Create symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->CONST()) {
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->SIGNED()) {
        symbolTypeSpecifiers.setSigned(true);
      } else if (specifier->UNSIGNED()) {
        symbolTypeSpecifiers.setSigned(false);

        // Check if there is a negative value attached. If yes, print a compiler warning
        if (ctx->MINUS())
          CompilerWarning(*ctx->MINUS()->getSymbol(), NEGATIVE_VALUE_TO_UNSIGNED_VAR,
                          "Please mind that assigning a negative value to an unsigned variable causes a wrap-around")
              .print();
      } else if (specifier->PUBLIC()) {
        symbolTypeSpecifiers.setPublic(true);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a global variable definition");
      }
    }
  }

  // Insert into symbol table
  currentScope->insert(variableName, symbolType, symbolTypeSpecifiers, state, *ctx->start);

  return nullptr;
}

std::any AnalyzerVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId, SCOPE_THREAD_BODY);
  currentScope->setCapturingRequired(); // Requires capturing because the LLVM IR will end up in a separate function

  // Visit statement list in new scope
  visit(ctx->stmtLst());

  // Return to old scope
  currentScope = currentScope->getParent();

  return SymbolType(TY_BYTE).toPointer(err.get(), *ctx->start);
}

std::any AnalyzerVisitor::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId, SCOPE_UNSAFE_BODY);

  // Enable unsafe operations
  allowUnsafeOperations = true;

  // Visit statement list in new scope
  visit(ctx->stmtLst());

  // Disable unsafe operations again
  allowUnsafeOperations = false;

  // Return to old scope
  currentScope = currentScope->getParent();

  return nullptr;
}

std::any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto head = ctx->forHead();

  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId, SCOPE_FOR_BODY);

  // Visit loop variable declaration in new scope
  visit(head->declStmt());

  // Visit condition in new scope
  auto conditionType = any_cast<SymbolType>(visit(head->assignExpr()[0]));
  if (!conditionType.is(TY_BOOL))
    throw err->get(*head->assignExpr()[0]->start, CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool");

  // Visit incrementer in new scope
  visit(head->assignExpr()[1]);

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(ctx->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();
  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto head = ctx->foreachHead();

  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId, SCOPE_FOREACH_BODY);

  // Check type of the array
  expectedType = SymbolType(TY_DYN);
  auto arrayType = any_cast<SymbolType>(visit(head->assignExpr()));
  if (!arrayType.isArray() && !arrayType.is(TY_STRING))
    throw err->get(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                   "Can only apply foreach loop on an array type. You provided " + arrayType.getName(false));

  // Check index assignment or declaration
  SymbolType indexType;
  if (head->declStmt().size() >= 2) {
    indexType = any_cast<SymbolType>(visit(head->declStmt().front()));

    // Set declared variable to initialized, because we increment it internally in the loop
    if (!head->declStmt().front()->assignExpr()) {
      std::string varName = head->declStmt().front()->IDENTIFIER()->toString();
      SymbolTableEntry *entry = currentScope->lookup(varName);
      assert(entry != nullptr);
      entry->updateState(INITIALIZED, err.get(), *head->declStmt().front()->IDENTIFIER()->getSymbol());
    }

    // Check if index type is int
    if (!indexType.is(TY_INT))
      throw err->get(*head->declStmt().front()->start, ARRAY_INDEX_NO_INTEGER,
                     "Index in foreach loop must be of type int. You provided " + indexType.getName(false));
  } else {
    // Declare the variable with the default index variable name
    SymbolType symbolType = SymbolType(TY_INT);
    auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
    symbolTypeSpecifiers.setConst(true);
    currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, symbolType, symbolTypeSpecifiers, INITIALIZED, *ctx->start);
  }

  // Check type of the item
  auto itemType = any_cast<SymbolType>(visit(head->declStmt().back()));
  std::string itemVarName = head->declStmt().back()->IDENTIFIER()->toString();
  SymbolTableEntry *itemVarSymbol = currentScope->lookup(itemVarName);
  assert(itemVarSymbol != nullptr);
  if (itemType.is(TY_DYN)) {
    itemType = arrayType.getContainedTy();
    itemVarSymbol->updateType(itemType, false);
  } else {
    if (itemType != arrayType.getContainedTy())
      throw err->get(*head->declStmt().back()->start, OPERATOR_WRONG_DATA_TYPE,
                     "Foreach loop item type does not match array type. Expected " + arrayType.getName(false) + ", provided " +
                         itemType.getName(false));
  }
  itemVarSymbol->updateState(INITIALIZED, err.get(), *head->declStmt().back()->IDENTIFIER()->getSymbol());

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(ctx->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();

  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId, SCOPE_WHILE_BODY);

  // Visit condition
  auto conditionType = any_cast<SymbolType>(visit(ctx->assignExpr()));
  if (!conditionType.is(TY_BOOL))
    throw err->get(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool");

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(ctx->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();

  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  // Create a new scope
  std::string scopeId = ScopeIdUtil::getScopeId(ctx);
  currentScope = currentScope->createChildBlock(scopeId, SCOPE_IF_BODY);

  // Visit condition
  auto conditionType = any_cast<SymbolType>(visit(ctx->assignExpr()));
  if (!conditionType.is(TY_BOOL))
    throw err->get(*ctx->assignExpr()->start, CONDITION_MUST_BE_BOOL, "If condition must be of type bool");

  // Visit statement list in new scope
  visit(ctx->stmtLst());

  // Return to old scope
  currentScope = currentScope->getParent();

  // Visit else statement if it exists
  if (ctx->elseStmt())
    visit(ctx->elseStmt());

  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  if (ctx->ifStmt()) { // Visit if statement in the case of an else if branch
    visit(ctx->ifStmt());
  } else { // Make a new scope in case of an else branch
    // Create a new scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->createChildBlock(scopeId, SCOPE_IF_BODY);

    // Visit statement list in new scope
    visit(ctx->stmtLst());

    // Return to old scope
    currentScope = currentScope->getParent();
  }
  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertConditionType = std::any_cast<SymbolType>(visit(ctx->assignExpr()));

  // Check if assertStmt evaluates to bool
  if (!assertConditionType.is(TY_BOOL))
    throw err->get(*ctx->assignExpr()->start, ASSERTION_CONDITION_BOOL, "The asserted condition must be of type bool");

  return assertConditionType;
}

std::any AnalyzerVisitor::visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) {
  NamedArgList namedArgList;
  bool metOptional = false;
  for (const auto &arg : ctx->declStmt()) {
    std::string argName = arg->IDENTIFIER()->toString();
    auto argType = any_cast<SymbolType>(visit(arg));

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (argType.is(TY_DYN))
      throw err->get(*arg->start, FCT_ARG_IS_TYPE_DYN, "Type of argument '" + arg->IDENTIFIER()->toString() + "' is invalid");

    // Ensure that no optional argument comes after a mandatory argument
    if (arg->ASSIGN()) {
      metOptional = true;
    } else if (metOptional) {
      throw err->get(*arg->start, INVALID_ARGUMENT_ORDER, "Mandatory arguments must go before any optional arguments");
    }

    namedArgList.emplace_back(argName, argType, metOptional);
  }
  return namedArgList;
}

std::any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  std::string variableName = ctx->IDENTIFIER()->toString();
  // Check if symbol already exists in the symbol table
  if (currentScope->lookupStrict(variableName))
    throw err->get(*ctx->start, VARIABLE_DECLARED_TWICE, "The variable '" + variableName + "' was declared more than once");

  // Get the type of the symbol
  SymbolType symbolType = expectedType = any_cast<SymbolType>(visit(ctx->dataType()));

  // Visit the right side
  SymbolState initialState = DECLARED;
  if (ctx->assignExpr()) {
    auto rhsTy = any_cast<SymbolType>(visit(ctx->assignExpr()));
    // Check if type has to be inferred or both types are fixed
    symbolType = opRuleManager->getAssignResultType(*ctx->start, symbolType, rhsTy);
    initialState = INITIALIZED;

    // If the rhs is of type array and was the array initialization, there must be a size attached
    if (symbolType.isArray() && symbolType.getArraySize() == 0 && currentVarName.empty())
      throw err->get(*ctx->dataType()->start, ARRAY_SIZE_INVALID, "The declaration of an array type must have a size attached");
  }

  // Build symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (ctx->declSpecifiers()) {
    for (const auto &specifier : ctx->declSpecifiers()->declSpecifier()) {
      if (specifier->CONST()) {
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->SIGNED()) {
        symbolTypeSpecifiers.setSigned(true);
      } else if (specifier->UNSIGNED()) {
        symbolTypeSpecifiers.setSigned(false);
      } else {
        throw err->get(*specifier->start, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use the " + specifier->getText() + " specifier on a local variable declaration");
      }
    }
  }

  // Insert variable into symbol table
  currentScope->insert(variableName, symbolType, symbolTypeSpecifiers, initialState, *ctx->start);

  return symbolType;
}

std::any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  // Noop
  return nullptr;
}

std::any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  SymbolTableEntry *returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
  if (returnVariable) { // Return variable => function
    expectedType = returnVariable->getType();

    // Check if there is a value attached to the return statement
    if (ctx->assignExpr()) {
      // Visit the value
      auto returnType = any_cast<SymbolType>(visit(ctx->assignExpr()));

      // Check data type of return statement
      if (returnVariable->getType().is(TY_DYN)) {
        // Set explicit return type to the return variable
        returnVariable->updateType(returnType, false);
      } else {
        // Check if return type matches with function definition
        if (returnType != returnVariable->getType())
          throw err->get(*ctx->assignExpr()->start, OPERATOR_WRONG_DATA_TYPE,
                         "Passed wrong data type to return statement. Expected " + returnVariable->getType().getName(false) +
                             " but got " + returnType.getName(false));
      }

      // Set the return variable to initialized
      returnVariable->updateState(INITIALIZED, err.get(), *ctx->start);
    }

    // Check if result variable is initialized
    if (returnVariable->getState() != INITIALIZED)
      throw err->get(*ctx->start, RETURN_WITHOUT_VALUE_RESULT,
                     "Return without value, but result variable is not initialized yet");
    returnVariable->setUsed();

    return returnVariable->getType();
  }
  // No return variable => procedure
  if (ctx->assignExpr())
    throw err->get(*ctx->assignExpr()->start, RETURN_WITH_VALUE_IN_PROCEDURE,
                   "Return statements in procedures may not have a value attached");
  return SymbolType(TY_DYN);
}

std::any AnalyzerVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  int breakCount = 1;
  if (ctx->INTEGER()) {
    // Check if the stated number is valid
    breakCount = std::stoi(ctx->INTEGER()->toString());
    if (breakCount < 1)
      throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_BREAK_NUMBER,
                     "Break count must be >= 1, you provided " + ctx->INTEGER()->toString());
  }
  // Check if we can break this often
  if (breakCount > nestedLoopCounter)
    throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_BREAK_NUMBER,
                   "We can only break " + std::to_string(nestedLoopCounter) + " time(s) here");
  return SymbolType(TY_INT);
}

std::any AnalyzerVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  int continueCount = 1;
  if (ctx->INTEGER()) {
    // Check if the stated number is valid
    continueCount = std::stoi(ctx->INTEGER()->toString());
    if (continueCount < 1)
      throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_CONTINUE_NUMBER,
                     "Continue count must be >= 1, you provided " + ctx->INTEGER()->toString());
  }
  // Check if we can continue this often
  if (continueCount > nestedLoopCounter)
    throw err->get(*ctx->INTEGER()->getSymbol(), INVALID_CONTINUE_NUMBER,
                   "We can only continue " + std::to_string(nestedLoopCounter) + " time(s) here");
  return SymbolType(TY_INT);
}

std::any AnalyzerVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
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

std::any AnalyzerVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  std::string templateString = ctx->STRING_LITERAL()->toString();
  templateString = templateString.substr(1, templateString.size() - 2);

  // Check if assignment types match placeholder types
  std::size_t index = templateString.find_first_of('%');
  int placeholderCount = 0;
  while (index != std::string::npos) {
    // Check if there is another assignExpr
    if (ctx->assignExpr().size() <= placeholderCount)
      throw err->get(*ctx->STRING_LITERAL()->getSymbol(), PRINTF_ARG_COUNT_ERROR,
                     "The placeholder string contains more placeholders that arguments were passed");

    auto assignment = ctx->assignExpr()[placeholderCount];
    auto assignmentType = any_cast<SymbolType>(visit(assignment));
    switch (templateString[index + 1]) {
    case 'c': {
      if (!assignmentType.is(TY_CHAR))
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects char, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 'd':
    case 'i':
    case 'l':
    case 'o':
    case 'u':
    case 'x':
    case 'X': {
      if (!assignmentType.isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_BOOL}))
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects int, byte or bool, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 'a':
    case 'A':
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G': {
      if (!assignmentType.is(TY_DOUBLE))
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects double, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 's': {
      if (!assignmentType.is(TY_STRING) && !assignmentType.isPointerOf(TY_CHAR) && !assignmentType.isArrayOf(TY_CHAR))
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects string, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!assignmentType.isPointer() && !assignmentType.isArray())
        throw err->get(*assignment->start, PRINTF_TYPE_ERROR,
                       "Template string expects pointer, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    }
    index = templateString.find_first_of('%', index + 1);
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < ctx->assignExpr().size())
    throw err->get(*ctx->start, PRINTF_ARG_COUNT_ERROR,
                   "The placeholder string contains less placeholders that arguments were passed");

  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  if (ctx->assignExpr()) {
    // Check if the assignExpr is a type when the type keyword is present
    auto symbolType = any_cast<SymbolType>(visit(ctx->assignExpr()));
    if (ctx->TYPE() && !symbolType.isOneOf({TY_STRUCT, TY_GENERIC}))
      throw err->get(*ctx->assignExpr()->start, EXPECTED_TYPE, "This identifier does not correspond to a type");
  }
  return SymbolType(TY_INT);
}

std::any AnalyzerVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto argType = any_cast<SymbolType>(visit(ctx->assignExpr()));

  // Check if arg is of type array
  if (!argType.isArray())
    throw err->get(*ctx->assignExpr()->getStart(), EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays");

  return SymbolType(TY_INT);
}

std::any AnalyzerVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) {
  // Nothing to check here. Tid builtin has no arguments
  return SymbolType(TY_INT);
}

std::any AnalyzerVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) {
  SymbolType bytePtr = SymbolType(TY_BYTE).toPointer(err.get(), *ctx->start);
  for (const auto &assignExpr : ctx->assignExpr()) {
    auto argSymbolType = any_cast<SymbolType>(visit(assignExpr));
    if (argSymbolType == bytePtr && argSymbolType.isArrayOf(bytePtr))
      throw err->get(*assignExpr->start, JOIN_ARG_MUST_BE_TID,
                     "You have to pass a thread id (byte*) or a array of thread ids (byte*[]) to to join builtin");
  }

  // Return the number of threads that were joined
  return SymbolType(TY_INT);
}

std::any AnalyzerVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  // Check if there is an assign operator applied
  if (ctx->assignOp()) { // This is an assignment
    // Get symbol type of right side
    auto rhsTy = any_cast<SymbolType>(visit(ctx->assignExpr()));

    // Visit the left side
    currentVarName = ""; // Reset the current variable name
    auto lhsTy = any_cast<SymbolType>(visit(ctx->prefixUnaryExpr()));
    std::string variableName = currentVarName;

    // Take a look at the operator
    if (ctx->assignOp()->ASSIGN()) {
      rhsTy = opRuleManager->getAssignResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->PLUS_EQUAL()) {
      rhsTy = opRuleManager->getPlusEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->MINUS_EQUAL()) {
      rhsTy = opRuleManager->getMinusEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->MUL_EQUAL()) {
      rhsTy = opRuleManager->getMulEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->DIV_EQUAL()) {
      rhsTy = opRuleManager->getDivEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->REM_EQUAL()) {
      rhsTy = opRuleManager->getRemEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->SHL_EQUAL()) {
      rhsTy = opRuleManager->getSHLEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->SHR_EQUAL()) {
      rhsTy = opRuleManager->getSHREqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->AND_EQUAL()) {
      rhsTy = opRuleManager->getAndEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->OR_EQUAL()) {
      rhsTy = opRuleManager->getOrEqualResultType(*ctx->start, lhsTy, rhsTy);
    } else if (ctx->assignOp()->XOR_EQUAL()) {
      rhsTy = opRuleManager->getXorEqualResultType(*ctx->start, lhsTy, rhsTy);
    }

    if (!variableName.empty()) { // Variable is involved on the left side
      // Check if the symbol exists
      if (!currentEntry)
        throw err->get(*ctx->prefixUnaryExpr()->start, REFERENCED_UNDEFINED_VARIABLE,
                       "The variable '" + variableName + "' was referenced before defined");

      // Perform type inference
      if (lhsTy.is(TY_DYN))
        currentEntry->updateType(rhsTy, false);

      // Update state in symbol table
      if (!currentEntry->getType().isOneOf({TY_FUNCTION, TY_PROCEDURE}))
        currentEntry->updateState(INITIALIZED, err.get(), *ctx->prefixUnaryExpr()->start);

      // In case the lhs variable is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(variableName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);

      // Print compiler warning if the rhs size exceeds the lhs size
      if (lhsTy.isArray() && rhsTy.getArraySize() > lhsTy.getArraySize())
        CompilerWarning(*ctx->assignExpr()->start, ARRAY_TOO_MANY_VALUES,
                        "You provided more values "
                        "than your array can hold. Excess variables are being ignored by the compiler.")
            .print();
    }

    return rhsTy;
  } else if (ctx->ternaryExpr()) {
    return visit(ctx->ternaryExpr());
  } else if (ctx->threadDef()) {
    return visit(ctx->threadDef());
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any AnalyzerVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  // Check if there is a ternary operator applied
  if (ctx->children.size() > 1) {
    auto condition = ctx->logicalOrExpr()[0];
    auto conditionType = any_cast<SymbolType>(visit(condition));
    auto trueType = any_cast<SymbolType>(visit(ctx->logicalOrExpr()[1]));
    auto falseType = any_cast<SymbolType>(visit(ctx->logicalOrExpr()[2]));
    // Check if the condition evaluates to boolean
    if (!conditionType.is(TY_BOOL))
      throw err->get(*condition->start, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool");
    // Check if trueType and falseType are matching
    if (trueType != falseType)
      throw err->get(*ctx->start, OPERATOR_WRONG_DATA_TYPE, "True and false operands in ternary must be of same data type");
    return trueType;
  }
  return visit(ctx->logicalOrExpr()[0]);
}

std::any AnalyzerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  // Check if a logical or operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->logicalAndExpr()[0]));
    for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(ctx->logicalAndExpr()[i]));
      lhsTy = opRuleManager->getLogicalOrResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->logicalAndExpr()[0]);
}

std::any AnalyzerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  // Check if a logical and operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->bitwiseOrExpr()[0]));
    for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(ctx->bitwiseOrExpr()[i]));
      lhsTy = opRuleManager->getLogicalAndResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->bitwiseOrExpr()[0]);
}

std::any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  // Check if a bitwise or operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->bitwiseXorExpr()[0]));
    for (int i = 1; i < ctx->bitwiseXorExpr().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(ctx->bitwiseXorExpr()[i]));
      lhsTy = opRuleManager->getBitwiseOrResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->bitwiseXorExpr()[0]);
}

std::any AnalyzerVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  // Check if a bitwise xor operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->bitwiseAndExpr()[0]));
    for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(ctx->bitwiseAndExpr()[i]));
      lhsTy = opRuleManager->getBitwiseXorResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->bitwiseAndExpr()[0]);
}

std::any AnalyzerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  // Check if a bitwise and operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->equalityExpr()[0]));
    for (int i = 1; i < ctx->equalityExpr().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(ctx->equalityExpr()[i]));
      lhsTy = opRuleManager->getBitwiseAndResultType(*ctx->start, lhsTy, rhsTy);
    }
    return lhsTy;
  }
  return visit(ctx->equalityExpr()[0]);
}

std::any AnalyzerVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  // Check if at least one equality operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->relationalExpr()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(ctx->relationalExpr()[1]));

    if (ctx->EQUAL()) // Operator was equal
      return opRuleManager->getEqualResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->NOT_EQUAL()) // Operator was not equal
      return opRuleManager->getNotEqualResultType(*ctx->start, lhsTy, rhsTy);
  }
  return visit(ctx->relationalExpr()[0]);
}

std::any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  // Check if a relational operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->shiftExpr()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(ctx->shiftExpr()[1]));

    if (ctx->LESS()) // Operator was less
      return opRuleManager->getLessResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->GREATER()) // Operator was greater
      return opRuleManager->getGreaterResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->LESS_EQUAL()) // Operator was less equal
      return opRuleManager->getLessEqualResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->GREATER_EQUAL()) // Operator was greater equal
      return opRuleManager->getGreaterEqualResultType(*ctx->start, lhsTy, rhsTy);
  }
  return visit(ctx->shiftExpr()[0]);
}

std::any AnalyzerVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  // Check if at least one shift operator is applied
  if (ctx->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(ctx->additiveExpr()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(ctx->additiveExpr()[1]));

    if (ctx->SHL()) // Operator was shl
      return opRuleManager->getShiftLeftResultType(*ctx->start, lhsTy, rhsTy);
    else if (ctx->SHR()) // Operator was shr
      return opRuleManager->getShiftRightResultType(*ctx->start, lhsTy, rhsTy);
  }
  return visit(ctx->additiveExpr()[0]);
}

std::any AnalyzerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  // Check if at least one additive operator is applied
  if (ctx->multiplicativeExpr().size() > 1) {
    auto currentType = any_cast<SymbolType>(visit(ctx->multiplicativeExpr()[0]));
    // Check if data types are compatible
    unsigned int operatorIndex = 1;
    for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
      auto op = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[operatorIndex]);
      const size_t tokenType = op->getSymbol()->getType();
      auto next = ctx->multiplicativeExpr()[i];
      auto nextType = any_cast<SymbolType>(visit(next));

      if (tokenType == SpiceParser::PLUS) { // Operator was plus
        currentType = opRuleManager->getPlusResultType(*next->start, currentType, nextType);
      } else if (tokenType == SpiceParser::MINUS) { // Operator was minus
        currentType = opRuleManager->getMinusResultType(*next->start, currentType, nextType);
      }

      operatorIndex += 2;
    }
    return currentType;
  }
  return visit(ctx->multiplicativeExpr()[0]);
}

std::any AnalyzerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  // Check if at least one multiplicative operator is applied
  if (ctx->castExpr().size() > 1) {
    auto currentType = any_cast<SymbolType>(visit(ctx->castExpr()[0]));
    // Check if data types are compatible
    unsigned int operatorIndex = 1;
    for (int i = 1; i < ctx->castExpr().size(); i++) {
      auto op = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[operatorIndex]);
      const size_t tokenType = op->getSymbol()->getType();
      auto next = ctx->castExpr()[i];
      auto nextType = any_cast<SymbolType>(visit(next));

      if (tokenType == SpiceParser::MUL) { // Operator is mul
        currentType = opRuleManager->getMulResultType(*next->start, currentType, nextType);
      } else if (tokenType == SpiceParser::DIV) { // Operator is div
        currentType = opRuleManager->getDivResultType(*next->start, currentType, nextType);
      } else if (tokenType == SpiceParser::REM) { // Operator is rem
        currentType = opRuleManager->getRemResultType(*next->start, currentType, nextType);
      }

      operatorIndex += 2;
    }
    return currentType;
  }
  return visit(ctx->castExpr()[0]);
}

std::any AnalyzerVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  std::any rhs = visit(ctx->prefixUnaryExpr());

  if (ctx->LPAREN()) { // Cast is applied
    auto dstType = any_cast<SymbolType>(visit(ctx->dataType()));
    return opRuleManager->getCastResultType(*ctx->start, dstType, any_cast<SymbolType>(rhs));
  }

  return rhs;
}

std::any AnalyzerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  currentVarName = "";                  // Reset the current variable name
  scopePath.clear();                    // Clear the scope path
  currentThisType = SymbolType(TY_DYN); // Reset this type

  auto lhs = any_cast<SymbolType>(visit(ctx->postfixUnaryExpr()));

  unsigned int tokenCounter = 0;
  while (tokenCounter < ctx->children.size() - 1) {
    auto token = dynamic_cast<SpiceParser::PrefixUnaryOpContext *>(ctx->children[tokenCounter]);
    if (token->MINUS()) { // Consider - operator
      lhs = opRuleManager->getPrefixMinusResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->PLUS_PLUS()) { // Consider ++ operator
      lhs = opRuleManager->getPrefixPlusPlusResultType(*ctx->postfixUnaryExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), *token->start);

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    } else if (token->MINUS_MINUS()) { // Consider -- operator
      lhs = opRuleManager->getPrefixMinusMinusResultType(*ctx->postfixUnaryExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), *token->start);

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    } else if (token->NOT()) { // Consider ! operator
      lhs = opRuleManager->getPrefixNotResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->BITWISE_NOT()) { // Consider ~ operator
      lhs = opRuleManager->getPrefixBitwiseNotResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->MUL()) { // Consider * operator
      lhs = opRuleManager->getPrefixMulResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->BITWISE_AND()) { // Consider & operator
      lhs = opRuleManager->getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
    } else if (token->LOGICAL_AND()) { // Consider doubled & operator
      lhs = opRuleManager->getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
      lhs = opRuleManager->getPrefixBitwiseAndResultType(*ctx->postfixUnaryExpr()->start, lhs);
    }
    tokenCounter++;
  }

  return lhs;
}

std::any AnalyzerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto lhs = any_cast<SymbolType>(visit(ctx->atomicExpr()));

  unsigned int tokenCounter = 1;
  while (tokenCounter < ctx->children.size()) {
    auto token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
    const size_t tokenType = token->getSymbol()->getType();
    if (tokenType == SpiceParser::LBRACKET) { // Subscript operator
      tokenCounter++;                         // Consume LBRACKET

      std::string arrayName = currentVarName; // Save array name
      ScopePath scopePathBackup = scopePath;  // Save scope path

      auto rule = dynamic_cast<antlr4::RuleContext *>(ctx->children[tokenCounter]);
      auto indexType = any_cast<SymbolType>(visit(rule));
      tokenCounter++; // Consume assignExpr

      if (!indexType.is(TY_INT))
        throw err->get(*ctx->start, ARRAY_INDEX_NO_INTEGER, "Array index must be of type int");
      if (!lhs.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
        throw err->get(*ctx->start, OPERATOR_WRONG_DATA_TYPE,
                       "Can only apply subscript operator on array type, got " + lhs.getName(true));

      if (lhs.is(TY_PTR) && !allowUnsafeOperations)
        throw err->get(
            *ctx->start, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
            "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.");

      // Get array item type
      lhs = lhs.getContainedTy();

      currentVarName = arrayName;  // Restore array name
      scopePath = scopePathBackup; // Restore scope path

      // Retrieve scope for the new scope path fragment
      if (lhs.isBaseType(TY_STRUCT)) { // Struct
        SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
        assert(accessScope != nullptr);

        std::string structSignature = Struct::getSignature(lhs.getBaseType().getSubType(), lhs.getBaseType().getTemplateTypes());
        SymbolTable *newAccessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
        assert(newAccessScope != nullptr);
        // Push the retrieved scope to the scope path
        scopePath.pushFragment("[idx]", newAccessScope);
      }
    } else if (tokenType == SpiceParser::DOT) { // Consider member access
      tokenCounter++;                           // Consume dot
      // Visit rhs
      auto postfixUnary = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(ctx->children[tokenCounter]);
      lhs = any_cast<SymbolType>(visit(postfixUnary));
    } else if (tokenType == SpiceParser::PLUS_PLUS) { // Consider ++ operator
      lhs = opRuleManager->getPostfixPlusPlusResultType(*ctx->atomicExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), *token->getSymbol());

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    } else if (tokenType == SpiceParser::MINUS_MINUS) { // Consider -- operator
      lhs = opRuleManager->getPostfixMinusMinusResultType(*ctx->atomicExpr()->start, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), *token->getSymbol());

      // In case the lhs is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(currentVarName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
    }
    tokenCounter++; // Consume token
  }

  if (lhs.is(TY_INVALID))
    throw err->get(*ctx->start, REFERENCED_UNDEFINED_VARIABLE,
                   "Variable '" + currentVarName + "' was referenced before declared");

  return lhs;
}

std::any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  if (ctx->value())
    return visit(ctx->value());

  if (ctx->IDENTIFIER()) {
    currentVarName = ctx->IDENTIFIER()->toString();

    // Check if this is a reserved keyword
    if (std::find(RESERVED_KEYWORDS.begin(), RESERVED_KEYWORDS.end(), currentVarName) != RESERVED_KEYWORDS.end())
      throw err->get(*ctx->start, RESERVED_KEYWORD,
                     "'" + currentVarName +
                         "' is a reserved keyword for future development of the language. Please use another identifier instead");

    // Load symbol table entry
    SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
    assert(accessScope);

    // Load symbol table entry
    SymbolTableEntry *entry = currentEntry = accessScope->lookup(currentVarName);

    // Check if symbol exists. If it does not exist, just return because it could be the function name of a function call
    // The existence of the variable is checked in the visitPostfixUnaryExpr method.
    if (!entry)
      return SymbolType(TY_INVALID);

    if (accessScope->isImported(currentScope)) {
      // Check if the entry is public if it is imported
      if (!entry->getSpecifiers().isPublic())
        throw err->get(*ctx->IDENTIFIER()->getSymbol(), INSUFFICIENT_VISIBILITY,
                       "Cannot access '" + currentVarName + "' due to its private visibility");

      // Check if the entry is an external global variable and needs to be imported
      if (entry->isGlobal() && !entry->getType().isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_IMPORT}))
        initExtGlobal(*ctx->IDENTIFIER()->getSymbol(), accessScope, scopePath.getScopePrefix(true), entry->getName());
    }

    // Set symbol to used
    entry->setUsed();

    // Retrieve scope for the new scope path fragment
    if (entry->getType().is(TY_IMPORT)) { // Import
      accessScope = accessScope->lookupTable(entry->getName());
    } else if (entry->getType().isBaseType(TY_STRUCT)) { // Struct
      std::string structSignature =
          Struct::getSignature(entry->getType().getBaseType().getSubType(), entry->getType().getBaseType().getTemplateTypes());

      // Get struct table
      accessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);

      // Retrieve the original type if the struct was imported
      Capture *structCapture = currentScope->lookupCapture(structSignature);
      currentThisType = structCapture ? structCapture->getEntry()->getType() : entry->getType();
      currentThisType.setTemplateTypes(entry->getType().getTemplateTypes());

      // Check if the entry is public if it is imported
      if (structCapture && !structCapture->getEntry()->getSpecifiers().isPublic() &&
          accessScope->getParent()->isImported(currentScope))
        throw err->get(*ctx->IDENTIFIER()->getSymbol(), INSUFFICIENT_VISIBILITY,
                       "Cannot access '" + structSignature + "' due to its private visibility");
    } else {
      // Check if we have seen a 'this.' prefix, because the generator needs that
      if (entry->getScope()->getScopeType() == SCOPE_STRUCT && currentThisType.is(TY_DYN))
        throw err->get(*ctx->start, REFERENCED_UNDEFINED_VARIABLE,
                       "The symbol '" + currentVarName + "' could not be found. Missing 'this.' prefix?");
    }
    assert(accessScope != nullptr);

    // Otherwise, push the retrieved scope to the scope path
    scopePath.pushFragment(currentVarName, accessScope);

    return entry->getType();
  }

  if (ctx->builtinCall())
    return visit(ctx->builtinCall());

  return visit(ctx->assignExpr());
}

std::any AnalyzerVisitor::visitValue(SpiceParser::ValueContext *ctx) {
  // Primitive value
  if (ctx->primitiveValue())
    return visit(ctx->primitiveValue());

  // Function call
  if (ctx->functionCall())
    return visit(ctx->functionCall());

  // Array initialization
  if (ctx->arrayInitialization())
    return visit(ctx->arrayInitialization());

  // Struct instantiation
  if (ctx->structInstantiation())
    return visit(ctx->structInstantiation());

  // Typed nil
  if (ctx->NIL()) {
    auto nilType = any_cast<SymbolType>(visit(ctx->dataType()));
    if (nilType.is(TY_DYN))
      throw err->get(*ctx->dataType()->start, UNEXPECTED_DYN_TYPE_SA, "Nil must have an explicit type");
    return nilType;
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  std::string functionName;
  SymbolType thisType = SymbolType(TY_DYN);
  bool constructorCall = false;
  for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
    std::string identifier = ctx->IDENTIFIER()[i]->toString();
    SymbolTableEntry *symbolEntry = accessScope->lookup(identifier);

    if (i < ctx->IDENTIFIER().size() - 1) {
      if (!symbolEntry)
        throw err->get(*ctx->IDENTIFIER()[i]->getSymbol(), REFERENCED_UNDEFINED_FUNCTION,
                       "Symbol '" + scopePath.getScopePrefix() + identifier + "' was used before defined");
      thisType = symbolEntry->getType().getBaseType();
    } else if (symbolEntry != nullptr && symbolEntry->getType().getBaseType().is(TY_STRUCT)) {
      // Get the concrete template types
      std::vector<SymbolType> concreteTemplateTypes;
      if (ctx->typeLst()) {
        for (const auto &dataType : ctx->typeLst()->dataType())
          concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
      }
      std::string structSignature = Struct::getSignature(identifier, concreteTemplateTypes);

      // Get the struct instance
      Struct *spiceStruct = accessScope->matchStruct(currentScope, identifier, concreteTemplateTypes, err.get(), *ctx->start);
      if (!spiceStruct)
        throw err->get(*ctx->start, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
      spiceStruct->setUsed();

      symbolEntry = accessScope->lookup(structSignature);
      assert(symbolEntry != nullptr);

      // Import struct if necessary
      if (accessScope->isImported(currentScope))
        thisType = initExtStruct(*ctx->IDENTIFIER()[i]->getSymbol(), accessScope, scopePath.getScopePrefix(true), identifier,
                                 concreteTemplateTypes);
      else
        thisType = symbolEntry->getType().getBaseType();

      functionName = CTOR_VARIABLE_NAME;
      constructorCall = true;
    } else {
      functionName = identifier;
      continue;
    }

    std::string tableName = symbolEntry->getType().is(TY_IMPORT) ? identifier : STRUCT_SCOPE_PREFIX + thisType.getName();
    accessScope = accessScope->lookupTable(tableName);
    assert(accessScope != nullptr);
    scopePath.pushFragment(identifier, accessScope);
  }
  assert(accessScope != nullptr);

  ScopePath scopePathBackup = scopePath;

  // Visit args
  std::vector<SymbolType> argTypes;
  if (ctx->argLst()) {
    for (const auto &arg : ctx->argLst()->assignExpr())
      argTypes.push_back(any_cast<SymbolType>(visit(arg)));
  }

  // Set to root scope if it did not change
  if (accessScope == currentScope)
    accessScope = rootScope;

  // Avoid this type import
  if (thisType.is(TY_IMPORT))
    thisType = SymbolType(TY_DYN);

  // Get the function/procedure instance
  antlr4::Token *token = ctx->IDENTIFIER().back()->getSymbol();
  SymbolType origThisType = thisType.replaceBaseSubType(CommonUtil::getLastFragment(thisType.getBaseType().getSubType(), "."));
  Function *spiceFunc = accessScope->matchFunction(currentScope, functionName, origThisType, argTypes, err.get(), *token);
  if (!spiceFunc) {
    // Build dummy function to get a better error message
    std::string codeLoc = CommonUtil::tokenToCodeLoc(*ctx->start);
    SymbolSpecifiers specifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));

    ArgList errArgTypes;
    for (auto &argType : argTypes)
      errArgTypes.emplace_back(argType, false);

    Function f(functionName, specifiers, thisType, SymbolType(TY_DYN), errArgTypes, {}, codeLoc);

    throw err->get(*ctx->start, REFERENCED_UNDEFINED_FUNCTION,
                   "Function/Procedure '" + f.getSignature() + "' could not be found");
  }
  spiceFunc->setUsed();

  // Get function entry
  SymbolTableEntry *functionEntry = accessScope->lookup(spiceFunc->getSignature());
  assert(functionEntry != nullptr);
  functionEntry->setUsed(); // Set the function to used

  // Check if the function entry has sufficient visibility
  if (accessScope->isImported(currentScope) && !functionEntry->getSpecifiers().isPublic())
    throw err->get(*token, INSUFFICIENT_VISIBILITY,
                   "Cannot access function/procedure '" + spiceFunc->getSignature() + "' due to its private visibility");

  // Return struct type on constructor call
  if (constructorCall)
    return thisType;

  // If the callee is a procedure, return type bool
  if (spiceFunc->isProcedure() || spiceFunc->getReturnType().is(TY_DYN))
    return SymbolType(TY_BOOL);

  // Retrieve the return type of the function
  SymbolType returnType = spiceFunc->getReturnType();

  // If the return type is an external struct, initialize it
  if (returnType.is(TY_STRUCT) && scopePathBackup.getCurrentScope()->isImported(currentScope))
    return initExtStruct(*ctx->start, scopePathBackup.getCurrentScope(), scopePathBackup.getScopePrefix(true),
                         returnType.getSubType(), thisType.getTemplateTypes());

  return returnType;
}

std::any AnalyzerVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  // Check if all values have the same type
  unsigned int actualSize = 0;
  SymbolType actualItemType = SymbolType(TY_DYN);
  if (ctx->argLst()) {
    for (unsigned int i = 0; i < ctx->argLst()->assignExpr().size(); i++) {
      auto itemType = any_cast<SymbolType>(visit(ctx->argLst()->assignExpr()[i]));
      if (actualItemType.is(TY_DYN)) {
        actualItemType = itemType;
      } else if (itemType != actualItemType) {
        throw err->get(*ctx->argLst()->assignExpr()[i]->start, ARRAY_ITEM_TYPE_NOT_MATCHING,
                       "All provided values have to be of the same data type. You provided " + actualItemType.getName(false) +
                           " and " + itemType.getName(false));
      }
      actualSize++;
    }
  }

  // Override actual array size if the expected type has a fixed size
  actualSize = expectedType.isArray() ? expectedType.getArraySize() : actualSize;

  // Check if actual item type is known now
  if (actualItemType.is(TY_DYN)) { // Not enough info to perform type inference, because of empty array {}
    if (expectedType.is(TY_DYN))
      throw err->get(*ctx->start, UNEXPECTED_DYN_TYPE_SA, "Not enough information to perform type inference");
    if (expectedType.is(TY_DYN))
      throw err->get(*ctx->start, ARRAY_ITEM_TYPE_NOT_MATCHING, "Cannot assign an array to a primitive data type");
    actualItemType = expectedType.getContainedTy();
  }

  return actualItemType.toArray(err.get(), *ctx->LBRACE()->getSymbol(), actualSize);
}

std::any AnalyzerVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Retrieve fully qualified struct name and the scope where to search it
  std::string accessScopePrefix;
  std::string structName;
  bool structIsImported = false;
  for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
    structName = ctx->IDENTIFIER()[i]->toString();
    if (i < ctx->IDENTIFIER().size() - 1) {
      SymbolTableEntry *symbolEntry = accessScope->lookup(structName);
      if (!symbolEntry)
        throw err->get(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                       "Symbol '" + accessScopePrefix + structName + "' was used before defined");
      accessScopePrefix += structName + ".";
      std::string tableName = symbolEntry->getType().is(TY_IMPORT) ? structName : STRUCT_SCOPE_PREFIX + structName;
      accessScope = accessScope->lookupTable(tableName);
      if (accessScope->isImported(currentScope))
        structIsImported = true;
    }
  }
  assert(accessScope != nullptr);

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (ctx->typeLst()) {
    for (const auto &dataType : ctx->typeLst()->dataType())
      concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
  }

  // Get the struct instance
  Struct *spiceStruct = accessScope->matchStruct(currentScope, structName, concreteTemplateTypes, err.get(), *ctx->start);
  if (!spiceStruct) {
    std::string structSignature = Struct::getSignature(structName, concreteTemplateTypes);
    throw err->get(*ctx->start, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
  }
  spiceStruct->setUsed();

  SymbolType structType;
  if (structIsImported) { // Imported struct
    structType = initExtStruct(*ctx->IDENTIFIER()[0]->getSymbol(), accessScope, accessScopePrefix,
                               ctx->IDENTIFIER().back()->toString(), concreteTemplateTypes);
  } else { // Not imported
    SymbolTableEntry *structSymbol =
        currentScope->lookup(accessScopePrefix + Struct::getSignature(structName, concreteTemplateTypes));
    if (!structSymbol)
      throw err->get(*ctx->IDENTIFIER().front()->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                     "Could not find struct '" + accessScopePrefix + structName + "'");
    structType = structSymbol->getType();
  }

  // Set template types to the struct
  std::vector<GenericType> genericTemplateTypes = spiceStruct->getTemplateTypes();
  std::vector<SymbolType> templateTypes;
  for (auto &genericType : genericTemplateTypes)
    templateTypes.emplace_back(genericType.getTypeChain());
  structType.setTemplateTypes(templateTypes);

  // Check if the number of fields matches
  SymbolTable *structTable = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + accessScopePrefix + structName);
  std::vector<SymbolType> fieldTypes;
  if (ctx->argLst()) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->getFieldTypes().size() != ctx->argLst()->assignExpr().size())
      throw err->get(*ctx->argLst()->start, NUMBER_OF_FIELDS_NOT_MATCHING,
                     "You've passed too less/many field values. Pass either none or all of them");

    // Check if the field types are matching
    for (int i = 0; i < ctx->argLst()->assignExpr().size(); i++) {
      // Get actual type
      auto assignExpr = ctx->argLst()->assignExpr()[i];
      auto actualType = any_cast<SymbolType>(visit(assignExpr));
      // Get expected type
      SymbolTableEntry *expectedField = structTable->lookupByIndex(i);
      assert(expectedField != nullptr);
      SymbolType expectedFieldType = expectedField->getType();
      // Check if type matches declaration
      if (actualType != expectedFieldType)
        throw err->get(*assignExpr->start, FIELD_TYPE_NOT_MATCHING,
                       "Expected type " + expectedFieldType.getName(false) + " for the field '" + expectedField->getName() +
                           "', but got " + actualType.getName(false));
    }
  }

  return structType;
}

std::any AnalyzerVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) {
  if (ctx->DOUBLE())
    return SymbolType(TY_DOUBLE);
  if (ctx->INTEGER())
    return SymbolType(TY_INT);
  if (ctx->SHORT())
    return SymbolType(TY_SHORT);
  if (ctx->LONG())
    return SymbolType(TY_LONG);
  if (ctx->CHAR_LITERAL())
    return SymbolType(TY_CHAR);
  if (ctx->STRING_LITERAL())
    return SymbolType(TY_STRING);
  return SymbolType(TY_BOOL);
}

std::any AnalyzerVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto type = any_cast<SymbolType>(visit(ctx->baseDataType()));

  unsigned int tokenCounter = 1;
  while (tokenCounter < ctx->children.size()) {
    auto token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
    if (token->getSymbol()->getType() == SpiceParser::MUL) { // Consider de-referencing operators
      type = type.toPointer(err.get(), *ctx->start);
    } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider array bracket pairs
      tokenCounter++;                                                    // Consume LBRACKET
      token = dynamic_cast<antlr4::tree::TerminalNode *>(ctx->children[tokenCounter]);
      unsigned int size = 0;                                       // Default to 0 when no size is attached
      if (token->getSymbol()->getType() == SpiceParser::INTEGER) { // Size is attached
        int signedSize = std::stoi(token->toString());
        // Check if size >1
        if (signedSize <= 1)
          throw err->get(*token->getSymbol(), ARRAY_SIZE_INVALID, "The size of an array must be > 1");
        size = signedSize;
        tokenCounter++; // Consume INTEGER
      }
      type = type.toArray(err.get(), *ctx->start, size);
    }
    tokenCounter++;
  }

  return type;
}

std::any AnalyzerVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
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
  if (ctx->customDataType()) // Struct or generic type
    return visit(ctx->customDataType());
  return SymbolType(TY_DYN);
}

std::any AnalyzerVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  // Check if it is a generic type
  std::string firstFragment = ctx->IDENTIFIER()[0]->toString();
  SymbolTableEntry *entry = currentScope->lookup(firstFragment);
  if (ctx->IDENTIFIER().size() == 1 && !entry && currentScope->lookupGenericType(firstFragment))
    return *static_cast<SymbolType *>(currentScope->lookupGenericType(firstFragment));

  // It is a struct type -> get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Get type name in format: a.b.c and retrieve the scope in parallel
  std::string accessScopePrefix;
  std::string structName;
  bool structIsImported = false;
  for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
    structName = ctx->IDENTIFIER()[i]->toString();
    if (i < ctx->IDENTIFIER().size() - 1)
      accessScopePrefix += structName + ".";
    SymbolTableEntry *symbolEntry = accessScope->lookup(structName);
    if (!symbolEntry)
      throw err->get(*ctx->IDENTIFIER()[0]->getSymbol(), UNKNOWN_DATATYPE, "Unknown symbol '" + structName + "'");

    std::string tableName = symbolEntry->getType().is(TY_IMPORT) ? structName : STRUCT_SCOPE_PREFIX + structName;
    accessScope = accessScope->lookupTable(tableName);
    if (accessScope->isImported(currentScope))
      structIsImported = true;
  }

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (ctx->typeLst()) {
    for (const auto &dataType : ctx->typeLst()->dataType())
      concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
  }

  // Set the struct instance to used
  Struct *externalSpiceStruct = accessScope->matchStruct(nullptr, structName, concreteTemplateTypes, err.get(), *ctx->start);
  assert(externalSpiceStruct != nullptr);
  externalSpiceStruct->setUsed();

  if (structIsImported) // Imported struct
    return initExtStruct(*ctx->start, accessScope, accessScopePrefix, structName, concreteTemplateTypes);

  // Check if struct was declared
  SymbolTableEntry *structSymbol = accessScope->lookup(structName);
  if (!structSymbol)
    throw err->get(*ctx->start, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
  structSymbol->setUsed();

  return SymbolType(TY_STRUCT, structName, concreteTemplateTypes);
}

SymbolType AnalyzerVisitor::initExtStruct(const antlr4::Token &token, SymbolTable *sourceScope,
                                          const std::string &structScopePrefix, const std::string &structName,
                                          const std::vector<SymbolType> &templateTypes) {
  // Get external struct name
  std::string newStructName = structScopePrefix + structName;

  // Create new struct type
  SymbolType newStructTy = SymbolType(TY_STRUCT, newStructName);
  newStructTy.setTemplateTypes(templateTypes);

  // Check if the struct is imported already
  std::string newStructSignature = Struct::getSignature(newStructName, templateTypes);
  Capture *globalCapture = rootScope->lookupCapture(newStructSignature);
  if (globalCapture)
    return newStructTy;

  // Check if external struct is declared
  std::string structSignature = Struct::getSignature(structName, templateTypes);
  SymbolTableEntry *externalStructSymbol = sourceScope->lookup(structSignature);
  if (!externalStructSymbol)
    throw err->get(token, REFERENCED_UNDEFINED_STRUCT, "Could not find struct '" + newStructName + "'");

  // Get the associated symbolTable of the external struct symbol
  SymbolTable *externalStructTable = sourceScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);

  // Initialize potential structs for field types
  for (auto &[_, entry] : externalStructTable->getSymbols()) {
    if (entry.getType().isBaseType(TY_STRUCT)) {
      std::string nestedStructName = entry.getType().getBaseType().getSubType();
      // Initialize nested struct
      initExtStruct(token, sourceScope, structScopePrefix, nestedStructName, entry.getType().getBaseType().getTemplateTypes());
      // Re-map type of field to the imported struct
      SymbolType newNestedStructType = entry.getType();
      newNestedStructType = newNestedStructType.replaceBaseSubType(structScopePrefix + nestedStructName);
      entry.updateType(newNestedStructType, true);
    }
  }

  // Set to DECLARED, so that the generator can set it to DEFINED as soon as the LLVM struct type was generated once
  Capture newGlobalCapture = Capture(externalStructSymbol, newStructSignature, DECLARED);
  rootScope->addCapture(newStructSignature, newGlobalCapture);
  externalStructSymbol->setUsed();

  // Set the struct instance to used
  Struct *externalSpiceStruct = sourceScope->matchStruct(nullptr, structName, templateTypes, err.get(), token);
  assert(externalSpiceStruct);
  externalSpiceStruct->setUsed();

  // Mount the external struct table to the new position in the root scope of the current source file
  rootScope->mountChildBlock(STRUCT_SCOPE_PREFIX + newStructSignature, externalStructTable, false);
  rootScope->lookupTable(STRUCT_SCOPE_PREFIX + newStructSignature)->disableCompilerWarnings(); // Disable compiler warnings

  return newStructTy;
}

SymbolType AnalyzerVisitor::initExtGlobal(const antlr4::Token &token, SymbolTable *sourceScope,
                                          const std::string &globalScopePrefix, const std::string &globalName) {
  // Get external global var name
  std::string newGlobalName = globalScopePrefix + globalName;

  // Check if the global var is imported already
  Capture *globalCapture = rootScope->lookupCapture(newGlobalName);
  if (globalCapture)
    return globalCapture->getEntry()->getType();

  // Check if external global var is declared
  SymbolTableEntry *externalGlobalSymbol = sourceScope->lookup(globalName);
  if (!externalGlobalSymbol)
    throw err->get(token, REFERENCED_UNDEFINED_VARIABLE, "Could not find global variable '" + newGlobalName + "'");

  // Set to DECLARED, so that the generator can set it to DEFINED as soon as the LLVM struct type was generated once
  Capture newGlobalCapture = Capture(externalGlobalSymbol, newGlobalName, DECLARED);
  rootScope->addCapture(newGlobalName, newGlobalCapture);
  externalGlobalSymbol->setUsed();

  return externalGlobalSymbol->getType();
}
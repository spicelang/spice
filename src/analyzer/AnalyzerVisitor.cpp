// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

#include <utility>

#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/SemanticError.h>
#include <symbol/Function.h>
#include <symbol/GenericType.h>
#include <symbol/Struct.h>
#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolTable.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>

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
  this->err = std::make_unique<ErrorFactory>();

  // Create OpRuleManager
  opRuleManager = std::make_unique<OpRuleManager>(err.get(), allowUnsafeOperations);
}

std::any AnalyzerVisitor::visitEntry(EntryNode *node) {
  // --- Pre-traversing actions

  // --- Traverse AST
  AstVisitor::visitChildren(node);

  // --- Post traversing actions
  // Remove non-substantiated functions and structs
  // Currently disabled because the generator relies on unsubstantiated structs in the main source file
  // if (requiresMainFct && secondRun)
  //  rootScope->purgeSubstantiationRemnants();

  // Reset the AST
  node->reset();

  bool reAnalyze = reAnalyzeRequired;

  // Check if the visitor got a main function
  if (requiresMainFct && !hasMainFunction)
    throw err->get(node->codeLoc, MISSING_MAIN_FUNCTION, "No main function found");

  // Print compiler warnings once the whole ast is present, but not for std files
  if (requiresMainFct && !isStdFile && !reAnalyze)
    rootScope->printCompilerWarnings();

  // Increment run number if the source file gets analyzed again
  runNumber++;

  reAnalyzeRequired = false;
  return reAnalyze;
}

std::any AnalyzerVisitor::visitMainFctDef(MainFctDefNode *node) {
  std::string mainSignature = MAIN_FUNCTION_NAME + "()";

  if (runNumber == 1) { // First run
    // Check if the function is already defined
    if (currentScope->lookup(mainSignature))
      throw err->get(node->codeLoc, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

    // Insert function name into the root symbol table
    SymbolType symbolType = SymbolType(TY_FUNCTION);
    currentScope->insert(mainSignature, symbolType, SymbolSpecifiers(symbolType), INITIALIZED, node->codeLoc);

    // Create the function scope
    node->fctScope = currentScope = currentScope->createChildBlock(mainSignature, SCOPE_FUNC_PROC_BODY);

    // Declare variable for the return value in the function scope
    SymbolType returnType = SymbolType(TY_INT);
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), INITIALIZED, node->codeLoc);
    currentScope->lookup(RETURN_VARIABLE_NAME)->setUsed();

    // Visit arguments in new scope
    if (node->hasArgs)
      visit(node->paramLst());

    // Return to root scope
    currentScope = currentScope->getParent();

    // Confirm main function
    hasMainFunction = true;
    currentScope->lookup(mainSignature)->setUsed();
  } else if (runNumber == 2) { // Second run
    // Do down into function scope
    currentScope = currentScope->getChild(mainSignature);

    // Visit statements in new scope
    visit(node->stmtLst());

    // Call destructors for variables, that are going out of scope
    std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
    for (SymbolTableEntry *varEntry : varsToDestruct)
      insertDestructorCall(node->codeLoc, varEntry);

    // Return to root scope
    currentScope = currentScope->getParent();
  }
  return nullptr;
}

std::any AnalyzerVisitor::visitFctDef(FctDefNode *node) {
  // Check if this is a global function or a method
  if (node->isMethod) {
    // Change to the struct scope
    currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + node->structName);
    if (!currentScope)
      throw err->get(node->codeLoc, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  if (runNumber == 1) { // First run
    // Check if name is dtor
    if (node->functionName == "dtor")
      throw err->get(node->codeLoc, DTOR_MUST_BE_PROCEDURE, "Destructors are not allowed to be of type function");

    // Create a new scope
    node->fctScope = currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

    // Get 'this' type
    std::vector<GenericType> templateTypes;
    SymbolType thisType = SymbolType(TY_DYN);
    SymbolType thisPtrType = thisType;
    if (node->isMethod) {
      SymbolTableEntry *structEntry = currentScope->lookup(node->structName);
      assert(structEntry != nullptr);
      thisType = structEntry->getType();
      thisPtrType = thisType.toPointer(err.get(), node->codeLoc);
      for (const auto &templateType : thisType.getTemplateTypes())
        templateTypes.emplace_back(templateType);
    }

    // Get template types
    if (node->isGeneric) {
      for (const auto &dataType : node->templateTypeLst()->dataTypes()) {
        auto templateType = any_cast<SymbolType>(visit(dataType));
        if (!templateType.is(TY_GENERIC))
          throw err->get(dataType->codeLoc, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
        assert(genericType != nullptr);
        templateTypes.push_back(*genericType);
      }
    }

    // Visit arguments in new scope
    std::vector<std::string> argNames;
    ArgList argTypes;
    if (node->hasParams) {
      auto namedArgList = any_cast<NamedParamList>(visit(node->paramLst()));
      for (const auto &namedArg : namedArgList) {
        std::string argName = std::get<0>(namedArg);
        SymbolType argType = std::get<1>(namedArg);
        bool argOptional = std::get<2>(namedArg);

        // Check if the type is present in the template for generic types
        if (argType.is(TY_GENERIC)) {
          if (std::none_of(templateTypes.begin(), templateTypes.end(), [&](const GenericType &t) { return t == argType; }))
            throw err->get(node->paramLst()->codeLoc, GENERIC_TYPE_NOT_IN_TEMPLATE,
                           "Generic arg type not included in function template");
        }

        argNames.push_back(argName);
        argTypes.push_back({argType, argOptional});
      }
    }

    // Declare 'this' variable in new scope
    if (node->isMethod) {
      SymbolSpecifiers thisTypeSpecifiers(thisPtrType);
      thisTypeSpecifiers.setConst(true);
      currentScope->insert(THIS_VARIABLE_NAME, thisPtrType, thisTypeSpecifiers, INITIALIZED, node->codeLoc);
    }

    // Declare variable for the return value in the function scope
    auto returnType = any_cast<SymbolType>(visit(node->returnType()));
    if (returnType.is(TY_DYN))
      throw err->get(node->codeLoc, UNEXPECTED_DYN_TYPE_SA, "Dyn return types are not allowed");
    if (returnType.isPointer())
      throw err->get(node->codeLoc, COMING_SOON_SA,
                     "Spice currently not supports pointer return types due to not supporting heap allocations.");
    currentScope->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, node->codeLoc);

    // Return to old scope
    currentScope = currentScope->getParent();

    // Build function specifiers
    auto fctSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
      for (const auto &specifier : specifierLst->specifiers()) {
        if (specifier->type == SpecifierNode::TY_INLINE)
          fctSymbolSpecifiers.setInline(true);
        else if (specifier->type == SpecifierNode::TY_PUBLIC)
          fctSymbolSpecifiers.setPublic(true);
        else
          throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
      }
    }

    // Insert function into the symbol table
    Function spiceFunc(node->functionName, fctSymbolSpecifiers, thisType, returnType, argTypes, templateTypes, node->codeLoc);
    currentScope->insertFunction(spiceFunc, err.get());

    // Rename / duplicate the original child block to reflect the substantiated versions of the function
    std::vector<Function> substantiatedFunctions = spiceFunc.substantiateOptionalArgs();
    currentScope->renameChildBlock(node->getScopeId(), substantiatedFunctions.front().getSignature());
    for (int i = 1; i < substantiatedFunctions.size(); i++)
      currentScope->copyChildBlock(substantiatedFunctions.front().getSignature(), substantiatedFunctions[i].getSignature());

    // Leave the struct scope
    if (node->isMethod)
      currentScope = currentScope->getParent();
  } else { // Other runs
    // Change to the struct scope
    if (node->isMethod)
      currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + node->structName);

    // Get manifestations of that function
    std::map<std::string, Function> *manifestations = currentScope->getFunctionManifestations(node->codeLoc);

    // Leave the struct scope
    if (node->isMethod)
      currentScope = currentScope->getParent();

    // Set the symbolTypeIndex to 0
    node->symbolTypeIndex = 0;

    if (manifestations) {
      for (auto &[mangledName, spiceFunc] : *manifestations) {
        // Check if the function is substantiated
        if (!spiceFunc.isFullySubstantiated())
          continue;

        if (spiceFunc.wasAlreadyAnalyzed())
          continue;

        // Change scope to the struct specialization
        if (node->isMethod) {
          std::string structSignature = Struct::getSignature(node->structName, spiceFunc.getThisType().getTemplateTypes());
          currentScope = currentScope->getChild(STRUCT_SCOPE_PREFIX + structSignature);
          assert(currentScope);
        }

        // Go down again in scope
        currentScope = currentScope->getChild(spiceFunc.getSignature());
        assert(currentScope != nullptr);

        // Replace this type
        /*if (spiceFunc.isMethodFunction()) {
          SymbolTableEntry *thisVar = currentScope->lookup(THIS_VARIABLE_NAME);
          assert(thisVar != nullptr);
          thisVar->updateType(spiceFunc.getThisType(), true);
        }*/

        // Morph the generic return type
        SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
        if (returnVarEntry->getType().is(TY_GENERIC)) {
          SymbolType returnType = spiceFunc.getReturnType();
          if (returnType.isPointer())
            throw err->get(node->codeLoc, COMING_SOON_SA,
                           "Spice currently not supports pointer return types due to not supporting heap allocations.");
          returnVarEntry->updateType(returnType, true);
        }

        // Get argument types
        std::vector<std::pair<std::string, SymbolType>> args;
        if (node->hasParams) {
          for (const auto paramDecl : node->paramLst()->params()) {
            SymbolTableEntry *argEntry = currentScope->lookup(paramDecl->varName);
            assert(argEntry);
            args.emplace_back(paramDecl->varName, argEntry->getType());
          }
        }

        // Morph the generic types to the replacements
        std::vector<SymbolType> newArgTypes = spiceFunc.getArgTypes();
        for (int i = 0; i < newArgTypes.size(); i++) {
          SymbolTableEntry *argEntry = currentScope->lookup(args[i].first);
          argEntry->updateType(newArgTypes[i], true);
        }

        // Visit statements in new scope
        visit(node->stmtLst());

        // Call destructors for variables, that are going out of scope
        std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
        for (SymbolTableEntry *varEntry : varsToDestruct)
          insertDestructorCall(node->codeLoc, varEntry);

        // Reset generic types
        for (const auto &arg : args) {
          SymbolTableEntry *argEntry = currentScope->lookup(arg.first);
          assert(argEntry);
          argEntry->updateType(arg.second, true);
        }

        // Check if return variable is now initialized
        if (currentScope->lookup(RETURN_VARIABLE_NAME)->getState() == DECLARED)
          throw err->get(node->codeLoc, FUNCTION_WITHOUT_RETURN_STMT, "Function without return statement");

        // Leave the function scope
        currentScope = currentScope->getParent();

        // Leave the struct scope
        if (node->isMethod)
          currentScope = currentScope->getParent();

        spiceFunc.setAnalyzed();

        // Increase the symbolTypeIndex
        node->symbolTypeIndex++;
      }
    }
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitProcDef(ProcDefNode *node) {
  if (runNumber == 1) { // First run
    // Change to the struct scope
    if (node->isMethod) {
      currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + node->structName);
      if (!currentScope)
        throw err->get(node->codeLoc, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
    }

    // Create a new scope
    node->procScope = currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

    // Get 'this' type
    std::vector<GenericType> templateTypes;
    SymbolType thisType = SymbolType(TY_DYN);
    SymbolType thisPtrType = thisType;
    if (node->isMethod) {
      SymbolTableEntry *structEntry = currentScope->lookup(node->structName);
      thisType = structEntry->getType();
      thisPtrType = thisType.toPointer(err.get(), node->codeLoc);
      for (const auto &templateType : thisType.getTemplateTypes())
        templateTypes.emplace_back(templateType);
    }

    // Get template types
    if (node->isGeneric) {
      for (const auto &dataType : node->templateTypeLst()->dataTypes()) {
        auto templateType = any_cast<SymbolType>(visit(dataType));
        if (!templateType.is(TY_GENERIC))
          throw err->get(dataType->codeLoc, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
        GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
        assert(genericType != nullptr);
        templateTypes.push_back(*genericType);
      }
    }

    if (node->hasParams && node->procedureName == "dtor")
      throw err->get(node->codeLoc, DTOR_WITH_PARAMS, "It is not allowed to specify parameters for destructors");

    // Visit arguments in new scope
    std::vector<std::string> argNames;
    ArgList argTypes;
    if (node->hasParams) {
      auto namedArgList = any_cast<NamedParamList>(visit(node->paramLst()));
      for (const auto &namedArg : namedArgList) {
        std::string argName = std::get<0>(namedArg);
        SymbolType argType = std::get<1>(namedArg);
        bool argOptional = std::get<2>(namedArg);

        // Check if the type is present in the template for generic types
        if (argType.is(TY_GENERIC)) {
          if (std::none_of(templateTypes.begin(), templateTypes.end(), [&](const GenericType &t) { return t == argType; }))
            throw err->get(node->paramLst()->codeLoc, GENERIC_TYPE_NOT_IN_TEMPLATE,
                           "Generic arg type not included in procedure template");
        }

        argNames.push_back(argName);
        argTypes.push_back({argType, argOptional});
      }
    }

    // Declare 'this' variable in new scope
    if (node->isMethod) {
      auto thisSymbolSpecifiers = SymbolSpecifiers(thisPtrType);
      thisSymbolSpecifiers.setConst(true);
      currentScope->insert(THIS_VARIABLE_NAME, thisPtrType, thisSymbolSpecifiers, INITIALIZED, node->codeLoc);
    }

    // Return to old scope
    currentScope = currentScope->getParent();

    // Build procedure specifiers
    auto procSymbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
      for (const auto &specifier : specifierLst->specifiers()) {
        if (specifier->type == SpecifierNode::TY_INLINE)
          procSymbolSpecifiers.setInline(true);
        else if (specifier->type == SpecifierNode::TY_PUBLIC)
          procSymbolSpecifiers.setPublic(true);
        else
          throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
      }
    }

    // Insert function into the symbol table
    Function spiceProc(node->procedureName, procSymbolSpecifiers, thisType, SymbolType(TY_DYN), argTypes, templateTypes,
                       node->codeLoc);
    currentScope->insertFunction(spiceProc, err.get());

    // Rename / duplicate the original child block to reflect the substantiated versions of the function
    std::vector<Function> substantiatedProcedures = spiceProc.substantiateOptionalArgs();
    currentScope->renameChildBlock(node->getScopeId(), substantiatedProcedures.front().getSignature());
    for (int i = 1; i < substantiatedProcedures.size(); i++)
      currentScope->copyChildBlock(substantiatedProcedures.front().getSignature(), substantiatedProcedures[i].getSignature());

    // Leave the struct scope
    if (node->isMethod)
      currentScope = currentScope->getParent();
  } else { // Other runs
    // Enter the struct scope
    if (node->isMethod)
      currentScope = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + node->structName);

    // Get manifestations of that procedure
    std::map<std::string, Function> *manifestations = currentScope->getFunctionManifestations(node->codeLoc);

    // Leave the struct scope
    if (node->isMethod)
      currentScope = currentScope->getParent();

    // Set the symbolTypeIndex to 0
    node->symbolTypeIndex = 0;

    if (manifestations) {
      for (auto &[mangledName, spiceProc] : *manifestations) {
        // Check if the function is substantiated
        if (!spiceProc.isFullySubstantiated())
          continue;

        if (spiceProc.wasAlreadyAnalyzed())
          continue;

        // Change scope to the struct specialization
        if (node->isMethod) {
          std::string structSignature = Struct::getSignature(node->structName, spiceProc.getThisType().getTemplateTypes());
          currentScope = currentScope->getChild(STRUCT_SCOPE_PREFIX + structSignature);
          assert(currentScope);
        }

        // Go down again in scope
        currentScope = currentScope->getChild(spiceProc.getSignature());
        assert(currentScope != nullptr);

        // Replace this type
        /*if (spiceProc.isMethodProcedure()) {
          SymbolTableEntry *thisVar = currentScope->lookup(THIS_VARIABLE_NAME);
          assert(thisVar != nullptr);
          thisVar->updateType(spiceProc.getThisType(), true);
        }*/

        // Get parameter types
        std::vector<std::pair<std::string, SymbolType>> params;
        if (node->paramLst()) {
          for (const auto paramDecl : node->paramLst()->params()) {
            SymbolTableEntry *paramEntry = currentScope->lookup(paramDecl->varName);
            assert(paramEntry);
            params.emplace_back(paramDecl->varName, paramEntry->getType());
          }
        }

        // Morph the generic types to the replacements
        std::vector<SymbolType> newArgTypes = spiceProc.getArgTypes();
        for (int i = 0; i < newArgTypes.size(); i++) {
          SymbolTableEntry *argEntry = currentScope->lookup(params[i].first);
          argEntry->updateType(newArgTypes[i], true);
        }

        // Visit statements in new scope
        visit(node->stmtLst());

        // Call destructors for variables, that are going out of scope
        std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
        for (SymbolTableEntry *varEntry : varsToDestruct)
          insertDestructorCall(node->codeLoc, varEntry);

        // Reset generic types
        for (const auto &arg : params) {
          SymbolTableEntry *argEntry = currentScope->lookup(arg.first);
          assert(argEntry);
          argEntry->updateType(arg.second, true);
        }

        // Leave the function scope
        currentScope = currentScope->getParent();

        // Leave the struct scope
        if (node->isMethod)
          currentScope = currentScope->getParent();

        spiceProc.setAnalyzed();

        // Increase the symbolTypeIndex
        node->symbolTypeIndex++;
      }
    }
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitStructDef(StructDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if struct already exists in this scope
  if (currentScope->lookup(node->structName))
    throw err->get(node->codeLoc, STRUCT_DECLARED_TWICE, "Duplicate struct '" + node->structName + "'");

  // Get template types
  std::vector<GenericType> genericTemplateTypes;
  std::vector<SymbolType> templateTypes;
  if (node->isGeneric) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes()) {
      auto templateType = any_cast<SymbolType>(visit(dataType));
      if (!templateType.is(TY_GENERIC))
        throw err->get(dataType->codeLoc, EXPECTED_GENERIC_TYPE, "A template list can only contain generic types");
      GenericType *genericType = currentScope->lookupGenericType(templateType.getSubType());
      assert(genericType != nullptr);
      genericTemplateTypes.push_back(*genericType);
      templateTypes.push_back(*genericType);
    }
  }

  // Build struct specifiers
  SymbolType symbolType = SymbolType(TY_STRUCT, node->structName, templateTypes);
  auto structSymbolSpecifiers = SymbolSpecifiers(symbolType);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const auto &specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        structSymbolSpecifiers.setPublic(true);
      else
        throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
    }
  }

  // Add the struct to the symbol table
  currentScope->insert(node->structName, symbolType, structSymbolSpecifiers, DECLARED, node->codeLoc);

  // Create scope for struct
  SymbolTable *structScope = currentScope = currentScope->createChildBlock(STRUCT_SCOPE_PREFIX + node->structName, SCOPE_STRUCT);

  // Insert a field for each field list entry
  std::vector<SymbolType> fieldTypes;
  for (const auto &field : node->fields()) {
    auto fieldType = any_cast<SymbolType>(visit(field->dataType()));

    if (fieldType.isBaseType(TY_GENERIC)) { // Check if the type is present in the template for generic types
      if (std::none_of(genericTemplateTypes.begin(), genericTemplateTypes.end(),
                       [&](const GenericType &t) { return t == fieldType.getBaseType(); }))
        throw err->get(field->dataType()->codeLoc, GENERIC_TYPE_NOT_IN_TEMPLATE,
                       "Generic field type not included in struct template");
    }

    auto fieldSymbolSpecifiers = SymbolSpecifiers(symbolType);
    if (SpecifierLstNode *specifierLst = field->specifierLst(); specifierLst) {
      for (const auto &specifier : specifierLst->specifiers()) {
        if (specifier->type == SpecifierNode::TY_CONST)
          throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT,
                         "Struct fields cannot have the const specifier attached");
        else if (specifier->type == SpecifierNode::TY_SIGNED)
          fieldSymbolSpecifiers.setSigned(true);
        else if (specifier->type == SpecifierNode::TY_UNSIGNED)
          fieldSymbolSpecifiers.setSigned(false);
        else if (specifier->type == SpecifierNode::TY_PUBLIC)
          fieldSymbolSpecifiers.setPublic(true);
        else
          throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
      }
    }

    // Add the field to the symbol table
    currentScope->insert(field->name, fieldType, fieldSymbolSpecifiers, DECLARED, field->codeLoc);

    fieldTypes.push_back(fieldType);
  }

  // Return to the old scope
  currentScope = currentScope->getParent();

  // Add struct
  Struct s(node->structName, structSymbolSpecifiers, fieldTypes, genericTemplateTypes, node->codeLoc);
  currentScope->insertStruct(s, err.get());
  s.setSymbolTable(structScope);

  return nullptr;
}

std::any AnalyzerVisitor::visitGenericTypeDef(GenericTypeDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if type already exists in this scope
  if (currentScope->lookup(node->typeName))
    throw err->get(node->codeLoc, GENERIC_TYPE_DECLARED_TWICE, "Duplicate symbol name '" + node->typeName + "'");

  // Get type conditions
  std::vector<SymbolType> typeConditions;
  for (const auto &typeAlt : node->typeAltsLst()->dataTypes()) {
    auto typeCondition = any_cast<SymbolType>(visit(typeAlt));
    typeConditions.push_back(typeCondition);
  }

  // Build symbol specifiers
  GenericType genericType = GenericType(node->typeName, typeConditions);
  auto structSymbolSpecifiers = SymbolSpecifiers(genericType);
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        structSymbolSpecifiers.setPublic(true);
      else
        throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a struct definition");
    }
  }

  // Add it to the symbol table
  currentScope->insertGenericType(node->typeName, genericType);

  return nullptr;
}

std::any AnalyzerVisitor::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (runNumber > 1)
    return nullptr;

  // Check if symbol already exists in the symbol table
  if (currentScope->lookup(node->varName))
    throw err->get(node->codeLoc, VARIABLE_DECLARED_TWICE,
                   "The global variable '" + node->varName + "' was declared more than once");

  // Check if symbol already exists in any imported module scope
  if (currentScope->lookupGlobal(node->varName, true))
    throw err->get(node->codeLoc, VARIABLE_DECLARED_TWICE,
                   "A global variable named '" + node->varName +
                       "' is already declared in another module. Please use a different name.");

  // Insert variable name to symbol table
  auto symbolType = any_cast<SymbolType>(visit(node->dataType()));

  SymbolState state = DECLARED;
  if (node->value()) { // Variable is initialized here
    auto valueType = any_cast<SymbolType>(visit(node->value()));
    // Infer type
    if (symbolType.is(TY_DYN)) {
      symbolType = valueType;
    } else if (symbolType != valueType) {
      throw err->get(node->value()->codeLoc, OPERATOR_WRONG_DATA_TYPE,
                     "Cannot apply the assign operator on different data types. You provided " + symbolType.getName(false) +
                         " and " + valueType.getName(false));
    }
    state = INITIALIZED;
  }

  // Check if the type is missing
  if (symbolType.is(TY_DYN))
    throw err->get(node->dataType()->codeLoc, GLOBAL_OF_TYPE_DYN, "Global variables must have an explicit data type");

  // Check if we would need to insert instructions in the global scope
  if (!symbolType.isPrimitive())
    throw err->get(node->dataType()->codeLoc, GLOBAL_OF_INVALID_TYPE, "Spice does not allow global variables of this type");

  // Create symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST) {
        // Check if a value is attached
        if (!node->value())
          throw err->get(node->codeLoc, GLOBAL_CONST_WITHOUT_VALUE, "You must specify a value for constant global variables");
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->type == SpecifierNode::TY_SIGNED) {
        symbolTypeSpecifiers.setSigned(true);
      } else if (specifier->type == SpecifierNode::TY_UNSIGNED) {
        symbolTypeSpecifiers.setSigned(false);
      } else if (specifier->type == SpecifierNode::TY_PUBLIC) {
        symbolTypeSpecifiers.setPublic(true);
      } else {
        throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use this specifier on a global variable definition");
      }
    }
  }

  // Insert into symbol table
  currentScope->insert(node->varName, symbolType, symbolTypeSpecifiers, state, node->codeLoc);

  return nullptr;
}

std::any AnalyzerVisitor::visitExtDecl(ExtDeclNode *node) {
  if (runNumber > 1)
    return nullptr;

  ArgList argTypes;
  if (node->hasArgs) {
    // Check if an argument is dyn
    for (const auto &arg : node->argTypeLst()->dataTypes()) {
      auto argType = any_cast<SymbolType>(visit(arg));
      if (argType.is(TY_DYN))
        throw err->get(arg->codeLoc, UNEXPECTED_DYN_TYPE_SA, "Dyn data type is not allowed as arg type for external functions");
      argTypes.emplace_back(argType, false);
    }
  }

  if (node->returnType()) { // Function
    // Check if return type is dyn
    auto returnType = any_cast<SymbolType>(visit(node->returnType()));
    if (returnType.is(TY_DYN))
      throw err->get(node->returnType()->codeLoc, UNEXPECTED_DYN_TYPE_SA,
                     "Dyn data type is not allowed as return type for external functions");

    // Insert function into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
    Function spiceFunc(node->extFunctionName, symbolSpecifiers, SymbolType(TY_DYN), returnType, argTypes, {}, node->codeLoc);
    currentScope->insertFunction(spiceFunc, err.get());

    // Add return symbol for function
    SymbolTable *functionTable = currentScope->createChildBlock(spiceFunc.getSignature(), SCOPE_FUNC_PROC_BODY);
    functionTable->insert(RETURN_VARIABLE_NAME, returnType, SymbolSpecifiers(returnType), DECLARED, node->codeLoc);
    functionTable->lookup(RETURN_VARIABLE_NAME)->setUsed();
  } else { // Procedure
    // Insert procedure into symbol table
    SymbolSpecifiers symbolSpecifiers = SymbolSpecifiers(SymbolType(TY_PROCEDURE));
    Function spiceProc(node->extFunctionName, symbolSpecifiers, SymbolType(TY_DYN), SymbolType(TY_DYN), argTypes, {},
                       node->codeLoc);
    currentScope->insertFunction(spiceProc, err.get());

    // Add empty scope for function body
    currentScope->createChildBlock(spiceProc.getSignature(), SCOPE_FUNC_PROC_BODY);
  }

  return nullptr;
}

std::any AnalyzerVisitor::visitThreadDef(ThreadDefNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_THREAD_BODY);
  currentScope->setCapturingRequired(); // Requires capturing because the LLVM IR will end up in a separate function

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->getParent();

  return node->setEvaluatedSymbolType(SymbolType(TY_BYTE).toPointer(err.get(), node->codeLoc));
}

std::any AnalyzerVisitor::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_UNSAFE_BODY);

  // Enable unsafe operations
  allowUnsafeOperations = true;

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Disable unsafe operations again
  allowUnsafeOperations = false;

  // Return to old scope
  currentScope = currentScope->getParent();

  return nullptr;
}

std::any AnalyzerVisitor::visitForLoop(ForLoopNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_FOR_BODY);

  // Visit loop variable declaration in new scope
  visit(node->initDecl());

  // Visit condition in new scope
  auto conditionType = any_cast<SymbolType>(visit(node->condAssign()));
  if (!conditionType.is(TY_BOOL))
    throw err->get(node->condAssign()->codeLoc, CONDITION_MUST_BE_BOOL, "For loop condition must be of type bool");

  // Visit incrementer in new scope
  visit(node->incAssign());

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(node->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();
  return nullptr;
}

std::any AnalyzerVisitor::visitForeachLoop(ForeachLoopNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_FOREACH_BODY);

  // Check type of the array
  expectedType = SymbolType(TY_DYN);
  auto arrayType = any_cast<SymbolType>(visit(node->arrayAssign()));
  if (!arrayType.isArray() && !arrayType.is(TY_STRING))
    throw err->get(node->arrayAssign()->codeLoc, OPERATOR_WRONG_DATA_TYPE,
                   "Can only apply foreach loop on an array type. You provided " + arrayType.getName(false));

  if (arrayType.getArraySize() == 0)
    throw err->get(node->arrayAssign()->codeLoc, OPERATOR_WRONG_DATA_TYPE,
                   "Can only apply foreach loop on an array type of which the size is known at compile time");

  // Check index assignment or declaration
  SymbolType indexType;
  if (node->idxVarDecl()) {
    indexType = any_cast<SymbolType>(visit(node->idxVarDecl()));

    // Set declared variable to initialized, because we increment it internally in the loop
    if (!node->idxVarDecl()->assignExpr()) {
      std::string varName = node->idxVarDecl()->varName;
      SymbolTableEntry *entry = currentScope->lookup(varName);
      assert(entry != nullptr);
      entry->updateState(INITIALIZED, err.get(), node->idxVarDecl()->codeLoc);
    }

    // Check if index type is int
    if (!indexType.is(TY_INT))
      throw err->get(node->idxVarDecl()->codeLoc, ARRAY_INDEX_NO_INTEGER,
                     "Index in foreach loop must be of type int. You provided " + indexType.getName(false));
  } else {
    // Declare the variable with the default index variable name
    SymbolType symbolType = SymbolType(TY_INT);
    auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
    symbolTypeSpecifiers.setConst(true);
    currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, symbolType, symbolTypeSpecifiers, INITIALIZED, node->codeLoc);
  }

  // Check type of the item
  auto itemType = any_cast<SymbolType>(visit(node->itemDecl()));
  SymbolTableEntry *itemVarSymbol = currentScope->lookup(node->itemDecl()->varName);
  assert(itemVarSymbol != nullptr);
  if (itemType.is(TY_DYN)) {
    itemType = arrayType.getContainedTy();
    itemVarSymbol->updateType(itemType, false);

    // Update symbolType of the declaration data type
    node->itemDecl()->dataType()->setEvaluatedSymbolType(itemType);
  } else {
    if (itemType != arrayType.getContainedTy())
      throw err->get(node->itemDecl()->codeLoc, OPERATOR_WRONG_DATA_TYPE,
                     "Foreach loop item type does not match array type. Expected " + arrayType.getName(false) + ", provided " +
                         itemType.getName(false));
  }
  itemVarSymbol->updateState(INITIALIZED, err.get(), node->itemDecl()->codeLoc);

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(node->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();

  return nullptr;
}

std::any AnalyzerVisitor::visitWhileLoop(WhileLoopNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_WHILE_BODY);

  // Visit condition
  auto conditionType = any_cast<SymbolType>(visit(node->condition()));
  if (!conditionType.is(TY_BOOL))
    throw err->get(node->condition()->codeLoc, CONDITION_MUST_BE_BOOL, "While loop condition must be of type bool");

  // Visit statement list in new scope
  nestedLoopCounter++;
  visit(node->stmtLst());
  nestedLoopCounter--;

  // Return to old scope
  currentScope = currentScope->getParent();

  return nullptr;
}

std::any AnalyzerVisitor::visitIfStmt(IfStmtNode *node) {
  // Create a new scope
  currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_IF_BODY);

  // Visit condition
  auto conditionType = any_cast<SymbolType>(visit(node->condition()));
  if (!conditionType.is(TY_BOOL))
    throw err->get(node->condition()->codeLoc, CONDITION_MUST_BE_BOOL, "If condition must be of type bool");

  // Visit statement list in new scope
  visit(node->stmtLst());

  // Return to old scope
  currentScope = currentScope->getParent();

  // Visit else statement if it exists
  if (node->elseStmt())
    visit(node->elseStmt());

  return nullptr;
}

std::any AnalyzerVisitor::visitElseStmt(ElseStmtNode *node) {
  if (node->isElseIf) { // Visit if statement in the case of an else if branch
    visit(node->ifStmt());
  } else { // Make a new scope in case of an else branch
    // Create a new scope
    currentScope = currentScope->createChildBlock(node->getScopeId(), SCOPE_IF_BODY);

    // Visit statement list in new scope
    visit(node->stmtLst());

    // Return to old scope
    currentScope = currentScope->getParent();
  }
  return nullptr;
}

std::any AnalyzerVisitor::visitAssertStmt(AssertStmtNode *node) {
  auto assertConditionType = any_cast<SymbolType>(visit(node->assignExpr()));

  // Check if assertStmt evaluates to bool
  if (!assertConditionType.is(TY_BOOL))
    throw err->get(node->assignExpr()->codeLoc, ASSERTION_CONDITION_BOOL, "The asserted condition must be of type bool");

  return nullptr;
}

std::any AnalyzerVisitor::visitParamLst(ParamLstNode *node) {
  NamedParamList namedParamList;
  bool metOptional = false;
  for (const auto &param : node->params()) {
    visit(param);
    SymbolType paramType = param->dataType()->getEvaluatedSymbolType();

    // Check if the type could be inferred. Dyn without a default value is forbidden
    if (paramType.is(TY_DYN))
      throw err->get(node->codeLoc, FCT_PARAM_IS_TYPE_DYN, "Type of parameter '" + param->varName + "' is invalid");

    // Ensure that no optional param comes after a mandatory param
    if (param->hasAssignment) {
      metOptional = true;
    } else if (metOptional) {
      throw err->get(param->codeLoc, INVALID_PARAM_ORDER, "Mandatory parameters must go before any optional parameters");
    }

    namedParamList.emplace_back(param->varName, paramType, metOptional);
  }
  return namedParamList;
}

std::any AnalyzerVisitor::visitDeclStmt(DeclStmtNode *node) {
  // Check if symbol already exists in the symbol table
  if (currentScope->lookupStrict(node->varName))
    throw err->get(node->codeLoc, VARIABLE_DECLARED_TWICE, "The variable '" + node->varName + "' was declared more than once");

  // Get the type of the symbol
  SymbolType symbolType = expectedType = any_cast<SymbolType>(visit(node->dataType()));

  // Visit the right side
  SymbolState initialState = DECLARED;
  if (node->hasAssignment) {
    auto rhsTy = any_cast<SymbolType>(visit(node->assignExpr()));
    // Check if type has to be inferred or both types are fixed
    symbolType = opRuleManager->getAssignResultType(node->codeLoc, symbolType, rhsTy);
    initialState = INITIALIZED;

    // Push symbolType to the declaration data type
    node->dataType()->setEvaluatedSymbolType(symbolType);

    // If the rhs is of type array and was the array initialization, there must be a size attached
    if (symbolType.isArray() && symbolType.getArraySize() == 0 && currentVarName.empty())
      throw err->get(node->dataType()->codeLoc, ARRAY_SIZE_INVALID, "The declaration of an array type must have a size attached");
  }

  // Build symbol specifiers
  auto symbolTypeSpecifiers = SymbolSpecifiers(symbolType);
  if (node->specifierLst()) {
    for (const auto &specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST) {
        symbolTypeSpecifiers.setConst(true);
      } else if (specifier->type == SpecifierNode::TY_SIGNED) {
        symbolTypeSpecifiers.setSigned(true);
        symbolType.setSigned(true);
      } else if (specifier->type == SpecifierNode::TY_UNSIGNED) {
        symbolTypeSpecifiers.setSigned(false);
        symbolType.setSigned(false);
      } else {
        throw err->get(specifier->codeLoc, SPECIFIER_AT_ILLEGAL_CONTEXT,
                       "Cannot use this specifier on a local variable declaration");
      }
    }
  }

  // Insert variable into symbol table
  currentScope->insert(node->varName, symbolType, symbolTypeSpecifiers, initialState, node->codeLoc);

  return symbolType;
}

std::any AnalyzerVisitor::visitImportStmt(ImportStmtNode * /*node*/) {
  // Noop
  return nullptr;
}

std::any AnalyzerVisitor::visitReturnStmt(ReturnStmtNode *node) {
  SymbolType returnType = SymbolType(TY_DYN);
  SymbolTableEntry *returnVariable = currentScope->lookup(RETURN_VARIABLE_NAME);
  if (returnVariable) { // Return variable => function
    expectedType = returnVariable->getType();

    // Check if there is a value attached to the return statement
    if (node->hasReturnValue) {
      // Visit the value
      returnType = any_cast<SymbolType>(visit(node->assignExpr()));

      // Check data type of return statement
      if (returnVariable->getType().is(TY_DYN)) {
        // Set explicit return type to the return variable
        returnVariable->updateType(returnType, false);
      } else {
        // Check if return type matches with function definition
        if (returnType != returnVariable->getType())
          throw err->get(node->assignExpr()->codeLoc, OPERATOR_WRONG_DATA_TYPE,
                         "Passed wrong data type to return statement. Expected " + returnVariable->getType().getName(false) +
                             " but got " + returnType.getName(false));
      }

      // Set the return variable to initialized
      returnVariable->updateState(INITIALIZED, err.get(), node->codeLoc);
    } else {
      returnType = returnVariable->getType();
    }

    // Check if result variable is initialized
    if (returnVariable->getState() != INITIALIZED)
      throw err->get(node->codeLoc, RETURN_WITHOUT_VALUE_RESULT,
                     "Return without value, but result variable is not initialized yet");
    returnVariable->setUsed();
  } else {
    // No return variable => procedure
    if (node->assignExpr())
      throw err->get(node->assignExpr()->codeLoc, RETURN_WITH_VALUE_IN_PROCEDURE,
                     "Return statements in procedures may not have a value attached");
  }

  // Call destructors for variables, that are going out of scope
  std::vector<SymbolTableEntry *> varsToDestruct = currentScope->getVarsGoingOutOfScope(true);
  for (SymbolTableEntry *varEntry : varsToDestruct)
    insertDestructorCall(node->codeLoc, varEntry);

  return nullptr;
}

std::any AnalyzerVisitor::visitBreakStmt(BreakStmtNode *node) {
  if (node->breakTimes != 1) {
    // Check if the stated number is valid
    if (node->breakTimes < 1)
      throw err->get(node->codeLoc, INVALID_BREAK_NUMBER,
                     "Break count must be >= 1, you provided " + std::to_string(node->breakTimes));
  }
  // Check if we can break this often
  if (node->breakTimes > nestedLoopCounter)
    throw err->get(node->codeLoc, INVALID_BREAK_NUMBER,
                   "We can only break " + std::to_string(nestedLoopCounter) + " time(s) here");
  return nullptr;
}

std::any AnalyzerVisitor::visitContinueStmt(ContinueStmtNode *node) {
  if (node->continueTimes != 1) {
    // Check if the stated number is valid
    if (node->continueTimes < 1)
      throw err->get(node->codeLoc, INVALID_CONTINUE_NUMBER,
                     "Continue count must be >= 1, you provided " + std::to_string(node->continueTimes));
  }
  // Check if we can continue this often
  if (node->continueTimes > nestedLoopCounter)
    throw err->get(node->codeLoc, INVALID_CONTINUE_NUMBER,
                   "We can only continue " + std::to_string(nestedLoopCounter) + " time(s) here");
  return nullptr;
}

std::any AnalyzerVisitor::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  std::size_t index = node->templatedString.find_first_of('%');
  int placeholderCount = 0;
  while (index != std::string::npos) {
    // Check if there is another assignExpr
    if (node->assignExpr().size() <= placeholderCount)
      throw err->get(node->codeLoc, PRINTF_ARG_COUNT_ERROR,
                     "The placeholder string contains more placeholders that arguments were passed");

    auto assignment = node->assignExpr()[placeholderCount];
    auto assignmentType = any_cast<SymbolType>(visit(assignment));
    switch (node->templatedString[index + 1]) {
    case 'c': {
      if (!assignmentType.is(TY_CHAR))
        throw err->get(assignment->codeLoc, PRINTF_TYPE_ERROR,
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
        throw err->get(assignment->codeLoc, PRINTF_TYPE_ERROR,
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
        throw err->get(assignment->codeLoc, PRINTF_TYPE_ERROR,
                       "Template string expects double, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 's': {
      if (!assignmentType.is(TY_STRING) && !assignmentType.isPointerOf(TY_CHAR) && !assignmentType.isArrayOf(TY_CHAR))
        throw err->get(assignment->codeLoc, PRINTF_TYPE_ERROR,
                       "Template string expects string, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!assignmentType.isPointer() && !assignmentType.isArray())
        throw err->get(assignment->codeLoc, PRINTF_TYPE_ERROR,
                       "Template string expects pointer, but got " + assignmentType.getName(false));
      placeholderCount++;
      break;
    }
    }
    index = node->templatedString.find_first_of('%', index + 1);
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->assignExpr().size())
    throw err->get(node->codeLoc, PRINTF_ARG_COUNT_ERROR,
                   "The placeholder string contains less placeholders that arguments were passed");

  return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));
}

std::any AnalyzerVisitor::visitSizeofCall(SizeofCallNode *node) {
  SymbolType symbolType;
  if (node->isType) { // Size of type
    symbolType = any_cast<SymbolType>(visit(node->dataType()));
  } else { // Size of value
    symbolType = any_cast<SymbolType>(visit(node->assignExpr()));
  }

  // Check if symbol type is dynamically sized array
  if (symbolType.is(TY_ARRAY) && symbolType.getArraySize() == -1)
    throw err->get(node->codeLoc, SIZEOF_DYNAMIC_SIZED_ARRAY, "Cannot get sizeof dynamically sized array at compile time");

  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any AnalyzerVisitor::visitLenCall(LenCallNode *node) {
  auto argType = any_cast<SymbolType>(visit(node->assignExpr()));

  // Check if arg is of type array
  if (!argType.isArray())
    throw err->get(node->assignExpr()->codeLoc, EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays");

  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any AnalyzerVisitor::visitTidCall(TidCallNode *node) {
  // Nothing to check here. Tid builtin has no arguments
  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any AnalyzerVisitor::visitJoinCall(JoinCallNode *node) {
  SymbolType bytePtr = SymbolType(TY_BYTE).toPointer(err.get(), node->codeLoc);
  for (const auto &assignExpr : node->assignExpressions()) {
    auto argSymbolType = any_cast<SymbolType>(visit(assignExpr));
    if (argSymbolType == bytePtr && argSymbolType.isArrayOf(bytePtr))
      throw err->get(assignExpr->codeLoc, JOIN_ARG_MUST_BE_TID,
                     "You have to pass a thread id (byte*) or a array of thread ids (byte*[]) to to join builtin");
  }

  // Return the number of threads that were joined
  return node->setEvaluatedSymbolType(SymbolType(TY_INT));
}

std::any AnalyzerVisitor::visitAssignExpr(AssignExprNode *node) {
  // Check if there is an assign operator applied
  if (node->hasOperator) { // This is an assignment
    // Get symbol type of right side
    auto rhsTy = any_cast<SymbolType>(visit(node->rhs()));

    // Visit the left side
    currentVarName = "";    // Reset the current variable name
    currentEntry = nullptr; // Reset the current entry
    auto lhsTy = any_cast<SymbolType>(visit(node->lhs()));
    std::string variableName = currentVarName;

    // Take a look at the operator
    if (node->op == AssignExprNode::OP_ASSIGN) {
      rhsTy = opRuleManager->getAssignResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_PLUS_EQUAL) {
      rhsTy = opRuleManager->getPlusEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_MINUS_EQUAL) {
      rhsTy = opRuleManager->getMinusEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_MUL_EQUAL) {
      rhsTy = opRuleManager->getMulEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_DIV_EQUAL) {
      rhsTy = opRuleManager->getDivEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_REM_EQUAL) {
      rhsTy = opRuleManager->getRemEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_SHL_EQUAL) {
      rhsTy = opRuleManager->getSHLEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_SHR_EQUAL) {
      rhsTy = opRuleManager->getSHREqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_AND_EQUAL) {
      rhsTy = opRuleManager->getAndEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_OR_EQUAL) {
      rhsTy = opRuleManager->getOrEqualResultType(node->codeLoc, lhsTy, rhsTy);
    } else if (node->op == AssignExprNode::OP_XOR_EQUAL) {
      rhsTy = opRuleManager->getXorEqualResultType(node->codeLoc, lhsTy, rhsTy);
    }

    if (!variableName.empty()) { // Variable is involved on the left side
      // Check if the symbol exists
      if (!currentEntry)
        throw err->get(node->lhs()->codeLoc, REFERENCED_UNDEFINED_VARIABLE,
                       "The variable '" + variableName + "' was referenced before defined");

      // Perform type inference
      if (lhsTy.is(TY_DYN))
        currentEntry->updateType(rhsTy, false);

      // Update state in symbol table
      if (!currentEntry->getType().isOneOf({TY_FUNCTION, TY_PROCEDURE}))
        currentEntry->updateState(INITIALIZED, err.get(), node->lhs()->codeLoc);

      // In case the lhs variable is captured, notify the capture about the write access
      Capture *lhsCapture = currentScope->lookupCapture(variableName);
      if (lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);

      // Print compiler warning if the rhs size exceeds the lhs size
      if (lhsTy.isArray() && rhsTy.getArraySize() > lhsTy.getArraySize())
        CompilerWarning(node->rhs()->codeLoc, ARRAY_TOO_MANY_VALUES,
                        "You provided more values "
                        "than your array can hold. Excess variables are being ignored by the compiler.")
            .print();
    }

    return node->setEvaluatedSymbolType(rhsTy);
  } else if (node->ternaryExpr()) {
    return visit(node->ternaryExpr());
  } else if (node->threadDef()) {
    return visit(node->threadDef());
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any AnalyzerVisitor::visitTernaryExpr(TernaryExprNode *node) {
  // Check if there is a ternary operator applied
  if (node->children.size() > 1) {
    auto condition = node->operands()[0];
    auto conditionType = any_cast<SymbolType>(visit(condition));
    auto trueType = any_cast<SymbolType>(visit(node->operands()[1]));
    auto falseType = any_cast<SymbolType>(visit(node->operands()[2]));
    // Check if the condition evaluates to boolean
    if (!conditionType.is(TY_BOOL))
      throw err->get(condition->codeLoc, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool");
    // Check if trueType and falseType are matching
    if (trueType != falseType)
      throw err->get(node->codeLoc, OPERATOR_WRONG_DATA_TYPE, "True and false operands in ternary must be of same data type");
    return node->setEvaluatedSymbolType(trueType);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = opRuleManager->getLogicalOrResultType(node->codeLoc, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitLogicalAndExpr(LogicalAndExprNode *node) {
  // Check if a logical and operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = opRuleManager->getLogicalAndResultType(node->codeLoc, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  // Check if a bitwise or operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = opRuleManager->getBitwiseOrResultType(node->codeLoc, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  // Check if a bitwise xor operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = opRuleManager->getBitwiseXorResultType(node->codeLoc, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  // Check if a bitwise and operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    for (int i = 1; i < node->operands().size(); i++) {
      auto rhsTy = any_cast<SymbolType>(visit(node->operands()[i]));
      lhsTy = opRuleManager->getBitwiseAndResultType(node->codeLoc, lhsTy, rhsTy);
    }
    return node->setEvaluatedSymbolType(lhsTy);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitEqualityExpr(EqualityExprNode *node) {
  // Check if at least one equality operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(node->operands()[1]));

    if (node->op == EqualityExprNode::OP_EQUAL) // Operator was equal
      return node->setEvaluatedSymbolType(opRuleManager->getEqualResultType(node->codeLoc, lhsTy, rhsTy));
    else if (node->op == EqualityExprNode::OP_NOT_EQUAL) // Operator was not equal
      return node->setEvaluatedSymbolType(opRuleManager->getNotEqualResultType(node->codeLoc, lhsTy, rhsTy));
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitRelationalExpr(RelationalExprNode *node) {
  // Check if a relational operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(node->operands()[1]));

    if (node->op == RelationalExprNode::OP_LESS) // Operator was less
      return node->setEvaluatedSymbolType(opRuleManager->getLessResultType(node->codeLoc, lhsTy, rhsTy));
    else if (node->op == RelationalExprNode::OP_GREATER) // Operator was greater
      return node->setEvaluatedSymbolType(opRuleManager->getGreaterResultType(node->codeLoc, lhsTy, rhsTy));
    else if (node->op == RelationalExprNode::OP_LESS_EQUAL) // Operator was less equal
      return node->setEvaluatedSymbolType(opRuleManager->getLessEqualResultType(node->codeLoc, lhsTy, rhsTy));
    else if (node->op == RelationalExprNode::OP_GREATER_EQUAL) // Operator was greater equal
      return node->setEvaluatedSymbolType(opRuleManager->getGreaterEqualResultType(node->codeLoc, lhsTy, rhsTy));
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitShiftExpr(ShiftExprNode *node) {
  // Check if at least one shift operator is applied
  if (node->children.size() > 1) {
    auto lhsTy = any_cast<SymbolType>(visit(node->operands()[0]));
    auto rhsTy = any_cast<SymbolType>(visit(node->operands()[1]));

    if (node->op == ShiftExprNode::OP_SHIFT_LEFT) // Operator was shl
      return node->setEvaluatedSymbolType(opRuleManager->getShiftLeftResultType(node->codeLoc, lhsTy, rhsTy));
    else if (node->op == ShiftExprNode::OP_SHIFT_RIGHT) // Operator was shr
      return node->setEvaluatedSymbolType(opRuleManager->getShiftRightResultType(node->codeLoc, lhsTy, rhsTy));
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitAdditiveExpr(AdditiveExprNode *node) {
  // Check if at least one additive operator is applied
  if (node->operands().size() > 1) {
    auto currentType = any_cast<SymbolType>(visit(node->operands()[0]));

    auto opQueue = node->opQueue;
    std::queue<std::pair<AdditiveExprNode::AdditiveOp, SymbolType>> newOpQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      MultiplicativeExprNode *operand = node->operands()[operandIndex++];
      assert(operand != nullptr);
      auto operandType = any_cast<SymbolType>(visit(operand));

      switch (opQueue.front().first) {
      case AdditiveExprNode::OP_PLUS:
        currentType = opRuleManager->getPlusResultType(operand->codeLoc, currentType, operandType);
        break;
      case AdditiveExprNode::OP_MINUS:
        currentType = opRuleManager->getMinusResultType(operand->codeLoc, currentType, operandType);
        break;
      default:
        throw std::runtime_error("Additive expr fall-through");
      }

      newOpQueue.emplace(opQueue.front().first, currentType);
      opQueue.pop();
    }
    node->opQueue = newOpQueue;

    return node->setEvaluatedSymbolType(currentType);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitMultiplicativeExpr(MultiplicativeExprNode *node) {
  // Check if at least one multiplicative operator is applied
  if (node->operands().size() > 1) {
    auto currentType = any_cast<SymbolType>(visit(node->operands()[0]));

    auto opQueue = node->opQueue;
    std::queue<std::pair<MultiplicativeExprNode::MultiplicativeOp, SymbolType>> newOpQueue;
    size_t operandIndex = 1;
    while (!opQueue.empty()) {
      CastExprNode *operand = node->operands()[operandIndex++];
      assert(operand != nullptr);
      auto operandType = any_cast<SymbolType>(visit(operand));

      switch (opQueue.front().first) {
      case MultiplicativeExprNode::OP_MUL:
        currentType = opRuleManager->getMulResultType(operand->codeLoc, currentType, operandType);
        break;
      case MultiplicativeExprNode::OP_DIV:
        currentType = opRuleManager->getDivResultType(operand->codeLoc, currentType, operandType);
        break;
      case MultiplicativeExprNode::OP_REM:
        currentType = opRuleManager->getRemResultType(operand->codeLoc, currentType, operandType);
        break;
      default:
        throw std::runtime_error("Multiplicative expr fall-through");
      }

      newOpQueue.emplace(opQueue.front().first, currentType);
      opQueue.pop();
    }
    node->opQueue = newOpQueue;

    return node->setEvaluatedSymbolType(currentType);
  }
  return visit(node->operands().front());
}

std::any AnalyzerVisitor::visitCastExpr(CastExprNode *node) {
  std::any rhs = visit(node->prefixUnaryExpr());

  if (node->isCasted) { // Cast is applied
    auto srcType = any_cast<SymbolType>(rhs);
    auto dstType = any_cast<SymbolType>(visit(node->dataType()));
    SymbolType symbolType = opRuleManager->getCastResultType(node->codeLoc, dstType, any_cast<SymbolType>(rhs));
    return node->setEvaluatedSymbolType(symbolType);
  }

  return rhs;
}

std::any AnalyzerVisitor::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  currentVarName = "";                  // Reset the current variable name
  scopePath.clear();                    // Clear the scope path
  currentThisType = SymbolType(TY_DYN); // Reset this type

  auto rhs = any_cast<SymbolType>(visit(node->postfixUnaryExpr()));

  auto opStack = node->opStack;
  std::stack<std::pair<PrefixUnaryExprNode::PrefixUnaryOp, SymbolType>> newOpStack;
  while (!opStack.empty()) {
    switch (opStack.top().first) {
    case PrefixUnaryExprNode::OP_MINUS:
      rhs = opRuleManager->getPrefixMinusResultType(node->postfixUnaryExpr()->codeLoc, rhs);
      break;
    case PrefixUnaryExprNode::OP_PLUS_PLUS:
      rhs = opRuleManager->getPrefixPlusPlusResultType(node->postfixUnaryExpr()->codeLoc, rhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), node->codeLoc);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    case PrefixUnaryExprNode::OP_MINUS_MINUS:
      rhs = opRuleManager->getPrefixMinusMinusResultType(node->postfixUnaryExpr()->codeLoc, rhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), node->codeLoc);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    case PrefixUnaryExprNode::OP_NOT:
      rhs = opRuleManager->getPrefixNotResultType(node->postfixUnaryExpr()->codeLoc, rhs);
      break;
    case PrefixUnaryExprNode::OP_BITWISE_NOT:
      rhs = opRuleManager->getPrefixBitwiseNotResultType(node->postfixUnaryExpr()->codeLoc, rhs);
      break;
    case PrefixUnaryExprNode::OP_INDIRECTION:
      rhs = opRuleManager->getPrefixMulResultType(node->postfixUnaryExpr()->codeLoc, rhs);
      break;
    case PrefixUnaryExprNode::OP_ADDRESS_OF:
      rhs = opRuleManager->getPrefixBitwiseAndResultType(node->postfixUnaryExpr()->codeLoc, rhs);
      break;
    default:
      throw std::runtime_error("Prefix unary fall-through");
    }

    newOpStack.emplace(opStack.top().first, rhs);
    opStack.pop();
  }
  node->opStack = newOpStack;

  return node->setEvaluatedSymbolType(rhs);
}

std::any AnalyzerVisitor::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  auto lhs = any_cast<SymbolType>(visit(node->atomicExpr()));
  if (lhs.is(TY_INVALID))
    throw ErrorFactory::get(node->codeLoc, REFERENCED_UNDEFINED_VARIABLE,
                            "Variable '" + node->atomicExpr()->identifier + "' was referenced before declared");

  size_t subscriptCounter = 0;
  size_t memberAccessCounter = 0;

  auto opQueue = node->opQueue;
  std::queue<std::pair<PostfixUnaryExprNode::PostfixUnaryOp, SymbolType>> newOpQueue;
  while (!opQueue.empty()) {
    switch (opQueue.front().first) {
    case PostfixUnaryExprNode::OP_SUBSCRIPT: {
      std::string arrayName = currentVarName;              // Save array name
      SymbolTableEntry *currentEntryBackup = currentEntry; // Save current entry
      ScopePath scopePathBackup = scopePath;               // Save scope path

      AssignExprNode *indexExpr = node->assignExpr()[subscriptCounter++];
      auto indexType = any_cast<SymbolType>(visit(indexExpr));

      if (!indexType.is(TY_INT))
        throw err->get(node->codeLoc, ARRAY_INDEX_NO_INTEGER, "Array index must be of type int");
      if (!lhs.isOneOf({TY_ARRAY, TY_STRING, TY_PTR}))
        throw err->get(node->codeLoc, OPERATOR_WRONG_DATA_TYPE,
                       "Can only apply subscript operator on array type, got " + lhs.getName(true));

      if (lhs.is(TY_PTR) && !allowUnsafeOperations) {
        throw err->get(
            node->codeLoc, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
            "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.");
      } else if (lhs.is(TY_ARRAY) && lhs.getArraySize() > 0 && indexExpr->hasCompileTimeValue()) {
        std::int32_t constIndex = indexExpr->getCompileTimeValue().intValue;
        size_t constSize = lhs.getArraySize();
        if (constIndex >= constSize)
          throw err->get(node->codeLoc, ARRAY_INDEX_OUT_OF_BOUNDS,
                         "You are trying to access element with index " + std::to_string(constIndex) + " of an array with size " +
                             std::to_string(constSize));
      }

      // Get array item type
      lhs = currentThisType = lhs.getContainedTy();

      currentVarName = arrayName;        // Restore array name
      scopePath = scopePathBackup;       // Restore scope path
      currentEntry = currentEntryBackup; // Restore current entry

      // Retrieve scope for the new scope path fragment
      if (lhs.isBaseType(TY_STRUCT)) {
        SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
        assert(accessScope != nullptr);

        std::string structSignature = Struct::getSignature(lhs.getBaseType().getSubType(), lhs.getBaseType().getTemplateTypes());
        SymbolTable *newAccessScope = accessScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);
        assert(newAccessScope != nullptr);
        // Push the retrieved scope to the scope path
        scopePath.pushFragment("[idx]", newAccessScope);
      }

      break;
    }
    case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
      // Check if lhs is struct
      if (!lhs.isBaseType(TY_STRUCT))
        throw err->get(node->codeLoc, MEMBER_ACCESS_ONLY_STRUCTS, "Cannot apply member access operator on " + lhs.getName());

      PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];
      lhs = any_cast<SymbolType>(visit(rhs)); // Visit rhs
      break;
    }
    case PostfixUnaryExprNode::OP_SCOPE_ACCESS: {
      // Check if lhs is import
      if (!lhs.is(TY_IMPORT))
        throw err->get(node->codeLoc, SCOPE_ACCESS_ONLY_IMPORTS, "Cannot apply scope access operator on " + lhs.getName());

      PostfixUnaryExprNode *rhs = node->postfixUnaryExpr()[memberAccessCounter++];
      lhs = any_cast<SymbolType>(visit(rhs)); // Visit rhs
      break;
    }
    case PostfixUnaryExprNode::OP_PLUS_PLUS: {
      lhs = opRuleManager->getPostfixPlusPlusResultType(node->atomicExpr()->codeLoc, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), node->codeLoc);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    }
    case PostfixUnaryExprNode::OP_MINUS_MINUS: {
      lhs = opRuleManager->getPostfixMinusMinusResultType(node->atomicExpr()->codeLoc, lhs);

      // Update state in symbol table
      if (currentEntry != nullptr)
        currentEntry->updateState(INITIALIZED, err.get(), node->codeLoc);

      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->lookupCapture(currentVarName); lhsCapture)
        lhsCapture->setCaptureMode(READ_WRITE);
      break;
    }
    default:
      throw std::runtime_error("PostfixUnary fall-through");
    }

    newOpQueue.emplace(opQueue.front().first, lhs);
    opQueue.pop();
  }
  node->opQueue = newOpQueue;

  if (lhs.is(TY_INVALID))
    throw err->get(node->codeLoc, REFERENCED_UNDEFINED_VARIABLE,
                   "Variable '" + currentVarName + "' was referenced before declared");

  return node->setEvaluatedSymbolType(lhs);
}

std::any AnalyzerVisitor::visitAtomicExpr(AtomicExprNode *node) {
  if (node->value())
    return visit(node->value());

  if (!node->identifier.empty()) {
    currentVarName = node->identifier;

    // Check if this is a reserved keyword
    if (std::find(RESERVED_KEYWORDS.begin(), RESERVED_KEYWORDS.end(), currentVarName) != RESERVED_KEYWORDS.end())
      throw err->get(node->codeLoc, RESERVED_KEYWORD,
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
        throw err->get(node->codeLoc, INSUFFICIENT_VISIBILITY,
                       "Cannot access '" + currentVarName + "' due to its private visibility");

      // Check if the entry is an external global variable and needs to be imported
      if (entry->isGlobal() && !entry->getType().isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_IMPORT}))
        initExtGlobal(accessScope, scopePath.getScopePrefix(true), entry->getName(), node->codeLoc);
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
        throw err->get(node->codeLoc, INSUFFICIENT_VISIBILITY,
                       "Cannot access '" + structSignature + "' due to its private visibility");

      // If the return type is an external struct, initialize it
      if (!scopePath.isEmpty() && scopePath.getCurrentScope()->isImported(currentScope)) {
        SymbolTableEntry *parentStruct = currentScope->lookup(scopePath.getLastScopeName());
        assert(parentStruct != nullptr);
        std::string scopePrefix = CommonUtil::getPrefix(parentStruct->getType().getSubType(), ".");
        SymbolType symbolType = initExtStruct(accessScope, scopePrefix, entry->getType().getBaseType().getSubType(),
                                              currentThisType.getTemplateTypes(), node->codeLoc);
        return node->setEvaluatedSymbolType(symbolType);
      }
    } else {
      // Check if we have seen a 'this.' prefix, because the generator needs that
      if (entry->getScope()->getScopeType() == SCOPE_STRUCT && currentThisType.is(TY_DYN))
        throw err->get(node->codeLoc, REFERENCED_UNDEFINED_VARIABLE,
                       "The symbol '" + currentVarName + "' could not be found. Missing 'this.' prefix?");
    }
    assert(accessScope != nullptr);

    // Otherwise, push the retrieved scope to the scope path
    scopePath.pushFragment(currentVarName, accessScope);

    return node->setEvaluatedSymbolType(entry->getType());
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

std::any AnalyzerVisitor::visitValue(ValueNode *node) {
  // Primitive value
  if (node->primitiveValue())
    return visit(node->primitiveValue());

  // Function call
  if (node->functionCall())
    return visit(node->functionCall());

  // Array initialization
  if (node->arrayInitialization())
    return visit(node->arrayInitialization());

  // Struct instantiation
  if (node->structInstantiation())
    return visit(node->structInstantiation());

  // Typed nil
  if (node->isNil) {
    auto nilType = any_cast<SymbolType>(visit(node->nilType()));
    if (nilType.is(TY_DYN))
      throw err->get(node->nilType()->codeLoc, UNEXPECTED_DYN_TYPE_SA, "Nil must have an explicit type");
    return node->setEvaluatedSymbolType(nilType);
  }

  throw std::runtime_error("Value fall-through");
}

std::any AnalyzerVisitor::visitPrimitiveValue(PrimitiveValueNode *node) {
  switch (node->type) {
  case PrimitiveValueNode::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(SymbolType(TY_DOUBLE));
  case PrimitiveValueNode::TYPE_INT:
    return node->setEvaluatedSymbolType(SymbolType(TY_INT));
  case PrimitiveValueNode::TYPE_SHORT:
    return node->setEvaluatedSymbolType(SymbolType(TY_SHORT));
  case PrimitiveValueNode::TYPE_LONG:
    return node->setEvaluatedSymbolType(SymbolType(TY_LONG));
  case PrimitiveValueNode::TYPE_BYTE:
    return node->setEvaluatedSymbolType(SymbolType(TY_BYTE));
  case PrimitiveValueNode::TYPE_CHAR:
    return node->setEvaluatedSymbolType(SymbolType(TY_CHAR));
  case PrimitiveValueNode::TYPE_STRING:
    return node->setEvaluatedSymbolType(SymbolType(TY_STRING));
  case PrimitiveValueNode::TYPE_BOOL:
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));
  }
  throw std::runtime_error("Primitive value fall-through");
}

std::any AnalyzerVisitor::visitFunctionCall(FunctionCallNode *node) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  std::string functionName;
  SymbolType thisType = SymbolType(TY_DYN);
  bool constructorCall = false;
  for (unsigned int i = 0; i < node->functionNameFragments.size(); i++) {
    std::string identifier = node->functionNameFragments[i];
    SymbolTableEntry *symbolEntry = accessScope->lookup(identifier);

    if (i < node->functionNameFragments.size() - 1) {
      if (!symbolEntry)
        throw err->get(node->codeLoc, REFERENCED_UNDEFINED_FUNCTION,
                       "Symbol '" + scopePath.getScopePrefix() + identifier + "' was used before defined");
      thisType = symbolEntry->getType().getBaseType();
    } else if (symbolEntry != nullptr && symbolEntry->getType().getBaseType().is(TY_STRUCT)) {
      // Get the concrete template types
      std::vector<SymbolType> concreteTemplateTypes;
      if (node->isGeneric) {
        for (const auto &dataType : node->templateTypeLst()->dataTypes())
          concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
      }
      std::string structSignature = Struct::getSignature(identifier, concreteTemplateTypes);

      // Get the struct instance
      Struct *spiceStruct = accessScope->matchStruct(currentScope, identifier, concreteTemplateTypes, err.get(), node->codeLoc);
      if (!spiceStruct)
        throw err->get(node->codeLoc, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
      spiceStruct->setUsed();

      symbolEntry = accessScope->lookup(structSignature);
      assert(symbolEntry != nullptr);

      // Import struct if necessary
      if (accessScope->isImported(currentScope))
        thisType = initExtStruct(accessScope, scopePath.getScopePrefix(true), identifier, concreteTemplateTypes, node->codeLoc);
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

    if (!accessScope)
      throw err->get(node->codeLoc, REFERENCED_UNDEFINED_FUNCTION, "Cannot call a function on '" + identifier + "'");

    scopePath.pushFragment(identifier, accessScope);
  }
  assert(accessScope != nullptr);

  ScopePath scopePathBackup = scopePath;

  // Visit args
  std::vector<SymbolType> argTypes;
  if (node->hasArgs) {
    for (const auto &arg : node->argLst()->args())
      argTypes.push_back(any_cast<SymbolType>(visit(arg)));
  }

  // Set to root scope if it did not change
  if (accessScope == currentScope)
    accessScope = rootScope;

  // Avoid this type import
  if (thisType.is(TY_IMPORT))
    thisType = SymbolType(TY_DYN);

  // Get the function/procedure instance
  SymbolType origThisType = thisType.replaceBaseSubType(CommonUtil::getLastFragment(thisType.getBaseType().getSubType(), "."));
  Function *spiceFunc = accessScope->matchFunction(currentScope, functionName, origThisType, argTypes, err.get(), node->codeLoc);
  if (!spiceFunc) {
    // Build dummy function to get a better error message
    SymbolSpecifiers specifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));

    ArgList errArgTypes;
    for (const auto &argType : argTypes)
      errArgTypes.emplace_back(argType, false);

    Function f(functionName, specifiers, thisType, SymbolType(TY_DYN), errArgTypes, {}, node->codeLoc);

    throw err->get(node->codeLoc, REFERENCED_UNDEFINED_FUNCTION,
                   "Function/Procedure '" + f.getSignature() + "' could not be found");
  }
  spiceFunc->setUsed();

  // Get function entry
  SymbolTableEntry *functionEntry = accessScope->lookup(spiceFunc->getSignature());
  assert(functionEntry != nullptr);
  functionEntry->setUsed(); // Set the function to used

  // Check if the function entry has sufficient visibility
  if (accessScope->isImported(currentScope) && !functionEntry->getSpecifiers().isPublic())
    throw err->get(node->codeLoc, INSUFFICIENT_VISIBILITY,
                   "Cannot access function/procedure '" + spiceFunc->getSignature() + "' due to its private visibility");

  // Analyze the function if not done yet. This is only necessary if we call a function in the same source file, which was
  // declared above.
  if (!accessScope->isImported(currentScope) && spiceFunc->getDeclCodeLoc().line < node->codeLoc.line)
    reAnalyzeRequired = true;

  // Return struct type on constructor call
  if (constructorCall)
    return node->setEvaluatedSymbolType(thisType);

  // If the callee is a procedure, return type bool
  if (spiceFunc->isProcedure() || spiceFunc->getReturnType().is(TY_DYN))
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));

  // Retrieve the return type of the function
  SymbolType returnType = spiceFunc->getReturnType();

  // If the return type is an external struct, initialize it
  if (!scopePathBackup.isEmpty() && returnType.is(TY_STRUCT) && scopePathBackup.getCurrentScope()->isImported(currentScope)) {
    SymbolType symbolType = initExtStruct(scopePathBackup.getCurrentScope(), scopePathBackup.getScopePrefix(true),
                                          returnType.getSubType(), thisType.getTemplateTypes(), node->codeLoc);
    return node->setEvaluatedSymbolType(symbolType);
  }

  return node->setEvaluatedSymbolType(returnType);
}

std::any AnalyzerVisitor::visitArrayInitialization(ArrayInitializationNode *node) {
  // Check if all values have the same type
  int actualSize = 0;
  SymbolType actualItemType = SymbolType(TY_DYN);
  if (node->itemLst()) {
    // Set the expected array type to the contained type
    SymbolType expectedTypeBackup = expectedType;
    expectedType = expectedType.isArray() ? expectedType.getContainedTy() : expectedType;

    for (const auto &arg : node->itemLst()->args()) {
      auto itemType = any_cast<SymbolType>(visit(arg));
      if (actualItemType.is(TY_DYN)) {
        actualItemType = itemType;
      } else if (itemType != actualItemType) {
        throw err->get(arg->codeLoc, ARRAY_ITEM_TYPE_NOT_MATCHING,
                       "All provided values have to be of the same data type. You provided " + actualItemType.getName(false) +
                           " and " + itemType.getName(false));
      }
      actualSize++;
    }

    // Restore the expected array type
    expectedType = expectedTypeBackup;
  }

  // Override actual array size if the expected type has a fixed size
  actualSize = expectedType.isArray() ? expectedType.getArraySize() : actualSize;

  // Check if actual item type is known now
  if (actualItemType.is(TY_DYN)) { // Not enough info to perform type inference, because of empty array {}
    if (expectedType.is(TY_DYN))
      throw err->get(node->codeLoc, UNEXPECTED_DYN_TYPE_SA, "Not enough information to perform type inference");
    if (expectedType.is(TY_DYN))
      throw err->get(node->codeLoc, ARRAY_ITEM_TYPE_NOT_MATCHING, "Cannot assign an array to a primitive data type");
    actualItemType = expectedType.getContainedTy();
  }

  return node->setEvaluatedSymbolType(actualItemType.toArray(err.get(), node->codeLoc, actualSize));
}

std::any AnalyzerVisitor::visitStructInstantiation(StructInstantiationNode *node) {
  // Get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Retrieve fully qualified struct name and the scope where to search it
  std::string accessScopePrefix;
  std::string structName;
  bool structIsImported = false;
  for (unsigned int i = 0; i < node->structNameFragments.size(); i++) {
    structName = node->structNameFragments[i];
    if (i < node->structNameFragments.size() - 1) {
      SymbolTableEntry *symbolEntry = accessScope->lookup(structName);
      if (!symbolEntry)
        throw err->get(node->codeLoc, REFERENCED_UNDEFINED_STRUCT,
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
  if (node->templateTypeLst()) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes())
      concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
  }

  // Get the struct instance
  Struct *spiceStruct = accessScope->matchStruct(currentScope, structName, concreteTemplateTypes, err.get(), node->codeLoc);
  if (!spiceStruct) {
    std::string structSignature = Struct::getSignature(structName, concreteTemplateTypes);
    throw err->get(node->codeLoc, REFERENCED_UNDEFINED_STRUCT, "Struct '" + structSignature + "' could not be found");
  }
  spiceStruct->setUsed();

  SymbolType structType;
  if (structIsImported) { // Imported struct
    structType =
        initExtStruct(accessScope, accessScopePrefix, node->structNameFragments.back(), concreteTemplateTypes, node->codeLoc);
  } else { // Not imported
    SymbolTableEntry *structSymbol =
        currentScope->lookup(accessScopePrefix + Struct::getSignature(structName, concreteTemplateTypes));
    if (!structSymbol)
      throw err->get(node->codeLoc, REFERENCED_UNDEFINED_STRUCT,
                     "Could not find struct '" + accessScopePrefix + structName + "'");
    structType = structSymbol->getType();
  }

  // Set template types to the struct
  std::vector<GenericType> genericTemplateTypes = spiceStruct->getTemplateTypes();
  std::vector<SymbolType> templateTypes;
  for (const auto &genericType : genericTemplateTypes)
    templateTypes.emplace_back(genericType.getTypeChain());
  structType.setTemplateTypes(templateTypes);

  // Check if the number of fields matches
  SymbolTable *structTable = currentScope->lookupTable(STRUCT_SCOPE_PREFIX + accessScopePrefix + structName);
  std::vector<SymbolType> fieldTypes;
  if (node->fieldLst()) { // Check if any fields are passed. Empty braces are also allowed
    if (spiceStruct->getFieldTypes().size() != node->fieldLst()->args().size())
      throw err->get(node->fieldLst()->codeLoc, NUMBER_OF_FIELDS_NOT_MATCHING,
                     "You've passed too less/many field values. Pass either none or all of them");

    // Check if the field types are matching
    for (int i = 0; i < node->fieldLst()->args().size(); i++) {
      // Get actual type
      auto assignExpr = node->fieldLst()->args()[i];
      auto actualType = any_cast<SymbolType>(visit(assignExpr));
      // Get expected type
      SymbolTableEntry *expectedField = structTable->lookupByIndex(i);
      assert(expectedField != nullptr);
      SymbolType expectedSymbolType = expectedField->getType();
      // Replace expected type with the capture name
      if (expectedSymbolType.is(TY_STRUCT))
        expectedSymbolType =
            expectedSymbolType.replaceBaseSubType(accessScopePrefix + expectedSymbolType.getBaseType().getSubType());
      // Check if type matches declaration
      if (actualType != expectedSymbolType)
        throw err->get(assignExpr->codeLoc, FIELD_TYPE_NOT_MATCHING,
                       "Expected type " + expectedSymbolType.getName(false) + " for the field '" + expectedField->getName() +
                           "', but got " + actualType.getName(false));
    }
  }

  return node->setEvaluatedSymbolType(structType);
}

std::any AnalyzerVisitor::visitDataType(DataTypeNode *node) {
  auto type = any_cast<SymbolType>(visit(node->baseDataType()));

  size_t assignExprCounter = 0;
  std::vector<AssignExprNode *> arraySizeExpr = node->arraySizeExpr();
  std::queue<DataTypeNode::TypeModifier> tmQueue = node->tmQueue;
  while (!tmQueue.empty()) {
    DataTypeNode::TypeModifier typeModifier = tmQueue.front();
    switch (typeModifier.modifierType) {
    case DataTypeNode::TYPE_PTR: {
      type = type.toPointer(err.get(), node->codeLoc);
      break;
    }
    case DataTypeNode::TYPE_ARRAY: {
      if (typeModifier.hasSize) {
        if (typeModifier.isSizeHardcoded) {
          if (typeModifier.hardcodedSize <= 1)
            throw err->get(node->codeLoc, ARRAY_SIZE_INVALID, "The size of an array must be > 1 and explicitly stated");
        } else {
          auto sizeType = any_cast<SymbolType>(visit(arraySizeExpr[assignExprCounter++]));
          if (!sizeType.isOneOf({TY_INT, TY_LONG, TY_SHORT}))
            throw err->get(node->codeLoc, ARRAY_SIZE_INVALID, "The array size must be of type int, long or short");
        }
      }
      type = type.toArray(err.get(), node->codeLoc, typeModifier.hardcodedSize);
      break;
    }
    default:
      throw std::runtime_error("Modifier type fall-through");
    }
    tmQueue.pop();
  }

  return node->setEvaluatedSymbolType(type);
}

std::any AnalyzerVisitor::visitBaseDataType(BaseDataTypeNode *node) {
  switch (node->type) {
  case BaseDataTypeNode::TYPE_DOUBLE:
    return node->setEvaluatedSymbolType(SymbolType(TY_DOUBLE));
  case BaseDataTypeNode::TYPE_INT:
    return node->setEvaluatedSymbolType(SymbolType(TY_INT));
  case BaseDataTypeNode::TYPE_SHORT:
    return node->setEvaluatedSymbolType(SymbolType(TY_SHORT));
  case BaseDataTypeNode::TYPE_LONG:
    return node->setEvaluatedSymbolType(SymbolType(TY_LONG));
  case BaseDataTypeNode::TYPE_BYTE:
    return node->setEvaluatedSymbolType(SymbolType(TY_BYTE));
  case BaseDataTypeNode::TYPE_CHAR:
    return node->setEvaluatedSymbolType(SymbolType(TY_CHAR));
  case BaseDataTypeNode::TYPE_STRING:
    return node->setEvaluatedSymbolType(SymbolType(TY_STRING));
  case BaseDataTypeNode::TYPE_BOOL:
    return node->setEvaluatedSymbolType(SymbolType(TY_BOOL));
  case BaseDataTypeNode::TY_CUSTOM:
    return node->setEvaluatedSymbolType(any_cast<SymbolType>(visit(node->customDataType())));
  default:
    return node->setEvaluatedSymbolType(SymbolType(TY_DYN));
  }
}

std::any AnalyzerVisitor::visitCustomDataType(CustomDataTypeNode *node) {
  // Check if it is a generic type
  std::string firstFragment = node->typeNameFragments.front();
  SymbolTableEntry *entry = currentScope->lookup(firstFragment);
  if (node->typeNameFragments.size() == 1 && !entry && currentScope->lookupGenericType(firstFragment)) {
    SymbolType symbolType = *static_cast<SymbolType *>(currentScope->lookupGenericType(firstFragment));
    return node->setEvaluatedSymbolType(symbolType);
  }

  // It is a struct type -> get the access scope
  SymbolTable *accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

  // Get type name in format: a.b.c and retrieve the scope in parallel
  std::string accessScopePrefix;
  std::string structName;
  bool structIsImported = false;
  for (unsigned int i = 0; i < node->typeNameFragments.size(); i++) {
    structName = node->typeNameFragments[i];
    if (i < node->typeNameFragments.size() - 1)
      accessScopePrefix += structName + ".";
    SymbolTableEntry *symbolEntry = accessScope->lookup(structName);
    if (!symbolEntry)
      throw err->get(node->codeLoc, UNKNOWN_DATATYPE, "Unknown symbol '" + structName + "'");
    if (!symbolEntry->getType().isOneOf({TY_STRUCT, TY_IMPORT}))
      throw err->get(node->codeLoc, EXPECTED_TYPE, "Expected type, but got " + symbolEntry->getType().getName());

    std::string tableName = symbolEntry->getType().is(TY_IMPORT) ? structName : STRUCT_SCOPE_PREFIX + structName;
    accessScope = accessScope->lookupTable(tableName);
    assert(accessScope != nullptr);
    if (accessScope->isImported(currentScope))
      structIsImported = true;
  }

  // Get the concrete template types
  std::vector<SymbolType> concreteTemplateTypes;
  if (node->templateTypeLst()) {
    for (const auto &dataType : node->templateTypeLst()->dataTypes())
      concreteTemplateTypes.push_back(any_cast<SymbolType>(visit(dataType)));
  }

  // Set the struct instance to used
  Struct *spiceStruct = accessScope->matchStruct(nullptr, structName, concreteTemplateTypes, err.get(), node->codeLoc);
  if (spiceStruct)
    spiceStruct->setUsed();

  if (structIsImported) { // Imported struct
    SymbolType symbolType = initExtStruct(accessScope, accessScopePrefix, structName, concreteTemplateTypes, node->codeLoc);
    return node->setEvaluatedSymbolType(symbolType);
  }

  // Check if struct was declared
  SymbolTableEntry *structSymbol = accessScope->lookup(structName);
  if (!structSymbol)
    throw err->get(node->codeLoc, UNKNOWN_DATATYPE, "Unknown datatype '" + structName + "'");
  structSymbol->setUsed();

  return node->setEvaluatedSymbolType(SymbolType(TY_STRUCT, structName, concreteTemplateTypes));
}

void AnalyzerVisitor::insertDestructorCall(const CodeLoc &codeLoc, SymbolTableEntry *varEntry) {
  assert(varEntry != nullptr && varEntry->getType().is(TY_STRUCT));

  // Create Spice function for destructor
  SymbolTableEntry *structEntry = currentScope->lookup(varEntry->getType().getName());
  SymbolTable *accessScope = structEntry->getScope();
  assert(accessScope != nullptr);
  accessScope = accessScope->getChild(STRUCT_SCOPE_PREFIX + structEntry->getName());
  assert(accessScope != nullptr);
  SymbolType thisType = varEntry->getType();
  accessScope->matchFunction(currentScope, DTOR_VARIABLE_NAME, thisType, {}, err.get(), codeLoc);
}

SymbolType AnalyzerVisitor::initExtStruct(SymbolTable *sourceScope, const std::string &structScopePrefix,
                                          const std::string &structName, const std::vector<SymbolType> &templateTypes,
                                          const CodeLoc &codeLoc) {
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
    throw err->get(codeLoc, REFERENCED_UNDEFINED_STRUCT, "Could not find struct '" + newStructName + "'");

  // Get the associated symbolTable of the external struct symbol
  SymbolTable *externalStructTable = sourceScope->lookupTable(STRUCT_SCOPE_PREFIX + structSignature);

  // Initialize potential structs for field types
  for (const auto &[_, entry] : externalStructTable->getSymbols()) {
    if (entry.getType().isBaseType(TY_STRUCT)) {
      std::string nestedStructName = CommonUtil::getLastFragment(entry.getType().getBaseType().getSubType(), ".");
      std::string nestedStructPrefix = CommonUtil::getPrefix(entry.getType().getBaseType().getSubType(), ".");
      // Initialize nested struct
      initExtStruct(sourceScope, nestedStructPrefix, nestedStructName, entry.getType().getBaseType().getTemplateTypes(), codeLoc);
    }
  }

  // Set to DECLARED, so that the generator can set it to DEFINED as soon as the LLVM struct type was generated once
  Capture newGlobalCapture(externalStructSymbol, newStructSignature, DECLARED);
  rootScope->addCapture(newStructSignature, newGlobalCapture);
  externalStructSymbol->setUsed();

  // Set the struct instance to used
  Struct *externalSpiceStruct = sourceScope->matchStruct(nullptr, structName, templateTypes, err.get(), codeLoc);
  assert(externalSpiceStruct);
  externalSpiceStruct->setUsed();

  // Mount the external struct table to the new position in the root scope of the current source file
  rootScope->mountChildBlock(STRUCT_SCOPE_PREFIX + newStructSignature, externalStructTable, false);
  rootScope->lookupTable(STRUCT_SCOPE_PREFIX + newStructSignature)->disableCompilerWarnings(); // Disable compiler warnings

  return newStructTy;
}

SymbolType AnalyzerVisitor::initExtGlobal(SymbolTable *sourceScope, const std::string &globalScopePrefix,
                                          const std::string &globalName, const CodeLoc &codeLoc) {
  // Get external global var name
  std::string newGlobalName = globalScopePrefix + globalName;

  // Check if the global var is imported already
  Capture *globalCapture = rootScope->lookupCapture(newGlobalName);
  if (globalCapture)
    return globalCapture->getEntry()->getType();

  // Check if external global var is declared
  SymbolTableEntry *externalGlobalSymbol = sourceScope->lookup(globalName);
  if (!externalGlobalSymbol)
    throw err->get(codeLoc, REFERENCED_UNDEFINED_VARIABLE, "Could not find global variable '" + newGlobalName + "'");

  // Set to DECLARED, so that the generator can set it to DEFINED as soon as the LLVM struct type was generated once
  Capture newGlobalCapture = Capture(externalGlobalSymbol, newGlobalName, DECLARED);
  rootScope->addCapture(newGlobalName, newGlobalCapture);
  externalGlobalSymbol->setUsed();

  return externalGlobalSymbol->getType();
}
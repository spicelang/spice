// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolTableBuilder.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>

SymbolTableBuilder::SymbolTableBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), rootScope(sourceFile->globalScope.get()) {}

STBResult SymbolTableBuilder::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;

  // Visit children
  visitChildren(node);

  // Reset the AST
  node->reset();

  // Check if the main function exists
  if (sourceFile->mainFile && !rootScope->lookup(std::string(MAIN_FUNCTION_NAME) + "()"))
    throw SemanticError(node, MISSING_MAIN_FUNCTION, "No main function found");
}

STBResult SymbolTableBuilder::visitMainFctDef(MainFctDefNode *node) {
  // Insert symbol for main function
  std::string mainSignature = std::string(MAIN_FUNCTION_NAME) + "()";
  SymbolTableEntry *mainFctEntry = currentScope->insert(mainSignature, SymbolSpecifiers::of(TY_FUNCTION), node);
  mainFctEntry->isUsed = true;

  // Create scope for main function body
  node->fctScope = currentScope = rootScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

  // Declare variable for the return value in the function scope
  SymbolTableEntry *resultVarEntry = node->fctScope->insert(RETURN_VARIABLE_NAME, SymbolSpecifiers::of(TY_INT), node);
  resultVarEntry->isUsed = true;

  // Visit arguments in new scope
  if (node->hasArgs)
    visit(node->paramLst());

  // Visit function body in new scope
  visit(node->stmtLst());

  // Return to root scope
  currentScope = rootScope;
}
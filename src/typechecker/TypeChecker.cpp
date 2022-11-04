// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;
  reVisitRequested = false;

  // Visit children
  ASTVisitor::visitChildren(node);

  // Reset the AST
  node->reset();

  // Check if the main function exists
  if (sourceFile->mainFile && !rootScope->lookup(std::string(MAIN_FUNCTION_NAME) + "()"))
    throw SemanticError(node, MISSING_MAIN_FUNCTION, "No main function found");

  return reVisitRequested;
}

std::any TypeChecker::visitMainFctDef(MainFctDefNode *node) {
  if (runNumber == 1) // First run
    return visitMainFctDef1(node);
  else if (runNumber == 2) // Second run
    return visitMainFctDef2(node);
  assert(false && "MainFctDef TypeChecker fall-through");
  return nullptr;
}

std::any TypeChecker::visitFctDef(FctDefNode *node) {
  if (runNumber == 1) // First run
    return visitFctDef1(node);
  else if (runNumber == 2) // Second run
    return visitFctDef2(node);
  assert(false && "FctDef TypeChecker fall-through");
  return nullptr;
}

std::any TypeChecker::visitProcDef(ProcDefNode *node) {
  if (runNumber == 1) // First run
    return visitProcDef1(node);
  else if (runNumber == 2) // Second run
    return visitProcDef2(node);
  assert(false && "ProcDef TypeChecker fall-through");
  return nullptr;
}

std::any TypeChecker::visitStructDef(StructDefNode *node) {
  if (runNumber == 1) // First run
    return visitStructDef1(node);
  else if (runNumber == 2) // Second run
    return visitStructDef2(node);
  assert(false && "StructDef TypeChecker fall-through");
  return nullptr;
}

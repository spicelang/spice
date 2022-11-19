// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolTableBuilder.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>

SymbolTableBuilder::SymbolTableBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), rootScope(sourceFile->globalScope.get()) {}

std::any SymbolTableBuilder::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;

  // Visit children
  visitChildren(node);

  // Reset the AST
  node->reset();

  // Check if the main function exists
  if (sourceFile->mainFile && !rootScope->lookup(std::string(MAIN_FUNCTION_NAME) + "()"))
    throw SemanticError(node, MISSING_MAIN_FUNCTION, "No main function found");

  return nullptr;
}

std::any SymbolTableBuilder::visitMainFctDef(MainFctDefNode *node) {
  const std::string fctSignature = std::string(MAIN_FUNCTION_NAME) + "()";

  // Check if the function is already defined
  if (rootScope->lookup(fctSignature))
    throw SemanticError(node, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

  // Insert symbol for main function
  SymbolTableEntry *mainFctEntry = currentScope->insert(fctSignature, SymbolSpecifiers::of(TY_FUNCTION), node);
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

  return nullptr;
}

std::any SymbolTableBuilder::visitFctDef(FctDefNode *node) {
  // Change to struct scope if this function is a method
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  // Create scope for the function
  node->fctScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

  // Create symbol for 'this' variable
  if (node->isMethod)
    currentScope->insert(THIS_VARIABLE_NAME, SymbolSpecifiers::of(TY_STRUCT), node);

  // Create symbol for 'result' variable
  currentScope->insert(RETURN_VARIABLE_NAME, SymbolSpecifiers(), node);

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit the function body
  visit(node->body());

  // Leave function body scope
  currentScope = node->fctScope->parent;

  // Build function specifiers
  auto specifiers = SymbolSpecifiers::of(TY_FUNCTION);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        specifiers.setInline(true);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
    }
  }

  // Insert symbol for function with a temporary name, depending on the code location
  // this has to be done due to overloading and because not having any types
  currentScope->insert(node->getTemporaryName(), specifiers, node);

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitProcDef(ProcDefNode *node) {
  // Change to struct scope if this procedure is a method
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  // Create scope for the procedure
  node->procScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY);

  // Create symbol for 'this' variable
  if (node->isMethod)
    currentScope->insert(THIS_VARIABLE_NAME, SymbolSpecifiers::of(TY_STRUCT), node);

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit the procedure body
  visit(node->body());

  // Leave procedure body scope
  currentScope = node->procScope->parent;

  // Build procedure specifiers
  auto specifiers = SymbolSpecifiers::of(TY_PROCEDURE);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        specifiers.setInline(true);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a procedure definition");
    }
  }

  // Insert symbol for procedure with a temporary name, depending on the code location
  // this has to be done due to overloading and because not having any types
  currentScope->insert(node->getTemporaryName(), specifiers, node);

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitStructDef(StructDefNode *node) {
  // Check if struct already exists
  if (rootScope->lookup(node->structName))
    throw SemanticError(node, STRUCT_DECLARED_TWICE, "Duplicate struct '" + node->structName + "'");

  // Create scope for the struct
  node->structScope = currentScope = rootScope->createChildScope(STRUCT_SCOPE_PREFIX + node->structName, SCOPE_STRUCT);

  // Visit struct fields
  for (FieldNode *field : node->fields())
    visit(field);

  // Leave the struct scope
  currentScope = node->structScope->parent;

  // Build struct specifiers
  auto specifiers = SymbolSpecifiers::of(TY_STRUCT);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a struct definition");
    }
  }

  // Add the struct to the symbol table
  rootScope->insert(node->structName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitInterfaceDef(InterfaceDefNode *node) {
  // Check if interface already exists
  if (rootScope->lookup(node->interfaceName))
    throw SemanticError(node, INTERFACE_DECLARED_TWICE, "Duplicate interface '" + node->interfaceName + "'");

  // Create scope for the interface
  node->interfaceScope = currentScope =
      rootScope->createChildScope(INTERFACE_SCOPE_PREFIX + node->interfaceName, SCOPE_INTERFACE);

  // Visit signatures
  for (SignatureNode *signature : node->signatures())
    visit(signature);

  // Leave the interface scope
  currentScope = node->interfaceScope->parent;

  // Build interface specifiers
  auto specifiers = SymbolSpecifiers::of(TY_INTERFACE);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an interface definition");
    }
  }

  // Add the interface to the symbol table
  rootScope->insert(node->interfaceName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitEnumDef(EnumDefNode *node) {
  // Check if enum already exists
  if (rootScope->lookup(node->enumName))
    throw SemanticError(node, ENUM_DECLARED_TWICE, "Duplicate interface '" + node->enumName + "'");

  // Create scope for the enum
  node->enumScope = currentScope = rootScope->createChildScope(ENUM_SCOPE_PREFIX + node->enumName, SCOPE_ENUM);

  // Visit items
  visit(node->itemLst());

  // Leave the enum scope
  currentScope = node->enumScope->parent;

  // Build enum specifiers
  auto specifiers = SymbolSpecifiers(TY_ENUM);
  if (node->specifierLst()) {
    for (const SpecifierNode *specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an enum definition");
    }
  }

  // Add the enum to the symbol table
  rootScope->insert(node->enumName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitGlobalVarDef(GlobalVarDefNode *node) {
  // Check if global already exists in this source file
  if (rootScope->lookup(node->varName))
    throw SemanticError(node, GLOBAL_DECLARED_TWICE, "Duplicate global variable '" + node->varName + "'");

  // Check if global already exists in an imported source file
  for (const auto &[name, dependency] : sourceFile->dependencies) {
    const SourceFile *importedSourceFile = dependency.first.get();
    if (importedSourceFile->exportedNameRegistry.contains(node->varName))
      throw SemanticError(node, GLOBAL_DECLARED_TWICE, "Duplicate global variable '" + node->varName + "' in other module");
  }

  // Create global specifiers
  SymbolSpecifiers specifiers;
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST)
        specifiers.setConst(true);
      else if (specifier->type == SpecifierNode::TY_SIGNED)
        specifiers.setSigned(true);
      else if (specifier->type == SpecifierNode::TY_UNSIGNED)
        specifiers.setSigned(false);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a global variable definition");
    }
  }

  // Add the global to the symbol table
  rootScope->insert(node->varName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitExtDecl(ExtDeclNode *node) {
  // Check if external declaration already exists
  if (rootScope->lookup(node->extFunctionName))
    throw SemanticError(node, FUNCTION_DECLARED_TWICE, "Duplicate external declaration of '" + node->extFunctionName + "'");

  // Build external declaration specifiers
  SymbolSpecifiers specifiers = SymbolSpecifiers::of(node->returnType() ? TY_FUNCTION : TY_PROCEDURE);

  // Add the external declaration to the symbol table
  rootScope->insert(node->extFunctionName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitThreadDef(ThreadDefNode *node) {
  // Create scope for the thread body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_THREAD_BODY);
  currentScope->symbolTable.capturingRequired = true; // Requires capturing because the LLVM IR will end up in a separate function

  // Visit body
  visit(node->body());

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Create scope for the unsafe block body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_UNSAFE_BODY);

  // Visit body
  visit(node->body());

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitForLoop(ForLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FOR_BODY);

  // Visit loop variable declaration
  visit(node->initDecl());

  // Visit body
  visit(node->body());

  // Leave for body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitForeachLoop(ForeachLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FOREACH_BODY);

  // Visit index variable declaration
  if (node->idxVarDecl()) {
    visit(node->idxVarDecl());
  } else {
    // Insert default index variable
    currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, SymbolSpecifiers(TY_INT), node);
  }

  // Visit item variable declaration
  visit(node->itemDecl());

  // Visit body
  visit(node->body());

  // Leave foreach body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitWhileLoop(WhileLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_WHILE_BODY);

  // Visit condition
  visit(node->condition());

  // Visit body
  visit(node->body());

  // Leave while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitIfStmt(IfStmtNode *node) {
  // Create scope for the then body
  node->thenBodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_IF_ELSE_BODY);

  // Visit condition
  visit(node->condition());

  // Visit then body
  visit(node->thenBody());

  // Leave then body scope
  currentScope = node->thenBodyScope->parent;

  // Visit else stmt
  if (node->elseStmt())
    visit(node->elseStmt());

  return nullptr;
}

std::any SymbolTableBuilder::visitElseStmt(ElseStmtNode *node) {
  if (node->isElseIf) {
    visit(node->ifStmt());
    return nullptr;
  }

  // Create scope for the else body
  node->elseBodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_IF_ELSE_BODY);

  // Visit else body
  visit(node->body());

  // Leave else body scope
  currentScope = node->elseBodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Create scope for the anonymous block body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_ANONYMOUS_BLOCK_BODY);

  // Visit body
  visit(node->body());

  // Leave anonymous block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitEnumItem(EnumItemNode *node) {
  // Check if enum item already exists in the same scope.
  if (currentScope->lookupStrict(node->itemName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The enum item '" + node->itemName + "' was declared more than once");

  // Add enum item entry to symbol table
  currentScope->insert(node->itemName, SymbolSpecifiers(TY_INT), node);

  return nullptr;
}

std::any SymbolTableBuilder::visitField(FieldNode *node) {
  // Check if field already exists in the same scope.
  if (currentScope->lookupStrict(node->fieldName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The field '" + node->fieldName + "' was declared more than once");

  // Build field specifiers
  SymbolSpecifiers specifiers;
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST)
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Struct fields cannot have the const specifier attached");
      else if (specifier->type == SpecifierNode::TY_SIGNED)
        specifiers.setSigned(true);
      else if (specifier->type == SpecifierNode::TY_UNSIGNED)
        specifiers.setSigned(false);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a field definition");
    }
  }

  // Add field entry to symbol table
  currentScope->insert(node->fieldName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitDeclStmt(DeclStmtNode *node) {
  // Check if variable already exists in the same scope.
  if (currentScope->lookupStrict(node->varName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The variable '" + node->varName + "' was declared more than once");

  // Visit the right side
  if (node->hasAssignment)
    visit(node->assignExpr());

  // Build variable specifiers
  SymbolSpecifiers specifiers;
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_CONST)
        specifiers.setConst(true);
      else if (specifier->type == SpecifierNode::TY_SIGNED)
        specifiers.setSigned(true);
      else if (specifier->type == SpecifierNode::TY_UNSIGNED)
        specifiers.setSigned(false);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a local variable declaration");
    }
  }

  // Add variable entry to symbol table
  currentScope->insert(node->varName, specifiers, node);

  return nullptr;
}
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "SymbolTableBuilder.h"

#include <SourceFile.h>
#include <ast/ASTBuilder.h>
#include <exception/SemanticError.h>

namespace spice::compiler {

SymbolTableBuilder::SymbolTableBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), rootScope(sourceFile->globalScope.get()) {}

std::any SymbolTableBuilder::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;

  // Visit children
  visitChildren(node);

  // Check if the main function exists
  if (sourceFile->mainFile && !hasMainFunction)
    throw SemanticError(node, MISSING_MAIN_FUNCTION, "No main function found");

  return nullptr;
}

std::any SymbolTableBuilder::visitMainFctDef(MainFctDefNode *node) {
  // Check if the function is already defined
  if (rootScope->lookup(node->getSignature()))
    throw SemanticError(node, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

  // Insert symbol for main function
  SymbolTableEntry *mainFctEntry = currentScope->insert(node->getSignature(), SymbolSpecifiers::of(TY_FUNCTION), node);
  mainFctEntry->used = true;

  // Create scope for main function body
  node->fctScope = currentScope = rootScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY, &node->codeLoc);
  currentScope->isGenericScope = false;

  // Declare variable for the return value in the function scope
  SymbolTableEntry *resultVarEntry = node->fctScope->insert(RETURN_VARIABLE_NAME, SymbolSpecifiers::of(TY_INT), node);
  resultVarEntry->used = true;

  // Visit arguments in new scope
  if (node->takesArgs)
    visit(node->paramLst());

  // Visit function body in new scope
  visit(node->body());

  // Return to root scope
  currentScope = rootScope;

  hasMainFunction = true;
  return nullptr;
}

std::any SymbolTableBuilder::visitFctDef(FctDefNode *node) {
  // Build function specifiers
  SymbolSpecifiers specifiers = SymbolSpecifiers::of(TY_FUNCTION);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        specifiers.setInline(true);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else if (specifier->type == SpecifierNode::TY_CONST)
        specifiers.setConst(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
    }
  }

  // Change to struct scope if this function is a method
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  // Create scope for the function
  node->fctScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY, &node->codeLoc);
  currentScope->isGenericScope = node->hasTemplateTypes || (node->structScope && node->structScope->isGenericScope);

  // Create symbol for 'this' variable
  if (node->isMethod) {
    SymbolSpecifiers thisVarSpecifiers = SymbolSpecifiers::of(TY_STRUCT);
    thisVarSpecifiers.setConst(specifiers.isConst()); // Make this 'const' if the function is 'const'
    currentScope->insert(THIS_VARIABLE_NAME, thisVarSpecifiers, node);
  }

  // Create symbol for 'result' variable
  currentScope->insert(RETURN_VARIABLE_NAME, SymbolSpecifiers(), node);

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit the function body
  visit(node->body());

  // Leave function body scope
  currentScope = node->fctScope->parent;

  // Insert symbol for function into the symbol table
  node->entry = currentScope->insert(node->getSymbolTableEntryName(), specifiers, node);

  // Add to external name registry
  // if a function has overloads, they both refer to the same entry in the registry. So we only register the name once
  if (!sourceFile->getNameRegistryEntry(node->fqFunctionName))
    sourceFile->addNameRegistryEntry(node->fqFunctionName, /*entry=*/nullptr, currentScope, /*keepNewOnCollision=*/true);

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitProcDef(ProcDefNode *node) {
  // Build procedure specifiers
  SymbolSpecifiers specifiers = SymbolSpecifiers::of(TY_PROCEDURE);
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        specifiers.setInline(true);
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        specifiers.setPublic(true);
      else if (specifier->type == SpecifierNode::TY_CONST)
        specifiers.setConst(true);
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a procedure definition");
    }
  }

  // Change to struct scope if this procedure is a method
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->structName + "' could not be found");
  }

  // Create scope for the procedure
  node->procScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FUNC_PROC_BODY, &node->codeLoc);
  currentScope->isGenericScope = node->hasTemplateTypes || (node->structScope && node->structScope->isGenericScope);

  // Create symbol for 'this' variable
  if (node->isMethod) {
    SymbolSpecifiers thisVarSpecifiers = SymbolSpecifiers::of(TY_STRUCT);
    thisVarSpecifiers.setConst(specifiers.isConst()); // Make this 'const' if the procedure is 'const'
    currentScope->insert(THIS_VARIABLE_NAME, thisVarSpecifiers, node);
  }

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit the procedure body
  visit(node->body());

  // Leave procedure body scope
  currentScope = node->procScope->parent;

  // Insert symbol for procedure into the symbol table
  node->entry = currentScope->insert(node->getSymbolTableEntryName(), specifiers, node);

  // Add to external name registry
  // if a procedure has overloads, they both refer to the same entry in the registry. So we only register the name once
  if (!sourceFile->getNameRegistryEntry(node->fqProcedureName))
    sourceFile->addNameRegistryEntry(node->fqProcedureName, /*entry=*/nullptr, currentScope, /*keepNewOnCollision=*/true);

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitStructDef(StructDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->structName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->structName + "'");

  // Create scope for the struct
  node->structScope = currentScope =
      rootScope->createChildScope(STRUCT_SCOPE_PREFIX + node->structName, SCOPE_STRUCT, &node->codeLoc);
  currentScope->isGenericScope = node->isGeneric;

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
  node->entry = rootScope->insert(node->structName, specifiers, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->structName, node->entry, node->structScope, /*keepNewOnCollision=*/true);

  return nullptr;
}

std::any SymbolTableBuilder::visitInterfaceDef(InterfaceDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->interfaceName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->interfaceName + "'");

  // Create scope for the interface
  node->interfaceScope = currentScope =
      rootScope->createChildScope(INTERFACE_SCOPE_PREFIX + node->interfaceName, SCOPE_INTERFACE, &node->codeLoc);

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
  node->entry = rootScope->insert(node->interfaceName, specifiers, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->interfaceName, node->entry, node->interfaceScope, /*keepNewOnCollision=*/true);

  return nullptr;
}

std::any SymbolTableBuilder::visitEnumDef(EnumDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->enumName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->enumName + "'");

  // Create scope for the enum
  node->enumScope = currentScope = rootScope->createChildScope(ENUM_SCOPE_PREFIX + node->enumName, SCOPE_ENUM, &node->codeLoc);

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
  node->entry = rootScope->insert(node->enumName, specifiers, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->enumName, node->entry, node->enumScope, /*keepNewOnCollision=*/true);

  return nullptr;
}

std::any SymbolTableBuilder::visitGenericTypeDef(GenericTypeDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->typeName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->typeName + "'");

  return nullptr;
}

std::any SymbolTableBuilder::visitAliasDef(AliasDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->aliasName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->aliasName + "'");

  // Add the alias to the symbol table
  const SymbolSpecifiers specifiers = SymbolSpecifiers::of(TY_ALIAS);
  node->entry = rootScope->insert(node->aliasName, specifiers, node);

  // Add another symbol for the aliased type container
  const std::string aliasedTypeContainerName = node->aliasName + ALIAS_CONTAINER_SUFFIX;
  node->aliasedTypeContainerEntry = rootScope->insert(aliasedTypeContainerName, specifiers, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitGlobalVarDef(GlobalVarDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->varName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->varName + "'");

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
  node->entry = rootScope->insert(node->varName, specifiers, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->varName, node->entry, currentScope, /*keepNewOnCollision=*/true);

  return nullptr;
}

std::any SymbolTableBuilder::visitExtDecl(ExtDeclNode *node) {
  // Check if this name already exists
  if (rootScope->lookup(node->extFunctionName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->extFunctionName + "'");

  // Build external declaration specifiers
  SymbolSpecifiers specifiers = SymbolSpecifiers::of(node->returnType() ? TY_FUNCTION : TY_PROCEDURE);

  // Add the external declaration to the symbol table
  node->entry = rootScope->insert(node->extFunctionName, specifiers, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->extFunctionName, node->entry, rootScope, /*keepNewOnCollision=*/true);

  return nullptr;
}

std::any SymbolTableBuilder::visitThreadDef(ThreadDefNode *node) {
  // Create scope for the thread body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_THREAD_BODY, &node->body()->codeLoc);
  currentScope->symbolTable.capturingRequired = true; // Requires capturing because the LLVM IR will end up in a separate function

  // Visit body
  visit(node->body());

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Create scope for the unsafe block body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_UNSAFE_BODY, &node->body()->codeLoc);

  // Visit body
  visit(node->body());

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitForLoop(ForLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FOR_BODY, &node->body()->codeLoc);

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
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_FOREACH_BODY, &node->body()->codeLoc);

  // Visit index variable declaration
  if (node->idxVarDecl()) {
    visit(node->idxVarDecl());
  } else {
    // Build default index variable specifiers
    SymbolSpecifiers specifiers(TY_INT);
    specifiers.setConst(true);

    // Add default index variable to symbol table
    currentScope->insert(FOREACH_DEFAULT_IDX_VARIABLE_NAME, specifiers, node);
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
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_WHILE_BODY, &node->body()->codeLoc);

  // Visit condition
  visit(node->condition());

  // Visit body
  visit(node->body());

  // Leave while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitDoWhileLoop(DoWhileLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), SCOPE_WHILE_BODY, &node->body()->codeLoc);

  // Visit condition
  visit(node->condition());

  // Visit body
  visit(node->body());

  // Leave do-while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitIfStmt(IfStmtNode *node) {
  // Create scope for the then body
  node->thenBodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), SCOPE_IF_ELSE_BODY, &node->thenBody()->codeLoc);

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
  // Visit if statement in the case of an else if branch
  if (node->isElseIf) {
    visit(node->ifStmt());
    return nullptr;
  }

  // Create scope for the else body
  node->elseBodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), SCOPE_IF_ELSE_BODY, &node->body()->codeLoc);

  // Visit else body
  visit(node->body());

  // Leave else body scope
  currentScope = node->elseBodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Create scope for the anonymous block body
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), SCOPE_ANONYMOUS_BLOCK_BODY, &node->body()->codeLoc);

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

  // Build enum item specifiers
  SymbolSpecifiers specifiers = SymbolSpecifiers::of(TY_INT);
  specifiers.setSigned(false); // Enum items are unsigned integers

  // Add enum item entry to symbol table
  SymbolTableEntry *enumItemEntry = currentScope->insert(node->itemName, specifiers, node);

  // Add external registry entry
  assert(node->enumDef != nullptr);
  sourceFile->addNameRegistryEntry(node->enumDef->enumName + SCOPE_ACCESS_TOKEN + node->itemName, enumItemEntry, currentScope,
                                   /*keepNewOnCollision=*/true);

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

} // namespace spice::compiler
// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "SymbolTableBuilder.h"

#include <SourceFile.h>
#include <ast/ASTBuilder.h>
#include <ast/Attributes.h>
#include <exception/SemanticError.h>
#include <global/GlobalResourceManager.h>

namespace spice::compiler {

SymbolTableBuilder::SymbolTableBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile), rootScope(sourceFile->globalScope.get()) {}

std::any SymbolTableBuilder::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;

  // Visit children
  visitChildren(node);

  // Check if the main function exists
  if (sourceFile->isMainFile && !cliOptions.noEntryFct && !hasMainFunction)
    throw SemanticError(node, MISSING_MAIN_FUNCTION, "No main function found", false);

  return nullptr;
}

std::any SymbolTableBuilder::visitMainFctDef(MainFctDefNode *node) {
  // Visit attributes
  if (node->attrs())
    visit(node->attrs());

  // Check if the function is already defined
  if (rootScope->lookupStrict(node->getSignature()))
    throw SemanticError(node, FUNCTION_DECLARED_TWICE, "Main function is declared twice");

  // Insert symbol for main function
  SymbolTableEntry *mainFctEntry = currentScope->insert(node->getSignature(), node);
  mainFctEntry->used = true;

  // Create scope for main function body
  const std::string scopeId = MainFctDefNode::getScopeId();
  node->bodyScope = currentScope = rootScope->createChildScope(scopeId, ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  currentScope->isGenericScope = false;

  // Declare variable for the return value in the function scope
  SymbolTableEntry *resultVarEntry = node->bodyScope->insert(RETURN_VARIABLE_NAME, node);
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
  // Visit attributes
  if (node->attrs())
    visit(node->attrs());

  // Build function specifiers
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        node->specifiers.isInline = true;
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->specifiers.isPublic = true;
      else if (specifier->type == SpecifierNode::TY_CONST)
        node->specifiers.isConst = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a function definition");
    }
  }

  // Change to struct scope if this function is a method
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->name->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->name->structName + "' could not be found");
  }

  // Create scope for the function
  node->scope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  currentScope->isGenericScope = node->hasTemplateTypes || (node->structScope && node->structScope->isGenericScope);

  // Create symbol for 'this' variable
  if (node->isMethod)
    currentScope->insert(THIS_VARIABLE_NAME, node);

  // Create symbol for 'result' variable
  currentScope->insert(RETURN_VARIABLE_NAME, node);

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit the function body
  visit(node->body());

  // Leave function body scope
  currentScope = node->scope->parent;

  // Insert symbol for function into the symbol table
  node->entry = currentScope->insert(node->getSymbolTableEntryName(), node);

  // Add to external name registry
  // if a function has overloads, they both refer to the same entry in the registry. So we only register the name once
  const NameRegistryEntry *existingRegistryEntry = sourceFile->getNameRegistryEntry(node->name->fqName);
  if (!existingRegistryEntry || existingRegistryEntry->targetEntry != node->entry)
    sourceFile->addNameRegistryEntry(node->name->fqName, TY_FUNCTION, node->entry, currentScope, true);

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitProcDef(ProcDefNode *node) {
  // Visit attributes
  if (node->attrs())
    visit(node->attrs());

  // Build procedure specifiers
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        node->specifiers.isInline = true;
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->specifiers.isPublic = true;
      else if (specifier->type == SpecifierNode::TY_CONST)
        node->specifiers.isConst = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a procedure definition");
    }
  }

  // Change to struct scope if this procedure is a method
  if (node->isMethod) {
    node->structScope = currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->name->structName);
    if (!currentScope)
      throw SemanticError(node, REFERENCED_UNDEFINED_STRUCT, "Struct '" + node->name->structName + "' could not be found");
  }

  // Create scope for the procedure
  node->scope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  currentScope->isGenericScope = node->hasTemplateTypes || (node->structScope && node->structScope->isGenericScope);
  currentScope->isDtorScope = node->isMethod && node->name->name == DTOR_FUNCTION_NAME;

  // Create symbol for 'this' variable
  if (node->isMethod)
    currentScope->insert(THIS_VARIABLE_NAME, node);

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit the procedure body
  visit(node->body());

  // Leave procedure body scope
  currentScope = node->scope->parent;

  // Insert symbol for procedure into the symbol table
  node->entry = currentScope->insert(node->getSymbolTableEntryName(), node);

  // Add to external name registry
  // if a procedure has overloads, they both refer to the same entry in the registry. So we only register the name once
  const NameRegistryEntry *existingRegistryEntry = sourceFile->getNameRegistryEntry(node->name->fqName);
  if (!existingRegistryEntry || existingRegistryEntry->targetEntry != node->entry)
    sourceFile->addNameRegistryEntry(node->name->fqName, TY_PROCEDURE, node->entry, currentScope, true);

  // Leave the struct scope
  if (node->isMethod)
    currentScope = node->structScope->parent;

  // Check if this is a constructor
  node->isCtor = node->name->nameFragments.back() == CTOR_FUNCTION_NAME;

  return nullptr;
}

std::any SymbolTableBuilder::visitStructDef(StructDefNode *node) {
  // Visit attributes
  if (node->attrs())
    visit(node->attrs());

  // Check if this name already exists
  if (rootScope->lookupStrict(node->structName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->structName + "'");

  // Create scope for the struct
  node->structScope = currentScope =
      rootScope->createChildScope(STRUCT_SCOPE_PREFIX + node->structName, ScopeType::STRUCT, &node->codeLoc);
  currentScope->isGenericScope = node->hasTemplateTypes;

  // Insert implicit field for each interface type
  if (node->hasInterfaces) {
    for (DataTypeNode *interfaceNode : node->interfaceTypeLst()->dataTypes) {
      const std::string &interfaceName = interfaceNode->baseDataType()->customDataType()->typeNameFragments.back();
      SymbolTableEntry *interfaceFieldEntry = currentScope->insert("this." + interfaceName, interfaceNode);
      interfaceFieldEntry->used = true;
      interfaceFieldEntry->isImplicitField = true;
    }
  }

  // Visit children
  visitChildren(node);

  // Leave the struct scope
  currentScope = node->structScope->parent;

  // Build struct specifiers
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->structSpecifiers.isPublic = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a struct definition");
    }
  }

  // Add the struct to the symbol table
  node->entry = rootScope->insert(node->structName, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->structName, node->typeId, node->entry, node->structScope, true);

  return nullptr;
}

std::any SymbolTableBuilder::visitInterfaceDef(InterfaceDefNode *node) {
  // Visit attributes
  if (node->attrs())
    visit(node->attrs());

  // Check if this name already exists
  if (rootScope->lookupStrict(node->interfaceName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->interfaceName + "'");

  // Create scope for the interface
  node->interfaceScope = currentScope =
      rootScope->createChildScope(INTERFACE_SCOPE_PREFIX + node->interfaceName, ScopeType::INTERFACE, &node->codeLoc);

  // Visit signatures
  for (SignatureNode *signature : node->signatures())
    visit(signature);

  // Leave the interface scope
  currentScope = node->interfaceScope->parent;

  // Build interface specifiers
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->interfaceSpecifiers.isPublic = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an interface definition");
    }
  }

  // Add the interface to the symbol table
  node->entry = rootScope->insert(node->interfaceName, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->interfaceName, node->typeId, node->entry, node->interfaceScope, true);

  return nullptr;
}

std::any SymbolTableBuilder::visitEnumDef(EnumDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookupStrict(node->enumName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->enumName + "'");

  // Create scope for the enum
  node->enumScope = currentScope =
      rootScope->createChildScope(ENUM_SCOPE_PREFIX + node->enumName, ScopeType::ENUM, &node->codeLoc);

  // Visit items
  visit(node->itemLst());

  // Leave the enum scope
  currentScope = node->enumScope->parent;

  // Build enum specifiers
  if (node->specifierLst()) {
    for (const SpecifierNode *specifier : node->specifierLst()->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->enumSpecifiers.isPublic = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an enum definition");
    }
  }

  // Add the enum to the symbol table
  node->entry = rootScope->insert(node->enumName, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->enumName, node->typeId, node->entry, node->enumScope, true);

  return nullptr;
}

std::any SymbolTableBuilder::visitGenericTypeDef(GenericTypeDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookupStrict(node->typeName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->typeName + "'");

  // Create the generic type to the symbol table
  node->entry = rootScope->insert(node->typeName, node);
  node->entry->used = true; // Generic types are always used

  return nullptr;
}

std::any SymbolTableBuilder::visitAliasDef(AliasDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookupStrict(node->aliasName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->aliasName + "'");

  // Build alias specifiers
  if (SpecifierLstNode *specifierLst = node->specifierLst(); specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->aliasSpecifiers.isPublic = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on an alias definition");
    }
  }

  // Add the alias to the symbol table
  node->entry = rootScope->insert(node->aliasName, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->aliasName, node->typeId, node->entry, rootScope, true);

  // Add another symbol for the aliased type container
  const std::string aliasedTypeContainerName = node->aliasName + ALIAS_CONTAINER_SUFFIX;
  node->aliasedTypeContainerEntry = rootScope->insert(aliasedTypeContainerName, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitGlobalVarDef(GlobalVarDefNode *node) {
  // Check if this name already exists
  if (rootScope->lookupStrict(node->varName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->varName + "'");

  // Check if global already exists in an imported source file
  for (const auto &[name, dependency] : sourceFile->dependencies)
    if (dependency->exportedNameRegistry.contains(node->varName))
      throw SemanticError(node, GLOBAL_DECLARED_TWICE, "Duplicate global variable '" + node->varName + "' in other module");

  // Add the global to the symbol table
  node->entry = rootScope->insert(node->varName, node);
  // Register the name in the exported name registry
  sourceFile->addNameRegistryEntry(node->varName, TY_INVALID, node->entry, currentScope, true);

  return nullptr;
}

std::any SymbolTableBuilder::visitExtDecl(ExtDeclNode *node) {
  // Visit attributes
  if (node->attrs())
    visit(node->attrs());

  // Check if this name already exists
  if (rootScope->lookupStrict(node->extFunctionName))
    throw SemanticError(node, DUPLICATE_SYMBOL, "Duplicate symbol '" + node->extFunctionName + "'");

  // Create scope for the external function (this is required in case of forceSubstantiation in FunctionManager::matchFunction)
  rootScope->createChildScope(node->getScopeId(), ScopeType::FUNC_PROC_BODY, &node->codeLoc);

  // Add the external declaration to the symbol table
  node->entry = rootScope->insert(node->extFunctionName, node);
  // Register the name in the exported name registry
  const uint64_t typeId = node->returnType() ? TY_FUNCTION : TY_PROCEDURE;
  sourceFile->addNameRegistryEntry(node->extFunctionName, typeId, node->entry, rootScope, /*keepNewOnCollision=*/true);

  return nullptr;
}

std::any SymbolTableBuilder::visitUnsafeBlock(UnsafeBlockNode *node) {
  // Create scope for the unsafe block body
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::UNSAFE_BODY, &node->body->codeLoc);

  // Visit body
  visit(node->body);

  // Leave thread body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitForLoop(ForLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::FOR_BODY, &node->body->codeLoc);

  // Visit loop variable declaration
  visit(node->initDecl);

  // Visit body
  visit(node->body);

  // Leave for body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitForeachLoop(ForeachLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::FOREACH_BODY, &node->body->codeLoc);

  // Visit index variable declaration
  if (node->idxVarDecl)
    visit(node->idxVarDecl);

  // Visit item variable declaration
  visit(node->itemVarDecl);

  // Visit body
  visit(node->body);

  // Leave foreach body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitWhileLoop(WhileLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::WHILE_BODY, &node->body->codeLoc);

  // Visit condition
  visit(node->condition);

  // Visit body
  visit(node->body);

  // Leave while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitDoWhileLoop(DoWhileLoopNode *node) {
  // Create scope for the loop body
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::WHILE_BODY, &node->body->codeLoc);

  // Visit body
  visit(node->body);

  // Visit condition
  visit(node->condition);

  // Leave do-while body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitIfStmt(IfStmtNode *node) {
  // Create scope for the then body
  node->thenBodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::IF_ELSE_BODY, &node->thenBody->codeLoc);

  // Visit condition
  visit(node->condition);

  // Visit then body
  visit(node->thenBody);

  // Leave then body scope
  currentScope = node->thenBodyScope->parent;

  // Visit else stmt
  if (node->elseStmt)
    visit(node->elseStmt);

  return nullptr;
}

std::any SymbolTableBuilder::visitElseStmt(ElseStmtNode *node) {
  // Visit if statement in the case of an else if branch
  if (node->isElseIf) {
    visit(node->ifStmt);
    return nullptr;
  }

  // Create scope for the else body
  node->elseBodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::IF_ELSE_BODY, &node->body->codeLoc);

  // Visit else body
  visit(node->body);

  // Leave else body scope
  currentScope = node->elseBodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitCaseBranch(CaseBranchNode *node) {
  // Create scope for the case branch
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::CASE_BODY, &node->body->codeLoc);

  // Visit case body
  visit(node->body);

  // Leave case body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitDefaultBranch(DefaultBranchNode *node) {
  // Create scope for the default branch
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::DEFAULT_BODY, &node->body->codeLoc);

  // Visit default body
  visit(node->body);

  // Leave default body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Create scope for the anonymous block body
  node->bodyScope = currentScope =
      currentScope->createChildScope(node->getScopeId(), ScopeType::ANONYMOUS_BLOCK_BODY, &node->body->codeLoc);

  // Visit body
  visit(node->body);

  // Leave anonymous block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitEnumItem(EnumItemNode *node) {
  // Check if enum item already exists in the same scope.
  if (currentScope->lookupStrict(node->itemName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The enum item '" + node->itemName + "' was declared more than once");

  // Add enum item entry to symbol table
  SymbolTableEntry *enumItemEntry = currentScope->insert(node->itemName, node);

  // Add external registry entry
  assert(node->enumDef != nullptr);
  const std::string name = node->enumDef->enumName + SCOPE_ACCESS_TOKEN + node->itemName;
  sourceFile->addNameRegistryEntry(name, TY_INT, enumItemEntry, currentScope, true);

  return nullptr;
}

std::any SymbolTableBuilder::visitField(FieldNode *node) {
  // Check if field already exists in the same scope.
  if (currentScope->lookupStrict(node->fieldName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The field '" + node->fieldName + "' was declared more than once");

  // Add field entry to symbol table
  currentScope->insert(node->fieldName, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitSignature(SignatureNode *node) {
  // Build signature specifiers
  if (SpecifierLstNode *specifierLst = node->specifierLst; specifierLst) {
    for (const SpecifierNode *specifier : specifierLst->specifiers()) {
      if (specifier->type == SpecifierNode::TY_INLINE)
        node->signatureSpecifiers.isInline = true;
      else if (specifier->type == SpecifierNode::TY_PUBLIC)
        node->signatureSpecifiers.isPublic = true;
      else if (specifier->type == SpecifierNode::TY_CONST)
        node->signatureSpecifiers.isConst = true;
      else
        throw SemanticError(specifier, SPECIFIER_AT_ILLEGAL_CONTEXT, "Cannot use this specifier on a signature definition");
    }
  }

  // Add signature entry to symbol table
  node->entry = currentScope->insert(node->methodName, node);

  return nullptr;
}

std::any SymbolTableBuilder::visitDeclStmt(DeclStmtNode *node) {
  // Check if variable already exists in the same scope.
  if (currentScope->lookupStrict(node->varName))
    throw SemanticError(node, VARIABLE_DECLARED_TWICE, "The variable '" + node->varName + "' was declared more than once");

  // Visit the right side
  if (node->hasAssignment)
    visit(node->assignExpr);

  // Add variable entry to symbol table
  SymbolTableEntry *varEntry = currentScope->insert(node->varName, node);
  varEntry->isParam = node->isParam;

  return nullptr;
}

std::any SymbolTableBuilder::visitModAttr(ModAttrNode *node) {
  // Visit attributes
  visitChildren(node);

  // Retrieve attributes
  const AttrLstNode *attrs = node->attrLst();

  // Collect linker flags
  std::vector<const CompileTimeValue *> linkerFlagValues;
  // core.linker.flag
  std::vector<const CompileTimeValue *> values = attrs->getAttrValuesByName(ATTR_CORE_LINKER_FLAG);
  linkerFlagValues.insert(linkerFlagValues.end(), values.begin(), values.end());
  // core.linux.linker.flag
  if (sourceFile->targetMachine->getTargetTriple().isOSLinux()) {
    values = attrs->getAttrValuesByName(ATTR_CORE_LINUX_LINKER_FLAG);
    linkerFlagValues.insert(linkerFlagValues.end(), values.begin(), values.end());
  }
  // core.windows.linker.flag
  if (sourceFile->targetMachine->getTargetTriple().isOSWindows()) {
    values = attrs->getAttrValuesByName(ATTR_CORE_WINDOWS_LINKER_FLAG);
    linkerFlagValues.insert(linkerFlagValues.end(), values.begin(), values.end());
  }
  for (const CompileTimeValue *value : linkerFlagValues)
    resourceManager.linker.addLinkerFlag(resourceManager.compileTimeStringValues.at(value->stringValueOffset));

  // core.linker.additional_source
  for (const CompileTimeValue *value : attrs->getAttrValuesByName(ATTR_CORE_LINKER_ADDITIONAL_SOURCE)) {
    const std::string &stringValue = resourceManager.compileTimeStringValues.at(value->stringValueOffset);
    const std::filesystem::path path = sourceFile->filePath.parent_path() / stringValue;
    resourceManager.linker.addAdditionalSourcePath(std::filesystem::canonical(path));
  }

  return nullptr;
}

std::any SymbolTableBuilder::visitAttr(AttrNode *node) {
  // Check if this attribute exists
  if (!ATTR_CONFIGS.contains(node->key))
    throw SemanticError(node, UNKNOWN_ATTR, "Unknown attribute '" + node->key + "'");

  // Check if the target is correct
  const AttrConfigValue &config = ATTR_CONFIGS.at(node->key);
  if ((node->target & config.target) == 0)
    throw SemanticError(node, INVALID_ATTR_TARGET, "Attribute '" + node->key + "' cannot be used on this target");

  // Check if a value is present
  if (!node->value() && config.type != AttrNode::TYPE_BOOL)
    throw SemanticError(node, MISSING_ATTR_VALUE, "Attribute '" + node->key + "' requires a value");

  return nullptr;
}

std::any SymbolTableBuilder::visitLambdaFunc(LambdaFuncNode *node) {
  // Create scope for the lambda body
  const CodeLoc &codeLoc = node->body()->codeLoc;
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::LAMBDA_BODY, &codeLoc);
  // Requires capturing because the LLVM IR will end up in a separate function
  currentScope->symbolTable.setCapturingRequired();
  // Set to async scope if this is an async lambda
  if (node->lambdaAttr() && node->lambdaAttr()->attrLst()->hasAttr(ATTR_ASYNC))
    node->bodyScope->isAsyncScope = node->lambdaAttr()->attrLst()->getAttrValueByName(ATTR_ASYNC)->boolValue;

  // Create symbol for 'result' variable
  currentScope->insert(RETURN_VARIABLE_NAME, node);

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit body
  visit(node->body());

  // Leave anonymous block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitLambdaProc(LambdaProcNode *node) {
  // Create scope for the lambda body
  const CodeLoc &codeLoc = node->body()->codeLoc;
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::LAMBDA_BODY, &codeLoc);
  // Requires capturing because the LLVM IR will end up in a separate function
  currentScope->symbolTable.setCapturingRequired();
  // Set to async scope if this is an async lambda
  if (node->lambdaAttr() && node->lambdaAttr()->attrLst()->hasAttr(ATTR_ASYNC))
    node->bodyScope->isAsyncScope = node->lambdaAttr()->attrLst()->getAttrValueByName(ATTR_ASYNC)->boolValue;

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit body
  visit(node->body());

  // Leave anonymous block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

std::any SymbolTableBuilder::visitLambdaExpr(LambdaExprNode *node) {
  // Create scope for the anonymous block body
  const CodeLoc &codeLoc = node->lambdaExpr()->codeLoc;
  node->bodyScope = currentScope = currentScope->createChildScope(node->getScopeId(), ScopeType::LAMBDA_BODY, &codeLoc);
  // Requires capturing because the LLVM IR will end up in a separate function
  currentScope->symbolTable.setCapturingRequired();

  // Create symbols for the parameters
  if (node->hasParams)
    visit(node->paramLst());

  // Visit lambda expression
  visit(node->lambdaExpr());

  // Leave anonymous block body scope
  currentScope = node->bodyScope->parent;

  return nullptr;
}

} // namespace spice::compiler
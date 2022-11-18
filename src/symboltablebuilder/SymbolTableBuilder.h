// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

const char *const MAIN_FUNCTION_NAME = "main";
const char *const RETURN_VARIABLE_NAME = "result";
const char *const THIS_VARIABLE_NAME = "this";
const char *const FOREACH_DEFAULT_IDX_VARIABLE_NAME = "idx";
const char *const CTOR_FUNCTION_NAME = "ctor";
const char *const DTOR_FUNCTION_NAME = "dtor";
const char *const STRUCT_SCOPE_PREFIX = "struct:";
const char *const INTERFACE_SCOPE_PREFIX = "interface:";
const char *const ENUM_SCOPE_PREFIX = "enum:";
const char *const UNUSED_VARIABLE_NAME = "_";

/**
 * Jobs:
 * - Build symbol table
 */
class SymbolTableBuilder : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  SymbolTableBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitFctDef(FctDefNode *node) override;
  /*STBResult visitProcDef(ProcDefNode *node) override;
  STBResult visitStructDef(StructDefNode *node) override;
  STBResult visitInterfaceDef(InterfaceDefNode *node) override;
  STBResult visitEnumDef(EnumDefNode *node) override;
  STBResult visitGlobalVarDef(GlobalVarDefNode *node) override;
  STBResult visitExtDecl(ExtDeclNode *node) override;*/

private:
  // Private members
  Scope *rootScope;
  Scope *currentScope = nullptr;
};
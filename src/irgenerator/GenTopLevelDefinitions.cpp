// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

std::any IRGenerator::visitMainFctDef(const MainFctDefNode *node) { return nullptr; }

std::any IRGenerator::visitFctDef(const FctDefNode *node) { return ParallelizableASTVisitor::visitFctDef(node); }

std::any IRGenerator::visitProcDef(const ProcDefNode *node) { return ParallelizableASTVisitor::visitProcDef(node); }

std::any IRGenerator::visitStructDef(const StructDefNode *node) { return ParallelizableASTVisitor::visitStructDef(node); }

std::any IRGenerator::visitInterfaceDef(const InterfaceDefNode *node) {
  return nullptr; // Noop (interfaces are high-level semantic-only structures)
}

std::any IRGenerator::visitEnumDef(const EnumDefNode *node) {
  return nullptr; // Noop (enums are high-level semantic-only structures)
}

std::any IRGenerator::visitGenericTypeDef(const GenericTypeDefNode *node) {
  return ParallelizableASTVisitor::visitGenericTypeDef(node);
}

std::any IRGenerator::visitGlobalVarDef(const GlobalVarDefNode *node) {
  return ParallelizableASTVisitor::visitGlobalVarDef(node);
}

std::any IRGenerator::visitExtDecl(const ExtDeclNode *node) { return ParallelizableASTVisitor::visitExtDecl(node); }
// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <exception/ParserError.h>
#include <typechecker/OpRuleManager.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

ASTBuilder::ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, antlr4::ANTLRInputStream *inputStream)
    : CompilerPass(resourceManager, sourceFile), inputStream(inputStream) {}

std::any ASTBuilder::visitEntry(SpiceParser::EntryContext *ctx) {
  const auto entryNode = createNode<EntryNode>(ctx);

  // Visit children
  for (ParserRuleContext::ParseTree *child : ctx->children) {
    if (auto *mainFctDefCtx = dynamic_cast<SpiceParser::MainFunctionDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<MainFctDefNode *>(visit(mainFctDefCtx)));
    else if (auto *fctDefCtx = dynamic_cast<SpiceParser::FunctionDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<FctDefNode *>(visit(fctDefCtx)));
    else if (auto *procDefCtx = dynamic_cast<SpiceParser::ProcedureDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<ProcDefNode *>(visit(procDefCtx)));
    else if (auto *structDefCtx = dynamic_cast<SpiceParser::StructDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<StructDefNode *>(visit(structDefCtx)));
    else if (auto *interfaceDefCtx = dynamic_cast<SpiceParser::InterfaceDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<InterfaceDefNode *>(visit(interfaceDefCtx)));
    else if (auto *enumDefCtx = dynamic_cast<SpiceParser::EnumDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<EnumDefNode *>(visit(enumDefCtx)));
    else if (auto *genericTypeDefCtx = dynamic_cast<SpiceParser::GenericTypeDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<GenericTypeDefNode *>(visit(genericTypeDefCtx)));
    else if (auto *aliasDefCtx = dynamic_cast<SpiceParser::AliasDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<AliasDefNode *>(visit(aliasDefCtx)));
    else if (auto *globalVarDefCtx = dynamic_cast<SpiceParser::GlobalVarDefContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<GlobalVarDefNode *>(visit(globalVarDefCtx)));
    else if (auto *importDefCtx = dynamic_cast<SpiceParser::ImportDefContext *>(child))
      entryNode->importDefs.push_back(std::any_cast<ImportDefNode *>(visit(importDefCtx)));
    else if (auto *extDeclCtx = dynamic_cast<SpiceParser::ExtDeclContext *>(child))
      entryNode->topLevelDefs.push_back(std::any_cast<ExtDeclNode *>(visit(extDeclCtx)));
    else if (auto *modAttrCtx = dynamic_cast<SpiceParser::ModAttrContext *>(child))
      entryNode->modAttrs.push_back(std::any_cast<ModAttrNode *>(visit(modAttrCtx)));
    else if (const auto *eofCtx = dynamic_cast<TerminalNode *>(child);
             !eofCtx || eofCtx->getSymbol()->getType() != SpiceParser::EOF)
      assert_fail("Unknown top level definition type"); // GCOV_EXCL_LINE
  }

  return concludeNode(entryNode);
}

std::any ASTBuilder::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  const auto mainFctDefNode = createNode<MainFctDefNode>(ctx);

  // Visit children
  if (ctx->topLevelDefAttr())
    mainFctDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
  if (ctx->paramLst()) {
    mainFctDefNode->takesArgs = true;
    mainFctDefNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  mainFctDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(mainFctDefNode);
}

std::any ASTBuilder::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  const auto fctDefNode = createNode<FctDefNode>(ctx);

  // Visit children
  if (ctx->topLevelDefAttr()) {
    fctDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are function attributes
    for (AttrNode *attr : fctDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }
  if (ctx->qualifierLst())
    fctDefNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  fctDefNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  fctDefNode->returnType->isReturnType = true;
  fctDefNode->name = std::any_cast<FctNameNode *>(visit(ctx->fctName()));
  fctDefNode->isMethod = fctDefNode->name->nameFragments.size() > 1;
  if (ctx->typeLst()) {
    fctDefNode->hasTemplateTypes = true;
    fctDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  if (ctx->paramLst()) {
    fctDefNode->hasParams = true;
    fctDefNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  fctDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(fctDefNode);
}

std::any ASTBuilder::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  const auto procDefNode = createNode<ProcDefNode>(ctx);

  // Visit children
  if (ctx->topLevelDefAttr()) {
    procDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are function attributes
    for (AttrNode *attr : procDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }
  if (ctx->qualifierLst())
    procDefNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  procDefNode->name = std::any_cast<FctNameNode *>(visit(ctx->fctName()));
  procDefNode->isMethod = procDefNode->name->nameFragments.size() > 1;
  if (ctx->typeLst()) {
    procDefNode->hasTemplateTypes = true;
    procDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  if (ctx->paramLst()) {
    procDefNode->hasParams = true;
    procDefNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  procDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(procDefNode);
}

std::any ASTBuilder::visitFctName(SpiceParser::FctNameContext *ctx) {
  const auto fctNameNode = createNode<FctNameNode>(ctx);

  // Extract function name
  if (ctx->TYPE_IDENTIFIER()) {
    const std::string typeIdentifier = getIdentifier(ctx->TYPE_IDENTIFIER());
    fctNameNode->structName = typeIdentifier;
    fctNameNode->fqName = typeIdentifier + MEMBER_ACCESS_TOKEN;
    fctNameNode->nameFragments.push_back(typeIdentifier);
  }
  if (ctx->IDENTIFIER()) {
    const std::string fctIdentifier = getIdentifier(ctx->IDENTIFIER());
    fctNameNode->name = fctIdentifier;
    fctNameNode->fqName += fctIdentifier;
    fctNameNode->nameFragments.push_back(fctIdentifier);
  }

  // Visit children
  if (ctx->overloadableOp())
    visit(ctx->overloadableOp());

  return concludeNode(fctNameNode);
}

std::any ASTBuilder::visitStructDef(SpiceParser::StructDefContext *ctx) {
  const auto structDefNode = createNode<StructDefNode>(ctx);

  // Enrich
  structDefNode->structName = getIdentifier(ctx->TYPE_IDENTIFIER());
  structDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  if (ctx->topLevelDefAttr()) {
    structDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));

    // Tell the attributes that they are struct attributes
    for (AttrNode *attr : structDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_STRUCT;

    // Check if a custom type id was set
    if (structDefNode->attrs && structDefNode->attrs->attrLst->hasAttr(ATTR_CORE_COMPILER_FIXED_TYPE_ID))
      structDefNode->typeId = structDefNode->attrs->attrLst->getAttrValueByName(ATTR_CORE_COMPILER_FIXED_TYPE_ID)->intValue;
  }
  if (ctx->qualifierLst())
    structDefNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  if (ctx->LESS()) {
    structDefNode->hasTemplateTypes = true;
    structDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(0)));
  }
  if (ctx->COLON()) {
    structDefNode->hasInterfaces = true;
    structDefNode->interfaceTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(structDefNode->hasTemplateTypes ? 1 : 0)));
  }
  for (SpiceParser::FieldContext *field : ctx->field())
    structDefNode->fields.push_back(std::any_cast<FieldNode *>(visit(field)));

  return concludeNode(structDefNode);
}

std::any ASTBuilder::visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) {
  const auto interfaceDefNode = createNode<InterfaceDefNode>(ctx);

  // Enrich
  interfaceDefNode->interfaceName = getIdentifier(ctx->TYPE_IDENTIFIER());
  interfaceDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  if (ctx->topLevelDefAttr()) {
    interfaceDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));

    // Tell the attributes that they are struct attributes
    for (AttrNode *attr : interfaceDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_INTERFACE;

    // Check if a custom type id was set
    if (interfaceDefNode->attrs && interfaceDefNode->attrs->attrLst->hasAttr(ATTR_CORE_COMPILER_FIXED_TYPE_ID))
      interfaceDefNode->typeId = interfaceDefNode->attrs->attrLst->getAttrValueByName(ATTR_CORE_COMPILER_FIXED_TYPE_ID)->intValue;
  }
  if (ctx->qualifierLst())
    interfaceDefNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  if (ctx->LESS()) {
    interfaceDefNode->hasTemplateTypes = true;
    interfaceDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  for (SpiceParser::SignatureContext *signature : ctx->signature())
    interfaceDefNode->signatures.push_back(std::any_cast<SignatureNode *>(visit(signature)));

  return concludeNode(interfaceDefNode);
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  const auto enumDefNode = createNode<EnumDefNode>(ctx);

  // Enrich
  enumDefNode->enumName = getIdentifier(ctx->TYPE_IDENTIFIER());
  enumDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  if (ctx->qualifierLst())
    enumDefNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  enumDefNode->itemLst = std::any_cast<EnumItemLstNode *>(visit(ctx->enumItemLst()));

  // Tell all items about the enum def
  for (EnumItemNode *enumItem : enumDefNode->itemLst->items)
    enumItem->enumDef = enumDefNode;

  return concludeNode(enumDefNode);
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  const auto genericTypeDefNode = createNode<GenericTypeDefNode>(ctx);

  // Enrich
  genericTypeDefNode->typeName = getIdentifier(ctx->TYPE_IDENTIFIER());

  // Visit children
  genericTypeDefNode->typeAltsLst = std::any_cast<TypeAltsLstNode *>(visit(ctx->typeAltsLst()));

  return concludeNode(genericTypeDefNode);
}

std::any ASTBuilder::visitAliasDef(SpiceParser::AliasDefContext *ctx) {
  const auto aliasDefNode = createNode<AliasDefNode>(ctx);

  // Enrich
  aliasDefNode->aliasName = getIdentifier(ctx->TYPE_IDENTIFIER());
  aliasDefNode->dataTypeString = ctx->dataType()->getText();
  aliasDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  if (ctx->qualifierLst())
    aliasDefNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  aliasDefNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  return concludeNode(aliasDefNode);
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  const auto globalVarDefNode = createNode<GlobalVarDefNode>(ctx);

  // Enrich
  globalVarDefNode->varName = getIdentifier(ctx->TYPE_IDENTIFIER());

  // Visit children
  globalVarDefNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  globalVarDefNode->dataType->isGlobalType = true;
  if (ctx->constant()) {
    globalVarDefNode->hasValue = true;
    globalVarDefNode->constant = std::any_cast<ConstantNode *>(visit(ctx->constant()));
  }

  return concludeNode(globalVarDefNode);
}

std::any ASTBuilder::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  const auto extDeclNode = createNode<ExtDeclNode>(ctx);

  // Enrich
  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER() ? ctx->IDENTIFIER() : ctx->TYPE_IDENTIFIER());

  // Visit children
  if (ctx->topLevelDefAttr()) {
    extDeclNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));

    // Tell the attributes that they are ext decl attributes
    for (AttrNode *attr : extDeclNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_EXT_DECL;
  }
  if (ctx->F()) {
    extDeclNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
    extDeclNode->returnType->isReturnType = true;
  }
  if (ctx->typeLst()) {
    extDeclNode->hasArgs = true;
    extDeclNode->argTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  extDeclNode->isVarArg = ctx->ELLIPSIS();

  return concludeNode(extDeclNode);
}

std::any ASTBuilder::visitImportDef(SpiceParser::ImportDefContext *ctx) {
  const auto importDefNode = createNode<ImportDefNode>(ctx);

  // Extract path
  const std::string pathStr = ctx->STRING_LIT()->getText();
  importDefNode->importPath = pathStr.substr(1, pathStr.size() - 2);

  // If no name is given, use the path as name
  importDefNode->importName = ctx->AS() ? getIdentifier(ctx->IDENTIFIER()) : importDefNode->importPath;

  return concludeNode(importDefNode);
}

std::any ASTBuilder::visitUnsafeBlock(SpiceParser::UnsafeBlockContext *ctx) {
  const auto unsafeBlockDefNode = createNode<UnsafeBlockNode>(ctx);

  // Visit children
  unsafeBlockDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(unsafeBlockDefNode);
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  const auto forLoopNode = createNode<ForLoopNode>(ctx);

  visit(ctx->forHead());
  forLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(forLoopNode);
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  const auto forLoopNode = resumeForExpansion<ForLoopNode>();

  // Visit children
  forLoopNode->initDecl = std::any_cast<DeclStmtNode *>(visit(ctx->declStmt()));
  forLoopNode->condAssign = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr(0)));
  forLoopNode->incAssign = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr(1)));

  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  const auto foreachLoopNode = createNode<ForeachLoopNode>(ctx);

  // Visit children
  visit(ctx->foreachHead());
  foreachLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  // Tell the foreach item that it is one
  foreachLoopNode->itemVarDecl->isForEachItem = true;

  return concludeNode(foreachLoopNode);
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  const auto foreachLoopNode = resumeForExpansion<ForeachLoopNode>();

  // Visit children
  if (ctx->declStmt().size() == 1) {
    foreachLoopNode->itemVarDecl = std::any_cast<DeclStmtNode *>(visit(ctx->declStmt(0)));
  } else if (ctx->declStmt().size() == 2) {
    foreachLoopNode->idxVarDecl = std::any_cast<DeclStmtNode *>(visit(ctx->declStmt(0)));
    foreachLoopNode->itemVarDecl = std::any_cast<DeclStmtNode *>(visit(ctx->declStmt(1)));
  } else {
    assert_fail("Invalid number of decl statements in foreach loop"); // GCOV_EXCL_LINE
  }
  foreachLoopNode->iteratorAssign = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return nullptr;
}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  const auto whileLoopNode = createNode<WhileLoopNode>(ctx);

  // Visit children
  whileLoopNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  whileLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(whileLoopNode);
}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {
  const auto doWhileLoopNode = createNode<DoWhileLoopNode>(ctx);

  // Visit children
  doWhileLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  doWhileLoopNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(doWhileLoopNode);
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  const auto ifStmtNode = createNode<IfStmtNode>(ctx);

  // Visit children
  ifStmtNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  ifStmtNode->thenBody = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  if (ctx->elseStmt())
    ifStmtNode->elseStmt = std::any_cast<ElseStmtNode *>(visit(ctx->elseStmt()));

  return concludeNode(ifStmtNode);
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  const auto elseStmtNode = createNode<ElseStmtNode>(ctx);

  // Visit children
  if (ctx->ifStmt()) {
    elseStmtNode->isElseIf = true;
    elseStmtNode->ifStmt = std::any_cast<IfStmtNode *>(visit(ctx->ifStmt()));
  } else {
    elseStmtNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  }

  return concludeNode(elseStmtNode);
}

std::any ASTBuilder::visitSwitchStmt(SpiceParser::SwitchStmtContext *ctx) {
  const auto switchStmtNode = createNode<SwitchStmtNode>(ctx);

  // Visit children
  switchStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  fetchChildrenIntoVector(switchStmtNode->caseBranches, ctx->caseBranch());
  if (ctx->defaultBranch()) {
    switchStmtNode->hasDefaultBranch = true;
    switchStmtNode->defaultBranch = std::any_cast<DefaultBranchNode *>(visit(ctx->defaultBranch()));
  }

  return concludeNode(switchStmtNode);
}

std::any ASTBuilder::visitCaseBranch(SpiceParser::CaseBranchContext *ctx) {
  const auto caseBranchNode = createNode<CaseBranchNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(caseBranchNode->caseConstants, ctx->caseConstant());
  caseBranchNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(caseBranchNode);
}

std::any ASTBuilder::visitDefaultBranch(SpiceParser::DefaultBranchContext *ctx) {
  const auto defaultBranchNode = createNode<DefaultBranchNode>(ctx);

  // Visit children
  defaultBranchNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(defaultBranchNode);
}

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  const auto anonymousBlockStmtNode = createNode<AnonymousBlockStmtNode>(ctx);

  // Visit children
  anonymousBlockStmtNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(anonymousBlockStmtNode);
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  const auto stmtLstNode = createNode<StmtLstNode>(ctx);

  // Enrich
  stmtLstNode->closingBraceCodeLoc = CodeLoc(ctx->getStop(), sourceFile);

  // Visit children
  for (ParserRuleContext::ParseTree *stmt : ctx->children) {
    if (auto *stmtCtx = dynamic_cast<SpiceParser::StmtContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<StmtNode *>(visit(stmtCtx)));
    else if (auto *forLoopCtx = dynamic_cast<SpiceParser::ForLoopContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<ForLoopNode *>(visit(forLoopCtx)));
    else if (auto *foreachLoopCtx = dynamic_cast<SpiceParser::ForeachLoopContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<ForeachLoopNode *>(visit(foreachLoopCtx)));
    else if (auto *whileLoopCtx = dynamic_cast<SpiceParser::WhileLoopContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<WhileLoopNode *>(visit(whileLoopCtx)));
    else if (auto *doWhileLoopCtx = dynamic_cast<SpiceParser::DoWhileLoopContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<DoWhileLoopNode *>(visit(doWhileLoopCtx)));
    else if (auto *ifStmtCtx = dynamic_cast<SpiceParser::IfStmtContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<IfStmtNode *>(visit(ifStmtCtx)));
    else if (auto *switchStmtCtx = dynamic_cast<SpiceParser::SwitchStmtContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<SwitchStmtNode *>(visit(switchStmtCtx)));
    else if (auto *assetStmtCtx = dynamic_cast<SpiceParser::AssertStmtContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<AssertStmtNode *>(visit(assetStmtCtx)));
    else if (auto *unsafeBlockCtx = dynamic_cast<SpiceParser::UnsafeBlockContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<UnsafeBlockNode *>(visit(unsafeBlockCtx)));
    else if (auto *anonymousScopeCtx = dynamic_cast<SpiceParser::AnonymousBlockStmtContext *>(stmt))
      stmtLstNode->statements.push_back(std::any_cast<AnonymousBlockStmtNode *>(visit(anonymousScopeCtx)));
    else
      assert(dynamic_cast<TerminalNode *>(stmt) != nullptr); // GCOV_EXCL_LINE
  }

  return concludeNode(stmtLstNode);
}

std::any ASTBuilder::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  const auto typeLstNode = createNode<TypeLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(typeLstNode->dataTypes, ctx->dataType());

  return concludeNode(typeLstNode);
}

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  const auto typeAltsLstNode = createNode<TypeAltsLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(typeAltsLstNode->dataTypes, ctx->dataType());

  return concludeNode(typeAltsLstNode);
}

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  const auto paramLstNode = createNode<ParamLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(paramLstNode->params, ctx->declStmt());

  // Set some flags to later detect that the decl statements are parameters
  for (DeclStmtNode *declStmt : paramLstNode->params) {
    declStmt->isFctParam = true;
    declStmt->dataType->isParamType = true;
  }

  return concludeNode(paramLstNode);
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  const auto argLstNode = createNode<ArgLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(argLstNode->args, ctx->assignExpr());
  argLstNode->argInfos.reserve(argLstNode->args.size());

  return concludeNode(argLstNode);
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  const auto enumItemLstNode = createNode<EnumItemLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(enumItemLstNode->items, ctx->enumItem());

  return concludeNode(enumItemLstNode);
}

std::any ASTBuilder::visitEnumItem(SpiceParser::EnumItemContext *ctx) {
  const auto enumItemNode = createNode<EnumItemNode>(ctx);

  // Enrich
  enumItemNode->itemName = getIdentifier(ctx->TYPE_IDENTIFIER());
  if (ctx->ASSIGN()) {
    enumItemNode->itemValue = parseInt(ctx->INT_LIT());
    enumItemNode->hasValue = true;
  }

  return concludeNode(enumItemNode);
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  const auto fieldNode = createNode<FieldNode>(ctx);

  // Enrich
  fieldNode->fieldName = getIdentifier(ctx->IDENTIFIER());

  // Visit children
  fieldNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  fieldNode->dataType->setFieldTypeRecursive();
  if (ctx->ternaryExpr())
    fieldNode->defaultValue = std::any_cast<TernaryExprNode *>(visit(ctx->ternaryExpr()));

  return concludeNode(fieldNode);
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {
  const auto signatureNode = createNode<SignatureNode>(ctx);

  // Extract method name
  signatureNode->methodName = getIdentifier(ctx->IDENTIFIER());

  // Visit children
  if (ctx->qualifierLst()) {
    signatureNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  }
  if (ctx->F()) {
    signatureNode->hasReturnType = true;
    signatureNode->signatureType = SignatureNode::SignatureType::TYPE_FUNCTION;
    signatureNode->signatureQualifiers = TypeQualifiers::of(TY_FUNCTION);
    signatureNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  } else {
    signatureNode->signatureType = SignatureNode::SignatureType::TYPE_PROCEDURE;
    signatureNode->signatureQualifiers = TypeQualifiers::of(TY_PROCEDURE);
  }
  if (ctx->F() ? ctx->LESS().size() == 2 : ctx->LESS().size() == 1) {
    signatureNode->hasTemplateTypes = true;
    signatureNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(0)));
  }
  if (ctx->typeLst().size() == 2 || (ctx->typeLst().size() == 1 && !signatureNode->hasTemplateTypes)) {
    signatureNode->hasParams = true;
    signatureNode->paramTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(signatureNode->hasTemplateTypes ? 1 : 0)));
  }

  return concludeNode(signatureNode);
}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) {
  if (ctx->declStmt())
    return static_cast<StmtNode *>(std::any_cast<DeclStmtNode *>(visit(ctx->declStmt())));
  if (ctx->exprStmt())
    return static_cast<StmtNode *>(std::any_cast<ExprStmtNode *>(visit(ctx->exprStmt())));
  if (ctx->returnStmt())
    return static_cast<StmtNode *>(std::any_cast<ReturnStmtNode *>(visit(ctx->returnStmt())));
  if (ctx->breakStmt())
    return static_cast<StmtNode *>(std::any_cast<BreakStmtNode *>(visit(ctx->breakStmt())));
  if (ctx->continueStmt())
    return static_cast<StmtNode *>(std::any_cast<ContinueStmtNode *>(visit(ctx->continueStmt())));
  if (ctx->fallthroughStmt())
    return static_cast<StmtNode *>(std::any_cast<FallthroughStmtNode *>(visit(ctx->fallthroughStmt())));
  assert_fail("Unknown statement type"); // GCOV_EXCL_LINE
  return nullptr;                        // GCOV_EXCL_LINE
}

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  const auto declStmtNode = createNode<DeclStmtNode>(ctx);

  // Enrich
  declStmtNode->varName = getIdentifier(ctx->IDENTIFIER());

  // Visit children
  declStmtNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  if (ctx->assignExpr()) {
    declStmtNode->hasAssignment = true;
    declStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  }

  return concludeNode(declStmtNode);
}

std::any ASTBuilder::visitExprStmt(SpiceParser::ExprStmtContext *ctx) {
  const auto exprStmtNode = createNode<ExprStmtNode>(ctx);

  // Enrich
  exprStmtNode->expr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(exprStmtNode);
}

std::any ASTBuilder::visitQualifierLst(SpiceParser::QualifierLstContext *ctx) {
  const auto qualifierLstNode = createNode<QualifierLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(qualifierLstNode->qualifiers, ctx->qualifier());

  // Check if qualifier combination is invalid
  bool seenSignedOrUnsigned = false;
  for (const QualifierNode *qualifier : qualifierLstNode->qualifiers) {
    // Check if we have both, signed and unsigned qualifier
    if (qualifier->type != QualifierNode::QualifierType::TY_SIGNED &&
        qualifier->type != QualifierNode::QualifierType::TY_UNSIGNED)
      continue;
    if (seenSignedOrUnsigned)
      throw ParserError(qualifier->codeLoc, INVALID_QUALIFIER_COMBINATION, "A variable can not be signed and unsigned");
    seenSignedOrUnsigned = true;
  }

  return concludeNode(qualifierLstNode);
}

std::any ASTBuilder::visitQualifier(SpiceParser::QualifierContext *ctx) {
  const auto qualifierNode = createNode<QualifierNode>(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto token = spice_pointer_cast<TerminalNode *>(subTree);
    const size_t symbolType = token->getSymbol()->getType();
    if (symbolType == SpiceParser::CONST)
      qualifierNode->type = QualifierNode::QualifierType::TY_CONST;
    else if (symbolType == SpiceParser::SIGNED)
      qualifierNode->type = QualifierNode::QualifierType::TY_SIGNED;
    else if (symbolType == SpiceParser::UNSIGNED)
      qualifierNode->type = QualifierNode::QualifierType::TY_UNSIGNED;
    else if (symbolType == SpiceParser::INLINE)
      qualifierNode->type = QualifierNode::QualifierType::TY_INLINE;
    else if (symbolType == SpiceParser::PUBLIC)
      qualifierNode->type = QualifierNode::QualifierType::TY_PUBLIC;
    else if (symbolType == SpiceParser::HEAP)
      qualifierNode->type = QualifierNode::QualifierType::TY_HEAP;
    else if (symbolType == SpiceParser::COMPOSE)
      qualifierNode->type = QualifierNode::QualifierType::TY_COMPOSITION;
    else
      assert_fail("Unknown qualifier type"); // GCOV_EXCL_LINE
  }

  return concludeNode(qualifierNode);
}

std::any ASTBuilder::visitModAttr(SpiceParser::ModAttrContext *ctx) {
  const auto modAttrNode = createNode<ModAttrNode>(ctx);

  // Visit children
  modAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : modAttrNode->attrLst->attributes)
    attr->target = AttrNode::TARGET_MODULE;

  return concludeNode(modAttrNode);
}

std::any ASTBuilder::visitTopLevelDefAttr(SpiceParser::TopLevelDefAttrContext *ctx) {
  const auto fctAttrNode = createNode<TopLevelDefinitionAttrNode>(ctx);

  // Visit children
  fctAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  return concludeNode(fctAttrNode);
}

std::any ASTBuilder::visitLambdaAttr(SpiceParser::LambdaAttrContext *ctx) {
  const auto lambdaAttrNode = createNode<LambdaAttrNode>(ctx);

  // Visit children
  lambdaAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : lambdaAttrNode->attrLst->attributes)
    attr->target = AttrNode::TARGET_LAMBDA;

  return concludeNode(lambdaAttrNode);
}

std::any ASTBuilder::visitAttrLst(SpiceParser::AttrLstContext *ctx) {
  const auto attrLstNode = createNode<AttrLstNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(attrLstNode->attributes, ctx->attr());

  return concludeNode(attrLstNode);
}

std::any ASTBuilder::visitAttr(SpiceParser::AttrContext *ctx) {
  const auto attrNode = createNode<AttrNode>(ctx);

  // Extract key
  for (const TerminalNode *keyFragment : ctx->IDENTIFIER()) {
    if (!attrNode->key.empty())
      attrNode->key += MEMBER_ACCESS_TOKEN;
    attrNode->key += keyFragment->getSymbol()->getText();
  }

  // Visit children
  if (ctx->constant()) {
    attrNode->value = std::any_cast<ConstantNode *>(visit(ctx->constant()));

    if (ctx->constant()->STRING_LIT())
      attrNode->type = AttrNode::AttrType::TYPE_STRING;
    else if (ctx->constant()->INT_LIT())
      attrNode->type = AttrNode::AttrType::TYPE_INT;
    else if (ctx->constant()->TRUE() || ctx->constant()->FALSE())
      attrNode->type = AttrNode::AttrType::TYPE_BOOL;
    else
      throw ParserError(attrNode->value->codeLoc, INVALID_ATTR_VALUE_TYPE, "Invalid attribute value type");
  } else {
    // If no value is given, use the bool type
    attrNode->type = AttrNode::AttrType::TYPE_BOOL;
  }

  return concludeNode(attrNode);
}

std::any ASTBuilder::visitCaseConstant(SpiceParser::CaseConstantContext *ctx) {
  const auto caseConstantNode = createNode<CaseConstantNode>(ctx);

  // Visit children
  if (ctx->constant()) {
    caseConstantNode->constant = std::any_cast<ConstantNode *>(visit(ctx->constant()));
  } else if (!ctx->TYPE_IDENTIFIER().empty()) {
    for (ParserRuleContext::ParseTree *subTree : ctx->children) {
      const auto terminal = dynamic_cast<TerminalNode *>(subTree);
      if (!terminal)
        continue;

      if (terminal->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
        const std::string fragment = getIdentifier(terminal);
        caseConstantNode->identifierFragments.push_back(fragment);
        if (!caseConstantNode->fqIdentifier.empty())
          caseConstantNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
        caseConstantNode->fqIdentifier += fragment;
      } else if (terminal->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        const std::string fragment = getIdentifier(terminal);
        caseConstantNode->identifierFragments.push_back(fragment);
        if (!caseConstantNode->fqIdentifier.empty())
          caseConstantNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
        caseConstantNode->fqIdentifier += fragment;
      }
    }
  } else {
    assert_fail("Unknown case constant type"); // GCOV_EXCL_LINE
  }

  return concludeNode(caseConstantNode);
}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  const auto returnStmtNode = createNode<ReturnStmtNode>(ctx);

  // Visit children
  if (ctx->assignExpr()) {
    returnStmtNode->hasReturnValue = true;
    returnStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  }

  return concludeNode(returnStmtNode);
}

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  const auto breakStmtNode = createNode<BreakStmtNode>(ctx);

  // Extract number of breaks
  if (ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  // Visit children
  visitChildren(ctx);

  return concludeNode(breakStmtNode);
}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  const auto continueStmtNode = createNode<ContinueStmtNode>(ctx);

  // Extract number of continues
  if (ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  // Visit children
  visitChildren(ctx);

  return concludeNode(continueStmtNode);
}

std::any ASTBuilder::visitFallthroughStmt(SpiceParser::FallthroughStmtContext *ctx) {
  const auto fallthroughStmtNode = createNode<FallthroughStmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(fallthroughStmtNode);
}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  const auto assertStmtNode = createNode<AssertStmtNode>(ctx);

  // Enrich
  const antlr4::misc::Interval interval(ctx->assignExpr()->start->getStartIndex(), ctx->assignExpr()->stop->getStopIndex());
  assertStmtNode->expressionString = inputStream->getText(interval);

  // Visit children
  assertStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(assertStmtNode);
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  const auto builtinCallNode = createNode<BuiltinCallNode>(ctx);

  if (ctx->printfCall()) {
    builtinCallNode->printfCall = std::any_cast<PrintfCallNode *>(visit(ctx->printfCall()));
  } else if (ctx->sizeOfCall()) {
    builtinCallNode->sizeofCall = std::any_cast<SizeofCallNode *>(visit(ctx->sizeOfCall()));
  } else if (ctx->alignOfCall()) {
    builtinCallNode->alignofCall = std::any_cast<AlignofCallNode *>(visit(ctx->alignOfCall()));
  } else if (ctx->lenCall()) {
    builtinCallNode->lenCall = std::any_cast<LenCallNode *>(visit(ctx->lenCall()));
  } else if (ctx->panicCall()) {
    builtinCallNode->panicCall = std::any_cast<PanicCallNode *>(visit(ctx->panicCall()));
  } else if (ctx->sysCall()) {
    builtinCallNode->sysCall = std::any_cast<SysCallNode *>(visit(ctx->sysCall()));
  } else {
    assert_fail("Unknown builtin call"); // GCOV_EXCL_LINE
  }

  return concludeNode(builtinCallNode);
}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  const auto printfCallNode = createNode<PrintfCallNode>(ctx);

  // Enrich
  std::string templatedString = ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  // Visit children
  fetchChildrenIntoVector(printfCallNode->args, ctx->assignExpr());

  return concludeNode(printfCallNode);
}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  const auto sizeofCallNode = createNode<SizeofCallNode>(ctx);

  // Visit children
  if (ctx->assignExpr()) {
    sizeofCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  } else {
    sizeofCallNode->isType = true;
    sizeofCallNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  }

  return concludeNode(sizeofCallNode);
}

std::any ASTBuilder::visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) {
  const auto alignofCallNode = createNode<AlignofCallNode>(ctx);

  // Visit children
  if (ctx->assignExpr()) {
    alignofCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  } else {
    alignofCallNode->isType = true;
    alignofCallNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  }

  return concludeNode(alignofCallNode);
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  const auto lenCallNode = createNode<LenCallNode>(ctx);

  // Visit children
  lenCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(lenCallNode);
}

std::any ASTBuilder::visitPanicCall(SpiceParser::PanicCallContext *ctx) {
  const auto panicCallNode = createNode<PanicCallNode>(ctx);

  // Visit children
  panicCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(panicCallNode);
}

std::any ASTBuilder::visitSysCall(SpiceParser::SysCallContext *ctx) {
  const auto sysCallNode = createNode<SysCallNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(sysCallNode->args, ctx->assignExpr());

  return concludeNode(sysCallNode);
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  const auto assignExprNode = createNode<AssignExprNode>(ctx);

  // Visit children
  if (ctx->ternaryExpr()) {
    assignExprNode->ternaryExpr = std::any_cast<TernaryExprNode *>(visit(ctx->ternaryExpr()));
  } else if (ctx->prefixUnaryExpr()) {
    assignExprNode->lhs = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));
    visit(ctx->assignOp());
    assignExprNode->rhs = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  } else {
    assert_fail("Invalid assign expression"); // GCOV_EXCL_LINE
  }

  return concludeNode(assignExprNode);
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  const auto ternaryExprNode = createNode<TernaryExprNode>(ctx);

  ternaryExprNode->condition = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(0)));
  if (ctx->logicalOrExpr().size() == 3) {
    ternaryExprNode->trueExpr = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(1)));
    ternaryExprNode->falseExpr = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(2)));
  } else if (ctx->logicalOrExpr().size() == 2) {
    ternaryExprNode->isShortened = true;
    ternaryExprNode->falseExpr = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(1)));
  }

  return concludeNode(ternaryExprNode);
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  const auto logicalOrExprNode = createNode<LogicalOrExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(logicalOrExprNode->operands, ctx->logicalAndExpr());

  return concludeNode(logicalOrExprNode);
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  const auto logicalAndExprNode = createNode<LogicalAndExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(logicalAndExprNode->operands, ctx->bitwiseOrExpr());

  return concludeNode(logicalAndExprNode);
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  const auto bitwiseOrExprNode = createNode<BitwiseOrExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(bitwiseOrExprNode->operands, ctx->bitwiseXorExpr());

  return concludeNode(bitwiseOrExprNode);
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  const auto bitwiseXorExprNode = createNode<BitwiseXorExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(bitwiseXorExprNode->operands, ctx->bitwiseAndExpr());

  return concludeNode(bitwiseXorExprNode);
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  const auto bitwiseAndExprNode = createNode<BitwiseAndExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(bitwiseAndExprNode->operands, ctx->equalityExpr());

  return concludeNode(bitwiseAndExprNode);
}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  const auto equalityExprNode = createNode<EqualityExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(equalityExprNode->operands, ctx->relationalExpr());

  // Extract operator
  if (ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::EqualityOp::OP_EQUAL;
  else if (ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::EqualityOp::OP_NOT_EQUAL;

  return concludeNode(equalityExprNode);
}

std::any ASTBuilder::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  const auto relationalExprNode = createNode<RelationalExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(relationalExprNode->operands, ctx->shiftExpr());

  // Extract operator
  if (ctx->LESS())
    relationalExprNode->op = RelationalExprNode::RelationalOp::OP_LESS;
  else if (ctx->GREATER())
    relationalExprNode->op = RelationalExprNode::RelationalOp::OP_GREATER;
  else if (ctx->LESS_EQUAL())
    relationalExprNode->op = RelationalExprNode::RelationalOp::OP_LESS_EQUAL;
  else if (ctx->GREATER_EQUAL())
    relationalExprNode->op = RelationalExprNode::RelationalOp::OP_GREATER_EQUAL;

  return concludeNode(relationalExprNode);
}

std::any ASTBuilder::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  const auto shiftExprNode = createNode<ShiftExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(shiftExprNode->operands, ctx->additiveExpr());

  bool seenFirstLess = false;
  bool seenFirstGreater = false;
  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto terminal = dynamic_cast<TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::LESS) {
      if (seenFirstLess)
        shiftExprNode->opQueue.emplace(ShiftExprNode::ShiftOp::OP_SHIFT_LEFT, TY_INVALID);
      seenFirstLess = !seenFirstLess;
      continue;
    }

    if (terminal->getSymbol()->getType() == SpiceParser::GREATER) {
      if (seenFirstGreater)
        shiftExprNode->opQueue.emplace(ShiftExprNode::ShiftOp::OP_SHIFT_RIGHT, TY_INVALID);
      seenFirstGreater = !seenFirstGreater;
      continue;
    }

    assert_fail("Invalid terminal symbol for additive expression"); // GCOV_EXCL_LINE
  }
  assert(!seenFirstLess && !seenFirstGreater);

  return concludeNode(shiftExprNode);
}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  const auto additiveExprNode = createNode<AdditiveExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(additiveExprNode->operands, ctx->multiplicativeExpr());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto terminal = dynamic_cast<TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::AdditiveOp::OP_PLUS, TY_INVALID);
    else if (terminal->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::AdditiveOp::OP_MINUS, TY_INVALID);
    else
      assert_fail("Invalid terminal symbol for additive expression"); // GCOV_EXCL_LINE
  }

  return concludeNode(additiveExprNode);
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  const auto multiplicativeExprNode = createNode<MultiplicativeExprNode>(ctx);

  // Visit children
  fetchChildrenIntoVector(multiplicativeExprNode->operands, ctx->castExpr());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto terminal = dynamic_cast<TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::MultiplicativeOp::OP_MUL, TY_INVALID);
    else if (terminal->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::MultiplicativeOp::OP_DIV, TY_INVALID);
    else if (terminal->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::MultiplicativeOp::OP_REM, TY_INVALID);
    else
      assert_fail("Invalid terminal symbol for multiplicative expression"); // GCOV_EXCL_LINE
  }

  return concludeNode(multiplicativeExprNode);
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  const auto castExprNode = createNode<CastExprNode>(ctx);

  if (ctx->dataType()) {
    castExprNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
    castExprNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
    castExprNode->isCast = true;
  } else {
    castExprNode->prefixUnaryExpr = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));
  }

  return concludeNode(castExprNode);
}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  const auto prefixUnaryExprNode = createNode<PrefixUnaryExprNode>(ctx);

  // Visit children
  if (ctx->postfixUnaryExpr()) {
    prefixUnaryExprNode->postfixUnaryExpr = std::any_cast<PostfixUnaryExprNode *>(visit(ctx->postfixUnaryExpr()));
  } else if (ctx->prefixUnaryExpr()) {
    // Extract operator
    if (ctx->MINUS())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_MINUS;
    else if (ctx->PLUS_PLUS())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_PLUS_PLUS;
    else if (ctx->MINUS_MINUS())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_MINUS_MINUS;
    else if (ctx->NOT())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_NOT;
    else if (ctx->BITWISE_NOT())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_BITWISE_NOT;
    else if (ctx->MUL())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_DEREFERENCE;
    else if (ctx->BITWISE_AND())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::PrefixUnaryOp::OP_ADDRESS_OF;

    prefixUnaryExprNode->prefixUnaryExpr = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));
  } else {
    assert_fail("Unknown prefix unary expression type"); // GCOV_EXCL_LINE
  }

  return concludeNode(prefixUnaryExprNode);
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  const auto postfixUnaryExprNode = createNode<PostfixUnaryExprNode>(ctx);

  if (ctx->atomicExpr()) {
    postfixUnaryExprNode->atomicExpr = std::any_cast<AtomicExprNode *>(visit(ctx->atomicExpr()));
  } else if (ctx->postfixUnaryExpr()) {
    postfixUnaryExprNode->postfixUnaryExpr = std::any_cast<PostfixUnaryExprNode *>(visit(ctx->postfixUnaryExpr()));

    // Extract operator
    if (ctx->assignExpr()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::PostfixUnaryOp::OP_SUBSCRIPT;
      postfixUnaryExprNode->subscriptIndexExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
    } else if (ctx->IDENTIFIER()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::PostfixUnaryOp::OP_MEMBER_ACCESS;
      postfixUnaryExprNode->identifier = getIdentifier(ctx->IDENTIFIER());
    } else if (ctx->PLUS_PLUS()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::PostfixUnaryOp::OP_PLUS_PLUS;
    } else if (ctx->MINUS_MINUS()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::PostfixUnaryOp::OP_MINUS_MINUS;
    }
  } else {
    assert_fail("Unknown postfix unary expression type"); // GCOV_EXCL_LINE
  }

  return concludeNode(postfixUnaryExprNode);
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  const auto atomicExprNode = createNode<AtomicExprNode>(ctx);

  // Visit children
  if (ctx->constant()) {
    atomicExprNode->constant = std::any_cast<ConstantNode *>(visit(ctx->constant()));
  } else if (ctx->value()) {
    atomicExprNode->value = std::any_cast<ValueNode *>(visit(ctx->value()));
  } else if (!ctx->IDENTIFIER().empty() || !ctx->TYPE_IDENTIFIER().empty()) {
    for (ParserRuleContext::ParseTree *subTree : ctx->children) {
      const auto terminal = dynamic_cast<TerminalNode *>(subTree);
      if (!terminal)
        continue;

      if (terminal->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
        std::string fragment = getIdentifier(terminal);
        atomicExprNode->identifierFragments.push_back(fragment);
        if (!atomicExprNode->fqIdentifier.empty())
          atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
        atomicExprNode->fqIdentifier += fragment;
      } else if (terminal->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        std::string fragment = getIdentifier(terminal);
        atomicExprNode->identifierFragments.push_back(fragment);
        if (!atomicExprNode->fqIdentifier.empty())
          atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
        atomicExprNode->fqIdentifier += fragment;
      }
    }
  } else if (ctx->builtinCall()) {
    atomicExprNode->builtinCall = std::any_cast<BuiltinCallNode *>(visit(ctx->builtinCall()));
  } else if (ctx->assignExpr()) {
    atomicExprNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  } else {
    assert_fail("Unknown atomic expression type"); // GCOV_EXCL_LINE
  }

  return concludeNode(atomicExprNode);
}

std::any ASTBuilder::visitValue(SpiceParser::ValueContext *ctx) {
  const auto valueNode = createNode<ValueNode>(ctx);

  // Visit children
  if (ctx->fctCall()) {
    valueNode->fctCall = std::any_cast<FctCallNode *>(visit(ctx->fctCall()));
  } else if (ctx->arrayInitialization()) {
    valueNode->arrayInitialization = std::any_cast<ArrayInitializationNode *>(visit(ctx->arrayInitialization()));
  } else if (ctx->structInstantiation()) {
    valueNode->structInstantiation = std::any_cast<StructInstantiationNode *>(visit(ctx->structInstantiation()));
  } else if (ctx->lambdaFunc()) {
    valueNode->lambdaFunc = std::any_cast<LambdaFuncNode *>(visit(ctx->lambdaFunc()));
  } else if (ctx->lambdaProc()) {
    valueNode->lambdaProc = std::any_cast<LambdaProcNode *>(visit(ctx->lambdaProc()));
  } else if (ctx->lambdaExpr()) {
    valueNode->lambdaExpr = std::any_cast<LambdaExprNode *>(visit(ctx->lambdaExpr()));
  } else if (ctx->dataType()) {
    valueNode->isNil = true;
    valueNode->nilType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  } else {
    assert_fail("Unknown value type"); // GCOV_EXCL_LINE
  }

  return concludeNode(valueNode);
}

std::any ASTBuilder::visitConstant(SpiceParser::ConstantContext *ctx) {
  const auto constantNode = createNode<ConstantNode>(ctx);

  // Enrich
  if (ctx->DOUBLE_LIT()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_DOUBLE;
    constantNode->compileTimeValue.doubleValue = std::stod(ctx->DOUBLE_LIT()->toString());
  } else if (ctx->INT_LIT()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_INT;
    constantNode->compileTimeValue.intValue = parseInt(ctx->INT_LIT());
  } else if (ctx->SHORT_LIT()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_SHORT;
    constantNode->compileTimeValue.shortValue = parseShort(ctx->SHORT_LIT());
  } else if (ctx->LONG_LIT()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_LONG;
    constantNode->compileTimeValue.longValue = parseLong(ctx->LONG_LIT());
  } else if (ctx->CHAR_LIT()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_CHAR;
    constantNode->compileTimeValue.charValue = parseChar(ctx->CHAR_LIT());
  } else if (ctx->STRING_LIT()) {
    // Save a pointer to the string in the compile time value
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_STRING;
    constantNode->compileTimeValue.stringValueOffset = resourceManager.compileTimeStringValues.size();
    // Add the string to the global compile time string list
    resourceManager.compileTimeStringValues.push_back(parseString(ctx->STRING_LIT()->toString()));
  } else if (ctx->TRUE()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_BOOL;
    constantNode->compileTimeValue.boolValue = true;
  } else if (ctx->FALSE()) {
    constantNode->type = ConstantNode::PrimitiveValueType::TYPE_BOOL;
    constantNode->compileTimeValue.boolValue = false;
  } else {
    assert_fail("Unknown constant type"); // GCOV_EXCL_LINE
  }

  return concludeNode(constantNode);
}

std::any ASTBuilder::visitFctCall(SpiceParser::FctCallContext *ctx) {
  const auto fctCallNode = createNode<FctCallNode>(ctx);

  for (antlr4::ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto terminal = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = terminal->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (terminal->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = terminal->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (terminal->getSymbol()->getType() == SpiceParser::SCOPE_ACCESS) {
      fctCallNode->fqFunctionName += SCOPE_ACCESS_TOKEN;
    } else if (terminal->getSymbol()->getType() == SpiceParser::DOT) {
      fctCallNode->fqFunctionName += MEMBER_ACCESS_TOKEN;
    }
  }

  // Visit children
  if (ctx->typeLst()) {
    fctCallNode->hasTemplateTypes = true;
    fctCallNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  if (ctx->argLst()) {
    fctCallNode->hasArgs = true;
    fctCallNode->argLst = std::any_cast<ArgLstNode *>(visit(ctx->argLst()));
  }

  return concludeNode(fctCallNode);
}

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  const auto arrayInitializationNode = createNode<ArrayInitializationNode>(ctx);

  // Visit children
  if (ctx->argLst())
    arrayInitializationNode->itemLst = std::any_cast<ArgLstNode *>(visit(ctx->argLst()));

  return concludeNode(arrayInitializationNode);
}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  const auto structInstantiationNode = createNode<StructInstantiationNode>(ctx);

  // Enrich
  for (antlr4::ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto terminal = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = terminal->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (terminal->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = terminal->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment;
    }
  }

  // Visit children
  if (ctx->typeLst()) {
    structInstantiationNode->hasTemplateTypes = true;
    structInstantiationNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  if (ctx->argLst())
    structInstantiationNode->fieldLst = std::any_cast<ArgLstNode *>(visit(ctx->argLst()));

  return concludeNode(structInstantiationNode);
}

std::any ASTBuilder::visitLambdaFunc(SpiceParser::LambdaFuncContext *ctx) {
  const auto lambdaFuncNode = createNode<LambdaFuncNode>(ctx);

  // Visit children
  lambdaFuncNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  if (ctx->paramLst()) {
    lambdaFuncNode->hasParams = true;
    lambdaFuncNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  if (ctx->lambdaAttr())
    lambdaFuncNode->lambdaAttr = std::any_cast<LambdaAttrNode *>(visit(ctx->lambdaAttr()));
  lambdaFuncNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(lambdaFuncNode);
}

std::any ASTBuilder::visitLambdaProc(SpiceParser::LambdaProcContext *ctx) {
  const auto lambdaProcNode = createNode<LambdaProcNode>(ctx);

  // Visit children
  if (ctx->paramLst()) {
    lambdaProcNode->hasParams = true;
    lambdaProcNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  if (ctx->lambdaAttr())
    lambdaProcNode->lambdaAttr = std::any_cast<LambdaAttrNode *>(visit(ctx->lambdaAttr()));
  lambdaProcNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(lambdaProcNode);
}

std::any ASTBuilder::visitLambdaExpr(SpiceParser::LambdaExprContext *ctx) {
  const auto lambdaExprNode = createNode<LambdaExprNode>(ctx);

  // Visit children
  if (ctx->paramLst()) {
    lambdaExprNode->hasParams = true;
    lambdaExprNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  lambdaExprNode->lambdaExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(lambdaExprNode);
}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  const auto dataTypeNode = createNode<DataTypeNode>(ctx);

  // Visit children
  if (ctx->qualifierLst())
    dataTypeNode->qualifierLst = std::any_cast<QualifierLstNode *>(visit(ctx->qualifierLst()));
  dataTypeNode->baseDataType = std::any_cast<BaseDataTypeNode *>(visit(ctx->baseDataType()));

  // Enrich
  for (size_t i = 0; i < ctx->children.size(); i++) {
    antlr4::tree::ParseTree *subTree = ctx->children.at(i);
    auto terminal = dynamic_cast<TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::MUL) {
      dataTypeNode->tmQueue.emplace(DataTypeNode::TypeModifierType::TYPE_PTR, false, 0);
    } else if (terminal->getSymbol()->getType() == SpiceParser::BITWISE_AND) {
      dataTypeNode->tmQueue.emplace(DataTypeNode::TypeModifierType::TYPE_REF, false, 0);
    } else if (terminal->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = ctx->children.at(i);
      terminal = dynamic_cast<TerminalNode *>(subTree);
      bool hasSize = false;
      unsigned int hardCodedSize = 0;
      std::string sizeVarName;
      if (terminal->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        hardCodedSize = std::stoi(terminal->getSymbol()->getText());
        i++; // Consume INT_LIT
      } else if (terminal->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        hasSize = true;
        sizeVarName = getIdentifier(terminal);
        i++; // Consume TYPE_IDENTIFIER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TypeModifierType::TYPE_ARRAY, hasSize, hardCodedSize, sizeVarName});
    }
  }

  return concludeNode(dataTypeNode);
}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  const auto baseDataTypeNode = createNode<BaseDataTypeNode>(ctx);

  // Enrich
  if (ctx->TYPE_DOUBLE()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_DOUBLE;
  } else if (ctx->TYPE_INT()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_INT;
  } else if (ctx->TYPE_SHORT()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_SHORT;
  } else if (ctx->TYPE_LONG()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_LONG;
  } else if (ctx->TYPE_BYTE()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_BYTE;
  } else if (ctx->TYPE_CHAR()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_CHAR;
  } else if (ctx->TYPE_STRING()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_STRING;
  } else if (ctx->TYPE_BOOL()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_BOOL;
  } else if (ctx->TYPE_DYN()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_DYN;
  } else if (ctx->customDataType()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_CUSTOM;
    baseDataTypeNode->customDataType = std::any_cast<CustomDataTypeNode *>(visit(ctx->customDataType()));
  } else if (ctx->functionDataType()) {
    baseDataTypeNode->type = BaseDataTypeNode::Type::TYPE_FUNCTION;
    baseDataTypeNode->functionDataType = std::any_cast<FunctionDataTypeNode *>(visit(ctx->functionDataType()));
  } else {
    assert_fail("Unknown base data type");
  }

  return concludeNode(baseDataTypeNode);
}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  const auto customDataTypeNode = createNode<CustomDataTypeNode>(ctx);

  // Enrich
  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto terminal = dynamic_cast<TerminalNode *>(subTree);
    if (!terminal)
      continue;

    if (terminal->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = terminal->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (terminal->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = terminal->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment;
    }
  }

  // Visit children
  if (ctx->typeLst())
    customDataTypeNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));

  return concludeNode(customDataTypeNode);
}

std::any ASTBuilder::visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) {
  const auto functionDataTypeNode = createNode<FunctionDataTypeNode>(ctx);

  // Enrich
  if (ctx->dataType()) {
    functionDataTypeNode->isFunction = ctx->dataType();
    functionDataTypeNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  }
  if (ctx->typeLst())
    functionDataTypeNode->paramTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));

  return concludeNode(functionDataTypeNode);
}

std::any ASTBuilder::visitAssignOp(SpiceParser::AssignOpContext *ctx) {
  const auto assignExprNode = resumeForExpansion<AssignExprNode>();

  // Extract assign operator
  if (ctx->ASSIGN())
    assignExprNode->op = AssignExprNode::AssignOp::OP_ASSIGN;
  else if (ctx->PLUS_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_PLUS_EQUAL;
  else if (ctx->MINUS_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_MINUS_EQUAL;
  else if (ctx->MUL_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_MUL_EQUAL;
  else if (ctx->DIV_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_DIV_EQUAL;
  else if (ctx->REM_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_REM_EQUAL;
  else if (ctx->SHL_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_SHL_EQUAL;
  else if (ctx->SHR_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_SHR_EQUAL;
  else if (ctx->AND_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_AND_EQUAL;
  else if (ctx->OR_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_OR_EQUAL;
  else if (ctx->XOR_EQUAL())
    assignExprNode->op = AssignExprNode::AssignOp::OP_XOR_EQUAL;
  else
    assert_fail("Unknown assign operator");

  return nullptr;
}

std::any ASTBuilder::visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) {
  const auto fctNameNode = resumeForExpansion<FctNameNode>();

  // Enrich
  if (ctx->PLUS())
    fctNameNode->name = OP_FCT_PLUS;
  else if (ctx->MINUS())
    fctNameNode->name = OP_FCT_MINUS;
  else if (ctx->MUL())
    fctNameNode->name = OP_FCT_MUL;
  else if (ctx->DIV())
    fctNameNode->name = OP_FCT_DIV;
  else if (ctx->EQUAL())
    fctNameNode->name = OP_FCT_EQUAL;
  else if (ctx->NOT_EQUAL())
    fctNameNode->name = OP_FCT_NOT_EQUAL;
  else if (ctx->LESS().size() == 2)
    fctNameNode->name = OP_FCT_SHL;
  else if (ctx->GREATER().size() == 2)
    fctNameNode->name = OP_FCT_SHR;
  else if (ctx->PLUS_EQUAL())
    fctNameNode->name = OP_FCT_PLUS_EQUAL;
  else if (ctx->MINUS_EQUAL())
    fctNameNode->name = OP_FCT_MINUS_EQUAL;
  else if (ctx->MUL_EQUAL())
    fctNameNode->name = OP_FCT_MUL_EQUAL;
  else if (ctx->DIV_EQUAL())
    fctNameNode->name = OP_FCT_DIV_EQUAL;
  else if (ctx->PLUS_PLUS())
    fctNameNode->name = OP_FCT_POSTFIX_PLUS_PLUS;
  else if (ctx->MINUS_MINUS())
    fctNameNode->name = OP_FCT_POSTFIX_MINUS_MINUS;
  else if (ctx->LBRACKET())
    fctNameNode->name = OP_FCT_SUBSCRIPT;
  else
    assert_fail("Unsupported overloadable operator"); // GCOV_EXCL_LINE

  fctNameNode->fqName = fctNameNode->name;
  fctNameNode->nameFragments.push_back(fctNameNode->name);

  return nullptr;
}

int32_t ASTBuilder::parseInt(TerminalNode *terminal) {
  const NumericParserCallback<int32_t> cb = [](const std::string &substr, short base, bool isSigned) -> int32_t {
    // Prepare limits
    const int64_t upperLimit = isSigned ? INT32_MAX : UINT32_MAX;
    const int64_t lowerLimit = isSigned ? INT32_MIN : 0;
    // Parse number and check for limits
    const int64_t number = std::stoll(substr, nullptr, base);
    if (number < lowerLimit || number > upperLimit)
      throw std::out_of_range("Number out of range");
    return static_cast<int32_t>(number);
  };
  return parseNumeric(terminal, cb);
}

int16_t ASTBuilder::parseShort(TerminalNode *terminal) {
  const NumericParserCallback<int16_t> cb = [](const std::string &substr, short base, bool isSigned) -> int16_t {
    // Prepare limits
    const int64_t upperLimit = isSigned ? INT16_MAX : UINT16_MAX;
    const int64_t lowerLimit = isSigned ? INT16_MIN : 0;
    // Parse number and check for limits
    const int64_t number = std::stoll(substr, nullptr, base);
    if (number < lowerLimit || number > upperLimit)
      throw std::out_of_range("Number out of range");
    return static_cast<int16_t>(number);
  };
  return parseNumeric(terminal, cb);
}

int64_t ASTBuilder::parseLong(TerminalNode *terminal) {
  const NumericParserCallback<int64_t> cb = [](const std::string &substr, short base, bool isSigned) -> int64_t {
    return isSigned ? std::stoll(substr, nullptr, base) : static_cast<int64_t>(std::stoull(substr, nullptr, base));
  };
  return parseNumeric(terminal, cb);
}

int8_t ASTBuilder::parseChar(TerminalNode *terminal) const {
  const std::string input = terminal->toString();
  if (input.length() == 3) // Normal char literals
    return input[1];

  if (input.length() == 4 && input[1] == '\\') { // Char literals with escape sequence
    switch (input[2]) {
    case '\'':
      return '\'';
    case '"':
      return '\"';
    case '\\':
      return '\\';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'v':
      return '\v';
    case '0':
      return '\0';
    default:
      const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
      throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid escape sequence " + input);
    }
  }

  const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
  throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid char literal " + input);
}

std::string ASTBuilder::parseString(std::string input) {
  input = input.substr(1, input.size() - 2);
  replaceEscapeChars(input);
  return input;
}

template <typename T> T ASTBuilder::parseNumeric(TerminalNode *terminal, const NumericParserCallback<T> &cb) {
  const std::string input = terminal->toString();

  // Set to signed if the input string does not end with 'u'
  const bool isUnsigned = input.ends_with('u') || input.ends_with("us") || input.ends_with("ul");

  try {
    if (input.length() >= 3) {
      if (input[0] == '0') {
        const std::string subStr = input.substr(2);
        switch (input[1]) {
        case 'd': // fall-through
        case 'D':
          return cb(subStr, 10, !isUnsigned);
        case 'b': // fall-through
        case 'B':
          return cb(subStr, 2, !isUnsigned);
        case 'h': // fall-through
        case 'H': // fall-through
        case 'x': // fall-through
        case 'X':
          return cb(subStr, 16, !isUnsigned);
        case 'o': // fall-through
        case 'O':
          return cb(subStr, 8, !isUnsigned);
        default: // default is decimal
          return cb(input, 10, !isUnsigned);
        }
      }
    }
    return cb(input, 10, !isUnsigned);
  } catch (std::out_of_range &) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "You tried to parse '" + input + "' as an integer, but it was no integer");
  }
}

void ASTBuilder::replaceEscapeChars(std::string &input) {
  const std::unordered_map<char, char> escapeMap = {
      {'a', '\a'}, {'b', '\b'},  {'f', '\f'}, {'n', '\n'},  {'r', '\r'}, {'t', '\t'},
      {'v', '\v'}, {'\\', '\\'}, {'?', '\?'}, {'\'', '\''}, {'"', '\"'},
  };

  size_t writeIndex = 0; // Index where the next character should be written
  for (size_t readIndex = 0; readIndex < input.length(); ++readIndex, ++writeIndex) {
    if (input[readIndex] == '\\' && readIndex + 1 < input.length()) {
      const char nextChar = input[readIndex + 1];
      if (escapeMap.contains(nextChar)) {
        // If the next character forms a valid escape sequence, replace it
        input[writeIndex] = escapeMap.at(nextChar);
        readIndex++; // Skip the next character as it's part of the escape sequence
      } else {
        // If it's not a valid escape sequence, just copy the backslash
        input[writeIndex] = input[readIndex];
      }
    } else {
      if (writeIndex != readIndex) {
        // If we've made replacements, shift the current character to the write position
        input[writeIndex] = input[readIndex];
      }
      // If no replacements were needed, writeIndex and readIndex are the same, and this does nothing
    }
  }
  // Resize the string to remove the unused portion
  input.resize(writeIndex);
}

std::string ASTBuilder::getIdentifier(TerminalNode *terminal) const {
  std::string identifier = terminal->getText();

  // Check if the identifier is 'String' and this is no std source file
  bool isReserved = !sourceFile->isStdFile && (identifier == STROBJ_NAME || identifier == RESULTOBJ_NAME);
  // Check if the list of reserved keywords contains the given identifier
  isReserved |= std::ranges::find(RESERVED_KEYWORDS, identifier) != std::end(RESERVED_KEYWORDS);
  // Print error message
  if (isReserved) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, RESERVED_KEYWORD, "'" + identifier + "' is a reserved keyword. Please use another name instead");
  }

  return identifier;
}

} // namespace spice::compiler
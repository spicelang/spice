// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <exception/ParserError.h>
#include <global/GlobalResourceManager.h>
#include <typechecker/OpRuleManager.h>
#include <util/CommonUtil.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

ASTBuilder::ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, antlr4::ANTLRInputStream *inputStream)
    : CompilerPass(resourceManager, sourceFile), inputStream(inputStream) {}

std::any ASTBuilder::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = createNode<EntryNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(entryNode);
}

std::any ASTBuilder::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  auto mainFctDefNode = createNode<MainFctDefNode>(ctx);

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
  auto fctDefNode = createNode<FctDefNode>(ctx);

  // Visit children
  if (ctx->topLevelDefAttr()) {
    fctDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are function attributes
    for (AttrNode *attr : fctDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }
  if (ctx->specifierLst())
    fctDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  fctDefNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  fctDefNode->returnType->isReturnType = true;
  fctDefNode->name = std::any_cast<FctNameNode *>(visit(ctx->fctName()));
  fctDefNode->name = fctDefNode->getChild<FctNameNode>();
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
  auto procDefNode = createNode<ProcDefNode>(ctx);

  // Visit children
  if (ctx->topLevelDefAttr()) {
    procDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are function attributes
    for (AttrNode *attr : procDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }
  if (ctx->specifierLst())
    procDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  procDefNode->name = std::any_cast<FctNameNode *>(visit(ctx->fctName()));
  procDefNode->name = procDefNode->getChild<FctNameNode>();
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
  auto fctNameNode = createNode<FctNameNode>(ctx);

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
  auto structDefNode = createNode<StructDefNode>(ctx);

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
  if (ctx->specifierLst())
    structDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
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
  auto interfaceDefNode = createNode<InterfaceDefNode>(ctx);

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
  if (ctx->specifierLst())
    interfaceDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  if (ctx->LESS()) {
    interfaceDefNode->hasTemplateTypes = true;
    interfaceDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
  }
  for (SpiceParser::SignatureContext *signature : ctx->signature())
    interfaceDefNode->signatures.push_back(std::any_cast<SignatureNode *>(visit(signature)));

  return concludeNode(interfaceDefNode);
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  auto enumDefNode = createNode<EnumDefNode>(ctx);

  // Enrich
  enumDefNode->enumName = getIdentifier(ctx->TYPE_IDENTIFIER());
  enumDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  if (ctx->specifierLst())
    enumDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  enumDefNode->itemLst = std::any_cast<EnumItemLstNode *>(visit(ctx->enumItemLst()));

  // Tell all items about the enum def
  for (EnumItemNode *enumItem : enumDefNode->itemLst->items)
    enumItem->enumDef = enumDefNode;

  return concludeNode(enumDefNode);
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  auto genericTypeDefNode = createNode<GenericTypeDefNode>(ctx);

  // Enrich
  genericTypeDefNode->typeName = getIdentifier(ctx->TYPE_IDENTIFIER());

  // Visit children
  genericTypeDefNode->typeAltsLst = std::any_cast<TypeAltsLstNode *>(visit(ctx->typeAltsLst()));

  return concludeNode(genericTypeDefNode);
}

std::any ASTBuilder::visitAliasDef(SpiceParser::AliasDefContext *ctx) {
  auto aliasDefNode = createNode<AliasDefNode>(ctx);

  // Enrich
  aliasDefNode->aliasName = getIdentifier(ctx->TYPE_IDENTIFIER());
  aliasDefNode->dataTypeString = ctx->dataType()->getText();
  aliasDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  if (ctx->specifierLst())
    aliasDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  aliasDefNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  return concludeNode(aliasDefNode);
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  auto globalVarDefNode = createNode<GlobalVarDefNode>(ctx);

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
  auto extDeclNode = createNode<ExtDeclNode>(ctx);

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
  auto importDefNode = createNode<ImportDefNode>(ctx);

  // Extract path
  const std::string pathStr = ctx->STRING_LIT()->getText();
  importDefNode->importPath = pathStr.substr(1, pathStr.size() - 2);

  // If no name is given, use the path as name
  importDefNode->importName = ctx->AS() ? getIdentifier(ctx->IDENTIFIER()) : importDefNode->importPath;

  return concludeNode(importDefNode);
}

std::any ASTBuilder::visitUnsafeBlock(SpiceParser::UnsafeBlockContext *ctx) {
  auto unsafeBlockDefNode = createNode<UnsafeBlockNode>(ctx);

  // Visit children
  unsafeBlockDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(unsafeBlockDefNode);
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto forLoopNode = createNode<ForLoopNode>(ctx);

  visit(ctx->forHead());
  forLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(forLoopNode);
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  auto forLoopNode = resumeForExpansion<ForLoopNode>();

  // Visit children
  forLoopNode->initDecl = std::any_cast<DeclStmtNode *>(visit(ctx->declStmt()));
  forLoopNode->condAssign = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr(0)));
  forLoopNode->incAssign = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr(1)));

  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto foreachLoopNode = createNode<ForeachLoopNode>(ctx);

  // Visit children
  visit(ctx->foreachHead());
  foreachLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  // Tell the foreach item that it is one
  foreachLoopNode->itemVarDecl->isForEachItem = true;

  return concludeNode(foreachLoopNode);
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  auto foreachLoopNode = resumeForExpansion<ForeachLoopNode>();

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
  auto whileLoopNode = createNode<WhileLoopNode>(ctx);

  // Visit children
  whileLoopNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  whileLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(whileLoopNode);
}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {
  auto doWhileLoopNode = createNode<DoWhileLoopNode>(ctx);

  // Visit children
  doWhileLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  doWhileLoopNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(doWhileLoopNode);
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  auto ifStmtNode = createNode<IfStmtNode>(ctx);

  // Visit children
  ifStmtNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  ifStmtNode->thenBody = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  if (ctx->elseStmt())
    ifStmtNode->elseStmt = std::any_cast<ElseStmtNode *>(visit(ctx->elseStmt()));

  return concludeNode(ifStmtNode);
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  auto elseStmtNode = createNode<ElseStmtNode>(ctx);

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
  auto switchStmtNode = createNode<SwitchStmtNode>(ctx);

  // Visit children
  switchStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  for (SpiceParser::CaseBranchContext *caseBranch : ctx->caseBranch())
    switchStmtNode->caseBranches.push_back(std::any_cast<CaseBranchNode *>(visit(caseBranch)));
  if (ctx->defaultBranch()) {
    switchStmtNode->hasDefaultBranch = true;
    switchStmtNode->defaultBranch = std::any_cast<DefaultBranchNode *>(visit(ctx->defaultBranch()));
  }

  return concludeNode(switchStmtNode);
}

std::any ASTBuilder::visitCaseBranch(SpiceParser::CaseBranchContext *ctx) {
  auto caseBranchNode = createNode<CaseBranchNode>(ctx);

  // Visit children
  for (SpiceParser::CaseConstantContext *caseConstant : ctx->caseConstant())
    caseBranchNode->caseConstants.push_back(std::any_cast<CaseConstantNode *>(visit(caseConstant)));
  caseBranchNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(caseBranchNode);
}

std::any ASTBuilder::visitDefaultBranch(SpiceParser::DefaultBranchContext *ctx) {
  auto defaultBranchNode = createNode<DefaultBranchNode>(ctx);

  // Visit children
  defaultBranchNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(defaultBranchNode);
}

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  auto anonymousBlockStmtNode = createNode<AnonymousBlockStmtNode>(ctx);

  // Visit children
  anonymousBlockStmtNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(anonymousBlockStmtNode);
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = createNode<StmtLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(stmtLstNode);
}

std::any ASTBuilder::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  auto typeLstNode = createNode<TypeLstNode>(ctx);

  // Visit children
  for (SpiceParser::DataTypeContext *dataType : ctx->dataType())
    typeLstNode->dataTypes.push_back(std::any_cast<DataTypeNode *>(visit(dataType)));

  return concludeNode(typeLstNode);
}

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = createNode<TypeAltsLstNode>(ctx);

  // Visit children
  for (SpiceParser::DataTypeContext *dataType : ctx->dataType())
    typeAltsLstNode->dataTypes.push_back(std::any_cast<DataTypeNode *>(visit(dataType)));

  return concludeNode(typeAltsLstNode);
}

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  auto paramLstNode = createNode<ParamLstNode>(ctx);

  // Visit children
  for (SpiceParser::DeclStmtContext *declStmt : ctx->declStmt()) {
    auto param = std::any_cast<DeclStmtNode *>(visit(declStmt));
    param->isParam = true;
    param->dataType->isParamType = true;
    paramLstNode->params.push_back(param);
  }

  return concludeNode(paramLstNode);
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  auto argLstNode = createNode<ArgLstNode>(ctx);

  // Visit children
  for (SpiceParser::AssignExprContext *assignExpr : ctx->assignExpr())
    argLstNode->args.push_back(std::any_cast<AssignExprNode *>(visit(assignExpr)));

  return concludeNode(argLstNode);
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  auto enumItemLstNode = createNode<EnumItemLstNode>(ctx);

  // Visit children
  for (SpiceParser::EnumItemContext *enumItem : ctx->enumItem())
    enumItemLstNode->items.push_back(std::any_cast<EnumItemNode *>(visit(enumItem)));

  return concludeNode(enumItemLstNode);
}

std::any ASTBuilder::visitEnumItem(SpiceParser::EnumItemContext *ctx) {
  auto enumItemNode = createNode<EnumItemNode>(ctx);

  // Enrich
  enumItemNode->itemName = getIdentifier(ctx->TYPE_IDENTIFIER());
  if (ctx->ASSIGN()) {
    enumItemNode->itemValue = parseInt(ctx->INT_LIT());
    enumItemNode->hasValue = true;
  }

  return concludeNode(enumItemNode);
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = createNode<FieldNode>(ctx);

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
  auto signatureNode = createNode<SignatureNode>(ctx);

  // Extract method name
  signatureNode->methodName = getIdentifier(ctx->IDENTIFIER());

  // Visit children
  if (ctx->specifierLst()) {
    signatureNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  }
  if (ctx->F()) {
    signatureNode->hasReturnType = true;
    signatureNode->signatureType = SignatureNode::TYPE_FUNCTION;
    signatureNode->signatureSpecifiers = TypeSpecifiers::of(TY_FUNCTION);
    signatureNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  } else {
    signatureNode->signatureType = SignatureNode::TYPE_PROCEDURE;
    signatureNode->signatureSpecifiers = TypeSpecifiers::of(TY_PROCEDURE);
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

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) { return visitChildren(ctx); }

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  auto declStmtNode = createNode<DeclStmtNode>(ctx);

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

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  auto specifierLstNode = createNode<SpecifierLstNode>(ctx);

  // Visit children
  bool seenSignedOrUnsigned = false;
  for (SpiceParser::SpecifierContext *specifierCtx : ctx->specifier()) {
    auto specifier = std::any_cast<SpecifierNode *>(visit(specifierCtx));
    specifierLstNode->specifiers.push_back(specifier);

    // Check if we have both, signed and unsigned specifier
    if (specifier->type != SpecifierNode::TY_SIGNED && specifier->type != SpecifierNode::TY_UNSIGNED)
      continue;
    if (seenSignedOrUnsigned)
      throw ParserError(specifier->codeLoc, INVALID_SPECIFIER_COMBINATION, "A variable can not be signed and unsigned");
    seenSignedOrUnsigned = true;
  }

  return concludeNode(specifierLstNode);
}

std::any ASTBuilder::visitSpecifier(SpiceParser::SpecifierContext *ctx) {
  auto specifierNode = createNode<SpecifierNode>(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    const auto token = spice_pointer_cast<TerminalNode *>(subTree);
    const size_t symbolType = token->getSymbol()->getType();
    if (symbolType == SpiceParser::CONST)
      specifierNode->type = SpecifierNode::TY_CONST;
    else if (symbolType == SpiceParser::SIGNED)
      specifierNode->type = SpecifierNode::TY_SIGNED;
    else if (symbolType == SpiceParser::UNSIGNED)
      specifierNode->type = SpecifierNode::TY_UNSIGNED;
    else if (symbolType == SpiceParser::INLINE)
      specifierNode->type = SpecifierNode::TY_INLINE;
    else if (symbolType == SpiceParser::PUBLIC)
      specifierNode->type = SpecifierNode::TY_PUBLIC;
    else if (symbolType == SpiceParser::HEAP)
      specifierNode->type = SpecifierNode::TY_HEAP;
    else if (symbolType == SpiceParser::COMPOSE)
      specifierNode->type = SpecifierNode::TY_COMPOSITION;
    else
      assert_fail("Unknown specifier type"); // GCOV_EXCL_LINE
  }

  return concludeNode(specifierNode);
}

std::any ASTBuilder::visitModAttr(SpiceParser::ModAttrContext *ctx) {
  auto modAttrNode = createNode<ModAttrNode>(ctx);

  // Visit children
  modAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : modAttrNode->attrLst->attributes)
    attr->target = AttrNode::TARGET_MODULE;

  return concludeNode(modAttrNode);
}

std::any ASTBuilder::visitTopLevelDefAttr(SpiceParser::TopLevelDefAttrContext *ctx) {
  auto fctAttrNode = createNode<TopLevelDefinitionAttrNode>(ctx);

  // Visit children
  fctAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  return concludeNode(fctAttrNode);
}

std::any ASTBuilder::visitLambdaAttr(SpiceParser::LambdaAttrContext *ctx) {
  auto lambdaAttrNode = createNode<LambdaAttrNode>(ctx);

  // Visit children
  lambdaAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : lambdaAttrNode->attrLst->attributes)
    attr->target = AttrNode::TARGET_LAMBDA;

  return concludeNode(lambdaAttrNode);
}

std::any ASTBuilder::visitAttrLst(SpiceParser::AttrLstContext *ctx) {
  auto attrLstNode = createNode<AttrLstNode>(ctx);

  // Visit children
  for (SpiceParser::AttrContext *attr : ctx->attr())
    attrLstNode->attributes.push_back(std::any_cast<AttrNode *>(visit(attr)));

  return concludeNode(attrLstNode);
}

std::any ASTBuilder::visitAttr(SpiceParser::AttrContext *ctx) {
  auto attrNode = createNode<AttrNode>(ctx);

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
      attrNode->type = AttrNode::TYPE_STRING;
    else if (ctx->constant()->INT_LIT())
      attrNode->type = AttrNode::TYPE_INT;
    else if (ctx->constant()->TRUE() || ctx->constant()->FALSE())
      attrNode->type = AttrNode::TYPE_BOOL;
    else
      throw ParserError(attrNode->value->codeLoc, INVALID_ATTR_VALUE_TYPE, "Invalid attribute value type");
  } else {
    // If no value is given, use the bool type
    attrNode->type = AttrNode::TYPE_BOOL;
  }

  return concludeNode(attrNode);
}

std::any ASTBuilder::visitCaseConstant(SpiceParser::CaseConstantContext *ctx) {
  auto caseConstantNode = createNode<CaseConstantNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = getIdentifier(t1);
      caseConstantNode->identifierFragments.push_back(fragment);
      if (!caseConstantNode->fqIdentifier.empty())
        caseConstantNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      caseConstantNode->fqIdentifier += fragment;
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = getIdentifier(t2);
      caseConstantNode->identifierFragments.push_back(fragment);
      if (!caseConstantNode->fqIdentifier.empty())
        caseConstantNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      caseConstantNode->fqIdentifier += fragment;
    }
  }

  return concludeNode(caseConstantNode);
}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  auto returnStmtNode = createNode<ReturnStmtNode>(ctx);

  // Enrich
  returnStmtNode->hasReturnValue = ctx->assignExpr();

  // Visit children
  visitChildren(ctx);

  return concludeNode(returnStmtNode);
}

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  auto breakStmtNode = createNode<BreakStmtNode>(ctx);

  // Extract number of breaks
  if (ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  // Visit children
  visitChildren(ctx);

  return concludeNode(breakStmtNode);
}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  auto continueStmtNode = createNode<ContinueStmtNode>(ctx);

  // Extract number of continues
  if (ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  // Visit children
  visitChildren(ctx);

  return concludeNode(continueStmtNode);
}

std::any ASTBuilder::visitFallthroughStmt(SpiceParser::FallthroughStmtContext *ctx) {
  auto fallthroughStmtNode = createNode<FallthroughStmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(fallthroughStmtNode);
}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertStmtNode = createNode<AssertStmtNode>(ctx);

  // Enrich
  antlr4::misc::Interval interval(ctx->assignExpr()->start->getStartIndex(), ctx->assignExpr()->stop->getStopIndex());
  assertStmtNode->expressionString = inputStream->getText(interval);

  // Visit children
  visitChildren(ctx);

  return concludeNode(assertStmtNode);
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  auto builtinCallNode = createNode<BuiltinCallNode>(ctx);

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
  } else {
    assert_fail("Unknown builtin call"); // GCOV_EXCL_LINE
  }

  return concludeNode(builtinCallNode);
}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  auto printfCallNode = createNode<PrintfCallNode>(ctx);

  // Enrich
  std::string templatedString = ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  // Visit children
  visitChildren(ctx);

  return concludeNode(printfCallNode);
}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  auto sizeofCallNode = createNode<SizeofCallNode>(ctx);

  // Check if type or value
  sizeofCallNode->isType = ctx->TYPE();

  // Visit children
  visitChildren(ctx);

  return concludeNode(sizeofCallNode);
}

std::any ASTBuilder::visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) {
  auto alignofCallNode = createNode<AlignofCallNode>(ctx);

  // Check if type or value
  alignofCallNode->isType = ctx->TYPE();

  // Visit children
  visitChildren(ctx);

  return concludeNode(alignofCallNode);
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto lenCallNode = createNode<LenCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(lenCallNode);
}

std::any ASTBuilder::visitPanicCall(SpiceParser::PanicCallContext *ctx) {
  auto panicCallNode = createNode<PanicCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(panicCallNode);
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  auto assignExprNode = createNode<AssignExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(assignExprNode);
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  auto ternaryExprNode = createNode<TernaryExprNode>(ctx);

  // Check if is shortened
  ternaryExprNode->isShortened = ctx->logicalOrExpr().size() == 2;

  // Visit children
  visitChildren(ctx);

  return concludeNode(ternaryExprNode);
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  auto logicalOrExprNode = createNode<LogicalOrExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(logicalOrExprNode);
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  auto logicalAndExprNode = createNode<LogicalAndExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(logicalAndExprNode);
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  auto bitwiseOrExprNode = createNode<BitwiseOrExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(bitwiseOrExprNode);
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  auto bitwiseXorExprNode = createNode<BitwiseXorExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(bitwiseXorExprNode);
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  auto bitwiseAndExprNode = createNode<BitwiseAndExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(bitwiseAndExprNode);
}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  auto equalityExprNode = createNode<EqualityExprNode>(ctx);

  // Extract operator
  if (ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_EQUAL;
  else if (ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_NOT_EQUAL;

  // Visit children
  visitChildren(ctx);

  return concludeNode(equalityExprNode);
}

std::any ASTBuilder::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  auto relationalExprNode = createNode<RelationalExprNode>(ctx);

  // Extract operator
  if (ctx->LESS())
    relationalExprNode->op = RelationalExprNode::OP_LESS;
  else if (ctx->GREATER())
    relationalExprNode->op = RelationalExprNode::OP_GREATER;
  else if (ctx->LESS_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_LESS_EQUAL;
  else if (ctx->GREATER_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_GREATER_EQUAL;

  // Visit children
  visitChildren(ctx);

  return concludeNode(relationalExprNode);
}

std::any ASTBuilder::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  auto shiftExprNode = createNode<ShiftExprNode>(ctx);

  // Extract operator
  if (!ctx->LESS().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_LEFT;
  else if (!ctx->GREATER().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_RIGHT;

  // Visit children
  visitChildren(ctx);

  return concludeNode(shiftExprNode);
}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  auto additiveExprNode = createNode<AdditiveExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_PLUS, TY_INVALID);
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_MINUS, TY_INVALID);
  }

  return concludeNode(additiveExprNode);
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  auto multiplicativeExprNode = createNode<MultiplicativeExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_MUL, TY_INVALID);
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_DIV, TY_INVALID);
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_REM, TY_INVALID);
  }

  return concludeNode(multiplicativeExprNode);
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  auto castExprNode = createNode<CastExprNode>(ctx);

  if (ctx->dataType()) {
    castExprNode->isCast = true;
    castExprNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  }
  castExprNode->prefixUnaryExpr = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));

  return concludeNode(castExprNode);
}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  auto prefixUnaryExprNode = createNode<PrefixUnaryExprNode>(ctx);

  // Visit children
  if (ctx->postfixUnaryExpr()) {
    prefixUnaryExprNode->postfixUnaryExpr = std::any_cast<PostfixUnaryExprNode *>(visit(ctx->postfixUnaryExpr()));
  } else if (ctx->prefixUnaryExpr()) {
    // Extract operator
    if (ctx->MINUS())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_MINUS;
    else if (ctx->PLUS_PLUS())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_PLUS_PLUS;
    else if (ctx->MINUS_MINUS())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_MINUS_MINUS;
    else if (ctx->NOT())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_NOT;
    else if (ctx->BITWISE_NOT())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_BITWISE_NOT;
    else if (ctx->MUL())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_DEREFERENCE;
    else if (ctx->BITWISE_AND())
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_ADDRESS_OF;

    prefixUnaryExprNode->prefixUnaryExpr = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));
  } else {
    assert_fail("Unknown prefix unary expression type"); // GCOV_EXCL_LINE
  }

  return concludeNode(prefixUnaryExprNode);
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto postfixUnaryExprNode = createNode<PostfixUnaryExprNode>(ctx);

  if (ctx->atomicExpr()) {
    postfixUnaryExprNode->atomicExpr = std::any_cast<AtomicExprNode *>(visit(ctx->atomicExpr()));
  } else if (ctx->postfixUnaryExpr()) {
    postfixUnaryExprNode->postfixUnaryExpr = std::any_cast<PostfixUnaryExprNode *>(visit(ctx->postfixUnaryExpr()));

    // Extract operator
    if (ctx->assignExpr()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_SUBSCRIPT;
      postfixUnaryExprNode->subscriptIndexExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
    } else if (ctx->IDENTIFIER()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MEMBER_ACCESS;
      postfixUnaryExprNode->identifier = getIdentifier(ctx->IDENTIFIER());
    } else if (ctx->PLUS_PLUS()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_PLUS_PLUS;
    } else if (ctx->MINUS_MINUS()) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MINUS_MINUS;
    }
  } else {
    assert_fail("Unknown postfix unary expression type"); // GCOV_EXCL_LINE
  }

  return concludeNode(postfixUnaryExprNode);
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  auto atomicExprNode = createNode<AtomicExprNode>(ctx);

  // Visit children
  if (ctx->constant()) {
    atomicExprNode->constant = std::any_cast<ConstantNode *>(visit(ctx->constant()));
  } else if (ctx->value()) {
    atomicExprNode->value = std::any_cast<ValueNode *>(visit(ctx->value()));
  } else if (!ctx->IDENTIFIER().empty() || !ctx->TYPE_IDENTIFIER().empty()) {
    for (ParserRuleContext::ParseTree *subTree : ctx->children) {
      if (auto t1 = dynamic_cast<TerminalNode *>(subTree);
          t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
        std::string fragment = getIdentifier(t1);
        atomicExprNode->identifierFragments.push_back(fragment);
        if (!atomicExprNode->fqIdentifier.empty())
          atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
        atomicExprNode->fqIdentifier += fragment;
      } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree);
                 t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        std::string fragment = getIdentifier(t2);
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
  auto valueNode = createNode<ValueNode>(ctx);

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
  auto constantNode = createNode<ConstantNode>(ctx);

  // Enrich
  CompileTimeValue &value = constantNode->compileTimeValue;
  if (ctx->DOUBLE_LIT()) {
    constantNode->type = ConstantNode::TYPE_DOUBLE;
    value.doubleValue = std::stod(ctx->DOUBLE_LIT()->toString());
  } else if (ctx->INT_LIT()) {
    constantNode->type = ConstantNode::TYPE_INT;
    value.intValue = parseInt(ctx->INT_LIT());
  } else if (ctx->SHORT_LIT()) {
    constantNode->type = ConstantNode::TYPE_SHORT;
    value.shortValue = parseShort(ctx->SHORT_LIT());
  } else if (ctx->LONG_LIT()) {
    constantNode->type = ConstantNode::TYPE_LONG;
    value.longValue = parseLong(ctx->LONG_LIT());
  } else if (ctx->CHAR_LIT()) {
    constantNode->type = ConstantNode::TYPE_CHAR;
    value.charValue = parseChar(ctx->CHAR_LIT());
  } else if (ctx->STRING_LIT()) {
    // Save a pointer to the string in the compile time value
    constantNode->type = ConstantNode::TYPE_STRING;
    value.stringValueOffset = resourceManager.compileTimeStringValues.size();
    // Add the string to the global compile time string list
    resourceManager.compileTimeStringValues.push_back(parseString(ctx->STRING_LIT()->toString()));
  } else if (ctx->TRUE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    value.boolValue = true;
  } else if (ctx->FALSE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    value.boolValue = false;
  } else {
    assert_fail("Unknown constant type"); // GCOV_EXCL_LINE
  }

  return concludeNode(constantNode);
}

std::any ASTBuilder::visitFctCall(SpiceParser::FctCallContext *ctx) {
  auto fctCallNode = createNode<FctCallNode>(ctx);

  for (antlr4::ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (auto t3 = dynamic_cast<TerminalNode *>(subTree);
               t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::SCOPE_ACCESS) {
      fctCallNode->fqFunctionName += SCOPE_ACCESS_TOKEN;
    } else if (auto t4 = dynamic_cast<TerminalNode *>(subTree); t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::DOT) {
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
  auto arrayInitializationNode = createNode<ArrayInitializationNode>(ctx);

  // Visit children
  if (ctx->argLst())
    arrayInitializationNode->itemLst = std::any_cast<ArgLstNode *>(visit(ctx->argLst()));

  return concludeNode(arrayInitializationNode);
}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  auto structInstantiationNode = createNode<StructInstantiationNode>(ctx);

  // Enrich
  for (antlr4::ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
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
  auto lambdaFuncNode = createNode<LambdaFuncNode>(ctx);

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
  auto lambdaProcNode = createNode<LambdaProcNode>(ctx);

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
  auto lambdaExprNode = createNode<LambdaExprNode>(ctx);

  // Visit children
  if (ctx->paramLst()) {
    lambdaExprNode->hasParams = true;
    lambdaExprNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
  }
  lambdaExprNode->lambdaExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(lambdaExprNode);
}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto dataTypeNode = createNode<DataTypeNode>(ctx);

  // Visit children
  if (ctx->specifierLst())
    dataTypeNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));
  dataTypeNode->baseDataType = std::any_cast<BaseDataTypeNode *>(visit(ctx->baseDataType()));

  // Enrich
  for (size_t i = 0; i < ctx->children.size(); i++) {
    antlr4::tree::ParseTree *subTree = ctx->children[i];
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL) {
      dataTypeNode->tmQueue.emplace(DataTypeNode::TYPE_PTR, false, 0);
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::BITWISE_AND) {
      dataTypeNode->tmQueue.emplace(DataTypeNode::TYPE_REF, false, 0);
    } else if (auto t3 = dynamic_cast<TerminalNode *>(subTree);
               t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = ctx->children[i];
      bool hasSize = false;
      unsigned int hardCodedSize = 0;
      std::string sizeVarName;
      if (auto t4 = dynamic_cast<TerminalNode *>(subTree); t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        hardCodedSize = std::stoi(t4->getSymbol()->getText());
        i++; // Consume INT_LIT
      } else if (auto t5 = dynamic_cast<TerminalNode *>(subTree);
                 t5 != nullptr && t5->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        hasSize = true;
        sizeVarName = getIdentifier(t5);
        i++; // Consume TYPE_IDENTIFIER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_ARRAY, hasSize, hardCodedSize, sizeVarName});
    }
  }

  return concludeNode(dataTypeNode);
}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  auto baseDataTypeNode = createNode<BaseDataTypeNode>(ctx);

  // Enrich
  if (ctx->TYPE_DOUBLE()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_DOUBLE;
  } else if (ctx->TYPE_INT()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_INT;
  } else if (ctx->TYPE_SHORT()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_SHORT;
  } else if (ctx->TYPE_LONG()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_LONG;
  } else if (ctx->TYPE_BYTE()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_BYTE;
  } else if (ctx->TYPE_CHAR()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CHAR;
  } else if (ctx->TYPE_STRING()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_STRING;
  } else if (ctx->TYPE_BOOL()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_BOOL;
  } else if (ctx->TYPE_DYN()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_DYN;
  } else if (ctx->customDataType()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CUSTOM;
    baseDataTypeNode->customDataType = std::any_cast<CustomDataTypeNode *>(visit(ctx->customDataType()));
  } else if (ctx->functionDataType()) {
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_FUNCTION;
    baseDataTypeNode->functionDataType = std::any_cast<FunctionDataTypeNode *>(visit(ctx->functionDataType()));
  } else {
    assert_fail("Unknown base data type");
  }

  return concludeNode(baseDataTypeNode);
}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  auto customDataTypeNode = createNode<CustomDataTypeNode>(ctx);

  // Enrich
  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
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
  auto functionDataTypeNode = createNode<FunctionDataTypeNode>(ctx);

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
  auto assignExprNode = spice_pointer_cast<AssignExprNode *>(parentStack.top());

  // Extract assign operator
  if (ctx->ASSIGN())
    assignExprNode->op = AssignExprNode::OP_ASSIGN;
  else if (ctx->PLUS_EQUAL())
    assignExprNode->op = AssignExprNode::OP_PLUS_EQUAL;
  else if (ctx->MINUS_EQUAL())
    assignExprNode->op = AssignExprNode::OP_MINUS_EQUAL;
  else if (ctx->MUL_EQUAL())
    assignExprNode->op = AssignExprNode::OP_MUL_EQUAL;
  else if (ctx->DIV_EQUAL())
    assignExprNode->op = AssignExprNode::OP_DIV_EQUAL;
  else if (ctx->REM_EQUAL())
    assignExprNode->op = AssignExprNode::OP_REM_EQUAL;
  else if (ctx->SHL_EQUAL())
    assignExprNode->op = AssignExprNode::OP_SHL_EQUAL;
  else if (ctx->SHR_EQUAL())
    assignExprNode->op = AssignExprNode::OP_SHR_EQUAL;
  else if (ctx->AND_EQUAL())
    assignExprNode->op = AssignExprNode::OP_AND_EQUAL;
  else if (ctx->OR_EQUAL())
    assignExprNode->op = AssignExprNode::OP_OR_EQUAL;
  else if (ctx->XOR_EQUAL())
    assignExprNode->op = AssignExprNode::OP_XOR_EQUAL;
  else
    assert_fail("Unknown assign operator");

  return nullptr;
}

std::any ASTBuilder::visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) {
  auto fctNameNode = spice_pointer_cast<FctNameNode *>(parentStack.top());

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
  else
    assert_fail("Unsupported overloadable operator"); // GCOV_EXCL_LINE

  fctNameNode->fqName = fctNameNode->name;
  fctNameNode->nameFragments.push_back(fctNameNode->name);

  return nullptr;
}

template <typename T> T *ASTBuilder::createNode(const ParserRuleContext *ctx) {
  ASTNode *parent = nullptr;
  if constexpr (!std::is_same_v<T, EntryNode>)
    parent = parentStack.top();

  // Create the new node
  T *node = resourceManager.astNodeAlloc.allocate<T>(getCodeLoc(ctx));
  resourceManager.astNodes.push_back(node);

  // If this is not the entry node, we need to add the new node to its parent
  if constexpr (!std::is_same_v<T, EntryNode>)
    parent->addChild(node);

  // This node is the parent for its children
  parentStack.push(node);

  return node;
}

template <typename T> T *ASTBuilder::concludeNode(T *node) {
  // This node is no longer the parent for its children
  assert(parentStack.top() == node);
  parentStack.pop();
  return node;
}

template <typename T> T *ASTBuilder::resumeForExpansion() { return spice_pointer_cast<T *>(parentStack.top()); }

int32_t ASTBuilder::parseInt(TerminalNode *terminal) {
  NumericParserCallback<int32_t> cb = [](const std::string &substr, short base, bool isSigned) -> int32_t {
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
  NumericParserCallback<int16_t> cb = [](const std::string &substr, short base, bool isSigned) -> int16_t {
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
    if (isSigned)
      return static_cast<int64_t>(std::stoll(substr, nullptr, base));
    else
      return static_cast<int64_t>(std::stoull(substr, nullptr, base));
  };
  return parseNumeric(terminal, cb);
}

int8_t ASTBuilder::parseChar(TerminalNode *terminal) {
  const std::string input = terminal->toString();
  if (input.length() == 3) { // Normal char literals
    return input[1];
  } else if (input.length() == 4 && input[1] == '\\') { // Char literals with escape sequence
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
  } else {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid char literal " + input);
  }
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
        case 'b':
        case 'B':
          return cb(subStr, 2, !isUnsigned);
        case 'h':
        case 'H':
        case 'x':
        case 'X':
          return cb(subStr, 16, !isUnsigned);
        case 'o':
        case 'O':
          return cb(subStr, 8, !isUnsigned);
        default:
          return cb(input, 10, !isUnsigned);
        }
      }
    }
    return cb(input, 10, !isUnsigned);
  } catch (std::out_of_range &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "You tried to parse '" + input + "' as an integer, but it was no integer");
  }
}

void ASTBuilder::replaceEscapeChars(std::string &input) {
  std::unordered_map<char, char> escapeMap = {
      {'a', '\a'}, {'b', '\b'},  {'f', '\f'}, {'n', '\n'},  {'r', '\r'}, {'t', '\t'},
      {'v', '\v'}, {'\\', '\\'}, {'?', '\?'}, {'\'', '\''}, {'"', '\"'},
  };

  size_t writeIndex = 0; // Index where the next character should be written
  for (size_t readIndex = 0; readIndex < input.length(); ++readIndex, ++writeIndex) {
    if (input[readIndex] == '\\' && readIndex + 1 < input.length()) {
      char nextChar = input[readIndex + 1];
      if (escapeMap.find(nextChar) != escapeMap.end()) {
        // If the next character forms a valid escape sequence, replace it
        input[writeIndex] = escapeMap[nextChar];
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

std::string ASTBuilder::getIdentifier(TerminalNode *terminal) {
  std::string identifier = terminal->getText();

  // Check if the identifier is 'String' and this is no std source file
  bool isReserved = !sourceFile->isStdFile && (identifier == STROBJ_NAME || identifier == RESULTOBJ_NAME);
  // Check if the list of reserved keywords contains the given identifier
  isReserved |= std::find(std::begin(RESERVED_KEYWORDS), std::end(RESERVED_KEYWORDS), identifier) != std::end(RESERVED_KEYWORDS);
  // Print error message
  if (isReserved) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, RESERVED_KEYWORD, "'" + identifier + "' is a reserved keyword. Please use another name instead");
  }

  return identifier;
}

} // namespace spice::compiler
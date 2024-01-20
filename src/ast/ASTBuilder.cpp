// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <exception/ParserError.h>
#include <typechecker/OpRuleManager.h>
#include <util/CommonUtil.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

#define AST_BUILDER_FAST_PATH(ctx, condition)                                                                                    \
  if (condition) [[likely]] {                                                                                                    \
    return visit(ctx->children.front());                                                                                         \
  }

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

  if (ctx->topLevelDefAttr()) {
    mainFctDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are struct attributes
    for (AttrNode *attr : mainFctDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }

  if (ctx->paramLst()) {
    mainFctDefNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
    mainFctDefNode->takesArgs = true;
  }

  mainFctDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(mainFctDefNode);
}

std::any ASTBuilder::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  auto fctDefNode = createNode<FctDefNode>(ctx);

  if (ctx->topLevelDefAttr()) {
    fctDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are struct attributes
    for (AttrNode *attr : fctDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }

  if (ctx->specifierLst())
    fctDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  // Tell the return type that it is one
  fctDefNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  fctDefNode->returnType->isReturnType = true;

  fctDefNode->name = std::any_cast<FctNameNode *>(visit(ctx->fctName()));
  fctDefNode->isMethod = fctDefNode->name->nameFragments.size() > 1;

  if (ctx->typeLst()) {
    fctDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
    fctDefNode->hasTemplateTypes = true;
  }

  if (ctx->paramLst()) {
    fctDefNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
    fctDefNode->hasParams = true;
  }

  return concludeNode(fctDefNode);
}

std::any ASTBuilder::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  auto procDefNode = createNode<ProcDefNode>(ctx);

  if (ctx->topLevelDefAttr()) {
    procDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are struct attributes
    for (AttrNode *attr : procDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_FCT_PROC;
  }

  if (ctx->specifierLst())
    procDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  procDefNode->name = std::any_cast<FctNameNode *>(visit(ctx->fctName()));
  procDefNode->isMethod = procDefNode->name->nameFragments.size() > 1;

  if (ctx->typeLst()) {
    procDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
    procDefNode->hasTemplateTypes = true;
  }

  if (ctx->paramLst()) {
    procDefNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
    procDefNode->hasParams = true;
  }

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
  } else {
    using OverloadedOpPair = std::pair<FctNameNode::OverloadedOperator, const char *>;
    const auto [overloadedOp, overloadedOpName] = std::any_cast<OverloadedOpPair>(visit(ctx->overloadableOp()));
    fctNameNode->overloadedOperator = overloadedOp;
    fctNameNode->fqName = overloadedOpName;
    fctNameNode->nameFragments.emplace_back(overloadedOpName);
  }

  return concludeNode(fctNameNode);
}

std::any ASTBuilder::visitStructDef(SpiceParser::StructDefContext *ctx) {
  auto structDefNode = createNode<StructDefNode>(ctx);

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

  structDefNode->structName = getIdentifier(ctx->TYPE_IDENTIFIER());
  structDefNode->typeId = resourceManager.getNextCustomTypeId();

  if (ctx->LESS()) {
    structDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(0)));
    structDefNode->hasTemplateTypes = true;
  }

  if (ctx->COLON()) {
    const size_t typeLstIdx = structDefNode->hasTemplateTypes ? 1 : 0;
    structDefNode->interfaceTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(typeLstIdx)));
    structDefNode->hasInterfaces = true;
  }

  for (SpiceParser::FieldContext *fieldCtx : ctx->field())
    structDefNode->fields.push_back(std::any_cast<FieldNode *>(visit(fieldCtx)));

  return concludeNode(structDefNode);
}

std::any ASTBuilder::visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) {
  auto interfaceDefNode = createNode<InterfaceDefNode>(ctx);

  if (ctx->topLevelDefAttr()) {
    interfaceDefNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    // Tell the attributes that they are interface attributes
    for (AttrNode *attr : interfaceDefNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_INTERFACE;
    // Check if a custom type id was set
    if (interfaceDefNode->attrs && interfaceDefNode->attrs->attrLst->hasAttr(ATTR_CORE_COMPILER_FIXED_TYPE_ID))
      interfaceDefNode->typeId = interfaceDefNode->attrs->attrLst->getAttrValueByName(ATTR_CORE_COMPILER_FIXED_TYPE_ID)->intValue;
  }

  if (ctx->specifierLst())
    interfaceDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  interfaceDefNode->interfaceName = getIdentifier(ctx->TYPE_IDENTIFIER());
  interfaceDefNode->typeId = resourceManager.getNextCustomTypeId();

  if (ctx->LESS()) {
    interfaceDefNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
    interfaceDefNode->hasTemplateTypes = true;
  }

  for (SpiceParser::SignatureContext *signatureCtx : ctx->signature())
    interfaceDefNode->signatures.push_back(std::any_cast<SignatureNode *>(visit(signatureCtx)));

  return concludeNode(interfaceDefNode);
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  auto enumDefNode = createNode<EnumDefNode>(ctx);

  if (ctx->specifierLst())
    enumDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  enumDefNode->enumName = getIdentifier(ctx->TYPE_IDENTIFIER());
  enumDefNode->typeId = resourceManager.getNextCustomTypeId();

  enumDefNode->itemLst = std::any_cast<EnumItemLstNode *>(visit(ctx->enumItemLst()));
  // Tell all items about the enum def
  for (EnumItemNode *enumItem : enumDefNode->itemLst->items)
    enumItem->enumDef = enumDefNode;

  return concludeNode(enumDefNode);
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  auto genericTypeDefNode = createNode<GenericTypeDefNode>(ctx);

  genericTypeDefNode->typeName = getIdentifier(ctx->TYPE_IDENTIFIER());

  genericTypeDefNode->typeAltsLst = std::any_cast<TypeAltsLstNode *>(visit(ctx->typeAltsLst()));

  return concludeNode(genericTypeDefNode);
}

std::any ASTBuilder::visitAliasDef(SpiceParser::AliasDefContext *ctx) {
  auto aliasDefNode = createNode<AliasDefNode>(ctx);

  if (ctx->specifierLst())
    aliasDefNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  aliasDefNode->aliasName = getIdentifier(ctx->TYPE_IDENTIFIER());
  aliasDefNode->dataTypeString = ctx->dataType()->getText();

  aliasDefNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  return concludeNode(aliasDefNode);
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  auto globalVarDefNode = createNode<GlobalVarDefNode>(ctx);

  globalVarDefNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  globalVarDefNode->dataType->isGlobalType = true; // Tell the data type that it is a global one

  globalVarDefNode->varName = getIdentifier(ctx->TYPE_IDENTIFIER());

  if (ctx->ASSIGN())
    globalVarDefNode->constant = std::any_cast<ConstantNode *>(visit(ctx->constant()));

  return concludeNode(globalVarDefNode);
}

std::any ASTBuilder::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  auto extDeclNode = createNode<ExtDeclNode>(ctx);

  if (ctx->topLevelDefAttr()) {
    extDeclNode->attrs = std::any_cast<TopLevelDefinitionAttrNode *>(visit(ctx->topLevelDefAttr()));
    for (AttrNode *attr : extDeclNode->attrs->attrLst->attributes)
      attr->target = AttrNode::TARGET_EXT_DECL;
  }

  if (ctx->F())
    extDeclNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER() ? ctx->IDENTIFIER() : ctx->TYPE_IDENTIFIER());

  if (ctx->typeLst()) {
    extDeclNode->argTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));
    extDeclNode->hasArgs = true;
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

  unsafeBlockDefNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(unsafeBlockDefNode);
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto forLoopNode = createNode<ForLoopNode>(ctx);

  forLoopNode->initDecl = std::any_cast<DeclStmtNode *>(visit(ctx->forHead()->declStmt()));
  forLoopNode->condAssign = std::any_cast<AssignExprNode *>(visit(ctx->forHead()->assignExpr(0)));
  forLoopNode->incAssign = std::any_cast<AssignExprNode *>(visit(ctx->forHead()->assignExpr(1)));

  return concludeNode(forLoopNode);
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  visitChildren(ctx);
  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto foreachLoopNode = createNode<ForeachLoopNode>(ctx);

  const bool hasIdxDecl = ctx->foreachHead()->declStmt().size() == 2;
  if (hasIdxDecl)
    foreachLoopNode->idxVarDecl = std::any_cast<DeclStmtNode *>(visit(ctx->foreachHead()->declStmt(0)));

  foreachLoopNode->itemVarDecl = std::any_cast<DeclStmtNode *>(visit(ctx->foreachHead()->declStmt(hasIdxDecl ? 1 : 0)));
  foreachLoopNode->itemVarDecl->isForEachItem = true; // Tell the foreach item that it is one

  foreachLoopNode->iteratorAssign = std::any_cast<AssignExprNode *>(visit(ctx->foreachHead()->assignExpr()));

  return concludeNode(foreachLoopNode);
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  visitChildren(ctx);
  return nullptr;
}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  auto whileLoopNode = createNode<WhileLoopNode>(ctx);

  whileLoopNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  whileLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(whileLoopNode);
}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {
  auto doWhileLoopNode = createNode<DoWhileLoopNode>(ctx);

  doWhileLoopNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  doWhileLoopNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(doWhileLoopNode);
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  auto ifStmtNode = createNode<IfStmtNode>(ctx);

  ifStmtNode->condition = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  ifStmtNode->thenBody = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  if (ctx->elseStmt())
    ifStmtNode->elseStmt = std::any_cast<ElseStmtNode *>(visit(ctx->elseStmt()));

  return concludeNode(ifStmtNode);
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  auto elseStmtNode = createNode<ElseStmtNode>(ctx);

  if (ctx->ifStmt()) {
    elseStmtNode->ifStmt = std::any_cast<IfStmtNode *>(visit(ctx->ifStmt()));
    elseStmtNode->isElseIf = true;
  } else {
    elseStmtNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));
  }

  return concludeNode(elseStmtNode);
}

std::any ASTBuilder::visitSwitchStmt(SpiceParser::SwitchStmtContext *ctx) {
  auto switchStmtNode = createNode<SwitchStmtNode>(ctx);

  switchStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  for (SpiceParser::CaseBranchContext *caseBranchCtx : ctx->caseBranch())
    switchStmtNode->caseBranches.push_back(std::any_cast<CaseBranchNode *>(visit(caseBranchCtx)));

  if (ctx->defaultBranch()) {
    switchStmtNode->defaultBranch = std::any_cast<DefaultBranchNode *>(visit(ctx->defaultBranch()));
    switchStmtNode->hasDefaultBranch = true;
  }

  return concludeNode(switchStmtNode);
}

std::any ASTBuilder::visitCaseBranch(SpiceParser::CaseBranchContext *ctx) {
  auto caseBranchNode = createNode<CaseBranchNode>(ctx);

  caseBranchNode->constantLst = std::any_cast<ConstantLstNode *>(visit(ctx->constantLst()));
  caseBranchNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(caseBranchNode);
}

std::any ASTBuilder::visitDefaultBranch(SpiceParser::DefaultBranchContext *ctx) {
  auto defaultBranchNode = createNode<DefaultBranchNode>(ctx);

  defaultBranchNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(defaultBranchNode);
}

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  auto anonymousBlockStmtNode = createNode<AnonymousBlockStmtNode>(ctx);

  anonymousBlockStmtNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(anonymousBlockStmtNode);
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = createNode<StmtLstNode>(ctx);

  for (SpiceParser::StmtContext *stmtCtx : ctx->stmt())
    stmtLstNode->stmts.push_back(std::any_cast<StmtNode *>(visit(stmtCtx)));

  return concludeNode(stmtLstNode);
}

std::any ASTBuilder::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  auto typeLstNode = createNode<TypeLstNode>(ctx);

  for (SpiceParser::DataTypeContext *dataTypeCtx : ctx->dataType())
    typeLstNode->dataTypes.push_back(std::any_cast<DataTypeNode *>(visit(dataTypeCtx)));

  return concludeNode(typeLstNode);
}

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = createNode<TypeAltsLstNode>(ctx);

  for (SpiceParser::DataTypeContext *dataTypeCtx : ctx->dataType())
    typeAltsLstNode->dataTypes.push_back(std::any_cast<DataTypeNode *>(visit(dataTypeCtx)));

  return concludeNode(typeAltsLstNode);
}

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  auto paramLstNode = createNode<ParamLstNode>(ctx);

  for (SpiceParser::DeclStmtContext *paramCtx : ctx->declStmt()) {
    auto param = std::any_cast<DeclStmtNode *>(visit(paramCtx));
    param->isParam = true;
    param->dataType->isParamType = true;
    paramLstNode->params.push_back(param);
  }

  return concludeNode(paramLstNode);
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  auto argLstNode = createNode<ArgLstNode>(ctx);

  for (SpiceParser::AssignExprContext *argCtx : ctx->assignExpr())
    argLstNode->args.push_back(std::any_cast<AssignExprNode *>(visit(argCtx)));

  return concludeNode(argLstNode);
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  auto enumItemLstNode = createNode<EnumItemLstNode>(ctx);

  for (SpiceParser::EnumItemContext *enumItemCtx : ctx->enumItem())
    enumItemLstNode->items.push_back(std::any_cast<EnumItemNode *>(visit(enumItemCtx)));

  return concludeNode(enumItemLstNode);
}

std::any ASTBuilder::visitEnumItem(SpiceParser::EnumItemContext *ctx) {
  auto enumItemNode = createNode<EnumItemNode>(ctx);

  enumItemNode->itemName = getIdentifier(ctx->TYPE_IDENTIFIER());
  if (ctx->ASSIGN()) {
    enumItemNode->itemValue = parseInt(nullptr, ctx->INT_LIT());
    enumItemNode->hasValue = true;
  }

  return concludeNode(enumItemNode);
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = createNode<FieldNode>(ctx);

  fieldNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  fieldNode->dataType->setFieldTypeRecursive(); // Tell the data type that it is a field type

  fieldNode->fieldName = getIdentifier(ctx->IDENTIFIER());

  if (ctx->ternaryExpr())
    fieldNode->defaultValue = std::any_cast<TernaryExprNode *>(visit(ctx->ternaryExpr()));

  return concludeNode(fieldNode);
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {
  auto signatureNode = createNode<SignatureNode>(ctx);

  signatureNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  if (ctx->F()) {
    signatureNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
    signatureNode->signatureType = SignatureNode::TYPE_FUNCTION;
    signatureNode->hasTemplateTypes = ctx->LESS().size() == 2;
    signatureNode->signatureSpecifiers = TypeSpecifiers::of(TY_FUNCTION);
  } else {
    signatureNode->signatureType = SignatureNode::TYPE_PROCEDURE;
    signatureNode->hasTemplateTypes = ctx->LESS().size() == 1;
    signatureNode->signatureSpecifiers = TypeSpecifiers::of(TY_PROCEDURE);
  }

  signatureNode->methodName = getIdentifier(ctx->IDENTIFIER());

  if (signatureNode->hasTemplateTypes)
    signatureNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(0)));

  if (signatureNode->hasParams)
    signatureNode->paramTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst(signatureNode->hasTemplateTypes ? 1 : 0)));

  return concludeNode(signatureNode);
}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) {
  auto stmtNode = createNode<StmtNode>(ctx);
  visitChildren(ctx);
  return concludeNode(stmtNode);
}

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  auto declStmtNode = createNode<DeclStmtNode>(ctx);

  declStmtNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
  if (ctx->assignExpr())
    declStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
  declStmtNode->varName = getIdentifier(ctx->IDENTIFIER());
  declStmtNode->hasAssignment = ctx->ASSIGN();

  return concludeNode(declStmtNode);
}

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  auto specifierLstNode = createNode<SpecifierLstNode>(ctx);

  bool seenSignedOrUnsigned = false;
  for (SpiceParser::SpecifierContext *specifierCtx : ctx->specifier()) {
    auto specifier = std::any_cast<SpecifierNode *>(visit(specifierCtx));
    if (specifier->type == SpecifierNode::TY_SIGNED || specifier->type == SpecifierNode::TY_UNSIGNED) {
      if (seenSignedOrUnsigned)
        throw ParserError(specifier->codeLoc, INVALID_SPECIFIER_COMBINATION, "A variable can not be signed and unsigned");
      seenSignedOrUnsigned = true;
    }
    specifierLstNode->specifiers.push_back(specifier);
  }

  return concludeNode(specifierLstNode);
}

std::any ASTBuilder::visitSpecifier(SpiceParser::SpecifierContext *ctx) {
  auto specifierNode = createNode<SpecifierNode>(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    auto token = spice_pointer_cast<TerminalNode *>(subTree);
    size_t symbolType = token->getSymbol()->getType();
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

  modAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));
  // Tell the attributes that they are module attributes
  for (AttrNode *attr : modAttrNode->attrLst->attributes)
    attr->target = AttrNode::TARGET_MODULE;

  return concludeNode(modAttrNode);
}

std::any ASTBuilder::visitTopLevelDefAttr(SpiceParser::TopLevelDefAttrContext *ctx) {
  auto fctAttrNode = createNode<TopLevelDefinitionAttrNode>(ctx);

  fctAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  return concludeNode(fctAttrNode);
}

std::any ASTBuilder::visitLambdaAttr(SpiceParser::LambdaAttrContext *ctx) {
  auto lambdaAttrNode = createNode<LambdaAttrNode>(ctx);

  lambdaAttrNode->attrLst = std::any_cast<AttrLstNode *>(visit(ctx->attrLst()));

  return concludeNode(lambdaAttrNode);
}

std::any ASTBuilder::visitAttrLst(SpiceParser::AttrLstContext *ctx) {
  auto attrLstNode = createNode<AttrLstNode>(ctx);

  for (SpiceParser::AttrContext *attrCtx : ctx->attr())
    attrLstNode->attributes.push_back(std::any_cast<AttrNode *>(visit(attrCtx)));

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

  // Come up with type
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

std::any ASTBuilder::visitConstantLst(SpiceParser::ConstantLstContext *ctx) {
  auto constantLstNode = createNode<ConstantLstNode>(ctx);

  for (SpiceParser::ConstantContext *constantCtx : ctx->constant())
    constantLstNode->constants.push_back(std::any_cast<ConstantNode *>(visit(constantCtx)));

  return concludeNode(constantLstNode);
}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  auto returnStmtNode = createNode<ReturnStmtNode>(ctx);

  if (ctx->assignExpr())
    returnStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(returnStmtNode);
}

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  auto breakStmtNode = createNode<BreakStmtNode>(ctx);

  // Extract number of breaks
  if (ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  return concludeNode(breakStmtNode);
}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  auto continueStmtNode = createNode<ContinueStmtNode>(ctx);

  // Extract number of continues
  if (ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  return concludeNode(continueStmtNode);
}

std::any ASTBuilder::visitFallthroughStmt(SpiceParser::FallthroughStmtContext *ctx) {
  auto fallthroughStmtNode = createNode<FallthroughStmtNode>(ctx);
  return concludeNode(fallthroughStmtNode);
}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertStmtNode = createNode<AssertStmtNode>(ctx);

  // Enrich
  antlr4::misc::Interval interval(ctx->assignExpr()->start->getStartIndex(), ctx->assignExpr()->stop->getStopIndex());
  assertStmtNode->expressionString = inputStream->getText(interval);

  assertStmtNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(assertStmtNode);
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  visitChildren(ctx);
  return nullptr;
}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  auto printfCallNode = createNode<PrintfCallNode>(ctx);

  std::string templatedString = ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  for (SpiceParser::AssignExprContext *argCtx : ctx->assignExpr())
    printfCallNode->args.push_back(std::any_cast<AssignExprNode *>(visit(argCtx)));

  return concludeNode(printfCallNode);
}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  auto sizeofCallNode = createNode<SizeofCallNode>(ctx);

  if (ctx->TYPE()) {
    sizeofCallNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
    sizeofCallNode->isType = true;
  } else {
    sizeofCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
    sizeofCallNode->isType = false;
  }

  return concludeNode(sizeofCallNode);
}

std::any ASTBuilder::visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) {
  auto alignofCallNode = createNode<AlignofCallNode>(ctx);

  if (ctx->TYPE()) {
    alignofCallNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));
    alignofCallNode->isType = true;
  } else {
    alignofCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
    alignofCallNode->isType = false;
  }

  return concludeNode(alignofCallNode);
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto lenCallNode = createNode<LenCallNode>(ctx);

  lenCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(lenCallNode);
}

std::any ASTBuilder::visitPanicCall(SpiceParser::PanicCallContext *ctx) {
  auto panicCallNode = createNode<PanicCallNode>(ctx);

  panicCallNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(panicCallNode);
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->assignOp())
  auto assignExprNode = createNode<AssignExprNode>(ctx);

  assignExprNode->lhs = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));
  assignExprNode->op = std::any_cast<AssignExprNode::AssignOp>(visit(ctx->assignOp()));
  assignExprNode->rhs = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(assignExprNode);
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->QUESTION_MARK())
  auto ternaryExprNode = createNode<TernaryExprNode>(ctx);

  ternaryExprNode->isShortened = ctx->logicalOrExpr().size() == 2;
  ternaryExprNode->condition = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(0)));
  if (ternaryExprNode->isShortened) {
    ternaryExprNode->falseValue = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(0)));
  } else {
    ternaryExprNode->trueValue = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(0)));
    ternaryExprNode->falseValue = std::any_cast<LogicalOrExprNode *>(visit(ctx->logicalOrExpr(1)));
  }

  return concludeNode(ternaryExprNode);
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->LOGICAL_OR().empty())
  auto logicalOrExprNode = createNode<LogicalOrExprNode>(ctx);

  for (SpiceParser::LogicalAndExprContext *logicalAndExprCtx : ctx->logicalAndExpr())
    logicalOrExprNode->operands.push_back(std::any_cast<LogicalAndExprNode *>(visit(logicalAndExprCtx)));

  return concludeNode(logicalOrExprNode);
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->LOGICAL_AND().empty())
  auto logicalAndExprNode = createNode<LogicalAndExprNode>(ctx);

  for (SpiceParser::BitwiseOrExprContext *bitwiseOrExprCtx : ctx->bitwiseOrExpr())
    logicalAndExprNode->operands.push_back(std::any_cast<BitwiseOrExprNode *>(visit(bitwiseOrExprCtx)));

  return concludeNode(logicalAndExprNode);
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->BITWISE_OR().empty())
  auto bitwiseOrExprNode = createNode<BitwiseOrExprNode>(ctx);

  for (SpiceParser::BitwiseXorExprContext *bitwiseXorExprCtx : ctx->bitwiseXorExpr())
    bitwiseOrExprNode->operands.push_back(std::any_cast<BitwiseXorExprNode *>(visit(bitwiseXorExprCtx)));

  return concludeNode(bitwiseOrExprNode);
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->BITWISE_XOR().empty())
  auto bitwiseXorExprNode = createNode<BitwiseXorExprNode>(ctx);

  for (SpiceParser::BitwiseAndExprContext *bitwiseAndExprCtx : ctx->bitwiseAndExpr())
    bitwiseXorExprNode->operands.push_back(std::any_cast<BitwiseAndExprNode *>(visit(bitwiseAndExprCtx)));

  return concludeNode(bitwiseXorExprNode);
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->BITWISE_AND().empty())
  auto bitwiseAndExprNode = createNode<BitwiseAndExprNode>(ctx);

  for (SpiceParser::EqualityExprContext *equalityExprCtx : ctx->equalityExpr())
    bitwiseAndExprNode->operands.push_back(std::any_cast<EqualityExprNode *>(visit(equalityExprCtx)));

  return concludeNode(bitwiseAndExprNode);
}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->EQUAL() && !ctx->NOT_EQUAL())
  auto equalityExprNode = createNode<EqualityExprNode>(ctx);

  equalityExprNode->lhs = std::any_cast<RelationalExprNode *>(visit(ctx->relationalExpr()[0]));
  if (ctx->relationalExpr().size() > 1)
    equalityExprNode->rhs = std::any_cast<RelationalExprNode *>(visit(ctx->relationalExpr()[1]));

  // Extract operator
  if (ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_EQUAL;
  else if (ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_NOT_EQUAL;

  return concludeNode(equalityExprNode);
}

std::any ASTBuilder::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->LESS() && !ctx->GREATER() && !ctx->LESS_EQUAL() && !ctx->GREATER_EQUAL())
  auto relationalExprNode = createNode<RelationalExprNode>(ctx);

  relationalExprNode->lhs = std::any_cast<ShiftExprNode *>(visit(ctx->shiftExpr()[0]));
  if (ctx->shiftExpr().size() > 1)
    relationalExprNode->rhs = std::any_cast<ShiftExprNode *>(visit(ctx->shiftExpr()[1]));

  // Extract operator
  if (ctx->LESS())
    relationalExprNode->op = RelationalExprNode::OP_LESS;
  else if (ctx->GREATER())
    relationalExprNode->op = RelationalExprNode::OP_GREATER;
  else if (ctx->LESS_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_LESS_EQUAL;
  else if (ctx->GREATER_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_GREATER_EQUAL;

  return concludeNode(relationalExprNode);
}

std::any ASTBuilder::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->LESS().empty() && ctx->GREATER().empty())
  auto shiftExprNode = createNode<ShiftExprNode>(ctx);

  shiftExprNode->lhs = std::any_cast<AdditiveExprNode *>(visit(ctx->additiveExpr()[0]));
  if (ctx->additiveExpr().size() > 1)
    shiftExprNode->rhs = std::any_cast<AdditiveExprNode *>(visit(ctx->additiveExpr()[1]));

  // Extract operator
  if (!ctx->LESS().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_LEFT;
  else if (!ctx->GREATER().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_RIGHT;

  return concludeNode(shiftExprNode);
}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->PLUS().empty() && ctx->MINUS().empty())
  auto additiveExprNode = createNode<AdditiveExprNode>(ctx);

  for (SpiceParser::MultiplicativeExprContext *multiplicativeExprCtx : ctx->multiplicativeExpr())
    additiveExprNode->operands.push_back(std::any_cast<MultiplicativeExprNode *>(visit(multiplicativeExprCtx)));

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_PLUS, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_MINUS, SymbolType(TY_INVALID));
  }

  return concludeNode(additiveExprNode);
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->MUL().empty() && ctx->DIV().empty() && ctx->REM().empty())
  auto multiplicativeExprNode = createNode<MultiplicativeExprNode>(ctx);

  for (SpiceParser::CastExprContext *castExprCtx : ctx->castExpr())
    multiplicativeExprNode->operands.push_back(std::any_cast<CastExprNode *>(visit(castExprCtx)));

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_MUL, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_DIV, SymbolType(TY_INVALID));
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_REM, SymbolType(TY_INVALID));
  }

  return concludeNode(multiplicativeExprNode);
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->LPAREN())
  auto castExprNode = createNode<CastExprNode>(ctx);

  if (ctx->LPAREN())
    castExprNode->dataType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  return concludeNode(castExprNode);
}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->prefixUnaryExpr())
  auto prefixUnaryExprNode = createNode<PrefixUnaryExprNode>(ctx);

  prefixUnaryExprNode->prefixUnaryExpr = std::any_cast<PrefixUnaryExprNode *>(visit(ctx->prefixUnaryExpr()));

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

  return concludeNode(prefixUnaryExprNode);
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->postfixUnaryExpr())
  auto postfixUnaryExprNode = createNode<PostfixUnaryExprNode>(ctx);

  postfixUnaryExprNode->postfixUnaryExpr = std::any_cast<PostfixUnaryExprNode *>(visit(ctx->postfixUnaryExpr()));

  if (ctx->LBRACKET()) {
    postfixUnaryExprNode->assignExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_SUBSCRIPT;
  } else if (ctx->DOT()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MEMBER_ACCESS;
    postfixUnaryExprNode->identifier = getIdentifier(ctx->IDENTIFIER());
  } else if (ctx->PLUS_PLUS()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_PLUS_PLUS;
  } else if (ctx->MINUS_MINUS()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MINUS_MINUS;
  }

  return concludeNode(postfixUnaryExprNode);
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, ctx->IDENTIFIER().empty() && ctx->TYPE_IDENTIFIER().empty())
  auto atomicExprNode = createNode<AtomicExprNode>(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
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

  return concludeNode(atomicExprNode);
}

std::any ASTBuilder::visitValue(SpiceParser::ValueContext *ctx) {
  AST_BUILDER_FAST_PATH(ctx, !ctx->NIL())
  auto valueNode = createNode<ValueNode>(ctx);

  if (ctx->NIL())
    valueNode->nilType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

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
    value.intValue = parseInt(constantNode, ctx->INT_LIT());
  } else if (ctx->SHORT_LIT()) {
    constantNode->type = ConstantNode::TYPE_SHORT;
    value.shortValue = parseShort(constantNode, ctx->SHORT_LIT());
  } else if (ctx->LONG_LIT()) {
    constantNode->type = ConstantNode::TYPE_LONG;
    value.longValue = parseLong(constantNode, ctx->LONG_LIT());
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

  // Enrich
  fctCallNode->hasArgs = fctCallNode->argLst;
  fctCallNode->hasTemplateTypes = fctCallNode->templateTypeLst;

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

  return concludeNode(fctCallNode);
}

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  auto arrayInitializationNode = createNode<ArrayInitializationNode>(ctx);

  if (ctx->argLst())
    arrayInitializationNode->itemLst = std::any_cast<ArgLstNode *>(visit(ctx->argLst()));

  return concludeNode(arrayInitializationNode);
}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  auto structInstantiationNode = createNode<StructInstantiationNode>(ctx);

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

  if (ctx->typeLst())
    structInstantiationNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));

  if (ctx->argLst())
    structInstantiationNode->fieldLst = std::any_cast<ArgLstNode *>(visit(ctx->argLst()));

  return concludeNode(structInstantiationNode);
}

std::any ASTBuilder::visitLambdaFunc(SpiceParser::LambdaFuncContext *ctx) {
  auto lambdaFuncNode = createNode<LambdaFuncNode>(ctx);

  lambdaFuncNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  if (ctx->paramLst()) {
    lambdaFuncNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
    lambdaFuncNode->hasParams = true;
  }

  lambdaFuncNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(lambdaFuncNode);
}

std::any ASTBuilder::visitLambdaProc(SpiceParser::LambdaProcContext *ctx) {
  auto lambdaProcNode = createNode<LambdaProcNode>(ctx);

  if (ctx->paramLst()) {
    lambdaProcNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
    lambdaProcNode->hasParams = true;
  }

  lambdaProcNode->body = std::any_cast<StmtLstNode *>(visit(ctx->stmtLst()));

  return concludeNode(lambdaProcNode);
}

std::any ASTBuilder::visitLambdaExpr(SpiceParser::LambdaExprContext *ctx) {
  auto lambdaExprNode = createNode<LambdaExprNode>(ctx);

  if (ctx->paramLst()) {
    lambdaExprNode->paramLst = std::any_cast<ParamLstNode *>(visit(ctx->paramLst()));
    lambdaExprNode->hasParams = true;
  }

  lambdaExprNode->lambdaExpr = std::any_cast<AssignExprNode *>(visit(ctx->assignExpr()));

  return concludeNode(lambdaExprNode);
}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto dataTypeNode = createNode<DataTypeNode>(ctx);

  if (ctx->specifierLst())
    dataTypeNode->specifierLst = std::any_cast<SpecifierLstNode *>(visit(ctx->specifierLst()));

  dataTypeNode->baseDataType = std::any_cast<BaseDataTypeNode *>(visit(ctx->baseDataType()));

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
    baseDataTypeNode->customDataType = std::any_cast<CustomDataTypeNode *>(visit(ctx->customDataType()));
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CUSTOM;
  } else if (ctx->functionDataType()) {
    baseDataTypeNode->functionDataType = std::any_cast<FunctionDataTypeNode *>(visit(ctx->functionDataType()));
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_FUNCTION;
  }

  return concludeNode(baseDataTypeNode);
}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  auto customDataTypeNode = createNode<CustomDataTypeNode>(ctx);

  for (TerminalNode *identifier : ctx->IDENTIFIER()) {
    const std::string fragment = identifier->toString();
    customDataTypeNode->typeNameFragments.push_back(fragment);
    customDataTypeNode->fqTypeName += fragment + SCOPE_ACCESS_TOKEN;
  }

  const std::string name = ctx->TYPE_IDENTIFIER()->toString();
  customDataTypeNode->typeNameFragments.push_back(name);
  customDataTypeNode->fqTypeName += name;

  if (ctx->typeLst())
    customDataTypeNode->templateTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));

  return concludeNode(customDataTypeNode);
}

std::any ASTBuilder::visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) {
  auto fctDataTypeNode = createNode<FunctionDataTypeNode>(ctx);

  if (ctx->dataType())
    fctDataTypeNode->returnType = std::any_cast<DataTypeNode *>(visit(ctx->dataType()));

  if (ctx->typeLst())
    fctDataTypeNode->paramTypeLst = std::any_cast<TypeLstNode *>(visit(ctx->typeLst()));

  return concludeNode(fctDataTypeNode);
}

std::any ASTBuilder::visitAssignOp(SpiceParser::AssignOpContext *ctx) {
  AssignExprNode::AssignOp assignOp = AssignExprNode::OP_NONE;

  if (ctx->ASSIGN())
    assignOp = AssignExprNode::OP_ASSIGN;
  else if (ctx->PLUS_EQUAL())
    assignOp = AssignExprNode::OP_PLUS_EQUAL;
  else if (ctx->MINUS_EQUAL())
    assignOp = AssignExprNode::OP_MINUS_EQUAL;
  else if (ctx->MUL_EQUAL())
    assignOp = AssignExprNode::OP_MUL_EQUAL;
  else if (ctx->DIV_EQUAL())
    assignOp = AssignExprNode::OP_DIV_EQUAL;
  else if (ctx->REM_EQUAL())
    assignOp = AssignExprNode::OP_REM_EQUAL;
  else if (ctx->SHL_EQUAL())
    assignOp = AssignExprNode::OP_SHL_EQUAL;
  else if (ctx->SHR_EQUAL())
    assignOp = AssignExprNode::OP_SHR_EQUAL;
  else if (ctx->AND_EQUAL())
    assignOp = AssignExprNode::OP_AND_EQUAL;
  else if (ctx->OR_EQUAL())
    assignOp = AssignExprNode::OP_OR_EQUAL;
  else if (ctx->XOR_EQUAL())
    assignOp = AssignExprNode::OP_XOR_EQUAL;
  else
    assert(false && "Unknown assign operator");

  return assignOp;
}

std::any ASTBuilder::visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) {
  FctNameNode::OverloadedOperator overloadedOperator = FctNameNode::OP_NONE;
  const char *overloadedOperatorName = nullptr;

  if (ctx->PLUS()) {
    overloadedOperator = FctNameNode::OP_PLUS;
    overloadedOperatorName = OP_FCT_PLUS;
  } else if (ctx->MINUS()) {
    overloadedOperator = FctNameNode::OP_MINUS;
    overloadedOperatorName = OP_FCT_MINUS;
  } else if (ctx->MUL()) {
    overloadedOperator = FctNameNode::OP_MUL;
    overloadedOperatorName = OP_FCT_MUL;
  } else if (ctx->DIV()) {
    overloadedOperator = FctNameNode::OP_DIV;
    overloadedOperatorName = OP_FCT_DIV;
  } else if (ctx->EQUAL()) {
    overloadedOperator = FctNameNode::OP_EQUAL;
    overloadedOperatorName = OP_FCT_EQUAL;
  } else if (ctx->NOT_EQUAL()) {
    overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    overloadedOperatorName = OP_FCT_NOT_EQUAL;
  } else if (ctx->LESS().size() == 2) {
    overloadedOperator = FctNameNode::OP_SHL;
    overloadedOperatorName = OP_FCT_SHL;
  } else if (ctx->GREATER().size() == 2) {
    overloadedOperator = FctNameNode::OP_SHR;
    overloadedOperatorName = OP_FCT_SHR;
  } else if (ctx->PLUS_EQUAL()) {
    overloadedOperator = FctNameNode::OP_PLUS_EQUAL;
    overloadedOperatorName = OP_FCT_PLUS_EQUAL;
  } else if (ctx->MINUS_EQUAL()) {
    overloadedOperator = FctNameNode::OP_MINUS_EQUAL;
    overloadedOperatorName = OP_FCT_MINUS_EQUAL;
  } else if (ctx->MUL_EQUAL()) {
    overloadedOperator = FctNameNode::OP_MUL_EQUAL;
    overloadedOperatorName = OP_FCT_MUL_EQUAL;
  } else if (ctx->DIV_EQUAL()) {
    overloadedOperator = FctNameNode::OP_DIV_EQUAL;
    overloadedOperatorName = OP_FCT_DIV_EQUAL;
  } else if (ctx->PLUS_PLUS()) {
    overloadedOperator = FctNameNode::OP_PLUS_PLUS;
    overloadedOperatorName = OP_FCT_POSTFIX_PLUS_PLUS;
  } else if (ctx->MINUS_MINUS()) {
    overloadedOperator = FctNameNode::OP_MINUS_MINUS;
    overloadedOperatorName = OP_FCT_POSTFIX_MINUS_MINUS;
  } else {
    assert(false && "Unsupported overloadable operator"); // GCOV_EXCL_LINE
  }

  return std::make_pair(overloadedOperator, overloadedOperatorName);
}

template <typename T> T *ASTBuilder::createNode(const ParserRuleContext *ctx) {
  ASTNode *parent = nullptr;
  if constexpr (!std::is_same_v<T, EntryNode>)
    parent = parentStack.top();

  // Create the new node
  T *node = resourceManager.astNodeAlloc.allocate<T>(getCodeLoc(ctx));
  node->parent = parent;
  resourceManager.astNodes.push_back(node);

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

int32_t ASTBuilder::parseInt(ConstantNode *constantNode, TerminalNode *terminal) {
  const NumericParserCallback<int32_t> cb = [](const std::string &substr, int base, bool isSigned) -> int32_t {
    // Prepare limits
    const int64_t upperLimit = isSigned ? INT32_MAX : UINT32_MAX;
    const int64_t lowerLimit = isSigned ? INT32_MIN : 0;
    // Parse number and check for limits
    const int64_t number = std::stoll(substr, nullptr, base);
    if (number < lowerLimit || number > upperLimit)
      throw std::out_of_range("Number out of range");
    return static_cast<int32_t>(number);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int16_t ASTBuilder::parseShort(ConstantNode *constantNode, TerminalNode *terminal) {
  const NumericParserCallback<int16_t> cb = [](const std::string &substr, int base, bool isSigned) -> int16_t {
    // Prepare limits
    const int64_t upperLimit = isSigned ? INT16_MAX : UINT16_MAX;
    const int64_t lowerLimit = isSigned ? INT16_MIN : 0;
    // Parse number and check for limits
    const int64_t number = std::stoll(substr, nullptr, base);
    if (number < lowerLimit || number > upperLimit)
      throw std::out_of_range("Number out of range");
    return static_cast<int16_t>(number);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int64_t ASTBuilder::parseLong(ConstantNode *constantNode, TerminalNode *terminal) {
  const NumericParserCallback<int64_t> cb = [](const std::string &substr, int base, bool isSigned) -> int64_t {
    return std::stoll(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
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

template <typename T>
T ASTBuilder::parseNumeric(ConstantNode *constantNode, TerminalNode *terminal,
                           std::function<T(const std::string &, int, bool)> cb) {
  const std::string input = terminal->toString();

  // Set to signed if the input string does not end with 'u'
  const bool isSigned = !input.ends_with('u');

  try {
    if (input.length() >= 3) {
      if (input[0] == '0') {
        const std::string subStr = input.substr(2);
        switch (input[1]) {
        case 'b':
        case 'B':
          return cb(subStr, 2, isSigned);
        case 'h':
        case 'H':
        case 'x':
        case 'X':
          return cb(subStr, 16, isSigned);
        case 'o':
        case 'O':
          return cb(subStr, 8, isSigned);
        default:
          return cb(input, 10, isSigned);
        }
      }
    }
    return cb(input, 10, isSigned);
  } catch (std::out_of_range &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "You tried to parse '" + input + "' as an integer, but it was no integer");
  }
}

void ASTBuilder::replaceEscapeChars(std::string &string) {
  CommonUtil::replaceAll(string, "\\a", "\a");
  CommonUtil::replaceAll(string, "\\b", "\b");
  CommonUtil::replaceAll(string, "\\f", "\f");
  CommonUtil::replaceAll(string, "\\n", "\n");
  CommonUtil::replaceAll(string, "\\r", "\r");
  CommonUtil::replaceAll(string, "\\t", "\t");
  CommonUtil::replaceAll(string, "\\v", "\v");
  CommonUtil::replaceAll(string, "\\'", "\'");
  CommonUtil::replaceAll(string, "\\\"", "\"");
  CommonUtil::replaceAll(string, "\\?", "\?");
}

std::string ASTBuilder::getIdentifier(TerminalNode *terminal) {
  std::string identifier = terminal->getText();

  // Check if the identifier is 'String' and this is no std source file
  bool isReserved = identifier == STROBJ_NAME && !sourceFile->stdFile;
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
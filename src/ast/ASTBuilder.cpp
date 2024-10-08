// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <exception/ParserError.h>
#include <global/GlobalResourceManager.h>
#include <typechecker/OpRuleManager.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

ASTBuilder::ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, antlr4::ANTLRInputStream *inputStream)
    : CompilerPass(resourceManager, sourceFile), inputStream(inputStream) {}

std::any ASTBuilder::visitEntry(SpiceParser::EntryContext *ctx) {
  const auto entryNode = createNode<EntryNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(entryNode);
}

std::any ASTBuilder::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  const auto mainFctDefNode = createNode<MainFctDefNode>(ctx);

  // Enrich
  mainFctDefNode->takesArgs = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(mainFctDefNode);
}

std::any ASTBuilder::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  const auto fctDefNode = createNode<FctDefNode>(ctx);

  // Enrich
  fctDefNode->hasParams = ctx->paramLst();
  fctDefNode->hasTemplateTypes = ctx->typeLst();

  // Visit children
  visitChildren(ctx);

  // Retrieve information from the function name
  fctDefNode->name = fctDefNode->getChild<FctNameNode>();
  fctDefNode->isMethod = fctDefNode->name->nameFragments.size() > 1;

  // Tell the return type that it is one
  fctDefNode->returnType()->isReturnType = true;

  // Tell the attributes that they are function attributes
  if (fctDefNode->attrs())
    for (AttrNode *attr : fctDefNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_FCT_PROC;

  return concludeNode(fctDefNode);
}

std::any ASTBuilder::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  const auto procDefNode = createNode<ProcDefNode>(ctx);

  // Enrich
  procDefNode->hasParams = ctx->paramLst();
  procDefNode->hasTemplateTypes = ctx->typeLst();

  // Visit children
  visitChildren(ctx);

  // Retrieve information from the procedure name
  procDefNode->name = procDefNode->getChild<FctNameNode>();
  procDefNode->isMethod = procDefNode->name->nameFragments.size() > 1;

  // Tell the attributes that they are procedure attributes
  if (procDefNode->attrs())
    for (AttrNode *attr : procDefNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_FCT_PROC;

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
  visitChildren(ctx);

  return concludeNode(fctNameNode);
}

std::any ASTBuilder::visitStructDef(SpiceParser::StructDefContext *ctx) {
  const auto structDefNode = createNode<StructDefNode>(ctx);

  // Enrich
  structDefNode->structName = getIdentifier(ctx->TYPE_IDENTIFIER());
  structDefNode->typeId = resourceManager.getNextCustomTypeId();
  structDefNode->hasTemplateTypes = ctx->LESS();
  structDefNode->hasInterfaces = ctx->COLON();

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are struct attributes
  if (structDefNode->attrs())
    for (AttrNode *attr : structDefNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_STRUCT;

  // Check if a custom type id was set
  if (structDefNode->attrs() && structDefNode->attrs()->attrLst()->hasAttr(ATTR_CORE_COMPILER_FIXED_TYPE_ID))
    structDefNode->typeId = structDefNode->attrs()->attrLst()->getAttrValueByName(ATTR_CORE_COMPILER_FIXED_TYPE_ID)->intValue;

  return concludeNode(structDefNode);
}

std::any ASTBuilder::visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) {
  const auto interfaceDefNode = createNode<InterfaceDefNode>(ctx);

  // Enrich
  interfaceDefNode->interfaceName = getIdentifier(ctx->TYPE_IDENTIFIER());
  interfaceDefNode->typeId = resourceManager.getNextCustomTypeId();
  interfaceDefNode->hasTemplateTypes = ctx->LESS();

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are interface attributes
  if (interfaceDefNode->attrs())
    for (AttrNode *attr : interfaceDefNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_INTERFACE;

  // Check if a custom type id was set
  if (interfaceDefNode->attrs() && interfaceDefNode->attrs()->attrLst()->hasAttr(ATTR_CORE_COMPILER_FIXED_TYPE_ID))
    interfaceDefNode->typeId =
        interfaceDefNode->attrs()->attrLst()->getAttrValueByName(ATTR_CORE_COMPILER_FIXED_TYPE_ID)->intValue;

  return concludeNode(interfaceDefNode);
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  const auto enumDefNode = createNode<EnumDefNode>(ctx);

  // Enrich
  enumDefNode->enumName = getIdentifier(ctx->TYPE_IDENTIFIER());
  enumDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  visitChildren(ctx);

  // Tell all items about the enum def
  for (EnumItemNode *enumItem : enumDefNode->itemLst()->items())
    enumItem->enumDef = enumDefNode;

  return concludeNode(enumDefNode);
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  const auto genericTypeDefNode = createNode<GenericTypeDefNode>(ctx);

  // Enrich
  genericTypeDefNode->typeName = getIdentifier(ctx->TYPE_IDENTIFIER());

  // Visit children
  visitChildren(ctx);

  return concludeNode(genericTypeDefNode);
}

std::any ASTBuilder::visitAliasDef(SpiceParser::AliasDefContext *ctx) {
  const auto aliasDefNode = createNode<AliasDefNode>(ctx);

  // Enrich
  aliasDefNode->aliasName = getIdentifier(ctx->TYPE_IDENTIFIER());
  aliasDefNode->dataTypeString = ctx->dataType()->getText();
  aliasDefNode->typeId = resourceManager.getNextCustomTypeId();

  // Visit children
  visitChildren(ctx);

  return concludeNode(aliasDefNode);
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  const auto globalVarDefNode = createNode<GlobalVarDefNode>(ctx);

  // Enrich
  globalVarDefNode->varName = getIdentifier(ctx->TYPE_IDENTIFIER());
  globalVarDefNode->hasValue = ctx->ASSIGN();

  // Visit children
  visitChildren(ctx);

  // Tell the data type that it is a global one
  globalVarDefNode->dataType()->isGlobalType = true;

  return concludeNode(globalVarDefNode);
}

std::any ASTBuilder::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  const auto extDeclNode = createNode<ExtDeclNode>(ctx);

  // Enrich
  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER() ? ctx->IDENTIFIER() : ctx->TYPE_IDENTIFIER());
  extDeclNode->hasArgs = ctx->typeLst();
  extDeclNode->isVarArg = ctx->ELLIPSIS();
  extDeclNode->hasReturnType = ctx->F();

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are ext decl attributes
  if (extDeclNode->attrs())
    for (AttrNode *attr : extDeclNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_EXT_DECL;

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
  visitChildren(ctx);

  return concludeNode(unsafeBlockDefNode);
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  const auto forLoopNode = createNode<ForLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(forLoopNode);
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  // Visit children
  visitChildren(ctx);

  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  const auto foreachLoopNode = createNode<ForeachLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell the foreach item that it is one
  foreachLoopNode->itemVarDecl()->isForEachItem = true;

  return concludeNode(foreachLoopNode);
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  // Visit children
  visitChildren(ctx);

  return nullptr;
}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  const auto whileLoopNode = createNode<WhileLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(whileLoopNode);
}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {
  const auto doWhileLoopNode = createNode<DoWhileLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(doWhileLoopNode);
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  const auto ifStmtNode = createNode<IfStmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ifStmtNode);
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  const auto elseStmtNode = createNode<ElseStmtNode>(ctx);

  // Enrich
  elseStmtNode->isElseIf = ctx->ifStmt();

  // Visit children
  visitChildren(ctx);

  return concludeNode(elseStmtNode);
}

std::any ASTBuilder::visitSwitchStmt(SpiceParser::SwitchStmtContext *ctx) {
  const auto switchStmtNode = createNode<SwitchStmtNode>(ctx);

  // Enrich
  switchStmtNode->hasDefaultBranch = ctx->defaultBranch();

  // Visit children
  visitChildren(ctx);

  return concludeNode(switchStmtNode);
}

std::any ASTBuilder::visitCaseBranch(SpiceParser::CaseBranchContext *ctx) {
  const auto caseBranchNode = createNode<CaseBranchNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(caseBranchNode);
}

std::any ASTBuilder::visitDefaultBranch(SpiceParser::DefaultBranchContext *ctx) {
  const auto defaultBranchNode = createNode<DefaultBranchNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(defaultBranchNode);
}

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  const auto anonymousBlockStmtNode = createNode<AnonymousBlockStmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(anonymousBlockStmtNode);
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  const auto stmtLstNode = createNode<StmtLstNode>(ctx);

  // Enrich
  stmtLstNode->closingBraceCodeLoc = CodeLoc(ctx->getStop(), sourceFile);

  // Visit children
  visitChildren(ctx);

  return concludeNode(stmtLstNode);
}

std::any ASTBuilder::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  const auto typeLstNode = createNode<TypeLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(typeLstNode);
}

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  const auto typeAltsLstNode = createNode<TypeAltsLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(typeAltsLstNode);
}

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  const auto paramLstNode = createNode<ParamLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell all params and param types that they are such
  for (DeclStmtNode *param : paramLstNode->params()) {
    param->isParam = true;
    param->dataType()->isParamType = true;
  }

  return concludeNode(paramLstNode);
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  const auto argLstNode = createNode<ArgLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(argLstNode);
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  const auto enumItemLstNode = createNode<EnumItemLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

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
  visitChildren(ctx);

  // Tell the data type that it is a field type
  fieldNode->dataType()->setFieldTypeRecursive();

  return concludeNode(fieldNode);
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {
  const auto signatureNode = createNode<SignatureNode>(ctx);

  // Extract method name
  signatureNode->methodName = getIdentifier(ctx->IDENTIFIER());
  // Extract signature type
  signatureNode->signatureType = ctx->F() ? SignatureNode::TYPE_FUNCTION : SignatureNode::TYPE_PROCEDURE;
  signatureNode->signatureSpecifiers = ctx->F() ? TypeSpecifiers::of(TY_FUNCTION) : TypeSpecifiers::of(TY_PROCEDURE);
  // Extract other metadata
  signatureNode->hasTemplateTypes = ctx->F() ? ctx->LESS().size() == 2 : ctx->LESS().size() == 1;
  signatureNode->hasParams = ctx->typeLst().size() == 2 || (ctx->typeLst().size() == 1 && !signatureNode->hasTemplateTypes);

  // Visit children
  visitChildren(ctx);

  return concludeNode(signatureNode);
}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) { return visitChildren(ctx); }

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  const auto declStmtNode = createNode<DeclStmtNode>(ctx);

  // Enrich
  declStmtNode->varName = getIdentifier(ctx->IDENTIFIER());
  declStmtNode->hasAssignment = ctx->ASSIGN();

  // Visit children
  visitChildren(ctx);

  return concludeNode(declStmtNode);
}

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  const auto specifierLstNode = createNode<SpecifierLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Check if we have both, signed and unsigned specifier
  bool seenSignedOrUnsigned = false;
  for (const SpecifierNode *specifier : specifierLstNode->specifiers()) {
    if (specifier->type != SpecifierNode::TY_SIGNED && specifier->type != SpecifierNode::TY_UNSIGNED)
      continue;
    if (seenSignedOrUnsigned)
      throw ParserError(specifier->codeLoc, INVALID_SPECIFIER_COMBINATION, "A variable can not be signed and unsigned");
    seenSignedOrUnsigned = true;
  }

  return concludeNode(specifierLstNode);
}

std::any ASTBuilder::visitSpecifier(SpiceParser::SpecifierContext *ctx) {
  const auto specifierNode = createNode<SpecifierNode>(ctx);

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
  const auto modAttrNode = createNode<ModAttrNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : modAttrNode->attrLst()->attributes())
    attr->target = AttrNode::TARGET_MODULE;

  return concludeNode(modAttrNode);
}

std::any ASTBuilder::visitTopLevelDefAttr(SpiceParser::TopLevelDefAttrContext *ctx) {
  const auto fctAttrNode = createNode<TopLevelDefinitionAttrNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(fctAttrNode);
}

std::any ASTBuilder::visitLambdaAttr(SpiceParser::LambdaAttrContext *ctx) {
  const auto lambdaAttrNode = createNode<LambdaAttrNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : lambdaAttrNode->attrLst()->attributes())
    attr->target = AttrNode::TARGET_LAMBDA;

  return concludeNode(lambdaAttrNode);
}

std::any ASTBuilder::visitAttrLst(SpiceParser::AttrLstContext *ctx) {
  const auto attrLstNode = createNode<AttrLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

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
  visitChildren(ctx);

  // Come up with type
  if (ctx->constant()) {
    if (ctx->constant()->STRING_LIT())
      attrNode->type = AttrNode::TYPE_STRING;
    else if (ctx->constant()->INT_LIT())
      attrNode->type = AttrNode::TYPE_INT;
    else if (ctx->constant()->TRUE() || ctx->constant()->FALSE())
      attrNode->type = AttrNode::TYPE_BOOL;
    else
      throw ParserError(attrNode->value()->codeLoc, INVALID_ATTR_VALUE_TYPE, "Invalid attribute value type");
  } else {
    // If no value is given, use the bool type
    attrNode->type = AttrNode::TYPE_BOOL;
  }

  return concludeNode(attrNode);
}

std::any ASTBuilder::visitCaseConstant(SpiceParser::CaseConstantContext *ctx) {
  const auto caseConstantNode = createNode<CaseConstantNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree);
        t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = getIdentifier(t1);
      caseConstantNode->identifierFragments.push_back(fragment);
      if (!caseConstantNode->fqIdentifier.empty())
        caseConstantNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      caseConstantNode->fqIdentifier += fragment;
    } else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
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
  const auto returnStmtNode = createNode<ReturnStmtNode>(ctx);

  // Enrich
  returnStmtNode->hasReturnValue = ctx->assignExpr();

  // Visit children
  visitChildren(ctx);

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
  visitChildren(ctx);

  return concludeNode(assertStmtNode);
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  // Visit children
  visitChildren(ctx);

  return nullptr;
}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  const auto printfCallNode = createNode<PrintfCallNode>(ctx);

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
  const auto sizeofCallNode = createNode<SizeofCallNode>(ctx);

  // Check if type or value
  sizeofCallNode->isType = ctx->TYPE();

  // Visit children
  visitChildren(ctx);

  return concludeNode(sizeofCallNode);
}

std::any ASTBuilder::visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) {
  const auto alignofCallNode = createNode<AlignofCallNode>(ctx);

  // Check if type or value
  alignofCallNode->isType = ctx->TYPE();

  // Visit children
  visitChildren(ctx);

  return concludeNode(alignofCallNode);
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  const auto lenCallNode = createNode<LenCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(lenCallNode);
}

std::any ASTBuilder::visitPanicCall(SpiceParser::PanicCallContext *ctx) {
  const auto panicCallNode = createNode<PanicCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(panicCallNode);
}

std::any ASTBuilder::visitSysCall(SpiceParser::SysCallContext *ctx) {
  const auto sysCallNode = createNode<SysCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(sysCallNode);
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  const auto assignExprNode = createNode<AssignExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(assignExprNode);
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  const auto ternaryExprNode = createNode<TernaryExprNode>(ctx);

  // Check if is shortened
  ternaryExprNode->isShortened = ctx->logicalOrExpr().size() == 2;

  // Visit children
  visitChildren(ctx);

  return concludeNode(ternaryExprNode);
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  const auto logicalOrExprNode = createNode<LogicalOrExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(logicalOrExprNode);
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  const auto logicalAndExprNode = createNode<LogicalAndExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(logicalAndExprNode);
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  const auto bitwiseOrExprNode = createNode<BitwiseOrExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(bitwiseOrExprNode);
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  const auto bitwiseXorExprNode = createNode<BitwiseXorExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(bitwiseXorExprNode);
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  const auto bitwiseAndExprNode = createNode<BitwiseAndExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(bitwiseAndExprNode);
}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  const auto equalityExprNode = createNode<EqualityExprNode>(ctx);

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
  const auto relationalExprNode = createNode<RelationalExprNode>(ctx);

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
  const auto shiftExprNode = createNode<ShiftExprNode>(ctx);

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
  const auto additiveExprNode = createNode<AdditiveExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_PLUS, TY_INVALID);
    else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
             t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_MINUS, TY_INVALID);
  }

  return concludeNode(additiveExprNode);
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  const auto multiplicativeExprNode = createNode<MultiplicativeExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_MUL, TY_INVALID);
    else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
             t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_DIV, TY_INVALID);
    else if (const auto t3 = dynamic_cast<TerminalNode *>(subTree);
             t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_REM, TY_INVALID);
  }

  return concludeNode(multiplicativeExprNode);
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  const auto castExprNode = createNode<CastExprNode>(ctx);

  // Enrich
  castExprNode->isCast = ctx->LPAREN();

  // Visit children
  visitChildren(ctx);

  return concludeNode(castExprNode);
}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  const auto prefixUnaryExprNode = createNode<PrefixUnaryExprNode>(ctx);

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

  // Visit children
  visitChildren(ctx);

  return concludeNode(prefixUnaryExprNode);
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  const auto postfixUnaryExprNode = createNode<PostfixUnaryExprNode>(ctx);

  // Extract operator
  if (ctx->LBRACKET()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_SUBSCRIPT;
  } else if (ctx->DOT()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MEMBER_ACCESS;
    postfixUnaryExprNode->identifier = getIdentifier(ctx->IDENTIFIER());
  } else if (ctx->PLUS_PLUS()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_PLUS_PLUS;
  } else if (ctx->MINUS_MINUS()) {
    postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MINUS_MINUS;
  }

  // Visit children
  visitChildren(ctx);

  return concludeNode(postfixUnaryExprNode);
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  const auto atomicExprNode = createNode<AtomicExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree);
        t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      std::string fragment = getIdentifier(t1);
      atomicExprNode->identifierFragments.push_back(fragment);
      if (!atomicExprNode->fqIdentifier.empty())
        atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      atomicExprNode->fqIdentifier += fragment;
    } else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
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
  const auto valueNode = createNode<ValueNode>(ctx);

  // Enrich
  valueNode->isNil = ctx->NIL();

  // Visit children
  visitChildren(ctx);

  return concludeNode(valueNode);
}

std::any ASTBuilder::visitConstant(SpiceParser::ConstantContext *ctx) {
  const auto constantNode = createNode<ConstantNode>(ctx);

  // Enrich
  auto &[doubleValue, intValue, shortValue, longValue, charValue, boolValue, stringValueOffset] = constantNode->compileTimeValue;
  if (ctx->DOUBLE_LIT()) {
    constantNode->type = ConstantNode::TYPE_DOUBLE;
    doubleValue = std::stod(ctx->DOUBLE_LIT()->toString());
  } else if (ctx->INT_LIT()) {
    constantNode->type = ConstantNode::TYPE_INT;
    intValue = parseInt(ctx->INT_LIT());
  } else if (ctx->SHORT_LIT()) {
    constantNode->type = ConstantNode::TYPE_SHORT;
    shortValue = parseShort(ctx->SHORT_LIT());
  } else if (ctx->LONG_LIT()) {
    constantNode->type = ConstantNode::TYPE_LONG;
    longValue = parseLong(ctx->LONG_LIT());
  } else if (ctx->CHAR_LIT()) {
    constantNode->type = ConstantNode::TYPE_CHAR;
    charValue = parseChar(ctx->CHAR_LIT());
  } else if (ctx->STRING_LIT()) {
    // Save a pointer to the string in the compile time value
    constantNode->type = ConstantNode::TYPE_STRING;
    stringValueOffset = resourceManager.compileTimeStringValues.size();
    // Add the string to the global compile time string list
    resourceManager.compileTimeStringValues.push_back(parseString(ctx->STRING_LIT()->toString()));
  } else if (ctx->TRUE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    boolValue = true;
  } else if (ctx->FALSE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    boolValue = false;
  } else {
    assert_fail("Unknown constant type"); // GCOV_EXCL_LINE
  }

  return concludeNode(constantNode);
}

std::any ASTBuilder::visitFctCall(SpiceParser::FctCallContext *ctx) {
  const auto fctCallNode = createNode<FctCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Enrich)
  fctCallNode->hasArgs = fctCallNode->argLst();
  fctCallNode->hasTemplateTypes = fctCallNode->templateTypeLst();

  for (antlr4::ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree);
        t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (const auto t3 = dynamic_cast<TerminalNode *>(subTree);
               t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::SCOPE_ACCESS) {
      fctCallNode->fqFunctionName += SCOPE_ACCESS_TOKEN;
    } else if (const auto t4 = dynamic_cast<TerminalNode *>(subTree);
               t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::DOT) {
      fctCallNode->fqFunctionName += MEMBER_ACCESS_TOKEN;
    }
  }

  return concludeNode(fctCallNode);
}

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  const auto arrayInitializationNode = createNode<ArrayInitializationNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(arrayInitializationNode);
}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  const auto structInstantiationNode = createNode<StructInstantiationNode>(ctx);

  // Enrich
  for (antlr4::ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree);
        t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment;
    }
  }

  // Visit children
  visitChildren(ctx);

  return concludeNode(structInstantiationNode);
}

std::any ASTBuilder::visitLambdaFunc(SpiceParser::LambdaFuncContext *ctx) {
  const auto lambdaFuncNode = createNode<LambdaFuncNode>(ctx);

  // Enrich
  lambdaFuncNode->hasParams = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(lambdaFuncNode);
}

std::any ASTBuilder::visitLambdaProc(SpiceParser::LambdaProcContext *ctx) {
  const auto lambdaProcNode = createNode<LambdaProcNode>(ctx);

  // Enrich
  lambdaProcNode->hasParams = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(lambdaProcNode);
}

std::any ASTBuilder::visitLambdaExpr(SpiceParser::LambdaExprContext *ctx) {
  const auto lambdaExprNode = createNode<LambdaExprNode>(ctx);

  // Enrich
  lambdaExprNode->hasParams = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(lambdaExprNode);
}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  const auto dataTypeNode = createNode<DataTypeNode>(ctx);

  // Enrich
  for (size_t i = 0; i < ctx->children.size(); i++) {
    antlr4::tree::ParseTree *subTree = ctx->children[i];
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL) {
      dataTypeNode->tmQueue.emplace(DataTypeNode::TYPE_PTR, false, 0);
    } else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::BITWISE_AND) {
      dataTypeNode->tmQueue.emplace(DataTypeNode::TYPE_REF, false, 0);
    } else if (const auto t3 = dynamic_cast<TerminalNode *>(subTree);
               t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = ctx->children[i];
      bool hasSize = false;
      unsigned int hardCodedSize = 0;
      std::string sizeVarName;
      if (const auto t4 = dynamic_cast<TerminalNode *>(subTree);
          t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        hardCodedSize = std::stoi(t4->getSymbol()->getText());
        i++; // Consume INT_LIT
      } else if (const auto t5 = dynamic_cast<TerminalNode *>(subTree);
                 t5 != nullptr && t5->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        hasSize = true;
        sizeVarName = getIdentifier(t5);
        i++; // Consume TYPE_IDENTIFIER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_ARRAY, hasSize, hardCodedSize, sizeVarName});
    }
  }

  // Visit children
  visitChildren(ctx);

  return concludeNode(dataTypeNode);
}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  const auto baseDataTypeNode = createNode<BaseDataTypeNode>(ctx);

  // Enrich
  if (ctx->TYPE_DOUBLE())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_DOUBLE;
  else if (ctx->TYPE_INT())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_INT;
  else if (ctx->TYPE_SHORT())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_SHORT;
  else if (ctx->TYPE_LONG())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_LONG;
  else if (ctx->TYPE_BYTE())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_BYTE;
  else if (ctx->TYPE_CHAR())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CHAR;
  else if (ctx->TYPE_STRING())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_STRING;
  else if (ctx->TYPE_BOOL())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_BOOL;
  else if (ctx->TYPE_DYN())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_DYN;
  else if (ctx->customDataType())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CUSTOM;
  else if (ctx->functionDataType())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_FUNCTION;
  else
    assert_fail("Unknown base data type");

  // Visit children
  visitChildren(ctx);

  return concludeNode(baseDataTypeNode);
}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  const auto customDataTypeNode = createNode<CustomDataTypeNode>(ctx);

  // Enrich
  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (const auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (const auto t2 = dynamic_cast<TerminalNode *>(subTree);
               t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment;
    }
  }

  // Visit children
  visitChildren(ctx);

  return concludeNode(customDataTypeNode);
}

std::any ASTBuilder::visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) {
  const auto functionDataTypeNode = createNode<FunctionDataTypeNode>(ctx);

  // Enrich
  functionDataTypeNode->isFunction = ctx->dataType();

  // Visit children
  visitChildren(ctx);

  return concludeNode(functionDataTypeNode);
}

std::any ASTBuilder::visitAssignOp(SpiceParser::AssignOpContext *ctx) {
  const auto assignExprNode = spice_pointer_cast<AssignExprNode *>(parentStack.top());

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
  const auto fctNameNode = spice_pointer_cast<FctNameNode *>(parentStack.top());

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
    if (isSigned)
      return std::stoll(substr, nullptr, base);
    else
      return static_cast<int64_t>(std::stoull(substr, nullptr, base));
  };
  return parseNumeric(terminal, cb);
}

int8_t ASTBuilder::parseChar(TerminalNode *terminal) const {
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
  } catch (std::out_of_range &) {
    const CodeLoc codeLoc(terminal->getSymbol(), sourceFile);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &) {
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
      if (escapeMap.contains(nextChar)) {
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
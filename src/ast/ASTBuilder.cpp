// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/ParserError.h>
#include <typechecker/OpRuleManager.h>
#include <util/CommonUtil.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

ASTBuilder::ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, antlr4::ANTLRInputStream *inputStream)
    : CompilerPass(resourceManager, sourceFile), filePath(sourceFile->filePath), inputStream(inputStream) {}

std::any ASTBuilder::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = createNode<EntryNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, entryNode);
}

std::any ASTBuilder::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  auto mainFctDefNode = createNode<MainFctDefNode>(ctx);

  // Enrich
  mainFctDefNode->takesArgs = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, mainFctDefNode);
}

std::any ASTBuilder::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  auto fctDefNode = createNode<FctDefNode>(ctx);

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

  return concludeNode(ctx, fctDefNode);
}

std::any ASTBuilder::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  auto procDefNode = createNode<ProcDefNode>(ctx);

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

  return concludeNode(ctx, procDefNode);
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
  visitChildren(ctx);

  return concludeNode(ctx, fctNameNode);
}

std::any ASTBuilder::visitStructDef(SpiceParser::StructDefContext *ctx) {
  auto structDefNode = createNode<StructDefNode>(ctx);

  // Enrich
  structDefNode->structName = getIdentifier(ctx->TYPE_IDENTIFIER());
  structDefNode->hasTemplateTypes = ctx->LESS();
  structDefNode->hasInterfaces = ctx->COLON();

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are struct attributes
  if (structDefNode->attrs())
    for (AttrNode *attr : structDefNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_STRUCT;

  return concludeNode(ctx, structDefNode);
}

std::any ASTBuilder::visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) {
  auto interfaceDefNode = createNode<InterfaceDefNode>(ctx);

  // Enrich
  interfaceDefNode->interfaceName = getIdentifier(ctx->TYPE_IDENTIFIER());
  interfaceDefNode->hasTemplateTypes = ctx->LESS();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, interfaceDefNode);
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  auto enumDefNode = createNode<EnumDefNode>(ctx);

  // Enrich
  enumDefNode->enumName = getIdentifier(ctx->TYPE_IDENTIFIER());

  // Visit children
  visitChildren(ctx);

  // Tell all items about the enum def
  for (EnumItemNode *enumItem : enumDefNode->itemLst()->items())
    enumItem->enumDef = enumDefNode;

  return concludeNode(ctx, enumDefNode);
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  auto genericTypeDefNode = createNode<GenericTypeDefNode>(ctx);

  // Enrich
  genericTypeDefNode->typeName = getIdentifier(ctx->TYPE_IDENTIFIER());

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, genericTypeDefNode);
}

std::any ASTBuilder::visitAliasDef(SpiceParser::AliasDefContext *ctx) {
  auto aliasDefNode = createNode<AliasDefNode>(ctx);

  // Enrich
  aliasDefNode->aliasName = getIdentifier(ctx->TYPE_IDENTIFIER());
  aliasDefNode->dataTypeString = ctx->dataType()->getText();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, aliasDefNode);
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  auto globalVarDefNode = createNode<GlobalVarDefNode>(ctx);

  // Enrich
  globalVarDefNode->varName = getIdentifier(ctx->TYPE_IDENTIFIER());
  globalVarDefNode->hasValue = ctx->ASSIGN();

  // Visit children
  visitChildren(ctx);

  // Tell the data type that it is a global one
  globalVarDefNode->dataType()->isGlobalType = true;

  return concludeNode(ctx, globalVarDefNode);
}

std::any ASTBuilder::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  auto extDeclNode = createNode<ExtDeclNode>(ctx);

  // Enrich
  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER() ? ctx->IDENTIFIER() : ctx->TYPE_IDENTIFIER());
  extDeclNode->hasArgs = ctx->typeLst();

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are ext decl attributes
  if (extDeclNode->attrs())
    for (AttrNode *attr : extDeclNode->attrs()->attrLst()->attributes())
      attr->target = AttrNode::TARGET_EXT_DECL;

  return concludeNode(ctx, extDeclNode);
}

std::any ASTBuilder::visitUnsafeBlock(SpiceParser::UnsafeBlockContext *ctx) {
  auto unsafeBlockDefNode = createNode<UnsafeBlockNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, unsafeBlockDefNode);
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto forLoopNode = createNode<ForLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, forLoopNode);
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  // Visit children
  visitChildren(ctx);

  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto foreachLoopNode = createNode<ForeachLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell the foreach item that it is one
  foreachLoopNode->itemVarDecl()->isForEachItem = true;

  return concludeNode(ctx, foreachLoopNode);
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  // Visit children
  visitChildren(ctx);

  return nullptr;
}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  auto whileLoopNode = createNode<WhileLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, whileLoopNode);
}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {
  auto doWhileLoopNode = createNode<DoWhileLoopNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, doWhileLoopNode);
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  auto ifStmtNode = createNode<IfStmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, ifStmtNode);
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  auto elseStmtNode = createNode<ElseStmtNode>(ctx);

  // Enrich
  elseStmtNode->isElseIf = ctx->ifStmt();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, elseStmtNode);
}

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  auto anonymousBlockStmtNode = createNode<AnonymousBlockStmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, anonymousBlockStmtNode);
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = createNode<StmtLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, stmtLstNode);
}

std::any ASTBuilder::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  auto typeLstNode = createNode<TypeLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, typeLstNode);
}

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = createNode<TypeAltsLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, typeAltsLstNode);
}

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  auto paramLstNode = createNode<ParamLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell all params and param types that they are such
  for (DeclStmtNode *param : paramLstNode->params()) {
    param->isParam = true;
    param->dataType()->isParamType = true;
  }

  return concludeNode(ctx, paramLstNode);
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  auto argLstNode = createNode<ArgLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, argLstNode);
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  auto enumItemLstNode = createNode<EnumItemLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, enumItemLstNode);
}

std::any ASTBuilder::visitEnumItem(SpiceParser::EnumItemContext *ctx) {
  auto enumItemNode = createNode<EnumItemNode>(ctx);

  // Enrich
  enumItemNode->itemName = getIdentifier(ctx->TYPE_IDENTIFIER());
  if (ctx->ASSIGN()) {
    enumItemNode->itemValue = parseInt(nullptr, ctx->INT_LIT());
    enumItemNode->hasValue = true;
  }

  return concludeNode(ctx, enumItemNode);
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = createNode<FieldNode>(ctx);

  // Enrich
  fieldNode->fieldName = getIdentifier(ctx->IDENTIFIER());

  // Visit children
  visitChildren(ctx);

  // Tell the data type that it is a field type
  fieldNode->dataType()->setFieldTypeRecursive();

  return concludeNode(ctx, fieldNode);
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {
  auto signatureNode = createNode<SignatureNode>(ctx);

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

  return concludeNode(ctx, signatureNode);
}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) {
  auto stmtNode = createNode<StmtNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, stmtNode);
}

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  auto declStmtNode = createNode<DeclStmtNode>(ctx);

  // Enrich
  declStmtNode->varName = getIdentifier(ctx->IDENTIFIER());
  declStmtNode->hasAssignment = ctx->ASSIGN();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, declStmtNode);
}

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  auto specifierLstNode = createNode<SpecifierLstNode>(ctx);

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

  return concludeNode(ctx, specifierLstNode);
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
      assert(false && "Unknown specifier type");
  }

  return concludeNode(ctx, specifierNode);
}

std::any ASTBuilder::visitModAttr(SpiceParser::ModAttrContext *ctx) {
  auto modAttrNode = createNode<ModAttrNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell the attributes that they are module attributes
  for (AttrNode *attr : modAttrNode->attrLst()->attributes())
    attr->target = AttrNode::TARGET_MODULE;

  return concludeNode(ctx, modAttrNode);
}

std::any ASTBuilder::visitTopLevelDefAttr(SpiceParser::TopLevelDefAttrContext *ctx) {
  auto fctAttrNode = createNode<TopLevelDefinitionAttrNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, fctAttrNode);
}

std::any ASTBuilder::visitAttrLst(SpiceParser::AttrLstContext *ctx) {
  auto attrLstNode = createNode<AttrLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, attrLstNode);
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

  return concludeNode(ctx, attrNode);
}

std::any ASTBuilder::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  auto importStmtNode = createNode<ImportStmtNode>(ctx);

  // Extract path
  const std::string pathStr = ctx->STRING_LIT()->getText();
  importStmtNode->importPath = pathStr.substr(1, pathStr.size() - 2);

  // If no name is given, use the path as name
  importStmtNode->importName = ctx->AS() ? getIdentifier(ctx->IDENTIFIER()) : importStmtNode->importPath;

  return concludeNode(ctx, importStmtNode);
}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  auto returnStmtNode = createNode<ReturnStmtNode>(ctx);

  // Enrich
  returnStmtNode->hasReturnValue = ctx->assignExpr();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, returnStmtNode);
}

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  auto breakStmtNode = createNode<BreakStmtNode>(ctx);

  // Extract number of breaks
  if (ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, breakStmtNode);
}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  auto continueStmtNode = createNode<ContinueStmtNode>(ctx);

  // Extract number of continues
  if (ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, continueStmtNode);
}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertStmtNode = createNode<AssertStmtNode>(ctx);

  // Enrich
  antlr4::misc::Interval interval(ctx->assignExpr()->start->getStartIndex(), ctx->assignExpr()->stop->getStopIndex());
  assertStmtNode->expressionString = inputStream->getText(interval);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, assertStmtNode);
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  // Visit children
  visitChildren(ctx);

  return nullptr;
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

  return concludeNode(ctx, printfCallNode);
}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  auto sizeofCallNode = createNode<SizeofCallNode>(ctx);

  // Check if type or value
  sizeofCallNode->isType = ctx->TYPE();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, sizeofCallNode);
}

std::any ASTBuilder::visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) {
  auto alignofCallNode = createNode<AlignofCallNode>(ctx);

  // Check if type or value
  alignofCallNode->isType = ctx->TYPE();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, alignofCallNode);
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto lenCallNode = createNode<LenCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, lenCallNode);
}

std::any ASTBuilder::visitPanicCall(SpiceParser::PanicCallContext *ctx) {
  auto panicCallNode = createNode<PanicCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, panicCallNode);
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  auto assignExprNode = createNode<AssignExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, assignExprNode);
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  auto ternaryExprNode = createNode<TernaryExprNode>(ctx);

  // Check if is shortened
  ternaryExprNode->isShortened = ctx->logicalOrExpr().size() == 2;

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, ternaryExprNode);
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  auto logicalOrExprNode = createNode<LogicalOrExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, logicalOrExprNode);
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  auto logicalAndExprNode = createNode<LogicalAndExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, logicalAndExprNode);
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  auto bitwiseOrExprNode = createNode<BitwiseOrExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, bitwiseOrExprNode);
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  auto bitwiseXorExprNode = createNode<BitwiseXorExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, bitwiseXorExprNode);
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  auto bitwiseAndExprNode = createNode<BitwiseAndExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, bitwiseAndExprNode);
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

  return concludeNode(ctx, equalityExprNode);
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

  return concludeNode(ctx, relationalExprNode);
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

  return concludeNode(ctx, shiftExprNode);
}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  auto additiveExprNode = createNode<AdditiveExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_PLUS, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_MINUS, SymbolType(TY_INVALID));
  }

  return concludeNode(ctx, additiveExprNode);
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  auto multiplicativeExprNode = createNode<MultiplicativeExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_MUL, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_DIV, SymbolType(TY_INVALID));
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_REM, SymbolType(TY_INVALID));
  }

  return concludeNode(ctx, multiplicativeExprNode);
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  auto castExprNode = createNode<CastExprNode>(ctx);

  // Enrich
  castExprNode->isCast = ctx->LPAREN();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, castExprNode);
}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  auto prefixUnaryExprNode = createNode<PrefixUnaryExprNode>(ctx);

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

  return concludeNode(ctx, prefixUnaryExprNode);
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto postfixUnaryExprNode = createNode<PostfixUnaryExprNode>(ctx);

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

  return concludeNode(ctx, postfixUnaryExprNode);
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  auto atomicExprNode = createNode<AtomicExprNode>(ctx);

  // Visit children
  visitChildren(ctx);

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

  return concludeNode(ctx, atomicExprNode);
}

std::any ASTBuilder::visitValue(SpiceParser::ValueContext *ctx) {
  auto valueNode = createNode<ValueNode>(ctx);

  // Enrich
  valueNode->isNil = ctx->NIL();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, valueNode);
}

std::any ASTBuilder::visitConstant(SpiceParser::ConstantContext *ctx) {
  auto constantNode = createNode<ConstantNode>(ctx);

  // Enrich
  if (ctx->DOUBLE_LIT()) {
    constantNode->type = ConstantNode::TYPE_DOUBLE;
    constantNode->compileTimeValue.doubleValue = std::stod(ctx->DOUBLE_LIT()->toString());
  } else if (ctx->INT_LIT()) {
    constantNode->type = ConstantNode::TYPE_INT;
    constantNode->compileTimeValue.intValue = parseInt(constantNode, ctx->INT_LIT());
  } else if (ctx->SHORT_LIT()) {
    constantNode->type = ConstantNode::TYPE_SHORT;
    constantNode->compileTimeValue.shortValue = parseShort(constantNode, ctx->SHORT_LIT());
  } else if (ctx->LONG_LIT()) {
    constantNode->type = ConstantNode::TYPE_LONG;
    constantNode->compileTimeValue.longValue = parseLong(constantNode, ctx->LONG_LIT());
  } else if (ctx->CHAR_LIT()) {
    constantNode->type = ConstantNode::TYPE_CHAR;
    constantNode->compileTimeValue.charValue = parseChar(ctx->CHAR_LIT());
  } else if (ctx->STRING_LIT()) {
    // Transfer ownership to the AST node
    constantNode->compileTimeStringValue = parseString(ctx->STRING_LIT()->toString());
    // Save a pointer to the string in the compile time value
    constantNode->type = ConstantNode::TYPE_STRING;
    constantNode->compileTimeValue.stringValue = constantNode->compileTimeStringValue.c_str();
  } else if (ctx->TRUE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    constantNode->compileTimeValue.boolValue = true;
  } else if (ctx->FALSE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    constantNode->compileTimeValue.boolValue = false;
  }
  constantNode->hasDirectCompileTimeValue = true;

  return concludeNode(ctx, constantNode);
}

std::any ASTBuilder::visitFctCall(SpiceParser::FctCallContext *ctx) {
  auto fctCallNode = createNode<FctCallNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Enrich)
  fctCallNode->hasArgs = fctCallNode->argLst();
  fctCallNode->hasTemplateTypes = fctCallNode->templateTypeLst();

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

  return concludeNode(ctx, fctCallNode);
}

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  auto arrayInitializationNode = createNode<ArrayInitializationNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, arrayInitializationNode);
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
  visitChildren(ctx);

  return concludeNode(ctx, structInstantiationNode);
}

std::any ASTBuilder::visitLambdaFunc(SpiceParser::LambdaFuncContext *ctx) {
  auto lambdaFuncNode = createNode<LambdaFuncNode>(ctx);

  // Enrich
  lambdaFuncNode->hasParams = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, lambdaFuncNode);
}

std::any ASTBuilder::visitLambdaProc(SpiceParser::LambdaProcContext *ctx) {
  auto lambdaProcNode = createNode<LambdaProcNode>(ctx);

  // Enrich
  lambdaProcNode->hasParams = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, lambdaProcNode);
}

std::any ASTBuilder::visitLambdaExpr(SpiceParser::LambdaExprContext *ctx) {
  auto lambdaExprNode = createNode<LambdaExprNode>(ctx);

  // Enrich
  lambdaExprNode->hasParams = ctx->paramLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, lambdaExprNode);
}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto dataTypeNode = createNode<DataTypeNode>(ctx);

  // Enrich
  for (int i = 0; i < ctx->children.size(); i++) {
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

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, dataTypeNode);
}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  auto baseDataTypeNode = createNode<BaseDataTypeNode>(ctx);

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

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, baseDataTypeNode);
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
  visitChildren(ctx);

  return concludeNode(ctx, customDataTypeNode);
}

std::any ASTBuilder::visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) {
  auto functionDataTypeNode = createNode<FunctionDataTypeNode>(ctx);

  // Enrich
  functionDataTypeNode->isFunction = ctx->dataType();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, functionDataTypeNode);
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
    assert(false && "Unknown assign operator");
  assignExprNode->hasOperator = true;

  return nullptr;
}

std::any ASTBuilder::visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) {
  auto fctNameNode = spice_pointer_cast<FctNameNode *>(parentStack.top());

  // Enrich
  if (ctx->PLUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_PLUS;
    fctNameNode->name = OP_FCT_PLUS;
  } else if (ctx->MINUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MINUS;
    fctNameNode->name = OP_FCT_MINUS;
  } else if (ctx->MUL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MUL;
    fctNameNode->name = OP_FCT_MUL;
  } else if (ctx->DIV()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_DIV;
    fctNameNode->name = OP_FCT_DIV;
  } else if (ctx->EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_EQUAL;
    fctNameNode->name = OP_FCT_EQUAL;
  } else if (ctx->NOT_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    fctNameNode->name = OP_FCT_NOT_EQUAL;
  } else if (ctx->LESS().size() == 2) {
    fctNameNode->overloadedOperator = FctNameNode::OP_SHL;
    fctNameNode->name = OP_FCT_SHL;
  } else if (ctx->GREATER().size() == 2) {
    fctNameNode->overloadedOperator = FctNameNode::OP_SHR;
    fctNameNode->name = OP_FCT_SHR;
  } else if (ctx->PLUS_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_PLUS_EQUAL;
    fctNameNode->name = OP_FCT_PLUS_EQUAL;
  } else if (ctx->MINUS_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MINUS_EQUAL;
    fctNameNode->name = OP_FCT_MINUS_EQUAL;
  } else if (ctx->MUL_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MUL_EQUAL;
    fctNameNode->name = OP_FCT_MUL_EQUAL;
  } else if (ctx->DIV_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_DIV_EQUAL;
    fctNameNode->name = OP_FCT_DIV_EQUAL;
  } else if (ctx->PLUS_PLUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_PLUS_PLUS;
    fctNameNode->name = OP_FCT_POSTFIX_PLUS_PLUS;
  } else if (ctx->MINUS_MINUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MINUS_MINUS;
    fctNameNode->name = OP_FCT_POSTFIX_MINUS_MINUS;
  } else {
    assert(false);
  }
  fctNameNode->fqName = fctNameNode->name;
  fctNameNode->nameFragments.push_back(fctNameNode->name);

  return nullptr;
}

template <typename T> T *ASTBuilder::createNode(const ParserRuleContext *ctx) {
  ASTNode *parent = nullptr;
  if constexpr (!std::is_same_v<T, EntryNode>)
    parent = parentStack.top();

  // Create the new node
  resourceManager.astNodes.push_back(std::make_unique<T>(parent, getCodeLoc(ctx)));
  T *node = static_cast<T *>(resourceManager.astNodes.back().get());
  node->reserveChildren(ctx->children.size());

  // If this is not the entry node, we need to add the new node to its parent
  if constexpr (!std::is_same_v<T, EntryNode>)
    parent->addChild(node);

  // This node is the parent for its children
  parentStack.push(node);

  return node;
}

template <typename T> T *ASTBuilder::concludeNode(const ParserRuleContext *ctx, T *node) {
  // For all nodes, except the entry and stmtLst nodes, create and save the error message
  if constexpr (!std::is_same_v<T, EntryNode> && !std::is_same_v<T, StmtLstNode>)
    saveErrorMessage(node, ctx);

  // This node is no longer the parent for its children
  assert(parentStack.top() == node);
  parentStack.pop();

  return node;
}

int32_t ASTBuilder::parseInt(ConstantNode *constantNode, TerminalNode *terminal) {
  std::function<int32_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoi(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int16_t ASTBuilder::parseShort(ConstantNode *constantNode, TerminalNode *terminal) {
  std::function<int16_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return (int16_t)std::stoi(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int64_t ASTBuilder::parseLong(ConstantNode *constantNode, TerminalNode *terminal) {
  std::function<int64_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
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
      const CodeLoc codeLoc(terminal->getSymbol(), filePath);
      throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid escape sequence " + input);
    }
  } else {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid char literal " + input);
  }
}

std::string ASTBuilder::parseString(std::string input) {
  input = input.substr(1, input.size() - 2);
  replaceEscapeChars(input);
  return input;
}

template <typename T>
T ASTBuilder::parseNumeric(ConstantNode *constantNode, TerminalNode *terminal, std::function<T(const std::string &, int)> cb) {
  const std::string input = terminal->toString();

  // Set to signed if the input string ends with 'u'
  if (constantNode)
    constantNode->isSigned = input.ends_with('u');

  try {
    if (input.length() >= 3) {
      const char c1 = input[0];
      const char c2 = input[1];
      const std::string subStr = input.substr(2);
      if (c1 == '0') {
        switch (c2) {
        case 'd':
        case 'D':
          return cb(subStr, 10);
        case 'b':
        case 'B':
          return cb(subStr, 2);
        case 'h':
        case 'H':
        case 'x':
        case 'X':
          return cb(subStr, 16);
        case 'o':
        case 'O':
          return cb(subStr, 8);
        default:
          return cb(input, 10);
        }
      }
    }
    return cb(input, 10);
  } catch (std::out_of_range &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
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
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, RESERVED_KEYWORD, "'" + identifier + "' is a reserved keyword. Please use another name instead");
  }

  return identifier;
}

void ASTBuilder::saveErrorMessage(ASTNode *node, const ParserRuleContext *ctx) {
  const antlr4::misc::Interval sourceInterval(ctx->start->getStartIndex(), ctx->stop->getStopIndex());
  antlr4::misc::Interval extSourceInterval(sourceInterval);

  // If we have a multi-line interval, only use the first line
  if (size_t offset = inputStream->getText(extSourceInterval).find('\n'); offset != std::string::npos)
    extSourceInterval.b = extSourceInterval.a + static_cast<ssize_t>(offset);

  size_t markerIndentation = 0;
  for (; markerIndentation < ERROR_MESSAGE_CONTEXT; markerIndentation++) {
    extSourceInterval.a--;
    if (extSourceInterval.a < 0 || inputStream->getText(extSourceInterval).find('\n') != std::string::npos) {
      extSourceInterval.a++;
      break;
    }
  }
  for (size_t suffixContext = 0; suffixContext < ERROR_MESSAGE_CONTEXT; suffixContext++) {
    extSourceInterval.b++;
    if (extSourceInterval.b > inputStream->size() || inputStream->getText(extSourceInterval).find('\n') != std::string::npos) {
      extSourceInterval.b--;
      break;
    }
  }

  // Trim start
  while (inputStream->getText(extSourceInterval)[0] == ' ') {
    extSourceInterval.a++;
    markerIndentation--;
  }

  // Trim end
  if (inputStream->getText(extSourceInterval)[extSourceInterval.length() - 1] == '\n')
    extSourceInterval.b--;

  const std::string lineNumberStr = std::to_string(ctx->start->getLine());
  markerIndentation += lineNumberStr.length() + 2;

  // Build error message
  std::stringstream ss;
  ss << lineNumberStr << "  " << inputStream->getText(extSourceInterval) << "\n";
  ss << std::string(markerIndentation, ' ') << std::string(std::min(sourceInterval.length(), extSourceInterval.length()), '^');
  node->errorMessage = ss.str();
}

} // namespace spice::compiler
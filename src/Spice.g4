// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

grammar Spice;

options {
  language = Cpp;
}

@parser::header {
  #include <util/ParserHelper.h>

  using namespace spice::compiler;
}

@parser::members {
  ParserHelper helper;
}

// Top level definitions and declarations
entry returns [Ptr<EntryNode> node]
@init {
  auto entryNode = helper.createNode<EntryNode>($ctx);
}
@after {
  helper.concludeNode($ctx, entryNode);
  $node = std::move(entryNode);
}: (mainFunctionDef | functionDef | procedureDef | structDef | interfaceDef | enumDef | genericTypeDef | aliasDef | globalVarDef | importStmt | extDecl | modAttr)*;

mainFunctionDef returns [Ptr<MainFctDefNode> node]
@init {
  auto mainFctDefNode = helper.createNode<MainFctDefNode>($ctx);
}
@after {
  if (mainFctDefNode->paramLst())
    mainFctDefNode->takesArgs = true;

  helper.concludeNode($ctx, mainFctDefNode);
  $node = std::move(mainFctDefNode);
}: fctAttr? F LESS TYPE_INT GREATER MAIN LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;

functionDef returns [Ptr<FctDefNode> node]
@init {
  auto fctDefNode = helper.createNode<FctDefNode>($ctx);
}
@after {
  // Let data type know that it is a return type
  fctDefNode->returnType()->isReturnType = true;

  if (fctDefNode->templateTypeLst())
    fctDefNode->hasTemplateTypes = true;
  if (fctDefNode->paramLst())
    fctDefNode->hasParams = true;

  // Retrieve information from the symbol name
  fctDefNode->fctName = fctDefNode->getChild<FctNameNode>();
  fctDefNode->isMethod = fctDefNode->fctName->nameFragments.size() > 1;

  helper.concludeNode($ctx, fctDefNode);
  $node = std::move(fctDefNode);
}: fctAttr? specifierLst? F LESS dataType GREATER fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;

procedureDef returns [Ptr<ProcDefNode> node]
@init {
  auto procDefNode = helper.createNode<ProcDefNode>($ctx);
}
@after {
  if (procDefNode->templateTypeLst())
    procDefNode->hasTemplateTypes = true;
  if (procDefNode->paramLst())
    procDefNode->hasParams = true;

  // Retrieve information from the symbol name
  procDefNode->procName = procDefNode->getChild<FctNameNode>();
  procDefNode->isMethod = procDefNode->procName->nameFragments.size() > 1;

  helper.concludeNode($ctx, procDefNode);
  $node = std::move(procDefNode);
}: fctAttr? specifierLst? P fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;

fctName returns [Ptr<FctNameNode> node]
@init {
  auto fctNameNode = helper.createNode<FctNameNode>($ctx);
}
@after {
  // Extract function name
  if ($ctx->TYPE_IDENTIFIER()) {
    const std::string typeIdentifier = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());
    fctNameNode->structName = typeIdentifier;
    fctNameNode->fqName = typeIdentifier + MEMBER_ACCESS_TOKEN;
    fctNameNode->nameFragments.push_back(typeIdentifier);
  }
  if ($ctx->IDENTIFIER()) {
    const std::string fctIdentifier = ParserHelper::getIdentifier($ctx->IDENTIFIER());
    fctNameNode->name = fctIdentifier;
    fctNameNode->fqName += fctIdentifier;
    fctNameNode->nameFragments.push_back(fctIdentifier);
  }
  // Extract overloaded operator
  if ($ctx->OPERATOR()) {
    SpiceParser::OverloadableOpContext *op = $ctx->overloadableOp();
    if (op->PLUS()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_PLUS;
      fctNameNode->name = OP_FCT_PLUS;
    } else if (op->MINUS()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_MINUS;
      fctNameNode->name = OP_FCT_MINUS;
    } else if (op->MUL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_MUL;
      fctNameNode->name = OP_FCT_MUL;
    } else if (op->DIV()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_DIV;
      fctNameNode->name = OP_FCT_DIV;
    } else if (op->EQUAL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_EQUAL;
      fctNameNode->name = OP_FCT_EQUAL;
    } else if (op->NOT_EQUAL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
      fctNameNode->name = OP_FCT_NOT_EQUAL;
    } else if (op->LESS().size() == 2) {
      fctNameNode->overloadedOperator = FctNameNode::OP_SHL;
      fctNameNode->name = OP_FCT_SHL;
    } else if (op->GREATER().size() == 2) {
      fctNameNode->overloadedOperator = FctNameNode::OP_SHR;
      fctNameNode->name = OP_FCT_SHR;
    } else if (op->PLUS_EQUAL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_PLUS_EQUAL;
      fctNameNode->name = OP_FCT_PLUS_EQUAL;
    } else if (op->MINUS_EQUAL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_MINUS_EQUAL;
      fctNameNode->name = OP_FCT_MINUS_EQUAL;
    } else if (op->MUL_EQUAL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_MUL_EQUAL;
      fctNameNode->name = OP_FCT_MUL_EQUAL;
    } else if (op->DIV_EQUAL()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_DIV_EQUAL;
      fctNameNode->name = OP_FCT_DIV_EQUAL;
    } else if (op->PLUS_PLUS()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_PLUS_PLUS;
      fctNameNode->name = OP_FCT_POSTFIX_PLUS_PLUS;
    } else if (op->MINUS_MINUS()) {
      fctNameNode->overloadedOperator = FctNameNode::OP_MINUS_MINUS;
      fctNameNode->name = OP_FCT_POSTFIX_MINUS_MINUS;
    } else {
      assert(false && "FctName parsing fall-through");
    }
    fctNameNode->fqName = fctNameNode->name;
    fctNameNode->nameFragments.push_back(fctNameNode->name);
  }

  helper.concludeNode($ctx, fctNameNode);
  $node = std::move(fctNameNode);
}: (TYPE_IDENTIFIER DOT)? IDENTIFIER | OPERATOR overloadableOp;

structDef returns [Ptr<StructDefNode> node]
@init {
  auto structDefNode = helper.createNode<StructDefNode>($ctx);
}
@after {
  structDefNode->structName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());
  if ($ctx->templateList)
    structDefNode->isGeneric = true;
  if ($ctx->interfaceTypeList)
    structDefNode->hasInterfaces = true;

  helper.concludeNode($ctx, structDefNode);
  $node = std::move(structDefNode);
}: specifierLst? TYPE TYPE_IDENTIFIER (LESS templateList=typeLst GREATER)? STRUCT (COLON interfaceTypeList=typeLst)? LBRACE field* RBRACE;

interfaceDef returns [Ptr<InterfaceDefNode> node]
@init {
  auto interfaceDefNode = helper.createNode<InterfaceDefNode>($ctx);
}
@after {
  interfaceDefNode->interfaceName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());

  if ($ctx->typeLst())
    interfaceDefNode->isGeneric = true;

  helper.concludeNode($ctx, interfaceDefNode);
  $node = std::move(interfaceDefNode);
}: specifierLst? TYPE TYPE_IDENTIFIER (LESS typeLst GREATER)? INTERFACE LBRACE signature+ RBRACE;

enumDef returns [Ptr<EnumDefNode> node]
@init {
  auto enumDefNode = helper.createNode<EnumDefNode>($ctx);
}
@after {
  enumDefNode->enumName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());

  helper.concludeNode($ctx, enumDefNode);
  $node = std::move(enumDefNode);
}: specifierLst? TYPE TYPE_IDENTIFIER ENUM LBRACE enumItemLst RBRACE;

genericTypeDef returns [Ptr<GenericTypeDefNode> node]
@init {
  auto genericTypeDefNode = helper.createNode<GenericTypeDefNode>($ctx);
}
@after {
  genericTypeDefNode->typeName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());

  helper.concludeNode($ctx, genericTypeDefNode);
  $node = std::move(genericTypeDefNode);
}: TYPE TYPE_IDENTIFIER typeAltsLst SEMICOLON;

aliasDef returns [Ptr<AliasDefNode> node]
@init {
  auto aliasDefNode = helper.createNode<AliasDefNode>($ctx);
}
@after {
  aliasDefNode->aliasName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());
  aliasDefNode->dataTypeString = $ctx->dataType()->getText();

  helper.concludeNode($ctx, aliasDefNode);
  $node = std::move(aliasDefNode);
}: TYPE TYPE_IDENTIFIER ALIAS dataType SEMICOLON;

globalVarDef returns [Ptr<GlobalVarDefNode> node]
@init {
  auto globalVarDefNode = helper.createNode<GlobalVarDefNode>($ctx);
}
@after {
  globalVarDefNode->varName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());
  globalVarDefNode->dataType()->isGlobalType = true;
  if (globalVarDefNode->constant())
    globalVarDefNode->hasValue = true;

  helper.concludeNode($ctx, globalVarDefNode);
  $node = std::move(globalVarDefNode);
}: dataType TYPE_IDENTIFIER (ASSIGN constant)? SEMICOLON;

extDecl returns [Ptr<ExtDeclNode> node]
@init {
  auto extDeclNode = helper.createNode<ExtDeclNode>($ctx);
}
@after {
  extDeclNode->extFunctionName = ParserHelper::getIdentifier($ctx->IDENTIFIER());
  if (extDeclNode->argTypeLst())
    extDeclNode->hasArgs = true;

  helper.concludeNode($ctx, extDeclNode);
  $node = std::move(extDeclNode);
}: fctAttr? EXT (F LESS dataType GREATER | P) IDENTIFIER LPAREN (typeLst ELLIPSIS?)? RPAREN SEMICOLON;

// Control structures
unsafeBlockDef returns [Ptr<UnsafeBlockDefNode> node]
@init {
  auto unsafeBlockDefNode = helper.createNode<UnsafeBlockDefNode>($ctx);
}
@after {
  helper.concludeNode($ctx, unsafeBlockDefNode);
  $node = std::move(unsafeBlockDefNode);
}: UNSAFE LBRACE stmtLst RBRACE;

forLoop returns [Ptr<ForLoopNode> node]
@init {
  auto forLoopNode = helper.createNode<ForLoopNode>($ctx);
}
@after {
  helper.concludeNode($ctx, forLoopNode);
  $node = std::move(forLoopNode);
}: FOR (forHead | LPAREN forHead RPAREN) LBRACE stmtLst RBRACE;

forHead: declStmt SEMICOLON assignExpr SEMICOLON assignExpr;

foreachLoop returns [Ptr<ForeachLoopNode> node]
@init {
  auto foreachLoopNode = helper.createNode<ForeachLoopNode>($ctx);
}
@after {
  foreachLoopNode->itemVarDecl()->isForEachItem = true;

  helper.concludeNode($ctx, foreachLoopNode);
  $node = std::move(foreachLoopNode);
}: FOREACH (foreachHead | LPAREN foreachHead RPAREN) LBRACE stmtLst RBRACE;

foreachHead: (declStmt COMMA)? declStmt COLON assignExpr;

whileLoop returns [Ptr<WhileLoopNode> node]
@init {
  auto whileLoopNode = helper.createNode<WhileLoopNode>($ctx);
}
@after {
  helper.concludeNode($ctx, whileLoopNode);
  $node = std::move(whileLoopNode);
}: WHILE assignExpr LBRACE stmtLst RBRACE;

doWhileLoop returns [Ptr<DoWhileLoopNode> node]
@init {
  auto doWhileLoopNode = helper.createNode<DoWhileLoopNode>($ctx);
}
@after {
  helper.concludeNode($ctx, doWhileLoopNode);
  $node = std::move(doWhileLoopNode);
}: DO LBRACE stmtLst RBRACE WHILE assignExpr SEMICOLON;

ifStmt returns [Ptr<IfStmtNode> node]
@init {
  auto ifStmtNode = helper.createNode<IfStmtNode>($ctx);
}
@after {
  helper.concludeNode($ctx, ifStmtNode);
  $node = std::move(ifStmtNode);
}: IF assignExpr LBRACE stmtLst RBRACE elseStmt?;

elseStmt returns [Ptr<ElseStmtNode> node]
@init {
  auto elseStmtNode = helper.createNode<ElseStmtNode>($ctx);
}
@after {
  if (elseStmtNode->ifStmt())
    elseStmtNode->isElseIf = true;

  helper.concludeNode($ctx, elseStmtNode);
  $node = std::move(elseStmtNode);
}: ELSE ifStmt | ELSE LBRACE stmtLst RBRACE;

anonymousBlockStmt returns [Ptr<AnonymousBlockStmtNode> node]
@init {
  auto anonymousBlockStmtNode = helper.createNode<AnonymousBlockStmtNode>($ctx);
}
@after {
  helper.concludeNode($ctx, anonymousBlockStmtNode);
  $node = std::move(anonymousBlockStmtNode);
}: LBRACE stmtLst RBRACE;

// Statements, declarations, definitions and lists
stmtLst returns [Ptr<StmtLstNode> node]
@init {
  auto stmtLstNode = helper.createNode<StmtLstNode>($ctx);
}
@after {
  helper.concludeNode($ctx, stmtLstNode);
  $node = std::move(stmtLstNode);
}: (stmt | forLoop | foreachLoop | whileLoop | doWhileLoop | ifStmt | assertStmt | unsafeBlockDef | anonymousBlockStmt)*;

typeLst returns [Ptr<TypeLstNode> node]
@init {
  auto typeLstNode = helper.createNode<TypeLstNode>($ctx);
}
@after {
  helper.concludeNode($ctx, typeLstNode);
  $node = std::move(typeLstNode);
}: dataType (COMMA dataType)*;

typeAltsLst returns [Ptr<TypeAltsLstNode> node]
@init {
  auto typeAltsLstNode = helper.createNode<TypeAltsLstNode>($ctx);
}
@after {
  helper.concludeNode($ctx, typeAltsLstNode);
  $node = std::move(typeAltsLstNode);
}: dataType (BITWISE_OR dataType)*;

paramLst returns [Ptr<ParamLstNode> node]
@init {
  auto paramLstNode = helper.createNode<ParamLstNode>($ctx);
}
@after {
  // Let all param decl stmts know that they are params
  for (DeclStmtNode* param : paramLstNode->params())
    param->isParam = true;

  helper.concludeNode($ctx, paramLstNode);
  $node = std::move(paramLstNode);
}: declStmt (COMMA declStmt)*;

argLst returns [Ptr<ArgLstNode> node]
@init {
  auto argLstNode = helper.createNode<ArgLstNode>($ctx);
}
@after {
  helper.concludeNode($ctx, argLstNode);
  $node = std::move(argLstNode);
}: assignExpr (COMMA assignExpr)*;

enumItemLst returns [Ptr<EnumItemLstNode> node]
@init {
  auto enumItemLstNode = helper.createNode<EnumItemLstNode>($ctx);
}
@after {
  helper.concludeNode($ctx, enumItemLstNode);
  $node = std::move(enumItemLstNode);
}: enumItem (COMMA enumItem)*;

enumItem returns [Ptr<EnumItemNode> node]
@init {
  auto enumItemNode = helper.createNode<EnumItemNode>($ctx);
}
@after {
  enumItemNode->itemName = ParserHelper::getIdentifier($ctx->TYPE_IDENTIFIER());
  if ($ctx->INT_LIT()) {
    enumItemNode->itemValue = helper.parseInt(nullptr, $ctx->INT_LIT());
    enumItemNode->hasValue = true;
  }

  helper.concludeNode($ctx, enumItemNode);
  $node = std::move(enumItemNode);
}: TYPE_IDENTIFIER (ASSIGN INT_LIT)?;

field returns [Ptr<FieldNode> node]
@init {
  auto fieldNode = helper.createNode<FieldNode>($ctx);
}
@after {
  fieldNode->fieldName = ParserHelper::getIdentifier($ctx->IDENTIFIER());

  helper.concludeNode($ctx, fieldNode);
  $node = std::move(fieldNode);
}: dataType IDENTIFIER (ASSIGN constant)?;

signature returns [Ptr<SignatureNode> node]
@init {
  auto signatureNode = helper.createNode<SignatureNode>($ctx);
}
@after {
  // Extract method name
  signatureNode->methodName = ParserHelper::getIdentifier($ctx->IDENTIFIER());
  // Extract signature type
  signatureNode->signatureType = $ctx->F() ? SignatureNode::TYPE_FUNCTION : SignatureNode::TYPE_PROCEDURE;
  signatureNode->signatureSpecifiers = $ctx->F() ? TypeSpecifiers::of(TY_FUNCTION) : TypeSpecifiers::of(TY_PROCEDURE);

  if ($ctx->F() ? $ctx->LESS().size() == 2 : $ctx->LESS().size() == 1)
    signatureNode->hasTemplateTypes = true;
  if ($ctx->typeLst().size() == 2 || ($ctx->typeLst().size() == 1 && !signatureNode->hasTemplateTypes))
    signatureNode->hasParams = true;

  helper.concludeNode($ctx, signatureNode);
  $node = std::move(signatureNode);
}: specifierLst? (F LESS dataType GREATER | P) IDENTIFIER (LESS typeLst GREATER)? LPAREN typeLst? RPAREN SEMICOLON;

stmt returns [Ptr<StmtNode> node]
@init {
  auto stmtNode = helper.createNode<StmtNode>($ctx);
}
@after {
  helper.concludeNode($ctx, stmtNode);
  $node = std::move(stmtNode);
}: (declStmt | assignExpr | returnStmt | breakStmt | continueStmt) SEMICOLON;

declStmt returns [Ptr<DeclStmtNode> node]
@init {
  auto declStmtNode = helper.createNode<DeclStmtNode>($ctx);
}
@after {
  declStmtNode->varName = ParserHelper::getIdentifier($ctx->IDENTIFIER());
  if (declStmtNode->assignExpr())
    declStmtNode->hasAssignment = true;

  helper.concludeNode($ctx, declStmtNode);
  $node = std::move(declStmtNode);
}: dataType IDENTIFIER (ASSIGN assignExpr)?;

specifierLst returns [Ptr<SpecifierLstNode> node]
@init {
  auto specifierLstNode = helper.createNode<SpecifierLstNode>($ctx);
}
@after {
  // Check if we have both, signed and unsigned specifier
  bool seenSignedOrUnsigned = false;
  for (const SpecifierNode *specifier : specifierLstNode->specifiers()) {
    if (specifier->type != SpecifierNode::TY_SIGNED && specifier->type != SpecifierNode::TY_UNSIGNED)
      continue;
    if (seenSignedOrUnsigned)
      throw ParserError(specifier->codeLoc, INVALID_SPECIFIER_COMBINATION, "A variable can not be signed and unsigned");
    seenSignedOrUnsigned = true;
  }

  helper.concludeNode($ctx, specifierLstNode);
  $node = std::move(specifierLstNode);
}: specifier+;

specifier returns [Ptr<SpecifierNode> node]
@init {
  auto specifierNode = helper.createNode<SpecifierNode>($ctx);
}
@after {
  for (ParserRuleContext::ParseTree *subTree : $ctx->children) {
    auto token = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
    assert(token != nullptr);
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
    else
      assert(false && "Specifier parsing fall-through");
  }

  helper.concludeNode($ctx, specifierNode);
  $node = std::move(specifierNode);
}: CONST | SIGNED | UNSIGNED | INLINE | PUBLIC | HEAP;

modAttr returns [Ptr<ModAttrNode> node]
@init {
  auto modAttrNode = helper.createNode<ModAttrNode>($ctx);
}
@after {
  helper.concludeNode($ctx, modAttrNode);
  $node = std::move(modAttrNode);
}: MOD_ATTR_PREAMBLE LBRACKET attrLst RBRACKET;

fctAttr returns [Ptr<FctAttrNode> node]
@init {
  auto fctAttrNode = helper.createNode<FctAttrNode>($ctx);
}
@after {
  helper.concludeNode($ctx, fctAttrNode);
  $node = std::move(fctAttrNode);
}: FCT_ATTR_PREAMBLE LBRACKET attrLst RBRACKET;

attrLst returns [Ptr<AttrLstNode> node]
@init {
  auto attrLstNode = helper.createNode<AttrLstNode>($ctx);
}
@after {
  helper.concludeNode($ctx, attrLstNode);
  $node = std::move(attrLstNode);
}: attr (COMMA attr)*;

attr returns [Ptr<AttrNode> node]
@init {
  auto attrNode = helper.createNode<AttrNode>($ctx);
}
@after {
  for (const antlr4::tree::TerminalNode *keyFragment : $ctx->IDENTIFIER()) {
    if (!attrNode->key.empty())
      attrNode->key += MEMBER_ACCESS_TOKEN;
    attrNode->key += keyFragment->getSymbol()->getText();
  }

  helper.concludeNode($ctx, attrNode);
  $node = std::move(attrNode);
}: IDENTIFIER (DOT IDENTIFIER)* ASSIGN constant;

importStmt returns [Ptr<ImportStmtNode> node]
@init {
  auto importStmtNode = helper.createNode<ImportStmtNode>($ctx);
}
@after {
  const std::string pathStr = $ctx->STRING_LIT()->getText();
  importStmtNode->importPath = pathStr.substr(1, pathStr.size() - 2);
  importStmtNode->importName = $ctx->AS() ? ParserHelper::getIdentifier($ctx->IDENTIFIER()) : importStmtNode->importPath;

  helper.concludeNode($ctx, importStmtNode);
  $node = std::move(importStmtNode);
}: IMPORT STRING_LIT (AS IDENTIFIER)? SEMICOLON;

returnStmt returns [Ptr<ReturnStmtNode> node]
@init {
  auto returnStmtNode = helper.createNode<ReturnStmtNode>($ctx);
}
@after {
  if (returnStmtNode->assignExpr())
    returnStmtNode->hasReturnValue = true;

  helper.concludeNode($ctx, returnStmtNode);
  $node = std::move(returnStmtNode);
}: RETURN assignExpr?;

breakStmt returns [Ptr<BreakStmtNode> node]
@init {
  auto breakStmtNode = helper.createNode<BreakStmtNode>($ctx);
}
@after {
  if ($ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi($ctx->INT_LIT()->toString());

  helper.concludeNode($ctx, breakStmtNode);
  $node = std::move(breakStmtNode);
}: BREAK INT_LIT?;

continueStmt returns [Ptr<ContinueStmtNode> node]
@init {
  auto continueStmtNode = helper.createNode<ContinueStmtNode>($ctx);
}
@after {
  if ($ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi($ctx->INT_LIT()->toString());

  helper.concludeNode($ctx, continueStmtNode);
  $node = std::move(continueStmtNode);
}: CONTINUE INT_LIT?;

assertStmt returns [Ptr<AssertStmtNode> node]
@init {
  auto assertStmtNode = helper.createNode<AssertStmtNode>($ctx);
}
@after {
  assertStmtNode->expressionString = $ctx->assignExpr()->getText();

  helper.concludeNode($ctx, assertStmtNode);
  $node = std::move(assertStmtNode);
}: ASSERT assignExpr SEMICOLON;

// Builtin functions
builtinCall: printfCall | sizeOfCall | alignOfCall | lenCall;

printfCall returns [Ptr<PrintfCallNode> node]
@init {
  auto printfCallNode = helper.createNode<PrintfCallNode>($ctx);
}
@after {
  // Extract templated string
  std::string templatedString = $ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  ParserHelper::replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  helper.concludeNode($ctx, printfCallNode);
  $node = std::move(printfCallNode);
}: PRINTF LPAREN STRING_LIT (COMMA assignExpr)* RPAREN;

sizeOfCall returns [Ptr<SizeofCallNode> node]
@init {
  auto sizeofCallNode = helper.createNode<SizeofCallNode>($ctx);
}
@after {
  sizeofCallNode->isType = $ctx->TYPE();

  helper.concludeNode($ctx, sizeofCallNode);
  $node = std::move(sizeofCallNode);
}: SIZEOF LPAREN (assignExpr | TYPE dataType) RPAREN;

alignOfCall returns [Ptr<AlignofCallNode> node]
@init {
  auto alignofCallNode = helper.createNode<AlignofCallNode>($ctx);
}
@after {
  alignofCallNode->isType = $ctx->TYPE();

  helper.concludeNode($ctx, alignofCallNode);
  $node = std::move(alignofCallNode);
}: ALIGNOF LPAREN (assignExpr | TYPE dataType) RPAREN;

lenCall returns [Ptr<LenCallNode> node]
@init {
  auto lenCallNode = helper.createNode<LenCallNode>($ctx);
}
@after {
  helper.concludeNode($ctx, lenCallNode);
  $node = std::move(lenCallNode);
}: LEN LPAREN assignExpr RPAREN;

// Expression loop
assignExpr returns [Ptr<AssignExprNode> node]
@init {
  auto assignExprNode = helper.createNode<AssignExprNode>($ctx);
}
@after {
  // Extract assign operator
  if (auto assignOp = $ctx->assignOp(); assignOp != nullptr) {
    if (assignOp->ASSIGN())
      assignExprNode->op = AssignExprNode::OP_ASSIGN;
    else if (assignOp->PLUS_EQUAL())
      assignExprNode->op = AssignExprNode::OP_PLUS_EQUAL;
    else if (assignOp->MINUS_EQUAL())
      assignExprNode->op = AssignExprNode::OP_MINUS_EQUAL;
    else if (assignOp->MUL_EQUAL())
      assignExprNode->op = AssignExprNode::OP_MUL_EQUAL;
    else if (assignOp->DIV_EQUAL())
      assignExprNode->op = AssignExprNode::OP_DIV_EQUAL;
    else if (assignOp->REM_EQUAL())
      assignExprNode->op = AssignExprNode::OP_REM_EQUAL;
    else if (assignOp->SHL_EQUAL())
      assignExprNode->op = AssignExprNode::OP_SHL_EQUAL;
    else if (assignOp->SHR_EQUAL())
      assignExprNode->op = AssignExprNode::OP_SHR_EQUAL;
    else if (assignOp->AND_EQUAL())
      assignExprNode->op = AssignExprNode::OP_AND_EQUAL;
    else if (assignOp->OR_EQUAL())
      assignExprNode->op = AssignExprNode::OP_OR_EQUAL;
    else if (assignOp->XOR_EQUAL())
      assignExprNode->op = AssignExprNode::OP_XOR_EQUAL;
    else
      assert(false && "AssignExpr parsing fall-through");
  }

  helper.concludeNode($ctx, assignExprNode);
  $node = std::move(assignExprNode);
}: prefixUnaryExpr assignOp assignExpr | ternaryExpr;

ternaryExpr returns [Ptr<TernaryExprNode> node]
@init {
  auto ternaryExprNode = helper.createNode<TernaryExprNode>($ctx);
}
@after {
  ternaryExprNode->isShortened = ternaryExprNode->operands().size() == 2;

  helper.concludeNode($ctx, ternaryExprNode);
  $node = std::move(ternaryExprNode);
}: logicalOrExpr (QUESTION_MARK logicalOrExpr? COLON logicalOrExpr)?;

logicalOrExpr returns [Ptr<LogicalOrExprNode> node]
@init {
  auto logicalOrExprNode = helper.createNode<LogicalOrExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, logicalOrExprNode);
  $node = std::move(logicalOrExprNode);
}: logicalAndExpr (LOGICAL_OR logicalAndExpr)*;

logicalAndExpr returns [Ptr<LogicalAndExprNode> node]
@init {
  auto logicalAndExprNode = helper.createNode<LogicalAndExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, logicalAndExprNode);
  $node = std::move(logicalAndExprNode);
}: bitwiseOrExpr (LOGICAL_AND bitwiseOrExpr)*;

bitwiseOrExpr returns [Ptr<BitwiseOrExprNode> node]
@init {
 auto bitwiseOrExprNode = helper.createNode<BitwiseOrExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, bitwiseOrExprNode);
  $node = std::move(bitwiseOrExprNode);
}: bitwiseXorExpr (BITWISE_OR bitwiseXorExpr)*;

bitwiseXorExpr returns [Ptr<BitwiseXorExprNode> node]
@init {
  auto bitwiseXorExprNode = helper.createNode<BitwiseXorExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, bitwiseXorExprNode);
  $node = std::move(bitwiseXorExprNode);
}: bitwiseAndExpr (BITWISE_XOR bitwiseAndExpr)*;

bitwiseAndExpr returns [Ptr<BitwiseAndExprNode> node]
@init {
  auto bitwiseAndExprNode = helper.createNode<BitwiseAndExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, bitwiseAndExprNode);
  $node = std::move(bitwiseAndExprNode);
}: equalityExpr (BITWISE_AND equalityExpr)*;

equalityExpr returns [Ptr<EqualityExprNode> node]
@init {
  auto equalityExprNode = helper.createNode<EqualityExprNode>($ctx);
}
@after {
  // Extract operator
  if ($ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_EQUAL;
  else if ($ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_NOT_EQUAL;

  helper.concludeNode($ctx, equalityExprNode);
  $node = std::move(equalityExprNode);
}: relationalExpr ((EQUAL | NOT_EQUAL) relationalExpr)?;

relationalExpr returns [Ptr<RelationalExprNode> node]
@init {
  auto relationalExprNode = helper.createNode<RelationalExprNode>($ctx);
}
@after {
  // Extract operator
  if ($ctx->LESS())
    relationalExprNode->op = RelationalExprNode::OP_LESS;
  else if ($ctx->GREATER())
    relationalExprNode->op = RelationalExprNode::OP_GREATER;
  else if ($ctx->LESS_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_LESS_EQUAL;
  else if ($ctx->GREATER_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_GREATER_EQUAL;

  helper.concludeNode($ctx, relationalExprNode);
  $node = std::move(relationalExprNode);
}: shiftExpr ((LESS | GREATER | LESS_EQUAL | GREATER_EQUAL) shiftExpr)?;

shiftExpr returns [Ptr<ShiftExprNode> node]
@init {
  auto shiftExprNode = helper.createNode<ShiftExprNode>($ctx);
}
@after {
  // Extract operator
  if (!$ctx->LESS().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_LEFT;
  else if (!$ctx->GREATER().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_RIGHT;

  helper.concludeNode($ctx, shiftExprNode);
  $node = std::move(shiftExprNode);
}: additiveExpr ((LESS LESS | GREATER GREATER) additiveExpr)?;

additiveExpr returns [Ptr<AdditiveExprNode> node]
@init {
  auto additiveExprNode = helper.createNode<AdditiveExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, additiveExprNode);
  $node = std::move(additiveExprNode);
}: multiplicativeExpr ((PLUS | MINUS) multiplicativeExpr)*;

multiplicativeExpr returns [Ptr<MultiplicativeExprNode> node]
@init {
  auto multiplicativeExprNode = helper.createNode<MultiplicativeExprNode>($ctx);
}
@after {
  helper.concludeNode($ctx, multiplicativeExprNode);
  $node = std::move(multiplicativeExprNode);
}: castExpr ((MUL | DIV | REM) castExpr)*;

castExpr returns [Ptr<CastExprNode> node]
@init {
  auto castExprNode = helper.createNode<CastExprNode>($ctx);
}
@after {
  castExprNode->isCasted = castExprNode->dataType();

  helper.concludeNode($ctx, castExprNode);
  $node = std::move(castExprNode);
}: LPAREN dataType RPAREN prefixUnaryExpr | prefixUnaryExpr;

prefixUnaryExpr returns [Ptr<PrefixUnaryExprNode> node]
@init {
  auto prefixUnaryExprNode = helper.createNode<PrefixUnaryExprNode>($ctx);
}
@after {
  // Extract assign operator
  if (auto prefixUnaryOp = $ctx->prefixUnaryOp()) {
    if (prefixUnaryOp->MINUS()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_MINUS;
    } else if (prefixUnaryOp->PLUS_PLUS()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_PLUS_PLUS;
    } else if (prefixUnaryOp->MINUS_MINUS()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_MINUS_MINUS;
    } else if (prefixUnaryOp->NOT()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_NOT;
    } else if (prefixUnaryOp->BITWISE_NOT()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_BITWISE_NOT;
    } else if (prefixUnaryOp->MUL()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_INDIRECTION;
    } else if (prefixUnaryOp->BITWISE_AND()) {
      prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_ADDRESS_OF;
    } else {
      assert(false && "PrefixUnaryExpr parsing fall-through");
    }
  }

  helper.concludeNode($ctx, prefixUnaryExprNode);
  $node = std::move(prefixUnaryExprNode);
}: postfixUnaryExpr | prefixUnaryOp prefixUnaryExpr;

postfixUnaryExpr returns [Ptr<PostfixUnaryExprNode> node]
@init {
  auto postfixUnaryExprNode = helper.createNode<PostfixUnaryExprNode>($ctx);
}
@after {
  if ($ctx->postfixUnaryExpr()) {
    for (ParserRuleContext::ParseTree *subTree : $ctx->children) {
      ParserRuleContext *rule;
      if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::LBRACKET) {
        postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_SUBSCRIPT;
      } else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::DOT) {
        postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MEMBER_ACCESS;
      } else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
        postfixUnaryExprNode->identifier = ParserHelper::getIdentifier(t3);
      } else if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::PLUS_PLUS) {
        postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_PLUS_PLUS;
      } else if (auto t5 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t5 != nullptr && t5->getSymbol()->getType() == SpiceParser::MINUS_MINUS) {
        postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MINUS_MINUS;
      }
    }
  }

  helper.concludeNode($ctx, postfixUnaryExprNode);
  $node = std::move(postfixUnaryExprNode);
}: atomicExpr | postfixUnaryExpr LBRACKET assignExpr RBRACKET | postfixUnaryExpr DOT IDENTIFIER | postfixUnaryExpr PLUS_PLUS | postfixUnaryExpr MINUS_MINUS;

atomicExpr returns [Ptr<AtomicExprNode> node]
@init {
  auto atomicExprNode = helper.createNode<AtomicExprNode>($ctx);
}
@after {
  for (ParserRuleContext::ParseTree *subTree : $ctx->children) {
    ParserRuleContext *rule;
    if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = ParserHelper::getIdentifier(t1);
      atomicExprNode->identifierFragments.push_back(fragment);
      if (!atomicExprNode->fqIdentifier.empty())
        atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      atomicExprNode->fqIdentifier += fragment;
    } else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = ParserHelper::getIdentifier(t2);
      atomicExprNode->identifierFragments.push_back(fragment);
      if (!atomicExprNode->fqIdentifier.empty())
        atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      atomicExprNode->fqIdentifier += fragment;
    }
  }

  helper.concludeNode($ctx, atomicExprNode);
  $node = std::move(atomicExprNode);
}: (constant | value | ((IDENTIFIER | TYPE_IDENTIFIER) SCOPE_ACCESS)* (IDENTIFIER | TYPE_IDENTIFIER) | builtinCall | LPAREN assignExpr RPAREN);

// Values
value returns [Ptr<ValueNode> node]
@init {
  auto valueNode = helper.createNode<ValueNode>($ctx);
}
@after {
  if ($ctx->NIL())
    valueNode->isNil = true;

  helper.concludeNode($ctx, valueNode);
  $node = std::move(valueNode);
}: fctCall | arrayInitialization | structInstantiation | NIL LESS dataType GREATER;

constant returns [Ptr<ConstantNode> node]
@init {
  auto constantNode = helper.createNode<ConstantNode>($ctx);
}
@after {
  if ($ctx->DOUBLE_LIT()) {
    constantNode->type = ConstantNode::TYPE_DOUBLE;
    constantNode->compileTimeValue.doubleValue = std::stod($ctx->DOUBLE_LIT()->toString());
  } else if ($ctx->INT_LIT()) {
    constantNode->type = ConstantNode::TYPE_INT;
    constantNode->compileTimeValue.doubleValue = std::stod($ctx->INT_LIT()->toString());
  } else if ($ctx->SHORT_LIT()) {
    constantNode->type = ConstantNode::TYPE_SHORT;
    constantNode->compileTimeValue.doubleValue = std::stod($ctx->SHORT_LIT()->toString());
  } else if ($ctx->LONG_LIT()) {
    constantNode->type = ConstantNode::TYPE_LONG;
    constantNode->compileTimeValue.doubleValue = std::stod($ctx->LONG_LIT()->toString());
  } else if ($ctx->CHAR_LIT()) {
    constantNode->type = ConstantNode::TYPE_CHAR;
    constantNode->compileTimeValue.doubleValue = std::stod($ctx->CHAR_LIT()->toString());
  } else if ($ctx->STRING_LIT()) {
    // Transfer ownership to the AST node
    constantNode->compileTimeStringValue = helper.parseString($ctx->STRING_LIT()->toString());
    // Save a pointer to the string in the compile time value
    constantNode->type = ConstantNode::TYPE_STRING;
    constantNode->compileTimeValue.stringValue = constantNode->compileTimeStringValue.c_str();
  } else if ($ctx->TRUE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    constantNode->compileTimeValue.boolValue = true;
  } else if ($ctx->FALSE()) {
    constantNode->type = ConstantNode::TYPE_BOOL;
    constantNode->compileTimeValue.boolValue = false;
  }

  constantNode->hasDirectCompileTimeValue = true;

  helper.concludeNode($ctx, constantNode);
  $node = std::move(constantNode);
}: DOUBLE_LIT | INT_LIT | SHORT_LIT | LONG_LIT | CHAR_LIT | STRING_LIT | TRUE | FALSE;

fctCall returns [Ptr<FctCallNode> node]
@init {
  auto fctCallNode = helper.createNode<FctCallNode>($ctx);
}
@after {
  if (fctCallNode->argLst())
    fctCallNode->hasArgs = true;
  if (fctCallNode->templateTypeLst())
    fctCallNode->hasTemplateTypes = true;

  for (antlr4::ParserRuleContext::ParseTree *subTree : $ctx->children) {
    ParserRuleContext *rule;
    if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      fctCallNode->fqFunctionName += fragment;
    } else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::SCOPE_ACCESS) {
      fctCallNode->fqFunctionName += SCOPE_ACCESS_TOKEN;
    } else if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::DOT) {
      fctCallNode->fqFunctionName += MEMBER_ACCESS_TOKEN;
    }
  }

  helper.concludeNode($ctx, fctCallNode);
  $node = std::move(fctCallNode);
}: (IDENTIFIER SCOPE_ACCESS)* (IDENTIFIER DOT)* (IDENTIFIER | TYPE_IDENTIFIER) (LESS typeLst GREATER)? LPAREN argLst? RPAREN;

arrayInitialization returns [Ptr<ArrayInitializationNode> node]
@init {
  auto arrayInitializationNode = helper.createNode<ArrayInitializationNode>($ctx);
}
@after {
  helper.concludeNode($ctx, arrayInitializationNode);
  $node = std::move(arrayInitializationNode);
}: LBRACE argLst? RBRACE;

structInstantiation returns [Ptr<StructInstantiationNode> node]
@init {
  auto structInstantiationNode = helper.createNode<StructInstantiationNode>($ctx);
}
@after {
  for (antlr4::ParserRuleContext::ParseTree *subTree : $ctx->children) {
    ParserRuleContext *rule;
    if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      structInstantiationNode->fqStructName += fragment;
    }
  }

  helper.concludeNode($ctx, structInstantiationNode);
  $node = std::move(structInstantiationNode);
}: (IDENTIFIER SCOPE_ACCESS)* TYPE_IDENTIFIER (LESS typeLst GREATER)? LBRACE argLst? RBRACE;

// Types
dataType returns [Ptr<DataTypeNode> node]
@init {
  auto dataTypeNode = helper.createNode<DataTypeNode>($ctx);
}
@after {
  for (int i = 0; i < $ctx->children.size(); i++) {
    auto subTree = $ctx->children[i];
    ParserRuleContext *rule;
    if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::MUL)
      dataTypeNode->tmQueue.emplace(DataTypeNode::TYPE_PTR, false, 0);
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::BITWISE_AND)
      dataTypeNode->tmQueue.emplace(DataTypeNode::TYPE_REF, false, 0);
    else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t3 != nullptr && t3->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = $ctx->children[i];
      bool hasSize = false;
      unsigned int hardCodedSize = 0;
      std::string sizeVarName;
      if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t4 != nullptr && t4->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        hardCodedSize = std::stoi(t4->getSymbol()->getText());
        i++; // Consume INTEGER
      } else if (auto t5 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t5 != nullptr && t5->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
        hasSize = true;
        sizeVarName = ParserHelper::getIdentifier(t5);
        i++; // Consume TYPE_IDENTIFIER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_ARRAY, hasSize, hardCodedSize, sizeVarName});
    }
  }

  helper.concludeNode($ctx, dataTypeNode);
  $node = std::move(dataTypeNode);
}: specifierLst? baseDataType (MUL | BITWISE_AND | LBRACKET (INT_LIT | TYPE_IDENTIFIER)? RBRACKET)*;

baseDataType returns [Ptr<BaseDataTypeNode> node]
@init {
  auto baseDataTypeNode = helper.createNode<BaseDataTypeNode>($ctx);
}
@after {
  if ($ctx->customDataType())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CUSTOM;
  else if ($ctx->functionDataType())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_FUNCTION;
  else if ($ctx->TYPE_DOUBLE())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_DOUBLE;
  else if ($ctx->TYPE_INT())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_INT;
  else if ($ctx->TYPE_SHORT())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_SHORT;
  else if ($ctx->TYPE_LONG())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_LONG;
  else if ($ctx->TYPE_BYTE())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_BYTE;
  else if ($ctx->TYPE_CHAR())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_CHAR;
  else if ($ctx->TYPE_STRING())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_STRING;
  else if ($ctx->TYPE_BOOL())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_BOOL;
  else if ($ctx->TYPE_DYN())
    baseDataTypeNode->type = BaseDataTypeNode::TYPE_DYN;

  helper.concludeNode($ctx, baseDataTypeNode);
  $node = std::move(baseDataTypeNode);
}: TYPE_DOUBLE | TYPE_INT | TYPE_SHORT | TYPE_LONG | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | customDataType | functionDataType;

customDataType returns [Ptr<CustomDataTypeNode> node]
@init {
  auto customDataTypeNode = helper.createNode<CustomDataTypeNode>($ctx);
}
@after {
  for (antlr4::ParserRuleContext::ParseTree *subTree : $ctx->children) {
    ParserRuleContext *rule;
    if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1 != nullptr && t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      const std::string fragment = t1->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment + SCOPE_ACCESS_TOKEN;
    } else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2 != nullptr && t2->getSymbol()->getType() == SpiceParser::TYPE_IDENTIFIER) {
      const std::string fragment = t2->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      customDataTypeNode->fqTypeName += fragment;
    }
  }

  helper.concludeNode($ctx, customDataTypeNode);
  $node = std::move(customDataTypeNode);
}: (IDENTIFIER SCOPE_ACCESS)* TYPE_IDENTIFIER (LESS typeLst GREATER)?;

functionDataType returns [Ptr<FunctionDataTypeNode> node]
@init {
  auto functionDataTypeNode = helper.createNode<FunctionDataTypeNode>($ctx);
}
@after {
  if (functionDataTypeNode->returnType())
    functionDataTypeNode->isFunction = true;

  helper.concludeNode($ctx, functionDataTypeNode);
  $node = std::move(functionDataTypeNode);
}: (P | F LESS dataType GREATER) LPAREN typeLst? RPAREN;

// Shorthands
assignOp: ASSIGN | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | REM_EQUAL | SHL_EQUAL | SHR_EQUAL | AND_EQUAL | OR_EQUAL | XOR_EQUAL;
prefixUnaryOp: MINUS | PLUS_PLUS | MINUS_MINUS | NOT | BITWISE_NOT | MUL | BITWISE_AND;
overloadableOp: PLUS | MINUS | MUL | DIV | EQUAL | NOT_EQUAL | LESS LESS | GREATER GREATER | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | PLUS_PLUS | MINUS_MINUS;

// Keyword tokens
TYPE_DOUBLE: 'double';
TYPE_INT: 'int';
TYPE_SHORT: 'short';
TYPE_LONG: 'long';
TYPE_BYTE: 'byte';
TYPE_CHAR: 'char';
TYPE_STRING: 'string';
TYPE_BOOL: 'bool';
TYPE_DYN: 'dyn';
CONST: 'const';
SIGNED: 'signed';
UNSIGNED: 'unsigned';
INLINE: 'inline';
PUBLIC: 'public';
HEAP: 'heap';
F: 'f';
P: 'p';
IF: 'if';
ELSE: 'else';
ASSERT: 'assert';
FOR: 'for';
FOREACH: 'foreach';
DO: 'do';
WHILE: 'while';
IMPORT: 'import';
BREAK: 'break';
CONTINUE: 'continue';
RETURN: 'return';
AS: 'as';
STRUCT: 'struct';
INTERFACE: 'interface';
TYPE: 'type';
ENUM: 'enum';
OPERATOR: 'operator';
ALIAS: 'alias';
UNSAFE: 'unsafe';
NIL: 'nil';
MAIN: 'main';
PRINTF: 'printf';
SIZEOF: 'sizeof';
ALIGNOF: 'alignof';
LEN: 'len';
EXT: 'ext';
TRUE: 'true';
FALSE: 'false';

// Operator tokens
LBRACE: '{';
RBRACE: '}';
LPAREN: '(';
RPAREN: ')';
LBRACKET: '[';
RBRACKET: ']';
LOGICAL_OR: '||';
LOGICAL_AND: '&&';
BITWISE_OR: '|';
BITWISE_XOR: '^';
BITWISE_AND: '&';
PLUS_PLUS: '++';
MINUS_MINUS: '--';
PLUS_EQUAL: '+=';
MINUS_EQUAL: '-=';
MUL_EQUAL: '*=';
DIV_EQUAL: '/=';
REM_EQUAL: '%=';
SHL_EQUAL: '<<=';
SHR_EQUAL: '>>=';
AND_EQUAL: '&=';
OR_EQUAL: '|=';
XOR_EQUAL: '^=';
PLUS: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
REM: '%';
NOT: '!';
BITWISE_NOT: '~';
GREATER: '>';
LESS: '<';
GREATER_EQUAL: '>=';
LESS_EQUAL: '<=';
EQUAL: '==';
NOT_EQUAL: '!=';
ASSIGN: '=';
QUESTION_MARK: '?';
SEMICOLON: ';';
COLON: ':';
COMMA: ',';
DOT: '.';
SCOPE_ACCESS: '::';
ELLIPSIS: '...';
FCT_ATTR_PREAMBLE: '#';
MOD_ATTR_PREAMBLE: '#!';

// Regex tokens
DOUBLE_LIT: [-]?[0-9]*[.][0-9]+([eE][+-]?[0-9]+)?;
INT_LIT: NUM_LIT;
SHORT_LIT: NUM_LIT 's';
LONG_LIT: NUM_LIT 'l';
CHAR_LIT: '\'' (~['\\\r\n] | '\\' (. | EOF)) '\'';
STRING_LIT: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
IDENTIFIER: [a-z_][a-zA-Z0-9_]*;
TYPE_IDENTIFIER: [A-Z][a-zA-Z0-9_]*;

fragment NUM_LIT: NUM_LIT_S | NUM_LIT_U;
fragment NUM_LIT_S: [-](DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT);
fragment NUM_LIT_U: (DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT)[u]?;
fragment DEC_LIT: ([0][dD])?[0-9]+;
fragment BIN_LIT: [0][bB][01]+;
fragment HEX_LIT: [0][xXhH][0-9a-fA-F]+;
fragment OCT_LIT: [0][oO][0-7]+;

// Skipped tokens
DOC_COMMENT: '/**' .*? '*/' -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS: [ \t\r\n]+ -> skip;
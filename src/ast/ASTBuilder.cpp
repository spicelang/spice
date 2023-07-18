// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/ParserError.h>
#include <typechecker/OpRuleManager.h>
#include <util/CommonUtil.h>

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
  fctDefNode->fctName = fctDefNode->getChild<FctNameNode>();
  fctDefNode->isMethod = fctDefNode->fctName->nameFragments.size() > 1;

  // Tell the return type that it is one
  fctDefNode->returnType()->isReturnType = true;

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
  procDefNode->procName = procDefNode->getChild<FctNameNode>();
  procDefNode->isMethod = procDefNode->procName->nameFragments.size() > 1;

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

  // Extract overloaded operator
  if (ctx->OPERATOR()) {
    SpiceParser::OverloadableOpContext *op = ctx->overloadableOp();
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
  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER());
  extDeclNode->hasArgs = ctx->typeLst();

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, extDeclNode);
}

std::any ASTBuilder::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {
  auto unsafeBlockDefNode = createNode<UnsafeBlockDefNode>(ctx);

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, unsafeBlockDefNode);
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {}

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

std::any ASTBuilder::visitFieldLst(SpiceParser::FieldLstContext *ctx) {
  auto fieldLstNode = createNode<FieldLstNode>(ctx);

  // Visit children
  visitChildren(ctx);

  // Tell all field types that they are such
  for (FieldNode *field : fieldLstNode->fields())
    field->dataType()->isFieldType = true;

  return concludeNode(ctx, fieldLstNode);
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = createNode<FieldNode>(ctx);

  // Enrich
  fieldNode->fieldName = getIdentifier(ctx->IDENTIFIER());

  // Visit children
  visitChildren(ctx);

  return concludeNode(ctx, fieldNode);
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) {}

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {}

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {}

std::any ASTBuilder::visitSpecifier(SpiceParser::SpecifierContext *ctx) {}

std::any ASTBuilder::visitModAttr(SpiceParser::ModAttrContext *ctx) {}

std::any ASTBuilder::visitFctAttr(SpiceParser::FctAttrContext *ctx) {}

std::any ASTBuilder::visitAttrLst(SpiceParser::AttrLstContext *ctx) {}

std::any ASTBuilder::visitAttr(SpiceParser::AttrContext *ctx) {}

std::any ASTBuilder::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {}

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {}

std::any ASTBuilder::visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) {}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {}

std::any ASTBuilder::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {}

std::any ASTBuilder::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {}

std::any ASTBuilder::visitValue(SpiceParser::ValueContext *ctx) {}

std::any ASTBuilder::visitConstant(SpiceParser::ConstantContext *ctx) {}

std::any ASTBuilder::visitFctCall(SpiceParser::FctCallContext *ctx) {}

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {}

std::any ASTBuilder::visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) {}

std::any ASTBuilder::visitAssignOp(SpiceParser::AssignOpContext *ctx) {}

std::any ASTBuilder::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) {}

std::any ASTBuilder::visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) {}

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
  // For all nodes, except the entry node, create and save the error message
  if constexpr (!std::is_same_v<T, EntryNode>)
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
  antlr4::misc::Interval extendedSourceInterval(sourceInterval);

  if (inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
    extendedSourceInterval.b = extendedSourceInterval.a;
    while (inputStream->getText(extendedSourceInterval).find('\n') == std::string::npos)
      extendedSourceInterval.b++;
  }

  size_t indentation = 0;
  for (; indentation < ERROR_MESSAGE_CONTEXT; indentation++) {
    extendedSourceInterval.a--;
    if (extendedSourceInterval.a < 0 || inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
      extendedSourceInterval.a++;
      break;
    }
  }
  for (size_t suffixContext = 0; suffixContext < ERROR_MESSAGE_CONTEXT; suffixContext++) {
    extendedSourceInterval.b++;
    if (extendedSourceInterval.b > inputStream->size() ||
        inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
      extendedSourceInterval.b--;
      break;
    }
  }

  // Trim start
  while (inputStream->getText(extendedSourceInterval)[0] == ' ') {
    extendedSourceInterval.a++;
    indentation--;
  }

  // Trim end
  if (inputStream->getText(extendedSourceInterval)[extendedSourceInterval.length() - 1] == '\n')
    extendedSourceInterval.b--;

  const std::string lineNumberStr = std::to_string(ctx->start->getLine());
  indentation += lineNumberStr.length() + 2;

  std::stringstream ss;
  ss << lineNumberStr << "  " << inputStream->getText(extendedSourceInterval) << "\n";
  for (size_t i = 0; i < indentation; i++)
    ss << " ";
  for (size_t i = 0; i < std::min(sourceInterval.length(), extendedSourceInterval.length()); i++)
    ss << "^";
  node->errorMessage = ss.str();
}

} // namespace spice::compiler
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <SpiceVisitor.h>

#include <functional>
#include <utility>

namespace spice::compiler {

// Forward declarations
class ASTNode;
class ConstantNode;

#define ERROR_MESSAGE_CONTEXT 20
const char *const RESERVED_KEYWORDS[8] = {"new", "switch", "case", "yield", "stash", "pick", "sync", "class"};
const char *const MEMBER_ACCESS_TOKEN = ".";
const char *const SCOPE_ACCESS_TOKEN = "::";

class ASTBuilder : private CompilerPass, public SpiceVisitor {
private:
  using TerminalNode = antlr4::tree::TerminalNode;
  using ParserRuleContext = antlr4::ParserRuleContext;

public:
  // Constructors
  ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, ASTNode *rootNode,
             antlr4::ANTLRInputStream *inputStream);

  // Public methods
  std::any visitEntry(SpiceParser::EntryContext *ctx) override;
  std::any visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) override;
  std::any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override;
  std::any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override;
  std::any visitFctName(SpiceParser::FctNameContext *ctx) override;
  std::any visitExtDecl(SpiceParser::ExtDeclContext *ctx) override;
  std::any visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) override;
  std::any visitAliasDef(SpiceParser::AliasDefContext *ctx) override;
  std::any visitStructDef(SpiceParser::StructDefContext *ctx) override;
  std::any visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) override;
  std::any visitEnumDef(SpiceParser::EnumDefContext *ctx) override;
  std::any visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) override;
  std::any visitThreadDef(SpiceParser::ThreadDefContext *ctx) override;
  std::any visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) override;
  std::any visitForLoop(SpiceParser::ForLoopContext *ctx) override;
  std::any visitForHead(SpiceParser::ForHeadContext *ctx) override;
  std::any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override;
  std::any visitForeachHead(SpiceParser::ForeachHeadContext *ctx) override;
  std::any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override;
  std::any visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) override;
  std::any visitIfStmt(SpiceParser::IfStmtContext *ctx) override;
  std::any visitElseStmt(SpiceParser::ElseStmtContext *ctx) override;
  std::any visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) override;
  std::any visitStmtLst(SpiceParser::StmtLstContext *ctx) override;
  std::any visitTypeLst(SpiceParser::TypeLstContext *ctx) override;
  std::any visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) override;
  std::any visitParamLst(SpiceParser::ParamLstContext *ctx) override;
  std::any visitArgLst(SpiceParser::ArgLstContext *ctx) override;
  std::any visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) override;
  std::any visitEnumItem(SpiceParser::EnumItemContext *ctx) override;
  std::any visitField(SpiceParser::FieldContext *ctx) override;
  std::any visitSignature(SpiceParser::SignatureContext *ctx) override;
  std::any visitStmt(SpiceParser::StmtContext *ctx) override;
  std::any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override;
  std::any visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) override;
  std::any visitSpecifier(SpiceParser::SpecifierContext *ctx) override;
  std::any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;
  std::any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override;
  std::any visitBreakStmt(SpiceParser::BreakStmtContext *ctx) override;
  std::any visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) override;
  std::any visitAssertStmt(SpiceParser::AssertStmtContext *ctx) override;
  std::any visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) override;
  std::any visitPrintfCall(SpiceParser::PrintfCallContext *ctx) override;
  std::any visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) override;
  std::any visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) override;
  std::any visitLenCall(SpiceParser::LenCallContext *ctx) override;
  std::any visitTidCall(SpiceParser::TidCallContext *ctx) override;
  std::any visitJoinCall(SpiceParser::JoinCallContext *ctx) override;
  std::any visitAssignExpr(SpiceParser::AssignExprContext *ctx) override;
  std::any visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) override;
  std::any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override;
  std::any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override;
  std::any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override;
  std::any visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) override;
  std::any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override;
  std::any visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) override;
  std::any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override;
  std::any visitShiftExpr(SpiceParser::ShiftExprContext *ctx) override;
  std::any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override;
  std::any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override;
  std::any visitCastExpr(SpiceParser::CastExprContext *ctx) override;
  std::any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) override;
  std::any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) override;
  std::any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override;
  std::any visitValue(SpiceParser::ValueContext *ctx) override;
  std::any visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) override;
  std::any visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) override;
  std::any visitConstant(SpiceParser::ConstantContext *ctx) override;
  std::any visitDataType(SpiceParser::DataTypeContext *ctx) override;
  std::any visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) override;
  std::any visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) override;
  std::any visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) override;
  std::any visitAssignOp(SpiceParser::AssignOpContext *ctx) override;
  std::any visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) override;
  std::any visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) override;

private:
  // Members
  ASTNode *currentNode;
  const std::string &filePath;
  antlr4::ANTLRInputStream *inputStream;
  bool isParam = false;
  bool isGlobal = false;
  bool isField = false;
  bool isReturnType = false;

  // Private methods
  int32_t parseInt(ConstantNode *constantNode, TerminalNode *terminal);
  int16_t parseShort(ConstantNode *constantNode, TerminalNode *terminal);
  int64_t parseLong(ConstantNode *constantNode, TerminalNode *terminal);
  int8_t parseChar(TerminalNode *terminal);
  static std::string parseString(std::string input);
  template <typename T>
  T parseNumeric(ConstantNode *constantNode, TerminalNode *terminal, std::function<T(const std::string &, int)> cb);
  static void replaceEscapeChars(std::string &string);
  std::string getIdentifier(TerminalNode *terminal);
  void saveErrorMessage(ASTNode *node, const ParserRuleContext *ctx);
};

} // namespace spice::compiler
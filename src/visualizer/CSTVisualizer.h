// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <SpiceLexer.h>
#include <SpiceVisitor.h>

#include <CompilerPass.h>

namespace spice::compiler {

/**
 * Visitor for debug purposes (is only executed in the compiler debug mode and when explicitly enabling it via cli flag)
 *
 * Jobs:
 * - Visualize CST
 */
class CSTVisualizer : private CompilerPass, public SpiceVisitor {
public:
  // Constructors
  CSTVisualizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile, const SpiceLexer *lexer,
                const SpiceParser *parser)
      : CompilerPass(resourceManager, sourceFile), vocabulary(lexer->getVocabulary()), ruleNames(parser->getRuleNames()){};

  // Visitor methods
  std::any visitEntry(SpiceParser::EntryContext *ctx) override { return buildRule(ctx); }
  std::any visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) override { return buildRule(ctx); }
  std::any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override { return buildRule(ctx); }
  std::any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override { return buildRule(ctx); }
  std::any visitFctName(SpiceParser::FctNameContext *ctx) override { return buildRule(ctx); }
  std::any visitExtDecl(SpiceParser::ExtDeclContext *ctx) override { return buildRule(ctx); }
  std::any visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) override { return buildRule(ctx); }
  std::any visitAliasDef(SpiceParser::AliasDefContext *ctx) override { return buildRule(ctx); }
  std::any visitStructDef(SpiceParser::StructDefContext *ctx) override { return buildRule(ctx); }
  std::any visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) override { return buildRule(ctx); }
  std::any visitEnumDef(SpiceParser::EnumDefContext *ctx) override { return buildRule(ctx); }
  std::any visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) override { return buildRule(ctx); }
  std::any visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) override { return buildRule(ctx); }
  std::any visitForLoop(SpiceParser::ForLoopContext *ctx) override { return buildRule(ctx); }
  std::any visitForHead(SpiceParser::ForHeadContext *ctx) override { return buildRule(ctx); }
  std::any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override { return buildRule(ctx); }
  std::any visitForeachHead(SpiceParser::ForeachHeadContext *ctx) override { return buildRule(ctx); }
  std::any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override { return buildRule(ctx); }
  std::any visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) override { return buildRule(ctx); }
  std::any visitIfStmt(SpiceParser::IfStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitElseStmt(SpiceParser::ElseStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitAssertStmt(SpiceParser::AssertStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitStmtLst(SpiceParser::StmtLstContext *ctx) override { return buildRule(ctx); }
  std::any visitFieldLst(SpiceParser::FieldLstContext *ctx) override { return buildRule(ctx); }
  std::any visitField(SpiceParser::FieldContext *ctx) override { return buildRule(ctx); }
  std::any visitSignature(SpiceParser::SignatureContext *ctx) override { return buildRule(ctx); }
  std::any visitTypeLst(SpiceParser::TypeLstContext *ctx) override { return buildRule(ctx); }
  std::any visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) override { return buildRule(ctx); }
  std::any visitParamLst(SpiceParser::ParamLstContext *ctx) override { return buildRule(ctx); }
  std::any visitArgLst(SpiceParser::ArgLstContext *ctx) override { return buildRule(ctx); }
  std::any visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) override { return buildRule(ctx); }
  std::any visitEnumItem(SpiceParser::EnumItemContext *ctx) override { return buildRule(ctx); }
  std::any visitStmt(SpiceParser::StmtContext *ctx) override { return buildRule(ctx); }
  std::any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) override { return buildRule(ctx); }
  std::any visitSpecifier(SpiceParser::SpecifierContext *ctx) override { return buildRule(ctx); }
  std::any visitModAttr(SpiceParser::ModAttrContext *ctx) override { return buildRule(ctx); }
  std::any visitFctAttr(SpiceParser::FctAttrContext *ctx) override { return buildRule(ctx); }
  std::any visitAttrLst(SpiceParser::AttrLstContext *ctx) override { return buildRule(ctx); }
  std::any visitAttr(SpiceParser::AttrContext *ctx) override { return buildRule(ctx); }
  std::any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitBreakStmt(SpiceParser::BreakStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) override { return buildRule(ctx); }
  std::any visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) override { return buildRule(ctx); }
  std::any visitPrintfCall(SpiceParser::PrintfCallContext *ctx) override { return buildRule(ctx); }
  std::any visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) override { return buildRule(ctx); }
  std::any visitAlignOfCall(SpiceParser::AlignOfCallContext *ctx) override { return buildRule(ctx); }
  std::any visitLenCall(SpiceParser::LenCallContext *ctx) override { return buildRule(ctx); }
  std::any visitAssignExpr(SpiceParser::AssignExprContext *ctx) override { return buildRule(ctx); }
  std::any visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) override { return buildRule(ctx); }
  std::any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override { return buildRule(ctx); }
  std::any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override { return buildRule(ctx); }
  std::any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override { return buildRule(ctx); }
  std::any visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) override { return buildRule(ctx); }
  std::any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override { return buildRule(ctx); }
  std::any visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) override { return buildRule(ctx); }
  std::any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override { return buildRule(ctx); }
  std::any visitShiftExpr(SpiceParser::ShiftExprContext *ctx) override { return buildRule(ctx); }
  std::any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override { return buildRule(ctx); }
  std::any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override { return buildRule(ctx); }
  std::any visitCastExpr(SpiceParser::CastExprContext *ctx) override { return buildRule(ctx); }
  std::any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) override { return buildRule(ctx); }
  std::any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) override { return buildRule(ctx); }
  std::any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override { return buildRule(ctx); }
  std::any visitValue(SpiceParser::ValueContext *ctx) override { return buildRule(ctx); }
  std::any visitFctCall(SpiceParser::FctCallContext *ctx) override { return buildRule(ctx); }
  std::any visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) override { return buildRule(ctx); }
  std::any visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) override { return buildRule(ctx); }
  std::any visitConstant(SpiceParser::ConstantContext *ctx) override { return buildRule(ctx); }
  std::any visitDataType(SpiceParser::DataTypeContext *ctx) override { return buildRule(ctx); }
  std::any visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) override { return buildRule(ctx); }
  std::any visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) override { return buildRule(ctx); }
  std::any visitFunctionDataType(SpiceParser::FunctionDataTypeContext *ctx) override { return buildRule(ctx); }
  std::any visitAssignOp(SpiceParser::AssignOpContext *ctx) override { return buildRule(ctx); }
  std::any visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) override { return buildRule(ctx); }

private:
  // Members
  const antlr4::dfa::Vocabulary &vocabulary;
  const std::vector<std::string> ruleNames;
  int currentTabs = 1;
  std::string parentNodeId;

  // Private methods
  std::string buildRule(antlr4::ParserRuleContext *ctx);
  [[nodiscard]] std::string getSpaces() const;
  [[nodiscard]] static std::string tokenToCodeLoc(const antlr4::Token &token);
};

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ScopeIdUtil.h"
#include "analyzer/AnalyzerVisitor.h"

/**
 * Get scope id for the MainFunctionDef AST node
 *
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::MainFunctionDefContext *) { return "main()"; }

/**
 * Get scope id for the FunctionDef AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::FunctionDefContext *ctx) {
  auto symbol = ctx->F()->getSymbol();
  std::string functionName = ctx->IDENTIFIER()[0]->toString();
  if (ctx->IDENTIFIER().size() > 1)
    functionName += "." + ctx->IDENTIFIER()[1]->toString();
  return "f:" + functionName + ":" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the ProcedureDef AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::ProcedureDefContext *ctx) {
  auto symbol = ctx->P()->getSymbol();
  std::string procedureName = ctx->IDENTIFIER()[0]->toString();
  if (ctx->IDENTIFIER().size() > 1)
    procedureName += "." + ctx->IDENTIFIER()[1]->toString();
  return "p:" + procedureName + ":" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the StructDef AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::StructDefContext *ctx) {
  return STRUCT_SCOPE_PREFIX + ctx->IDENTIFIER()->toString();
}

/**
 * Get scope id for the ThreadDef AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::ThreadDefContext *ctx) {
  auto symbol = ctx->THREAD()->getSymbol();
  return "thread:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the UnsafeBlockDefContext AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::UnsafeBlockDefContext *ctx) {
  auto symbol = ctx->UNSAFE()->getSymbol();
  return "unsafe:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the ForLoop AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::ForLoopContext *ctx) {
  auto symbol = ctx->FOR()->getSymbol();
  return "for:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the ForeachLoop AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::ForeachLoopContext *ctx) {
  auto symbol = ctx->FOREACH()->getSymbol();
  return "foreach:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the WhileLoop AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::WhileLoopContext *ctx) {
  auto symbol = ctx->WHILE()->getSymbol();
  return "while:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the IfStmt AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::IfStmtContext *ctx) {
  auto symbol = ctx->IF()->getSymbol();
  return "if:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}

/**
 * Get scope id for the ElseStmt AST node
 *
 * @param ctx Parser context
 * @return Scope id
 */
std::string ScopeIdUtil::getScopeId(SpiceParser::ElseStmtContext *ctx) {
  auto symbol = ctx->ELSE()->getSymbol();
  return "else:" + std::to_string(symbol->getLine()) + ":" + std::to_string(symbol->getCharPositionInLine());
}
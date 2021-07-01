
// Generated from D:/Dokumente/JustForFun GitHub Clones/spice/src\Spice.g4 by ANTLR 4.9.1


#include "SpiceVisitor.h"

#include "SpiceParser.h"


using namespace antlrcpp;
using namespace antlr4;

SpiceParser::SpiceParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

SpiceParser::~SpiceParser() {
  delete _interpreter;
}

std::string SpiceParser::getGrammarFileName() const {
  return "Spice.g4";
}

const std::vector<std::string>& SpiceParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& SpiceParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- EntryContext ------------------------------------------------------------------

SpiceParser::EntryContext::EntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::StmtContext *> SpiceParser::EntryContext::stmt() {
  return getRuleContexts<SpiceParser::StmtContext>();
}

SpiceParser::StmtContext* SpiceParser::EntryContext::stmt(size_t i) {
  return getRuleContext<SpiceParser::StmtContext>(i);
}

std::vector<SpiceParser::FunctionDefContext *> SpiceParser::EntryContext::functionDef() {
  return getRuleContexts<SpiceParser::FunctionDefContext>();
}

SpiceParser::FunctionDefContext* SpiceParser::EntryContext::functionDef(size_t i) {
  return getRuleContext<SpiceParser::FunctionDefContext>(i);
}

std::vector<SpiceParser::ProcedureDefContext *> SpiceParser::EntryContext::procedureDef() {
  return getRuleContexts<SpiceParser::ProcedureDefContext>();
}

SpiceParser::ProcedureDefContext* SpiceParser::EntryContext::procedureDef(size_t i) {
  return getRuleContext<SpiceParser::ProcedureDefContext>(i);
}


size_t SpiceParser::EntryContext::getRuleIndex() const {
  return SpiceParser::RuleEntry;
}


antlrcpp::Any SpiceParser::EntryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitEntry(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::EntryContext* SpiceParser::entry() {
  EntryContext *_localctx = _tracker.createInstance<EntryContext>(_ctx, getState());
  enterRule(_localctx, 0, SpiceParser::RuleEntry);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(61);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::DATA_TYPE)
      | (1ULL << SpiceParser::F)
      | (1ULL << SpiceParser::P)
      | (1ULL << SpiceParser::CONST)
      | (1ULL << SpiceParser::IMPORT)
      | (1ULL << SpiceParser::RETURN)
      | (1ULL << SpiceParser::IDENTIFIER)
      | (1ULL << SpiceParser::INTEGER)
      | (1ULL << SpiceParser::DOUBLE)
      | (1ULL << SpiceParser::STRING)
      | (1ULL << SpiceParser::BOOL)
      | (1ULL << SpiceParser::LPAREN)
      | (1ULL << SpiceParser::NOT)
      | (1ULL << SpiceParser::PLUS_PLUS)
      | (1ULL << SpiceParser::MINUS_MINUS))) != 0)) {
      setState(59);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SpiceParser::DATA_TYPE:
        case SpiceParser::CONST:
        case SpiceParser::IMPORT:
        case SpiceParser::RETURN:
        case SpiceParser::IDENTIFIER:
        case SpiceParser::INTEGER:
        case SpiceParser::DOUBLE:
        case SpiceParser::STRING:
        case SpiceParser::BOOL:
        case SpiceParser::LPAREN:
        case SpiceParser::NOT:
        case SpiceParser::PLUS_PLUS:
        case SpiceParser::MINUS_MINUS: {
          setState(56);
          stmt();
          break;
        }

        case SpiceParser::F: {
          setState(57);
          functionDef();
          break;
        }

        case SpiceParser::P: {
          setState(58);
          procedureDef();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(63);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDefContext ------------------------------------------------------------------

SpiceParser::FunctionDefContext::FunctionDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::FunctionDefContext::F() {
  return getToken(SpiceParser::F, 0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::LESS() {
  return getToken(SpiceParser::LESS, 0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::DATA_TYPE() {
  return getToken(SpiceParser::DATA_TYPE, 0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::GREATER() {
  return getToken(SpiceParser::GREATER, 0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::ParamLstContext* SpiceParser::FunctionDefContext::paramLst() {
  return getRuleContext<SpiceParser::ParamLstContext>(0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::StmtLstContext* SpiceParser::FunctionDefContext::stmtLst() {
  return getRuleContext<SpiceParser::StmtLstContext>(0);
}

tree::TerminalNode* SpiceParser::FunctionDefContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::FunctionDefContext::getRuleIndex() const {
  return SpiceParser::RuleFunctionDef;
}


antlrcpp::Any SpiceParser::FunctionDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitFunctionDef(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::FunctionDefContext* SpiceParser::functionDef() {
  FunctionDefContext *_localctx = _tracker.createInstance<FunctionDefContext>(_ctx, getState());
  enterRule(_localctx, 2, SpiceParser::RuleFunctionDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    match(SpiceParser::F);
    setState(65);
    match(SpiceParser::LESS);
    setState(66);
    match(SpiceParser::DATA_TYPE);
    setState(67);
    match(SpiceParser::GREATER);
    setState(68);
    match(SpiceParser::IDENTIFIER);
    setState(69);
    match(SpiceParser::LPAREN);
    setState(70);
    paramLst();
    setState(71);
    match(SpiceParser::RPAREN);
    setState(72);
    match(SpiceParser::LBRACE);
    setState(73);
    stmtLst();
    setState(74);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProcedureDefContext ------------------------------------------------------------------

SpiceParser::ProcedureDefContext::ProcedureDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::ProcedureDefContext::P() {
  return getToken(SpiceParser::P, 0);
}

tree::TerminalNode* SpiceParser::ProcedureDefContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::ProcedureDefContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::ParamLstContext* SpiceParser::ProcedureDefContext::paramLst() {
  return getRuleContext<SpiceParser::ParamLstContext>(0);
}

tree::TerminalNode* SpiceParser::ProcedureDefContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}

tree::TerminalNode* SpiceParser::ProcedureDefContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::StmtLstContext* SpiceParser::ProcedureDefContext::stmtLst() {
  return getRuleContext<SpiceParser::StmtLstContext>(0);
}

tree::TerminalNode* SpiceParser::ProcedureDefContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::ProcedureDefContext::getRuleIndex() const {
  return SpiceParser::RuleProcedureDef;
}


antlrcpp::Any SpiceParser::ProcedureDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitProcedureDef(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::ProcedureDefContext* SpiceParser::procedureDef() {
  ProcedureDefContext *_localctx = _tracker.createInstance<ProcedureDefContext>(_ctx, getState());
  enterRule(_localctx, 4, SpiceParser::RuleProcedureDef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    match(SpiceParser::P);
    setState(77);
    match(SpiceParser::IDENTIFIER);
    setState(78);
    match(SpiceParser::LPAREN);
    setState(79);
    paramLst();
    setState(80);
    match(SpiceParser::RPAREN);
    setState(81);
    match(SpiceParser::LBRACE);
    setState(82);
    stmtLst();
    setState(83);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForLoopContext ------------------------------------------------------------------

SpiceParser::ForLoopContext::ForLoopContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::ForLoopContext::FOR() {
  return getToken(SpiceParser::FOR, 0);
}

SpiceParser::AssignmentContext* SpiceParser::ForLoopContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}

std::vector<tree::TerminalNode *> SpiceParser::ForLoopContext::SEMICOLON() {
  return getTokens(SpiceParser::SEMICOLON);
}

tree::TerminalNode* SpiceParser::ForLoopContext::SEMICOLON(size_t i) {
  return getToken(SpiceParser::SEMICOLON, i);
}

std::vector<SpiceParser::TopLvlExprContext *> SpiceParser::ForLoopContext::topLvlExpr() {
  return getRuleContexts<SpiceParser::TopLvlExprContext>();
}

SpiceParser::TopLvlExprContext* SpiceParser::ForLoopContext::topLvlExpr(size_t i) {
  return getRuleContext<SpiceParser::TopLvlExprContext>(i);
}

tree::TerminalNode* SpiceParser::ForLoopContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::StmtLstContext* SpiceParser::ForLoopContext::stmtLst() {
  return getRuleContext<SpiceParser::StmtLstContext>(0);
}

tree::TerminalNode* SpiceParser::ForLoopContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::ForLoopContext::getRuleIndex() const {
  return SpiceParser::RuleForLoop;
}


antlrcpp::Any SpiceParser::ForLoopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitForLoop(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::ForLoopContext* SpiceParser::forLoop() {
  ForLoopContext *_localctx = _tracker.createInstance<ForLoopContext>(_ctx, getState());
  enterRule(_localctx, 6, SpiceParser::RuleForLoop);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(85);
    match(SpiceParser::FOR);
    setState(86);
    assignment();
    setState(87);
    match(SpiceParser::SEMICOLON);
    setState(88);
    topLvlExpr();
    setState(89);
    match(SpiceParser::SEMICOLON);
    setState(90);
    topLvlExpr();
    setState(91);
    match(SpiceParser::LBRACE);
    setState(92);
    stmtLst();
    setState(93);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileLoopContext ------------------------------------------------------------------

SpiceParser::WhileLoopContext::WhileLoopContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::WhileLoopContext::WHILE() {
  return getToken(SpiceParser::WHILE, 0);
}

SpiceParser::TopLvlExprContext* SpiceParser::WhileLoopContext::topLvlExpr() {
  return getRuleContext<SpiceParser::TopLvlExprContext>(0);
}

tree::TerminalNode* SpiceParser::WhileLoopContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::StmtLstContext* SpiceParser::WhileLoopContext::stmtLst() {
  return getRuleContext<SpiceParser::StmtLstContext>(0);
}

tree::TerminalNode* SpiceParser::WhileLoopContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::WhileLoopContext::getRuleIndex() const {
  return SpiceParser::RuleWhileLoop;
}


antlrcpp::Any SpiceParser::WhileLoopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitWhileLoop(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::WhileLoopContext* SpiceParser::whileLoop() {
  WhileLoopContext *_localctx = _tracker.createInstance<WhileLoopContext>(_ctx, getState());
  enterRule(_localctx, 8, SpiceParser::RuleWhileLoop);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(95);
    match(SpiceParser::WHILE);
    setState(96);
    topLvlExpr();
    setState(97);
    match(SpiceParser::LBRACE);
    setState(98);
    stmtLst();
    setState(99);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStmtContext ------------------------------------------------------------------

SpiceParser::IfStmtContext::IfStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::IfStmtContext::IF() {
  return getToken(SpiceParser::IF, 0);
}

SpiceParser::TopLvlExprContext* SpiceParser::IfStmtContext::topLvlExpr() {
  return getRuleContext<SpiceParser::TopLvlExprContext>(0);
}

tree::TerminalNode* SpiceParser::IfStmtContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::StmtLstContext* SpiceParser::IfStmtContext::stmtLst() {
  return getRuleContext<SpiceParser::StmtLstContext>(0);
}

tree::TerminalNode* SpiceParser::IfStmtContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::IfStmtContext::getRuleIndex() const {
  return SpiceParser::RuleIfStmt;
}


antlrcpp::Any SpiceParser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitIfStmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::IfStmtContext* SpiceParser::ifStmt() {
  IfStmtContext *_localctx = _tracker.createInstance<IfStmtContext>(_ctx, getState());
  enterRule(_localctx, 10, SpiceParser::RuleIfStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(101);
    match(SpiceParser::IF);
    setState(102);
    topLvlExpr();
    setState(103);
    match(SpiceParser::LBRACE);
    setState(104);
    stmtLst();
    setState(105);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtLstContext ------------------------------------------------------------------

SpiceParser::StmtLstContext::StmtLstContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::StmtContext *> SpiceParser::StmtLstContext::stmt() {
  return getRuleContexts<SpiceParser::StmtContext>();
}

SpiceParser::StmtContext* SpiceParser::StmtLstContext::stmt(size_t i) {
  return getRuleContext<SpiceParser::StmtContext>(i);
}

std::vector<SpiceParser::ForLoopContext *> SpiceParser::StmtLstContext::forLoop() {
  return getRuleContexts<SpiceParser::ForLoopContext>();
}

SpiceParser::ForLoopContext* SpiceParser::StmtLstContext::forLoop(size_t i) {
  return getRuleContext<SpiceParser::ForLoopContext>(i);
}

std::vector<SpiceParser::WhileLoopContext *> SpiceParser::StmtLstContext::whileLoop() {
  return getRuleContexts<SpiceParser::WhileLoopContext>();
}

SpiceParser::WhileLoopContext* SpiceParser::StmtLstContext::whileLoop(size_t i) {
  return getRuleContext<SpiceParser::WhileLoopContext>(i);
}

std::vector<SpiceParser::IfStmtContext *> SpiceParser::StmtLstContext::ifStmt() {
  return getRuleContexts<SpiceParser::IfStmtContext>();
}

SpiceParser::IfStmtContext* SpiceParser::StmtLstContext::ifStmt(size_t i) {
  return getRuleContext<SpiceParser::IfStmtContext>(i);
}


size_t SpiceParser::StmtLstContext::getRuleIndex() const {
  return SpiceParser::RuleStmtLst;
}


antlrcpp::Any SpiceParser::StmtLstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitStmtLst(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::StmtLstContext* SpiceParser::stmtLst() {
  StmtLstContext *_localctx = _tracker.createInstance<StmtLstContext>(_ctx, getState());
  enterRule(_localctx, 12, SpiceParser::RuleStmtLst);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::DATA_TYPE)
      | (1ULL << SpiceParser::IF)
      | (1ULL << SpiceParser::FOR)
      | (1ULL << SpiceParser::WHILE)
      | (1ULL << SpiceParser::CONST)
      | (1ULL << SpiceParser::IMPORT)
      | (1ULL << SpiceParser::RETURN)
      | (1ULL << SpiceParser::IDENTIFIER)
      | (1ULL << SpiceParser::INTEGER)
      | (1ULL << SpiceParser::DOUBLE)
      | (1ULL << SpiceParser::STRING)
      | (1ULL << SpiceParser::BOOL)
      | (1ULL << SpiceParser::LPAREN)
      | (1ULL << SpiceParser::NOT)
      | (1ULL << SpiceParser::PLUS_PLUS)
      | (1ULL << SpiceParser::MINUS_MINUS))) != 0)) {
      setState(111);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SpiceParser::DATA_TYPE:
        case SpiceParser::CONST:
        case SpiceParser::IMPORT:
        case SpiceParser::RETURN:
        case SpiceParser::IDENTIFIER:
        case SpiceParser::INTEGER:
        case SpiceParser::DOUBLE:
        case SpiceParser::STRING:
        case SpiceParser::BOOL:
        case SpiceParser::LPAREN:
        case SpiceParser::NOT:
        case SpiceParser::PLUS_PLUS:
        case SpiceParser::MINUS_MINUS: {
          setState(107);
          stmt();
          break;
        }

        case SpiceParser::FOR: {
          setState(108);
          forLoop();
          break;
        }

        case SpiceParser::WHILE: {
          setState(109);
          whileLoop();
          break;
        }

        case SpiceParser::IF: {
          setState(110);
          ifStmt();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(115);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamLstContext ------------------------------------------------------------------

SpiceParser::ParamLstContext::ParamLstContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::DeclStmtContext *> SpiceParser::ParamLstContext::declStmt() {
  return getRuleContexts<SpiceParser::DeclStmtContext>();
}

SpiceParser::DeclStmtContext* SpiceParser::ParamLstContext::declStmt(size_t i) {
  return getRuleContext<SpiceParser::DeclStmtContext>(i);
}

std::vector<SpiceParser::AssignmentContext *> SpiceParser::ParamLstContext::assignment() {
  return getRuleContexts<SpiceParser::AssignmentContext>();
}

SpiceParser::AssignmentContext* SpiceParser::ParamLstContext::assignment(size_t i) {
  return getRuleContext<SpiceParser::AssignmentContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::ParamLstContext::COMMA() {
  return getTokens(SpiceParser::COMMA);
}

tree::TerminalNode* SpiceParser::ParamLstContext::COMMA(size_t i) {
  return getToken(SpiceParser::COMMA, i);
}


size_t SpiceParser::ParamLstContext::getRuleIndex() const {
  return SpiceParser::RuleParamLst;
}


antlrcpp::Any SpiceParser::ParamLstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitParamLst(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::ParamLstContext* SpiceParser::paramLst() {
  ParamLstContext *_localctx = _tracker.createInstance<ParamLstContext>(_ctx, getState());
  enterRule(_localctx, 14, SpiceParser::RuleParamLst);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(118);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      setState(116);
      declStmt();
      break;
    }

    case 2: {
      setState(117);
      assignment();
      break;
    }

    default:
      break;
    }
    setState(127);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::COMMA) {
      setState(120);
      match(SpiceParser::COMMA);
      setState(123);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
      case 1: {
        setState(121);
        declStmt();
        break;
      }

      case 2: {
        setState(122);
        assignment();
        break;
      }

      default:
        break;
      }
      setState(129);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

SpiceParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::StmtContext::SEMICOLON() {
  return getToken(SpiceParser::SEMICOLON, 0);
}

SpiceParser::DeclStmtContext* SpiceParser::StmtContext::declStmt() {
  return getRuleContext<SpiceParser::DeclStmtContext>(0);
}

SpiceParser::AssignmentContext* SpiceParser::StmtContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}

SpiceParser::FunctionCallContext* SpiceParser::StmtContext::functionCall() {
  return getRuleContext<SpiceParser::FunctionCallContext>(0);
}

SpiceParser::TopLvlExprContext* SpiceParser::StmtContext::topLvlExpr() {
  return getRuleContext<SpiceParser::TopLvlExprContext>(0);
}

SpiceParser::ImportStmtContext* SpiceParser::StmtContext::importStmt() {
  return getRuleContext<SpiceParser::ImportStmtContext>(0);
}

SpiceParser::ReturnStmtContext* SpiceParser::StmtContext::returnStmt() {
  return getRuleContext<SpiceParser::ReturnStmtContext>(0);
}


size_t SpiceParser::StmtContext::getRuleIndex() const {
  return SpiceParser::RuleStmt;
}


antlrcpp::Any SpiceParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::StmtContext* SpiceParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 16, SpiceParser::RuleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      setState(130);
      declStmt();
      break;
    }

    case 2: {
      setState(131);
      assignment();
      break;
    }

    case 3: {
      setState(132);
      functionCall();
      break;
    }

    case 4: {
      setState(133);
      topLvlExpr();
      break;
    }

    case 5: {
      setState(134);
      importStmt();
      break;
    }

    case 6: {
      setState(135);
      returnStmt();
      break;
    }

    default:
      break;
    }
    setState(138);
    match(SpiceParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclStmtContext ------------------------------------------------------------------

SpiceParser::DeclStmtContext::DeclStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::DeclStmtContext::DATA_TYPE() {
  return getToken(SpiceParser::DATA_TYPE, 0);
}

tree::TerminalNode* SpiceParser::DeclStmtContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::DeclStmtContext::CONST() {
  return getToken(SpiceParser::CONST, 0);
}


size_t SpiceParser::DeclStmtContext::getRuleIndex() const {
  return SpiceParser::RuleDeclStmt;
}


antlrcpp::Any SpiceParser::DeclStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitDeclStmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::DeclStmtContext* SpiceParser::declStmt() {
  DeclStmtContext *_localctx = _tracker.createInstance<DeclStmtContext>(_ctx, getState());
  enterRule(_localctx, 18, SpiceParser::RuleDeclStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(141);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::CONST) {
      setState(140);
      match(SpiceParser::CONST);
    }
    setState(143);
    match(SpiceParser::DATA_TYPE);
    setState(144);
    match(SpiceParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionCallContext ------------------------------------------------------------------

SpiceParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::FunctionCallContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::FunctionCallContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::ParamLstContext* SpiceParser::FunctionCallContext::paramLst() {
  return getRuleContext<SpiceParser::ParamLstContext>(0);
}

tree::TerminalNode* SpiceParser::FunctionCallContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}


size_t SpiceParser::FunctionCallContext::getRuleIndex() const {
  return SpiceParser::RuleFunctionCall;
}


antlrcpp::Any SpiceParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::FunctionCallContext* SpiceParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 20, SpiceParser::RuleFunctionCall);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    match(SpiceParser::IDENTIFIER);
    setState(147);
    match(SpiceParser::LPAREN);
    setState(148);
    paramLst();
    setState(149);
    match(SpiceParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ImportStmtContext ------------------------------------------------------------------

SpiceParser::ImportStmtContext::ImportStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::ImportStmtContext::IMPORT() {
  return getToken(SpiceParser::IMPORT, 0);
}

tree::TerminalNode* SpiceParser::ImportStmtContext::STRING() {
  return getToken(SpiceParser::STRING, 0);
}


size_t SpiceParser::ImportStmtContext::getRuleIndex() const {
  return SpiceParser::RuleImportStmt;
}


antlrcpp::Any SpiceParser::ImportStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitImportStmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::ImportStmtContext* SpiceParser::importStmt() {
  ImportStmtContext *_localctx = _tracker.createInstance<ImportStmtContext>(_ctx, getState());
  enterRule(_localctx, 22, SpiceParser::RuleImportStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    match(SpiceParser::IMPORT);
    setState(152);
    match(SpiceParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

SpiceParser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::ReturnStmtContext::RETURN() {
  return getToken(SpiceParser::RETURN, 0);
}

SpiceParser::TopLvlExprContext* SpiceParser::ReturnStmtContext::topLvlExpr() {
  return getRuleContext<SpiceParser::TopLvlExprContext>(0);
}


size_t SpiceParser::ReturnStmtContext::getRuleIndex() const {
  return SpiceParser::RuleReturnStmt;
}


antlrcpp::Any SpiceParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::ReturnStmtContext* SpiceParser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 24, SpiceParser::RuleReturnStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(154);
    match(SpiceParser::RETURN);
    setState(155);
    topLvlExpr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TopLvlExprContext ------------------------------------------------------------------

SpiceParser::TopLvlExprContext::TopLvlExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::AssignmentContext* SpiceParser::TopLvlExprContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}


size_t SpiceParser::TopLvlExprContext::getRuleIndex() const {
  return SpiceParser::RuleTopLvlExpr;
}


antlrcpp::Any SpiceParser::TopLvlExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitTopLvlExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::TopLvlExprContext* SpiceParser::topLvlExpr() {
  TopLvlExprContext *_localctx = _tracker.createInstance<TopLvlExprContext>(_ctx, getState());
  enterRule(_localctx, 26, SpiceParser::RuleTopLvlExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(157);
    assignment();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

SpiceParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::TernaryContext* SpiceParser::AssignmentContext::ternary() {
  return getRuleContext<SpiceParser::TernaryContext>(0);
}

tree::TerminalNode* SpiceParser::AssignmentContext::ASSIGN_OP() {
  return getToken(SpiceParser::ASSIGN_OP, 0);
}

tree::TerminalNode* SpiceParser::AssignmentContext::PLUS_EQUAL() {
  return getToken(SpiceParser::PLUS_EQUAL, 0);
}

tree::TerminalNode* SpiceParser::AssignmentContext::MINUS_EQUAL() {
  return getToken(SpiceParser::MINUS_EQUAL, 0);
}

tree::TerminalNode* SpiceParser::AssignmentContext::MUL_EQUAL() {
  return getToken(SpiceParser::MUL_EQUAL, 0);
}

tree::TerminalNode* SpiceParser::AssignmentContext::DIV_EQUAL() {
  return getToken(SpiceParser::DIV_EQUAL, 0);
}

SpiceParser::DeclStmtContext* SpiceParser::AssignmentContext::declStmt() {
  return getRuleContext<SpiceParser::DeclStmtContext>(0);
}

tree::TerminalNode* SpiceParser::AssignmentContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}


size_t SpiceParser::AssignmentContext::getRuleIndex() const {
  return SpiceParser::RuleAssignment;
}


antlrcpp::Any SpiceParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::AssignmentContext* SpiceParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 28, SpiceParser::RuleAssignment);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(164);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      setState(161);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SpiceParser::DATA_TYPE:
        case SpiceParser::CONST: {
          setState(159);
          declStmt();
          break;
        }

        case SpiceParser::IDENTIFIER: {
          setState(160);
          match(SpiceParser::IDENTIFIER);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(163);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << SpiceParser::PLUS_EQUAL)
        | (1ULL << SpiceParser::MINUS_EQUAL)
        | (1ULL << SpiceParser::MUL_EQUAL)
        | (1ULL << SpiceParser::DIV_EQUAL)
        | (1ULL << SpiceParser::ASSIGN_OP))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      break;
    }

    default:
      break;
    }
    setState(166);
    ternary();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TernaryContext ------------------------------------------------------------------

SpiceParser::TernaryContext::TernaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::LogicalOrExprContext *> SpiceParser::TernaryContext::logicalOrExpr() {
  return getRuleContexts<SpiceParser::LogicalOrExprContext>();
}

SpiceParser::LogicalOrExprContext* SpiceParser::TernaryContext::logicalOrExpr(size_t i) {
  return getRuleContext<SpiceParser::LogicalOrExprContext>(i);
}

tree::TerminalNode* SpiceParser::TernaryContext::QUESTION_MARK() {
  return getToken(SpiceParser::QUESTION_MARK, 0);
}

tree::TerminalNode* SpiceParser::TernaryContext::COLON() {
  return getToken(SpiceParser::COLON, 0);
}


size_t SpiceParser::TernaryContext::getRuleIndex() const {
  return SpiceParser::RuleTernary;
}


antlrcpp::Any SpiceParser::TernaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitTernary(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::TernaryContext* SpiceParser::ternary() {
  TernaryContext *_localctx = _tracker.createInstance<TernaryContext>(_ctx, getState());
  enterRule(_localctx, 30, SpiceParser::RuleTernary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    logicalOrExpr();
    setState(174);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::QUESTION_MARK) {
      setState(169);
      match(SpiceParser::QUESTION_MARK);
      setState(170);
      logicalOrExpr();
      setState(171);
      match(SpiceParser::COLON);
      setState(172);
      logicalOrExpr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalOrExprContext ------------------------------------------------------------------

SpiceParser::LogicalOrExprContext::LogicalOrExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::LogicalAndExprContext *> SpiceParser::LogicalOrExprContext::logicalAndExpr() {
  return getRuleContexts<SpiceParser::LogicalAndExprContext>();
}

SpiceParser::LogicalAndExprContext* SpiceParser::LogicalOrExprContext::logicalAndExpr(size_t i) {
  return getRuleContext<SpiceParser::LogicalAndExprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::LogicalOrExprContext::LOGICAL_OR() {
  return getTokens(SpiceParser::LOGICAL_OR);
}

tree::TerminalNode* SpiceParser::LogicalOrExprContext::LOGICAL_OR(size_t i) {
  return getToken(SpiceParser::LOGICAL_OR, i);
}


size_t SpiceParser::LogicalOrExprContext::getRuleIndex() const {
  return SpiceParser::RuleLogicalOrExpr;
}


antlrcpp::Any SpiceParser::LogicalOrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitLogicalOrExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::LogicalOrExprContext* SpiceParser::logicalOrExpr() {
  LogicalOrExprContext *_localctx = _tracker.createInstance<LogicalOrExprContext>(_ctx, getState());
  enterRule(_localctx, 32, SpiceParser::RuleLogicalOrExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    logicalAndExpr();
    setState(181);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::LOGICAL_OR) {
      setState(177);
      match(SpiceParser::LOGICAL_OR);
      setState(178);
      logicalAndExpr();
      setState(183);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalAndExprContext ------------------------------------------------------------------

SpiceParser::LogicalAndExprContext::LogicalAndExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::BitwiseOrExprContext *> SpiceParser::LogicalAndExprContext::bitwiseOrExpr() {
  return getRuleContexts<SpiceParser::BitwiseOrExprContext>();
}

SpiceParser::BitwiseOrExprContext* SpiceParser::LogicalAndExprContext::bitwiseOrExpr(size_t i) {
  return getRuleContext<SpiceParser::BitwiseOrExprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::LogicalAndExprContext::LOGICAL_AND() {
  return getTokens(SpiceParser::LOGICAL_AND);
}

tree::TerminalNode* SpiceParser::LogicalAndExprContext::LOGICAL_AND(size_t i) {
  return getToken(SpiceParser::LOGICAL_AND, i);
}


size_t SpiceParser::LogicalAndExprContext::getRuleIndex() const {
  return SpiceParser::RuleLogicalAndExpr;
}


antlrcpp::Any SpiceParser::LogicalAndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitLogicalAndExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::LogicalAndExprContext* SpiceParser::logicalAndExpr() {
  LogicalAndExprContext *_localctx = _tracker.createInstance<LogicalAndExprContext>(_ctx, getState());
  enterRule(_localctx, 34, SpiceParser::RuleLogicalAndExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(184);
    bitwiseOrExpr();
    setState(189);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::LOGICAL_AND) {
      setState(185);
      match(SpiceParser::LOGICAL_AND);
      setState(186);
      bitwiseOrExpr();
      setState(191);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BitwiseOrExprContext ------------------------------------------------------------------

SpiceParser::BitwiseOrExprContext::BitwiseOrExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::BitwiseAndExprContext *> SpiceParser::BitwiseOrExprContext::bitwiseAndExpr() {
  return getRuleContexts<SpiceParser::BitwiseAndExprContext>();
}

SpiceParser::BitwiseAndExprContext* SpiceParser::BitwiseOrExprContext::bitwiseAndExpr(size_t i) {
  return getRuleContext<SpiceParser::BitwiseAndExprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::BitwiseOrExprContext::BITWISE_OR() {
  return getTokens(SpiceParser::BITWISE_OR);
}

tree::TerminalNode* SpiceParser::BitwiseOrExprContext::BITWISE_OR(size_t i) {
  return getToken(SpiceParser::BITWISE_OR, i);
}


size_t SpiceParser::BitwiseOrExprContext::getRuleIndex() const {
  return SpiceParser::RuleBitwiseOrExpr;
}


antlrcpp::Any SpiceParser::BitwiseOrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitBitwiseOrExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::BitwiseOrExprContext* SpiceParser::bitwiseOrExpr() {
  BitwiseOrExprContext *_localctx = _tracker.createInstance<BitwiseOrExprContext>(_ctx, getState());
  enterRule(_localctx, 36, SpiceParser::RuleBitwiseOrExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(192);
    bitwiseAndExpr();
    setState(197);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::BITWISE_OR) {
      setState(193);
      match(SpiceParser::BITWISE_OR);
      setState(194);
      bitwiseAndExpr();
      setState(199);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BitwiseAndExprContext ------------------------------------------------------------------

SpiceParser::BitwiseAndExprContext::BitwiseAndExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::EqualityContext *> SpiceParser::BitwiseAndExprContext::equality() {
  return getRuleContexts<SpiceParser::EqualityContext>();
}

SpiceParser::EqualityContext* SpiceParser::BitwiseAndExprContext::equality(size_t i) {
  return getRuleContext<SpiceParser::EqualityContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::BitwiseAndExprContext::BITWISE_AND() {
  return getTokens(SpiceParser::BITWISE_AND);
}

tree::TerminalNode* SpiceParser::BitwiseAndExprContext::BITWISE_AND(size_t i) {
  return getToken(SpiceParser::BITWISE_AND, i);
}


size_t SpiceParser::BitwiseAndExprContext::getRuleIndex() const {
  return SpiceParser::RuleBitwiseAndExpr;
}


antlrcpp::Any SpiceParser::BitwiseAndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitBitwiseAndExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::BitwiseAndExprContext* SpiceParser::bitwiseAndExpr() {
  BitwiseAndExprContext *_localctx = _tracker.createInstance<BitwiseAndExprContext>(_ctx, getState());
  enterRule(_localctx, 38, SpiceParser::RuleBitwiseAndExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(200);
    equality();
    setState(205);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::BITWISE_AND) {
      setState(201);
      match(SpiceParser::BITWISE_AND);
      setState(202);
      equality();
      setState(207);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualityContext ------------------------------------------------------------------

SpiceParser::EqualityContext::EqualityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::RelationalExprContext *> SpiceParser::EqualityContext::relationalExpr() {
  return getRuleContexts<SpiceParser::RelationalExprContext>();
}

SpiceParser::RelationalExprContext* SpiceParser::EqualityContext::relationalExpr(size_t i) {
  return getRuleContext<SpiceParser::RelationalExprContext>(i);
}

tree::TerminalNode* SpiceParser::EqualityContext::EQUAL() {
  return getToken(SpiceParser::EQUAL, 0);
}

tree::TerminalNode* SpiceParser::EqualityContext::NOT_EQUAL() {
  return getToken(SpiceParser::NOT_EQUAL, 0);
}


size_t SpiceParser::EqualityContext::getRuleIndex() const {
  return SpiceParser::RuleEquality;
}


antlrcpp::Any SpiceParser::EqualityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitEquality(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::EqualityContext* SpiceParser::equality() {
  EqualityContext *_localctx = _tracker.createInstance<EqualityContext>(_ctx, getState());
  enterRule(_localctx, 40, SpiceParser::RuleEquality);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(208);
    relationalExpr();
    setState(211);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::EQUAL

    || _la == SpiceParser::NOT_EQUAL) {
      setState(209);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::EQUAL

      || _la == SpiceParser::NOT_EQUAL)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(210);
      relationalExpr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationalExprContext ------------------------------------------------------------------

SpiceParser::RelationalExprContext::RelationalExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::AdditiveExprContext *> SpiceParser::RelationalExprContext::additiveExpr() {
  return getRuleContexts<SpiceParser::AdditiveExprContext>();
}

SpiceParser::AdditiveExprContext* SpiceParser::RelationalExprContext::additiveExpr(size_t i) {
  return getRuleContext<SpiceParser::AdditiveExprContext>(i);
}

tree::TerminalNode* SpiceParser::RelationalExprContext::LESS() {
  return getToken(SpiceParser::LESS, 0);
}

tree::TerminalNode* SpiceParser::RelationalExprContext::GREATER() {
  return getToken(SpiceParser::GREATER, 0);
}

tree::TerminalNode* SpiceParser::RelationalExprContext::LESS_EQUAL() {
  return getToken(SpiceParser::LESS_EQUAL, 0);
}

tree::TerminalNode* SpiceParser::RelationalExprContext::GREATER_EQUAL() {
  return getToken(SpiceParser::GREATER_EQUAL, 0);
}


size_t SpiceParser::RelationalExprContext::getRuleIndex() const {
  return SpiceParser::RuleRelationalExpr;
}


antlrcpp::Any SpiceParser::RelationalExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitRelationalExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::RelationalExprContext* SpiceParser::relationalExpr() {
  RelationalExprContext *_localctx = _tracker.createInstance<RelationalExprContext>(_ctx, getState());
  enterRule(_localctx, 42, SpiceParser::RuleRelationalExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(213);
    additiveExpr();
    setState(216);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::GREATER)
      | (1ULL << SpiceParser::LESS)
      | (1ULL << SpiceParser::GREATER_EQUAL)
      | (1ULL << SpiceParser::LESS_EQUAL))) != 0)) {
      setState(214);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << SpiceParser::GREATER)
        | (1ULL << SpiceParser::LESS)
        | (1ULL << SpiceParser::GREATER_EQUAL)
        | (1ULL << SpiceParser::LESS_EQUAL))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(215);
      additiveExpr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExprContext ------------------------------------------------------------------

SpiceParser::AdditiveExprContext::AdditiveExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::MultiplicativeExprContext *> SpiceParser::AdditiveExprContext::multiplicativeExpr() {
  return getRuleContexts<SpiceParser::MultiplicativeExprContext>();
}

SpiceParser::MultiplicativeExprContext* SpiceParser::AdditiveExprContext::multiplicativeExpr(size_t i) {
  return getRuleContext<SpiceParser::MultiplicativeExprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::AdditiveExprContext::PLUS() {
  return getTokens(SpiceParser::PLUS);
}

tree::TerminalNode* SpiceParser::AdditiveExprContext::PLUS(size_t i) {
  return getToken(SpiceParser::PLUS, i);
}

std::vector<tree::TerminalNode *> SpiceParser::AdditiveExprContext::MINUS() {
  return getTokens(SpiceParser::MINUS);
}

tree::TerminalNode* SpiceParser::AdditiveExprContext::MINUS(size_t i) {
  return getToken(SpiceParser::MINUS, i);
}


size_t SpiceParser::AdditiveExprContext::getRuleIndex() const {
  return SpiceParser::RuleAdditiveExpr;
}


antlrcpp::Any SpiceParser::AdditiveExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::AdditiveExprContext* SpiceParser::additiveExpr() {
  AdditiveExprContext *_localctx = _tracker.createInstance<AdditiveExprContext>(_ctx, getState());
  enterRule(_localctx, 44, SpiceParser::RuleAdditiveExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(218);
    multiplicativeExpr();
    setState(223);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::PLUS

    || _la == SpiceParser::MINUS) {
      setState(219);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::PLUS

      || _la == SpiceParser::MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(220);
      multiplicativeExpr();
      setState(225);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeExprContext ------------------------------------------------------------------

SpiceParser::MultiplicativeExprContext::MultiplicativeExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::PrefixUnaryContext *> SpiceParser::MultiplicativeExprContext::prefixUnary() {
  return getRuleContexts<SpiceParser::PrefixUnaryContext>();
}

SpiceParser::PrefixUnaryContext* SpiceParser::MultiplicativeExprContext::prefixUnary(size_t i) {
  return getRuleContext<SpiceParser::PrefixUnaryContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::MultiplicativeExprContext::MUL() {
  return getTokens(SpiceParser::MUL);
}

tree::TerminalNode* SpiceParser::MultiplicativeExprContext::MUL(size_t i) {
  return getToken(SpiceParser::MUL, i);
}

std::vector<tree::TerminalNode *> SpiceParser::MultiplicativeExprContext::DIV() {
  return getTokens(SpiceParser::DIV);
}

tree::TerminalNode* SpiceParser::MultiplicativeExprContext::DIV(size_t i) {
  return getToken(SpiceParser::DIV, i);
}


size_t SpiceParser::MultiplicativeExprContext::getRuleIndex() const {
  return SpiceParser::RuleMultiplicativeExpr;
}


antlrcpp::Any SpiceParser::MultiplicativeExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::MultiplicativeExprContext* SpiceParser::multiplicativeExpr() {
  MultiplicativeExprContext *_localctx = _tracker.createInstance<MultiplicativeExprContext>(_ctx, getState());
  enterRule(_localctx, 46, SpiceParser::RuleMultiplicativeExpr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(226);
    prefixUnary();
    setState(231);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::MUL

    || _la == SpiceParser::DIV) {
      setState(227);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::MUL

      || _la == SpiceParser::DIV)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(228);
      prefixUnary();
      setState(233);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrefixUnaryContext ------------------------------------------------------------------

SpiceParser::PrefixUnaryContext::PrefixUnaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::PostfixUnaryContext* SpiceParser::PrefixUnaryContext::postfixUnary() {
  return getRuleContext<SpiceParser::PostfixUnaryContext>(0);
}

tree::TerminalNode* SpiceParser::PrefixUnaryContext::NOT() {
  return getToken(SpiceParser::NOT, 0);
}

tree::TerminalNode* SpiceParser::PrefixUnaryContext::PLUS_PLUS() {
  return getToken(SpiceParser::PLUS_PLUS, 0);
}

tree::TerminalNode* SpiceParser::PrefixUnaryContext::MINUS_MINUS() {
  return getToken(SpiceParser::MINUS_MINUS, 0);
}


size_t SpiceParser::PrefixUnaryContext::getRuleIndex() const {
  return SpiceParser::RulePrefixUnary;
}


antlrcpp::Any SpiceParser::PrefixUnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitPrefixUnary(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::PrefixUnaryContext* SpiceParser::prefixUnary() {
  PrefixUnaryContext *_localctx = _tracker.createInstance<PrefixUnaryContext>(_ctx, getState());
  enterRule(_localctx, 48, SpiceParser::RulePrefixUnary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(235);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::NOT)
      | (1ULL << SpiceParser::PLUS_PLUS)
      | (1ULL << SpiceParser::MINUS_MINUS))) != 0)) {
      setState(234);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << SpiceParser::NOT)
        | (1ULL << SpiceParser::PLUS_PLUS)
        | (1ULL << SpiceParser::MINUS_MINUS))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
    setState(237);
    postfixUnary();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PostfixUnaryContext ------------------------------------------------------------------

SpiceParser::PostfixUnaryContext::PostfixUnaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::AtomicExprContext* SpiceParser::PostfixUnaryContext::atomicExpr() {
  return getRuleContext<SpiceParser::AtomicExprContext>(0);
}

tree::TerminalNode* SpiceParser::PostfixUnaryContext::PLUS_PLUS() {
  return getToken(SpiceParser::PLUS_PLUS, 0);
}

tree::TerminalNode* SpiceParser::PostfixUnaryContext::MINUS_MINUS() {
  return getToken(SpiceParser::MINUS_MINUS, 0);
}


size_t SpiceParser::PostfixUnaryContext::getRuleIndex() const {
  return SpiceParser::RulePostfixUnary;
}


antlrcpp::Any SpiceParser::PostfixUnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitPostfixUnary(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::PostfixUnaryContext* SpiceParser::postfixUnary() {
  PostfixUnaryContext *_localctx = _tracker.createInstance<PostfixUnaryContext>(_ctx, getState());
  enterRule(_localctx, 50, SpiceParser::RulePostfixUnary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(239);
    atomicExpr();
    setState(241);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::PLUS_PLUS

    || _la == SpiceParser::MINUS_MINUS) {
      setState(240);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::PLUS_PLUS

      || _la == SpiceParser::MINUS_MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomicExprContext ------------------------------------------------------------------

SpiceParser::AtomicExprContext::AtomicExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::ValueContext* SpiceParser::AtomicExprContext::value() {
  return getRuleContext<SpiceParser::ValueContext>(0);
}

tree::TerminalNode* SpiceParser::AtomicExprContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::AssignmentContext* SpiceParser::AtomicExprContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}

tree::TerminalNode* SpiceParser::AtomicExprContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}


size_t SpiceParser::AtomicExprContext::getRuleIndex() const {
  return SpiceParser::RuleAtomicExpr;
}


antlrcpp::Any SpiceParser::AtomicExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitAtomicExpr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::AtomicExprContext* SpiceParser::atomicExpr() {
  AtomicExprContext *_localctx = _tracker.createInstance<AtomicExprContext>(_ctx, getState());
  enterRule(_localctx, 52, SpiceParser::RuleAtomicExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(248);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SpiceParser::IDENTIFIER:
      case SpiceParser::INTEGER:
      case SpiceParser::DOUBLE:
      case SpiceParser::STRING:
      case SpiceParser::BOOL: {
        enterOuterAlt(_localctx, 1);
        setState(243);
        value();
        break;
      }

      case SpiceParser::LPAREN: {
        enterOuterAlt(_localctx, 2);
        setState(244);
        match(SpiceParser::LPAREN);
        setState(245);
        assignment();
        setState(246);
        match(SpiceParser::RPAREN);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

SpiceParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::ValueContext::STRING() {
  return getToken(SpiceParser::STRING, 0);
}

tree::TerminalNode* SpiceParser::ValueContext::BOOL() {
  return getToken(SpiceParser::BOOL, 0);
}

tree::TerminalNode* SpiceParser::ValueContext::INTEGER() {
  return getToken(SpiceParser::INTEGER, 0);
}

tree::TerminalNode* SpiceParser::ValueContext::DOUBLE() {
  return getToken(SpiceParser::DOUBLE, 0);
}

tree::TerminalNode* SpiceParser::ValueContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

SpiceParser::FunctionCallContext* SpiceParser::ValueContext::functionCall() {
  return getRuleContext<SpiceParser::FunctionCallContext>(0);
}


size_t SpiceParser::ValueContext::getRuleIndex() const {
  return SpiceParser::RuleValue;
}


antlrcpp::Any SpiceParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::ValueContext* SpiceParser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 54, SpiceParser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(256);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(250);
      match(SpiceParser::STRING);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(251);
      match(SpiceParser::BOOL);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(252);
      match(SpiceParser::INTEGER);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(253);
      match(SpiceParser::DOUBLE);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(254);
      match(SpiceParser::IDENTIFIER);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(255);
      functionCall();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> SpiceParser::_decisionToDFA;
atn::PredictionContextCache SpiceParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN SpiceParser::_atn;
std::vector<uint16_t> SpiceParser::_serializedATN;

std::vector<std::string> SpiceParser::_ruleNames = {
  "entry", "functionDef", "procedureDef", "forLoop", "whileLoop", "ifStmt", 
  "stmtLst", "paramLst", "stmt", "declStmt", "functionCall", "importStmt", 
  "returnStmt", "topLvlExpr", "assignment", "ternary", "logicalOrExpr", 
  "logicalAndExpr", "bitwiseOrExpr", "bitwiseAndExpr", "equality", "relationalExpr", 
  "additiveExpr", "multiplicativeExpr", "prefixUnary", "postfixUnary", "atomicExpr", 
  "value"
};

std::vector<std::string> SpiceParser::_literalNames = {
  "", "", "'f'", "'p'", "'if'", "'for'", "'foreach'", "'while'", "'const'", 
  "'import'", "'return'", "", "", "", "", "", "'true'", "'false'", "'{'", 
  "'}'", "'('", "')'", "'['", "']'", "'||'", "'&&'", "'|'", "'&'", "'!'", 
  "'++'", "'--'", "'+='", "'-='", "'*='", "'/='", "'+'", "'-'", "'*'", "'/'", 
  "'>'", "'<'", "'>='", "'<='", "'=='", "'!='", "'='", "'\u003F'", "';'", 
  "':'", "','"
};

std::vector<std::string> SpiceParser::_symbolicNames = {
  "", "DATA_TYPE", "F", "P", "IF", "FOR", "FOREACH", "WHILE", "CONST", "IMPORT", 
  "RETURN", "IDENTIFIER", "INTEGER", "DOUBLE", "STRING", "BOOL", "TRUE", 
  "FALSE", "LBRACE", "RBRACE", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", 
  "LOGICAL_OR", "LOGICAL_AND", "BITWISE_OR", "BITWISE_AND", "NOT", "PLUS_PLUS", 
  "MINUS_MINUS", "PLUS_EQUAL", "MINUS_EQUAL", "MUL_EQUAL", "DIV_EQUAL", 
  "PLUS", "MINUS", "MUL", "DIV", "GREATER", "LESS", "GREATER_EQUAL", "LESS_EQUAL", 
  "EQUAL", "NOT_EQUAL", "ASSIGN_OP", "QUESTION_MARK", "SEMICOLON", "COLON", 
  "COMMA", "COMMENT", "LINE_COMMENT", "WS"
};

dfa::Vocabulary SpiceParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> SpiceParser::_tokenNames;

SpiceParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x36, 0x105, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x7, 0x2, 0x3e, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x41, 0xb, 0x2, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x72, 0xa, 0x8, 
    0xc, 0x8, 0xe, 0x8, 0x75, 0xb, 0x8, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x79, 
    0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x7e, 0xa, 0x9, 0x7, 
    0x9, 0x80, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x83, 0xb, 0x9, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x8b, 0xa, 0xa, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x5, 0xb, 0x90, 0xa, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xa4, 0xa, 0x10, 0x3, 0x10, 0x5, 
    0x10, 0xa7, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 
    0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x5, 0x11, 0xb1, 0xa, 0x11, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0xb6, 0xa, 0x12, 0xc, 0x12, 0xe, 
    0x12, 0xb9, 0xb, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0xbe, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0xc1, 0xb, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x7, 0x14, 0xc6, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0xc9, 0xb, 
    0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0xce, 0xa, 0x15, 0xc, 
    0x15, 0xe, 0x15, 0xd1, 0xb, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 
    0x16, 0xd6, 0xa, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0xdb, 
    0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x7, 0x18, 0xe0, 0xa, 0x18, 
    0xc, 0x18, 0xe, 0x18, 0xe3, 0xb, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 
    0x7, 0x19, 0xe8, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0xeb, 0xb, 0x19, 0x3, 
    0x1a, 0x5, 0x1a, 0xee, 0xa, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 
    0x1b, 0x5, 0x1b, 0xf4, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
    0x1c, 0x3, 0x1c, 0x5, 0x1c, 0xfb, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x103, 0xa, 0x1d, 
    0x3, 0x1d, 0x2, 0x2, 0x1e, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 
    0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 
    0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x2, 0x9, 0x4, 0x2, 
    0x21, 0x24, 0x2f, 0x2f, 0x3, 0x2, 0x2d, 0x2e, 0x3, 0x2, 0x29, 0x2c, 
    0x3, 0x2, 0x25, 0x26, 0x3, 0x2, 0x27, 0x28, 0x3, 0x2, 0x1e, 0x20, 0x3, 
    0x2, 0x1f, 0x20, 0x2, 0x10c, 0x2, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x4, 0x42, 
    0x3, 0x2, 0x2, 0x2, 0x6, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x8, 0x57, 0x3, 0x2, 
    0x2, 0x2, 0xa, 0x61, 0x3, 0x2, 0x2, 0x2, 0xc, 0x67, 0x3, 0x2, 0x2, 0x2, 
    0xe, 0x73, 0x3, 0x2, 0x2, 0x2, 0x10, 0x78, 0x3, 0x2, 0x2, 0x2, 0x12, 
    0x8a, 0x3, 0x2, 0x2, 0x2, 0x14, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x16, 0x94, 
    0x3, 0x2, 0x2, 0x2, 0x18, 0x99, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x9c, 0x3, 
    0x2, 0x2, 0x2, 0x1c, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xa6, 0x3, 0x2, 
    0x2, 0x2, 0x20, 0xaa, 0x3, 0x2, 0x2, 0x2, 0x22, 0xb2, 0x3, 0x2, 0x2, 
    0x2, 0x24, 0xba, 0x3, 0x2, 0x2, 0x2, 0x26, 0xc2, 0x3, 0x2, 0x2, 0x2, 
    0x28, 0xca, 0x3, 0x2, 0x2, 0x2, 0x2a, 0xd2, 0x3, 0x2, 0x2, 0x2, 0x2c, 
    0xd7, 0x3, 0x2, 0x2, 0x2, 0x2e, 0xdc, 0x3, 0x2, 0x2, 0x2, 0x30, 0xe4, 
    0x3, 0x2, 0x2, 0x2, 0x32, 0xed, 0x3, 0x2, 0x2, 0x2, 0x34, 0xf1, 0x3, 
    0x2, 0x2, 0x2, 0x36, 0xfa, 0x3, 0x2, 0x2, 0x2, 0x38, 0x102, 0x3, 0x2, 
    0x2, 0x2, 0x3a, 0x3e, 0x5, 0x12, 0xa, 0x2, 0x3b, 0x3e, 0x5, 0x4, 0x3, 
    0x2, 0x3c, 0x3e, 0x5, 0x6, 0x4, 0x2, 0x3d, 0x3a, 0x3, 0x2, 0x2, 0x2, 
    0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x3e, 
    0x41, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x40, 
    0x3, 0x2, 0x2, 0x2, 0x40, 0x3, 0x3, 0x2, 0x2, 0x2, 0x41, 0x3f, 0x3, 
    0x2, 0x2, 0x2, 0x42, 0x43, 0x7, 0x4, 0x2, 0x2, 0x43, 0x44, 0x7, 0x2a, 
    0x2, 0x2, 0x44, 0x45, 0x7, 0x3, 0x2, 0x2, 0x45, 0x46, 0x7, 0x29, 0x2, 
    0x2, 0x46, 0x47, 0x7, 0xd, 0x2, 0x2, 0x47, 0x48, 0x7, 0x16, 0x2, 0x2, 
    0x48, 0x49, 0x5, 0x10, 0x9, 0x2, 0x49, 0x4a, 0x7, 0x17, 0x2, 0x2, 0x4a, 
    0x4b, 0x7, 0x14, 0x2, 0x2, 0x4b, 0x4c, 0x5, 0xe, 0x8, 0x2, 0x4c, 0x4d, 
    0x7, 0x15, 0x2, 0x2, 0x4d, 0x5, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4f, 0x7, 
    0x5, 0x2, 0x2, 0x4f, 0x50, 0x7, 0xd, 0x2, 0x2, 0x50, 0x51, 0x7, 0x16, 
    0x2, 0x2, 0x51, 0x52, 0x5, 0x10, 0x9, 0x2, 0x52, 0x53, 0x7, 0x17, 0x2, 
    0x2, 0x53, 0x54, 0x7, 0x14, 0x2, 0x2, 0x54, 0x55, 0x5, 0xe, 0x8, 0x2, 
    0x55, 0x56, 0x7, 0x15, 0x2, 0x2, 0x56, 0x7, 0x3, 0x2, 0x2, 0x2, 0x57, 
    0x58, 0x7, 0x7, 0x2, 0x2, 0x58, 0x59, 0x5, 0x1e, 0x10, 0x2, 0x59, 0x5a, 
    0x7, 0x31, 0x2, 0x2, 0x5a, 0x5b, 0x5, 0x1c, 0xf, 0x2, 0x5b, 0x5c, 0x7, 
    0x31, 0x2, 0x2, 0x5c, 0x5d, 0x5, 0x1c, 0xf, 0x2, 0x5d, 0x5e, 0x7, 0x14, 
    0x2, 0x2, 0x5e, 0x5f, 0x5, 0xe, 0x8, 0x2, 0x5f, 0x60, 0x7, 0x15, 0x2, 
    0x2, 0x60, 0x9, 0x3, 0x2, 0x2, 0x2, 0x61, 0x62, 0x7, 0x9, 0x2, 0x2, 
    0x62, 0x63, 0x5, 0x1c, 0xf, 0x2, 0x63, 0x64, 0x7, 0x14, 0x2, 0x2, 0x64, 
    0x65, 0x5, 0xe, 0x8, 0x2, 0x65, 0x66, 0x7, 0x15, 0x2, 0x2, 0x66, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 0x7, 0x6, 0x2, 0x2, 0x68, 0x69, 0x5, 
    0x1c, 0xf, 0x2, 0x69, 0x6a, 0x7, 0x14, 0x2, 0x2, 0x6a, 0x6b, 0x5, 0xe, 
    0x8, 0x2, 0x6b, 0x6c, 0x7, 0x15, 0x2, 0x2, 0x6c, 0xd, 0x3, 0x2, 0x2, 
    0x2, 0x6d, 0x72, 0x5, 0x12, 0xa, 0x2, 0x6e, 0x72, 0x5, 0x8, 0x5, 0x2, 
    0x6f, 0x72, 0x5, 0xa, 0x6, 0x2, 0x70, 0x72, 0x5, 0xc, 0x7, 0x2, 0x71, 
    0x6d, 0x3, 0x2, 0x2, 0x2, 0x71, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x71, 0x6f, 
    0x3, 0x2, 0x2, 0x2, 0x71, 0x70, 0x3, 0x2, 0x2, 0x2, 0x72, 0x75, 0x3, 
    0x2, 0x2, 0x2, 0x73, 0x71, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x3, 0x2, 
    0x2, 0x2, 0x74, 0xf, 0x3, 0x2, 0x2, 0x2, 0x75, 0x73, 0x3, 0x2, 0x2, 
    0x2, 0x76, 0x79, 0x5, 0x14, 0xb, 0x2, 0x77, 0x79, 0x5, 0x1e, 0x10, 0x2, 
    0x78, 0x76, 0x3, 0x2, 0x2, 0x2, 0x78, 0x77, 0x3, 0x2, 0x2, 0x2, 0x78, 
    0x79, 0x3, 0x2, 0x2, 0x2, 0x79, 0x81, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7d, 
    0x7, 0x33, 0x2, 0x2, 0x7b, 0x7e, 0x5, 0x14, 0xb, 0x2, 0x7c, 0x7e, 0x5, 
    0x1e, 0x10, 0x2, 0x7d, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7c, 0x3, 0x2, 
    0x2, 0x2, 0x7e, 0x80, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x7a, 0x3, 0x2, 0x2, 
    0x2, 0x80, 0x83, 0x3, 0x2, 0x2, 0x2, 0x81, 0x7f, 0x3, 0x2, 0x2, 0x2, 
    0x81, 0x82, 0x3, 0x2, 0x2, 0x2, 0x82, 0x11, 0x3, 0x2, 0x2, 0x2, 0x83, 
    0x81, 0x3, 0x2, 0x2, 0x2, 0x84, 0x8b, 0x5, 0x14, 0xb, 0x2, 0x85, 0x8b, 
    0x5, 0x1e, 0x10, 0x2, 0x86, 0x8b, 0x5, 0x16, 0xc, 0x2, 0x87, 0x8b, 0x5, 
    0x1c, 0xf, 0x2, 0x88, 0x8b, 0x5, 0x18, 0xd, 0x2, 0x89, 0x8b, 0x5, 0x1a, 
    0xe, 0x2, 0x8a, 0x84, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x85, 0x3, 0x2, 0x2, 
    0x2, 0x8a, 0x86, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x87, 0x3, 0x2, 0x2, 0x2, 
    0x8a, 0x88, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x89, 0x3, 0x2, 0x2, 0x2, 0x8b, 
    0x8c, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 0x7, 0x31, 0x2, 0x2, 0x8d, 0x13, 
    0x3, 0x2, 0x2, 0x2, 0x8e, 0x90, 0x7, 0xa, 0x2, 0x2, 0x8f, 0x8e, 0x3, 
    0x2, 0x2, 0x2, 0x8f, 0x90, 0x3, 0x2, 0x2, 0x2, 0x90, 0x91, 0x3, 0x2, 
    0x2, 0x2, 0x91, 0x92, 0x7, 0x3, 0x2, 0x2, 0x92, 0x93, 0x7, 0xd, 0x2, 
    0x2, 0x93, 0x15, 0x3, 0x2, 0x2, 0x2, 0x94, 0x95, 0x7, 0xd, 0x2, 0x2, 
    0x95, 0x96, 0x7, 0x16, 0x2, 0x2, 0x96, 0x97, 0x5, 0x10, 0x9, 0x2, 0x97, 
    0x98, 0x7, 0x17, 0x2, 0x2, 0x98, 0x17, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9a, 
    0x7, 0xb, 0x2, 0x2, 0x9a, 0x9b, 0x7, 0x10, 0x2, 0x2, 0x9b, 0x19, 0x3, 
    0x2, 0x2, 0x2, 0x9c, 0x9d, 0x7, 0xc, 0x2, 0x2, 0x9d, 0x9e, 0x5, 0x1c, 
    0xf, 0x2, 0x9e, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x9f, 0xa0, 0x5, 0x1e, 0x10, 
    0x2, 0xa0, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xa1, 0xa4, 0x5, 0x14, 0xb, 0x2, 
    0xa2, 0xa4, 0x7, 0xd, 0x2, 0x2, 0xa3, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa3, 
    0xa2, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa7, 
    0x9, 0x2, 0x2, 0x2, 0xa6, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa7, 0x3, 
    0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa8, 0xa9, 0x5, 0x20, 
    0x11, 0x2, 0xa9, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xb0, 0x5, 0x22, 0x12, 
    0x2, 0xab, 0xac, 0x7, 0x30, 0x2, 0x2, 0xac, 0xad, 0x5, 0x22, 0x12, 0x2, 
    0xad, 0xae, 0x7, 0x32, 0x2, 0x2, 0xae, 0xaf, 0x5, 0x22, 0x12, 0x2, 0xaf, 
    0xb1, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xab, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb1, 
    0x3, 0x2, 0x2, 0x2, 0xb1, 0x21, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xb7, 0x5, 
    0x24, 0x13, 0x2, 0xb3, 0xb4, 0x7, 0x1a, 0x2, 0x2, 0xb4, 0xb6, 0x5, 0x24, 
    0x13, 0x2, 0xb5, 0xb3, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb9, 0x3, 0x2, 0x2, 
    0x2, 0xb7, 0xb5, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb8, 0x3, 0x2, 0x2, 0x2, 
    0xb8, 0x23, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xba, 
    0xbf, 0x5, 0x26, 0x14, 0x2, 0xbb, 0xbc, 0x7, 0x1b, 0x2, 0x2, 0xbc, 0xbe, 
    0x5, 0x26, 0x14, 0x2, 0xbd, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xc1, 0x3, 
    0x2, 0x2, 0x2, 0xbf, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc0, 0x3, 0x2, 
    0x2, 0x2, 0xc0, 0x25, 0x3, 0x2, 0x2, 0x2, 0xc1, 0xbf, 0x3, 0x2, 0x2, 
    0x2, 0xc2, 0xc7, 0x5, 0x28, 0x15, 0x2, 0xc3, 0xc4, 0x7, 0x1c, 0x2, 0x2, 
    0xc4, 0xc6, 0x5, 0x28, 0x15, 0x2, 0xc5, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc6, 
    0xc9, 0x3, 0x2, 0x2, 0x2, 0xc7, 0xc5, 0x3, 0x2, 0x2, 0x2, 0xc7, 0xc8, 
    0x3, 0x2, 0x2, 0x2, 0xc8, 0x27, 0x3, 0x2, 0x2, 0x2, 0xc9, 0xc7, 0x3, 
    0x2, 0x2, 0x2, 0xca, 0xcf, 0x5, 0x2a, 0x16, 0x2, 0xcb, 0xcc, 0x7, 0x1d, 
    0x2, 0x2, 0xcc, 0xce, 0x5, 0x2a, 0x16, 0x2, 0xcd, 0xcb, 0x3, 0x2, 0x2, 
    0x2, 0xce, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xcd, 0x3, 0x2, 0x2, 0x2, 
    0xcf, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xd0, 0x29, 0x3, 0x2, 0x2, 0x2, 0xd1, 
    0xcf, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd5, 0x5, 0x2c, 0x17, 0x2, 0xd3, 0xd4, 
    0x9, 0x3, 0x2, 0x2, 0xd4, 0xd6, 0x5, 0x2c, 0x17, 0x2, 0xd5, 0xd3, 0x3, 
    0x2, 0x2, 0x2, 0xd5, 0xd6, 0x3, 0x2, 0x2, 0x2, 0xd6, 0x2b, 0x3, 0x2, 
    0x2, 0x2, 0xd7, 0xda, 0x5, 0x2e, 0x18, 0x2, 0xd8, 0xd9, 0x9, 0x4, 0x2, 
    0x2, 0xd9, 0xdb, 0x5, 0x2e, 0x18, 0x2, 0xda, 0xd8, 0x3, 0x2, 0x2, 0x2, 
    0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 0x2d, 0x3, 0x2, 0x2, 0x2, 0xdc, 
    0xe1, 0x5, 0x30, 0x19, 0x2, 0xdd, 0xde, 0x9, 0x5, 0x2, 0x2, 0xde, 0xe0, 
    0x5, 0x30, 0x19, 0x2, 0xdf, 0xdd, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe3, 0x3, 
    0x2, 0x2, 0x2, 0xe1, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe2, 0x3, 0x2, 
    0x2, 0x2, 0xe2, 0x2f, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe1, 0x3, 0x2, 0x2, 
    0x2, 0xe4, 0xe9, 0x5, 0x32, 0x1a, 0x2, 0xe5, 0xe6, 0x9, 0x6, 0x2, 0x2, 
    0xe6, 0xe8, 0x5, 0x32, 0x1a, 0x2, 0xe7, 0xe5, 0x3, 0x2, 0x2, 0x2, 0xe8, 
    0xeb, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xea, 
    0x3, 0x2, 0x2, 0x2, 0xea, 0x31, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xe9, 0x3, 
    0x2, 0x2, 0x2, 0xec, 0xee, 0x9, 0x7, 0x2, 0x2, 0xed, 0xec, 0x3, 0x2, 
    0x2, 0x2, 0xed, 0xee, 0x3, 0x2, 0x2, 0x2, 0xee, 0xef, 0x3, 0x2, 0x2, 
    0x2, 0xef, 0xf0, 0x5, 0x34, 0x1b, 0x2, 0xf0, 0x33, 0x3, 0x2, 0x2, 0x2, 
    0xf1, 0xf3, 0x5, 0x36, 0x1c, 0x2, 0xf2, 0xf4, 0x9, 0x8, 0x2, 0x2, 0xf3, 
    0xf2, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf4, 0x3, 0x2, 0x2, 0x2, 0xf4, 0x35, 
    0x3, 0x2, 0x2, 0x2, 0xf5, 0xfb, 0x5, 0x38, 0x1d, 0x2, 0xf6, 0xf7, 0x7, 
    0x16, 0x2, 0x2, 0xf7, 0xf8, 0x5, 0x1e, 0x10, 0x2, 0xf8, 0xf9, 0x7, 0x17, 
    0x2, 0x2, 0xf9, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xf5, 0x3, 0x2, 0x2, 
    0x2, 0xfa, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xfb, 0x37, 0x3, 0x2, 0x2, 0x2, 
    0xfc, 0x103, 0x7, 0x10, 0x2, 0x2, 0xfd, 0x103, 0x7, 0x11, 0x2, 0x2, 
    0xfe, 0x103, 0x7, 0xe, 0x2, 0x2, 0xff, 0x103, 0x7, 0xf, 0x2, 0x2, 0x100, 
    0x103, 0x7, 0xd, 0x2, 0x2, 0x101, 0x103, 0x5, 0x16, 0xc, 0x2, 0x102, 
    0xfc, 0x3, 0x2, 0x2, 0x2, 0x102, 0xfd, 0x3, 0x2, 0x2, 0x2, 0x102, 0xfe, 
    0x3, 0x2, 0x2, 0x2, 0x102, 0xff, 0x3, 0x2, 0x2, 0x2, 0x102, 0x100, 0x3, 
    0x2, 0x2, 0x2, 0x102, 0x101, 0x3, 0x2, 0x2, 0x2, 0x103, 0x39, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0x3d, 0x3f, 0x71, 0x73, 0x78, 0x7d, 0x81, 0x8a, 0x8f, 
    0xa3, 0xa6, 0xb0, 0xb7, 0xbf, 0xc7, 0xcf, 0xd5, 0xda, 0xe1, 0xe9, 0xed, 
    0xf3, 0xfa, 0x102, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

SpiceParser::Initializer SpiceParser::_init;

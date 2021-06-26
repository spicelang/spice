
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

std::vector<SpiceParser::Function_defContext *> SpiceParser::EntryContext::function_def() {
  return getRuleContexts<SpiceParser::Function_defContext>();
}

SpiceParser::Function_defContext* SpiceParser::EntryContext::function_def(size_t i) {
  return getRuleContext<SpiceParser::Function_defContext>(i);
}

std::vector<SpiceParser::Procedure_defContext *> SpiceParser::EntryContext::procedure_def() {
  return getRuleContexts<SpiceParser::Procedure_defContext>();
}

SpiceParser::Procedure_defContext* SpiceParser::EntryContext::procedure_def(size_t i) {
  return getRuleContext<SpiceParser::Procedure_defContext>(i);
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
    setState(63);
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
      setState(61);
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
          setState(58);
          stmt();
          break;
        }

        case SpiceParser::F: {
          setState(59);
          function_def();
          break;
        }

        case SpiceParser::P: {
          setState(60);
          procedure_def();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(65);
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

//----------------- Function_defContext ------------------------------------------------------------------

SpiceParser::Function_defContext::Function_defContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::Function_defContext::F() {
  return getToken(SpiceParser::F, 0);
}

tree::TerminalNode* SpiceParser::Function_defContext::LESS() {
  return getToken(SpiceParser::LESS, 0);
}

tree::TerminalNode* SpiceParser::Function_defContext::DATA_TYPE() {
  return getToken(SpiceParser::DATA_TYPE, 0);
}

tree::TerminalNode* SpiceParser::Function_defContext::GREATER() {
  return getToken(SpiceParser::GREATER, 0);
}

tree::TerminalNode* SpiceParser::Function_defContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::Function_defContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::Param_lstContext* SpiceParser::Function_defContext::param_lst() {
  return getRuleContext<SpiceParser::Param_lstContext>(0);
}

tree::TerminalNode* SpiceParser::Function_defContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}

tree::TerminalNode* SpiceParser::Function_defContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::Stmt_lstContext* SpiceParser::Function_defContext::stmt_lst() {
  return getRuleContext<SpiceParser::Stmt_lstContext>(0);
}

tree::TerminalNode* SpiceParser::Function_defContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::Function_defContext::getRuleIndex() const {
  return SpiceParser::RuleFunction_def;
}


antlrcpp::Any SpiceParser::Function_defContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitFunction_def(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Function_defContext* SpiceParser::function_def() {
  Function_defContext *_localctx = _tracker.createInstance<Function_defContext>(_ctx, getState());
  enterRule(_localctx, 2, SpiceParser::RuleFunction_def);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(66);
    match(SpiceParser::F);
    setState(67);
    match(SpiceParser::LESS);
    setState(68);
    match(SpiceParser::DATA_TYPE);
    setState(69);
    match(SpiceParser::GREATER);
    setState(70);
    match(SpiceParser::IDENTIFIER);
    setState(71);
    match(SpiceParser::LPAREN);
    setState(72);
    param_lst();
    setState(73);
    match(SpiceParser::RPAREN);
    setState(74);
    match(SpiceParser::LBRACE);
    setState(75);
    stmt_lst();
    setState(76);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Procedure_defContext ------------------------------------------------------------------

SpiceParser::Procedure_defContext::Procedure_defContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::Procedure_defContext::P() {
  return getToken(SpiceParser::P, 0);
}

tree::TerminalNode* SpiceParser::Procedure_defContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::Procedure_defContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::Param_lstContext* SpiceParser::Procedure_defContext::param_lst() {
  return getRuleContext<SpiceParser::Param_lstContext>(0);
}

tree::TerminalNode* SpiceParser::Procedure_defContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}

tree::TerminalNode* SpiceParser::Procedure_defContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::Stmt_lstContext* SpiceParser::Procedure_defContext::stmt_lst() {
  return getRuleContext<SpiceParser::Stmt_lstContext>(0);
}

tree::TerminalNode* SpiceParser::Procedure_defContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::Procedure_defContext::getRuleIndex() const {
  return SpiceParser::RuleProcedure_def;
}


antlrcpp::Any SpiceParser::Procedure_defContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitProcedure_def(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Procedure_defContext* SpiceParser::procedure_def() {
  Procedure_defContext *_localctx = _tracker.createInstance<Procedure_defContext>(_ctx, getState());
  enterRule(_localctx, 4, SpiceParser::RuleProcedure_def);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    match(SpiceParser::P);
    setState(79);
    match(SpiceParser::IDENTIFIER);
    setState(80);
    match(SpiceParser::LPAREN);
    setState(81);
    param_lst();
    setState(82);
    match(SpiceParser::RPAREN);
    setState(83);
    match(SpiceParser::LBRACE);
    setState(84);
    stmt_lst();
    setState(85);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_loopContext ------------------------------------------------------------------

SpiceParser::For_loopContext::For_loopContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::For_loopContext::FOR() {
  return getToken(SpiceParser::FOR, 0);
}

SpiceParser::AssignmentContext* SpiceParser::For_loopContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}

std::vector<tree::TerminalNode *> SpiceParser::For_loopContext::SEMICOLON() {
  return getTokens(SpiceParser::SEMICOLON);
}

tree::TerminalNode* SpiceParser::For_loopContext::SEMICOLON(size_t i) {
  return getToken(SpiceParser::SEMICOLON, i);
}

std::vector<SpiceParser::Top_lvl_exprContext *> SpiceParser::For_loopContext::top_lvl_expr() {
  return getRuleContexts<SpiceParser::Top_lvl_exprContext>();
}

SpiceParser::Top_lvl_exprContext* SpiceParser::For_loopContext::top_lvl_expr(size_t i) {
  return getRuleContext<SpiceParser::Top_lvl_exprContext>(i);
}

tree::TerminalNode* SpiceParser::For_loopContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::Stmt_lstContext* SpiceParser::For_loopContext::stmt_lst() {
  return getRuleContext<SpiceParser::Stmt_lstContext>(0);
}

tree::TerminalNode* SpiceParser::For_loopContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::For_loopContext::getRuleIndex() const {
  return SpiceParser::RuleFor_loop;
}


antlrcpp::Any SpiceParser::For_loopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitFor_loop(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::For_loopContext* SpiceParser::for_loop() {
  For_loopContext *_localctx = _tracker.createInstance<For_loopContext>(_ctx, getState());
  enterRule(_localctx, 6, SpiceParser::RuleFor_loop);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(87);
    match(SpiceParser::FOR);
    setState(88);
    assignment();
    setState(89);
    match(SpiceParser::SEMICOLON);
    setState(90);
    top_lvl_expr();
    setState(91);
    match(SpiceParser::SEMICOLON);
    setState(92);
    top_lvl_expr();
    setState(93);
    match(SpiceParser::LBRACE);
    setState(94);
    stmt_lst();
    setState(95);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- For_each_loopContext ------------------------------------------------------------------

SpiceParser::For_each_loopContext::For_each_loopContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::For_each_loopContext::FOREACH() {
  return getToken(SpiceParser::FOREACH, 0);
}

tree::TerminalNode* SpiceParser::For_each_loopContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::For_each_loopContext::COLON() {
  return getToken(SpiceParser::COLON, 0);
}

SpiceParser::ValueContext* SpiceParser::For_each_loopContext::value() {
  return getRuleContext<SpiceParser::ValueContext>(0);
}

tree::TerminalNode* SpiceParser::For_each_loopContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::Stmt_lstContext* SpiceParser::For_each_loopContext::stmt_lst() {
  return getRuleContext<SpiceParser::Stmt_lstContext>(0);
}

tree::TerminalNode* SpiceParser::For_each_loopContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::For_each_loopContext::getRuleIndex() const {
  return SpiceParser::RuleFor_each_loop;
}


antlrcpp::Any SpiceParser::For_each_loopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitFor_each_loop(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::For_each_loopContext* SpiceParser::for_each_loop() {
  For_each_loopContext *_localctx = _tracker.createInstance<For_each_loopContext>(_ctx, getState());
  enterRule(_localctx, 8, SpiceParser::RuleFor_each_loop);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(97);
    match(SpiceParser::FOREACH);
    setState(98);
    match(SpiceParser::IDENTIFIER);
    setState(99);
    match(SpiceParser::COLON);
    setState(100);
    value();
    setState(101);
    match(SpiceParser::LBRACE);
    setState(102);
    stmt_lst();
    setState(103);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- While_loopContext ------------------------------------------------------------------

SpiceParser::While_loopContext::While_loopContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::While_loopContext::WHILE() {
  return getToken(SpiceParser::WHILE, 0);
}

SpiceParser::Top_lvl_exprContext* SpiceParser::While_loopContext::top_lvl_expr() {
  return getRuleContext<SpiceParser::Top_lvl_exprContext>(0);
}

tree::TerminalNode* SpiceParser::While_loopContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::Stmt_lstContext* SpiceParser::While_loopContext::stmt_lst() {
  return getRuleContext<SpiceParser::Stmt_lstContext>(0);
}

tree::TerminalNode* SpiceParser::While_loopContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::While_loopContext::getRuleIndex() const {
  return SpiceParser::RuleWhile_loop;
}


antlrcpp::Any SpiceParser::While_loopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitWhile_loop(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::While_loopContext* SpiceParser::while_loop() {
  While_loopContext *_localctx = _tracker.createInstance<While_loopContext>(_ctx, getState());
  enterRule(_localctx, 10, SpiceParser::RuleWhile_loop);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(105);
    match(SpiceParser::WHILE);
    setState(106);
    top_lvl_expr();
    setState(107);
    match(SpiceParser::LBRACE);
    setState(108);
    stmt_lst();
    setState(109);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- If_stmtContext ------------------------------------------------------------------

SpiceParser::If_stmtContext::If_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::If_stmtContext::IF() {
  return getToken(SpiceParser::IF, 0);
}

SpiceParser::Top_lvl_exprContext* SpiceParser::If_stmtContext::top_lvl_expr() {
  return getRuleContext<SpiceParser::Top_lvl_exprContext>(0);
}

tree::TerminalNode* SpiceParser::If_stmtContext::LBRACE() {
  return getToken(SpiceParser::LBRACE, 0);
}

SpiceParser::Stmt_lstContext* SpiceParser::If_stmtContext::stmt_lst() {
  return getRuleContext<SpiceParser::Stmt_lstContext>(0);
}

tree::TerminalNode* SpiceParser::If_stmtContext::RBRACE() {
  return getToken(SpiceParser::RBRACE, 0);
}


size_t SpiceParser::If_stmtContext::getRuleIndex() const {
  return SpiceParser::RuleIf_stmt;
}


antlrcpp::Any SpiceParser::If_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitIf_stmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::If_stmtContext* SpiceParser::if_stmt() {
  If_stmtContext *_localctx = _tracker.createInstance<If_stmtContext>(_ctx, getState());
  enterRule(_localctx, 12, SpiceParser::RuleIf_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(111);
    match(SpiceParser::IF);
    setState(112);
    top_lvl_expr();
    setState(113);
    match(SpiceParser::LBRACE);
    setState(114);
    stmt_lst();
    setState(115);
    match(SpiceParser::RBRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Stmt_lstContext ------------------------------------------------------------------

SpiceParser::Stmt_lstContext::Stmt_lstContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::StmtContext *> SpiceParser::Stmt_lstContext::stmt() {
  return getRuleContexts<SpiceParser::StmtContext>();
}

SpiceParser::StmtContext* SpiceParser::Stmt_lstContext::stmt(size_t i) {
  return getRuleContext<SpiceParser::StmtContext>(i);
}

std::vector<SpiceParser::For_loopContext *> SpiceParser::Stmt_lstContext::for_loop() {
  return getRuleContexts<SpiceParser::For_loopContext>();
}

SpiceParser::For_loopContext* SpiceParser::Stmt_lstContext::for_loop(size_t i) {
  return getRuleContext<SpiceParser::For_loopContext>(i);
}

std::vector<SpiceParser::For_each_loopContext *> SpiceParser::Stmt_lstContext::for_each_loop() {
  return getRuleContexts<SpiceParser::For_each_loopContext>();
}

SpiceParser::For_each_loopContext* SpiceParser::Stmt_lstContext::for_each_loop(size_t i) {
  return getRuleContext<SpiceParser::For_each_loopContext>(i);
}

std::vector<SpiceParser::While_loopContext *> SpiceParser::Stmt_lstContext::while_loop() {
  return getRuleContexts<SpiceParser::While_loopContext>();
}

SpiceParser::While_loopContext* SpiceParser::Stmt_lstContext::while_loop(size_t i) {
  return getRuleContext<SpiceParser::While_loopContext>(i);
}

std::vector<SpiceParser::If_stmtContext *> SpiceParser::Stmt_lstContext::if_stmt() {
  return getRuleContexts<SpiceParser::If_stmtContext>();
}

SpiceParser::If_stmtContext* SpiceParser::Stmt_lstContext::if_stmt(size_t i) {
  return getRuleContext<SpiceParser::If_stmtContext>(i);
}


size_t SpiceParser::Stmt_lstContext::getRuleIndex() const {
  return SpiceParser::RuleStmt_lst;
}


antlrcpp::Any SpiceParser::Stmt_lstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitStmt_lst(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Stmt_lstContext* SpiceParser::stmt_lst() {
  Stmt_lstContext *_localctx = _tracker.createInstance<Stmt_lstContext>(_ctx, getState());
  enterRule(_localctx, 14, SpiceParser::RuleStmt_lst);
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
    setState(124);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::DATA_TYPE)
      | (1ULL << SpiceParser::IF)
      | (1ULL << SpiceParser::FOR)
      | (1ULL << SpiceParser::FOREACH)
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
      setState(122);
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
          setState(117);
          stmt();
          break;
        }

        case SpiceParser::FOR: {
          setState(118);
          for_loop();
          break;
        }

        case SpiceParser::FOREACH: {
          setState(119);
          for_each_loop();
          break;
        }

        case SpiceParser::WHILE: {
          setState(120);
          while_loop();
          break;
        }

        case SpiceParser::IF: {
          setState(121);
          if_stmt();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(126);
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

//----------------- Param_lstContext ------------------------------------------------------------------

SpiceParser::Param_lstContext::Param_lstContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Decl_stmtContext *> SpiceParser::Param_lstContext::decl_stmt() {
  return getRuleContexts<SpiceParser::Decl_stmtContext>();
}

SpiceParser::Decl_stmtContext* SpiceParser::Param_lstContext::decl_stmt(size_t i) {
  return getRuleContext<SpiceParser::Decl_stmtContext>(i);
}

std::vector<SpiceParser::AssignmentContext *> SpiceParser::Param_lstContext::assignment() {
  return getRuleContexts<SpiceParser::AssignmentContext>();
}

SpiceParser::AssignmentContext* SpiceParser::Param_lstContext::assignment(size_t i) {
  return getRuleContext<SpiceParser::AssignmentContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Param_lstContext::COMMA() {
  return getTokens(SpiceParser::COMMA);
}

tree::TerminalNode* SpiceParser::Param_lstContext::COMMA(size_t i) {
  return getToken(SpiceParser::COMMA, i);
}


size_t SpiceParser::Param_lstContext::getRuleIndex() const {
  return SpiceParser::RuleParam_lst;
}


antlrcpp::Any SpiceParser::Param_lstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitParam_lst(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Param_lstContext* SpiceParser::param_lst() {
  Param_lstContext *_localctx = _tracker.createInstance<Param_lstContext>(_ctx, getState());
  enterRule(_localctx, 16, SpiceParser::RuleParam_lst);
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
    setState(129);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      setState(127);
      decl_stmt();
      break;
    }

    case 2: {
      setState(128);
      assignment();
      break;
    }

    default:
      break;
    }
    setState(138);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::COMMA) {
      setState(131);
      match(SpiceParser::COMMA);
      setState(134);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
      case 1: {
        setState(132);
        decl_stmt();
        break;
      }

      case 2: {
        setState(133);
        assignment();
        break;
      }

      default:
        break;
      }
      setState(140);
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

SpiceParser::Decl_stmtContext* SpiceParser::StmtContext::decl_stmt() {
  return getRuleContext<SpiceParser::Decl_stmtContext>(0);
}

SpiceParser::AssignmentContext* SpiceParser::StmtContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}

SpiceParser::Function_callContext* SpiceParser::StmtContext::function_call() {
  return getRuleContext<SpiceParser::Function_callContext>(0);
}

SpiceParser::Top_lvl_exprContext* SpiceParser::StmtContext::top_lvl_expr() {
  return getRuleContext<SpiceParser::Top_lvl_exprContext>(0);
}

SpiceParser::Import_stmtContext* SpiceParser::StmtContext::import_stmt() {
  return getRuleContext<SpiceParser::Import_stmtContext>(0);
}

SpiceParser::Return_stmtContext* SpiceParser::StmtContext::return_stmt() {
  return getRuleContext<SpiceParser::Return_stmtContext>(0);
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
  enterRule(_localctx, 18, SpiceParser::RuleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(147);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      setState(141);
      decl_stmt();
      break;
    }

    case 2: {
      setState(142);
      assignment();
      break;
    }

    case 3: {
      setState(143);
      function_call();
      break;
    }

    case 4: {
      setState(144);
      top_lvl_expr();
      break;
    }

    case 5: {
      setState(145);
      import_stmt();
      break;
    }

    case 6: {
      setState(146);
      return_stmt();
      break;
    }

    default:
      break;
    }
    setState(149);
    match(SpiceParser::SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Decl_stmtContext ------------------------------------------------------------------

SpiceParser::Decl_stmtContext::Decl_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::Decl_stmtContext::DATA_TYPE() {
  return getToken(SpiceParser::DATA_TYPE, 0);
}

tree::TerminalNode* SpiceParser::Decl_stmtContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::Decl_stmtContext::CONST() {
  return getToken(SpiceParser::CONST, 0);
}


size_t SpiceParser::Decl_stmtContext::getRuleIndex() const {
  return SpiceParser::RuleDecl_stmt;
}


antlrcpp::Any SpiceParser::Decl_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitDecl_stmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Decl_stmtContext* SpiceParser::decl_stmt() {
  Decl_stmtContext *_localctx = _tracker.createInstance<Decl_stmtContext>(_ctx, getState());
  enterRule(_localctx, 20, SpiceParser::RuleDecl_stmt);
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
    setState(152);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::CONST) {
      setState(151);
      match(SpiceParser::CONST);
    }
    setState(154);
    match(SpiceParser::DATA_TYPE);
    setState(155);
    match(SpiceParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Function_callContext ------------------------------------------------------------------

SpiceParser::Function_callContext::Function_callContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::Function_callContext::IDENTIFIER() {
  return getToken(SpiceParser::IDENTIFIER, 0);
}

tree::TerminalNode* SpiceParser::Function_callContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::Param_lstContext* SpiceParser::Function_callContext::param_lst() {
  return getRuleContext<SpiceParser::Param_lstContext>(0);
}

tree::TerminalNode* SpiceParser::Function_callContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}


size_t SpiceParser::Function_callContext::getRuleIndex() const {
  return SpiceParser::RuleFunction_call;
}


antlrcpp::Any SpiceParser::Function_callContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitFunction_call(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Function_callContext* SpiceParser::function_call() {
  Function_callContext *_localctx = _tracker.createInstance<Function_callContext>(_ctx, getState());
  enterRule(_localctx, 22, SpiceParser::RuleFunction_call);

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
    match(SpiceParser::IDENTIFIER);
    setState(158);
    match(SpiceParser::LPAREN);
    setState(159);
    param_lst();
    setState(160);
    match(SpiceParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Import_stmtContext ------------------------------------------------------------------

SpiceParser::Import_stmtContext::Import_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::Import_stmtContext::IMPORT() {
  return getToken(SpiceParser::IMPORT, 0);
}

tree::TerminalNode* SpiceParser::Import_stmtContext::STRING() {
  return getToken(SpiceParser::STRING, 0);
}


size_t SpiceParser::Import_stmtContext::getRuleIndex() const {
  return SpiceParser::RuleImport_stmt;
}


antlrcpp::Any SpiceParser::Import_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitImport_stmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Import_stmtContext* SpiceParser::import_stmt() {
  Import_stmtContext *_localctx = _tracker.createInstance<Import_stmtContext>(_ctx, getState());
  enterRule(_localctx, 24, SpiceParser::RuleImport_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(162);
    match(SpiceParser::IMPORT);
    setState(163);
    match(SpiceParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

SpiceParser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SpiceParser::Return_stmtContext::RETURN() {
  return getToken(SpiceParser::RETURN, 0);
}

SpiceParser::Top_lvl_exprContext* SpiceParser::Return_stmtContext::top_lvl_expr() {
  return getRuleContext<SpiceParser::Top_lvl_exprContext>(0);
}


size_t SpiceParser::Return_stmtContext::getRuleIndex() const {
  return SpiceParser::RuleReturn_stmt;
}


antlrcpp::Any SpiceParser::Return_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitReturn_stmt(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Return_stmtContext* SpiceParser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 26, SpiceParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(165);
    match(SpiceParser::RETURN);
    setState(166);
    top_lvl_expr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Top_lvl_exprContext ------------------------------------------------------------------

SpiceParser::Top_lvl_exprContext::Top_lvl_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::AssignmentContext* SpiceParser::Top_lvl_exprContext::assignment() {
  return getRuleContext<SpiceParser::AssignmentContext>(0);
}


size_t SpiceParser::Top_lvl_exprContext::getRuleIndex() const {
  return SpiceParser::RuleTop_lvl_expr;
}


antlrcpp::Any SpiceParser::Top_lvl_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitTop_lvl_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Top_lvl_exprContext* SpiceParser::top_lvl_expr() {
  Top_lvl_exprContext *_localctx = _tracker.createInstance<Top_lvl_exprContext>(_ctx, getState());
  enterRule(_localctx, 28, SpiceParser::RuleTop_lvl_expr);

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

SpiceParser::Decl_stmtContext* SpiceParser::AssignmentContext::decl_stmt() {
  return getRuleContext<SpiceParser::Decl_stmtContext>(0);
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
  enterRule(_localctx, 30, SpiceParser::RuleAssignment);
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
    setState(175);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 10, _ctx)) {
    case 1: {
      setState(172);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SpiceParser::DATA_TYPE:
        case SpiceParser::CONST: {
          setState(170);
          decl_stmt();
          break;
        }

        case SpiceParser::IDENTIFIER: {
          setState(171);
          match(SpiceParser::IDENTIFIER);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(174);
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
    setState(177);
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

std::vector<SpiceParser::Logical_or_exprContext *> SpiceParser::TernaryContext::logical_or_expr() {
  return getRuleContexts<SpiceParser::Logical_or_exprContext>();
}

SpiceParser::Logical_or_exprContext* SpiceParser::TernaryContext::logical_or_expr(size_t i) {
  return getRuleContext<SpiceParser::Logical_or_exprContext>(i);
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
  enterRule(_localctx, 32, SpiceParser::RuleTernary);
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
    setState(179);
    logical_or_expr();
    setState(185);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::QUESTION_MARK) {
      setState(180);
      match(SpiceParser::QUESTION_MARK);
      setState(181);
      logical_or_expr();
      setState(182);
      match(SpiceParser::COLON);
      setState(183);
      logical_or_expr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Logical_or_exprContext ------------------------------------------------------------------

SpiceParser::Logical_or_exprContext::Logical_or_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Logical_and_exprContext *> SpiceParser::Logical_or_exprContext::logical_and_expr() {
  return getRuleContexts<SpiceParser::Logical_and_exprContext>();
}

SpiceParser::Logical_and_exprContext* SpiceParser::Logical_or_exprContext::logical_and_expr(size_t i) {
  return getRuleContext<SpiceParser::Logical_and_exprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Logical_or_exprContext::LOGICAL_OR() {
  return getTokens(SpiceParser::LOGICAL_OR);
}

tree::TerminalNode* SpiceParser::Logical_or_exprContext::LOGICAL_OR(size_t i) {
  return getToken(SpiceParser::LOGICAL_OR, i);
}


size_t SpiceParser::Logical_or_exprContext::getRuleIndex() const {
  return SpiceParser::RuleLogical_or_expr;
}


antlrcpp::Any SpiceParser::Logical_or_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitLogical_or_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Logical_or_exprContext* SpiceParser::logical_or_expr() {
  Logical_or_exprContext *_localctx = _tracker.createInstance<Logical_or_exprContext>(_ctx, getState());
  enterRule(_localctx, 34, SpiceParser::RuleLogical_or_expr);
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
    setState(187);
    logical_and_expr();
    setState(192);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::LOGICAL_OR) {
      setState(188);
      match(SpiceParser::LOGICAL_OR);
      setState(189);
      logical_and_expr();
      setState(194);
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

//----------------- Logical_and_exprContext ------------------------------------------------------------------

SpiceParser::Logical_and_exprContext::Logical_and_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Bitwise_or_exprContext *> SpiceParser::Logical_and_exprContext::bitwise_or_expr() {
  return getRuleContexts<SpiceParser::Bitwise_or_exprContext>();
}

SpiceParser::Bitwise_or_exprContext* SpiceParser::Logical_and_exprContext::bitwise_or_expr(size_t i) {
  return getRuleContext<SpiceParser::Bitwise_or_exprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Logical_and_exprContext::LOGICAL_AND() {
  return getTokens(SpiceParser::LOGICAL_AND);
}

tree::TerminalNode* SpiceParser::Logical_and_exprContext::LOGICAL_AND(size_t i) {
  return getToken(SpiceParser::LOGICAL_AND, i);
}


size_t SpiceParser::Logical_and_exprContext::getRuleIndex() const {
  return SpiceParser::RuleLogical_and_expr;
}


antlrcpp::Any SpiceParser::Logical_and_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitLogical_and_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Logical_and_exprContext* SpiceParser::logical_and_expr() {
  Logical_and_exprContext *_localctx = _tracker.createInstance<Logical_and_exprContext>(_ctx, getState());
  enterRule(_localctx, 36, SpiceParser::RuleLogical_and_expr);
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
    setState(195);
    bitwise_or_expr();
    setState(200);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::LOGICAL_AND) {
      setState(196);
      match(SpiceParser::LOGICAL_AND);
      setState(197);
      bitwise_or_expr();
      setState(202);
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

//----------------- Bitwise_or_exprContext ------------------------------------------------------------------

SpiceParser::Bitwise_or_exprContext::Bitwise_or_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Bitwise_and_exprContext *> SpiceParser::Bitwise_or_exprContext::bitwise_and_expr() {
  return getRuleContexts<SpiceParser::Bitwise_and_exprContext>();
}

SpiceParser::Bitwise_and_exprContext* SpiceParser::Bitwise_or_exprContext::bitwise_and_expr(size_t i) {
  return getRuleContext<SpiceParser::Bitwise_and_exprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Bitwise_or_exprContext::BITWISE_OR() {
  return getTokens(SpiceParser::BITWISE_OR);
}

tree::TerminalNode* SpiceParser::Bitwise_or_exprContext::BITWISE_OR(size_t i) {
  return getToken(SpiceParser::BITWISE_OR, i);
}


size_t SpiceParser::Bitwise_or_exprContext::getRuleIndex() const {
  return SpiceParser::RuleBitwise_or_expr;
}


antlrcpp::Any SpiceParser::Bitwise_or_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitBitwise_or_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Bitwise_or_exprContext* SpiceParser::bitwise_or_expr() {
  Bitwise_or_exprContext *_localctx = _tracker.createInstance<Bitwise_or_exprContext>(_ctx, getState());
  enterRule(_localctx, 38, SpiceParser::RuleBitwise_or_expr);
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
    setState(203);
    bitwise_and_expr();
    setState(208);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::BITWISE_OR) {
      setState(204);
      match(SpiceParser::BITWISE_OR);
      setState(205);
      bitwise_and_expr();
      setState(210);
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

//----------------- Bitwise_and_exprContext ------------------------------------------------------------------

SpiceParser::Bitwise_and_exprContext::Bitwise_and_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::EqualityContext *> SpiceParser::Bitwise_and_exprContext::equality() {
  return getRuleContexts<SpiceParser::EqualityContext>();
}

SpiceParser::EqualityContext* SpiceParser::Bitwise_and_exprContext::equality(size_t i) {
  return getRuleContext<SpiceParser::EqualityContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Bitwise_and_exprContext::BITWISE_AND() {
  return getTokens(SpiceParser::BITWISE_AND);
}

tree::TerminalNode* SpiceParser::Bitwise_and_exprContext::BITWISE_AND(size_t i) {
  return getToken(SpiceParser::BITWISE_AND, i);
}


size_t SpiceParser::Bitwise_and_exprContext::getRuleIndex() const {
  return SpiceParser::RuleBitwise_and_expr;
}


antlrcpp::Any SpiceParser::Bitwise_and_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitBitwise_and_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Bitwise_and_exprContext* SpiceParser::bitwise_and_expr() {
  Bitwise_and_exprContext *_localctx = _tracker.createInstance<Bitwise_and_exprContext>(_ctx, getState());
  enterRule(_localctx, 40, SpiceParser::RuleBitwise_and_expr);
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
    setState(211);
    equality();
    setState(216);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::BITWISE_AND) {
      setState(212);
      match(SpiceParser::BITWISE_AND);
      setState(213);
      equality();
      setState(218);
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

std::vector<SpiceParser::Relational_exprContext *> SpiceParser::EqualityContext::relational_expr() {
  return getRuleContexts<SpiceParser::Relational_exprContext>();
}

SpiceParser::Relational_exprContext* SpiceParser::EqualityContext::relational_expr(size_t i) {
  return getRuleContext<SpiceParser::Relational_exprContext>(i);
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
  enterRule(_localctx, 42, SpiceParser::RuleEquality);
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
    setState(219);
    relational_expr();
    setState(222);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::EQUAL

    || _la == SpiceParser::NOT_EQUAL) {
      setState(220);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::EQUAL

      || _la == SpiceParser::NOT_EQUAL)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(221);
      relational_expr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Relational_exprContext ------------------------------------------------------------------

SpiceParser::Relational_exprContext::Relational_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Additive_exprContext *> SpiceParser::Relational_exprContext::additive_expr() {
  return getRuleContexts<SpiceParser::Additive_exprContext>();
}

SpiceParser::Additive_exprContext* SpiceParser::Relational_exprContext::additive_expr(size_t i) {
  return getRuleContext<SpiceParser::Additive_exprContext>(i);
}

tree::TerminalNode* SpiceParser::Relational_exprContext::LESS() {
  return getToken(SpiceParser::LESS, 0);
}

tree::TerminalNode* SpiceParser::Relational_exprContext::GREATER() {
  return getToken(SpiceParser::GREATER, 0);
}

tree::TerminalNode* SpiceParser::Relational_exprContext::LESS_EQUAL() {
  return getToken(SpiceParser::LESS_EQUAL, 0);
}

tree::TerminalNode* SpiceParser::Relational_exprContext::GREATER_EQUAL() {
  return getToken(SpiceParser::GREATER_EQUAL, 0);
}


size_t SpiceParser::Relational_exprContext::getRuleIndex() const {
  return SpiceParser::RuleRelational_expr;
}


antlrcpp::Any SpiceParser::Relational_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitRelational_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Relational_exprContext* SpiceParser::relational_expr() {
  Relational_exprContext *_localctx = _tracker.createInstance<Relational_exprContext>(_ctx, getState());
  enterRule(_localctx, 44, SpiceParser::RuleRelational_expr);
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
    setState(224);
    additive_expr();
    setState(227);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::GREATER)
      | (1ULL << SpiceParser::LESS)
      | (1ULL << SpiceParser::GREATER_EQUAL)
      | (1ULL << SpiceParser::LESS_EQUAL))) != 0)) {
      setState(225);
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
      setState(226);
      additive_expr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Additive_exprContext ------------------------------------------------------------------

SpiceParser::Additive_exprContext::Additive_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Multiplicative_exprContext *> SpiceParser::Additive_exprContext::multiplicative_expr() {
  return getRuleContexts<SpiceParser::Multiplicative_exprContext>();
}

SpiceParser::Multiplicative_exprContext* SpiceParser::Additive_exprContext::multiplicative_expr(size_t i) {
  return getRuleContext<SpiceParser::Multiplicative_exprContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Additive_exprContext::PLUS() {
  return getTokens(SpiceParser::PLUS);
}

tree::TerminalNode* SpiceParser::Additive_exprContext::PLUS(size_t i) {
  return getToken(SpiceParser::PLUS, i);
}

std::vector<tree::TerminalNode *> SpiceParser::Additive_exprContext::MINUS() {
  return getTokens(SpiceParser::MINUS);
}

tree::TerminalNode* SpiceParser::Additive_exprContext::MINUS(size_t i) {
  return getToken(SpiceParser::MINUS, i);
}


size_t SpiceParser::Additive_exprContext::getRuleIndex() const {
  return SpiceParser::RuleAdditive_expr;
}


antlrcpp::Any SpiceParser::Additive_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitAdditive_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Additive_exprContext* SpiceParser::additive_expr() {
  Additive_exprContext *_localctx = _tracker.createInstance<Additive_exprContext>(_ctx, getState());
  enterRule(_localctx, 46, SpiceParser::RuleAdditive_expr);
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
    setState(229);
    multiplicative_expr();
    setState(234);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::PLUS

    || _la == SpiceParser::MINUS) {
      setState(230);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::PLUS

      || _la == SpiceParser::MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(231);
      multiplicative_expr();
      setState(236);
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

//----------------- Multiplicative_exprContext ------------------------------------------------------------------

SpiceParser::Multiplicative_exprContext::Multiplicative_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SpiceParser::Prefix_unaryContext *> SpiceParser::Multiplicative_exprContext::prefix_unary() {
  return getRuleContexts<SpiceParser::Prefix_unaryContext>();
}

SpiceParser::Prefix_unaryContext* SpiceParser::Multiplicative_exprContext::prefix_unary(size_t i) {
  return getRuleContext<SpiceParser::Prefix_unaryContext>(i);
}

std::vector<tree::TerminalNode *> SpiceParser::Multiplicative_exprContext::MUL() {
  return getTokens(SpiceParser::MUL);
}

tree::TerminalNode* SpiceParser::Multiplicative_exprContext::MUL(size_t i) {
  return getToken(SpiceParser::MUL, i);
}

std::vector<tree::TerminalNode *> SpiceParser::Multiplicative_exprContext::DIV() {
  return getTokens(SpiceParser::DIV);
}

tree::TerminalNode* SpiceParser::Multiplicative_exprContext::DIV(size_t i) {
  return getToken(SpiceParser::DIV, i);
}


size_t SpiceParser::Multiplicative_exprContext::getRuleIndex() const {
  return SpiceParser::RuleMultiplicative_expr;
}


antlrcpp::Any SpiceParser::Multiplicative_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitMultiplicative_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Multiplicative_exprContext* SpiceParser::multiplicative_expr() {
  Multiplicative_exprContext *_localctx = _tracker.createInstance<Multiplicative_exprContext>(_ctx, getState());
  enterRule(_localctx, 48, SpiceParser::RuleMultiplicative_expr);
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
    setState(237);
    prefix_unary();
    setState(242);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SpiceParser::MUL

    || _la == SpiceParser::DIV) {
      setState(238);
      _la = _input->LA(1);
      if (!(_la == SpiceParser::MUL

      || _la == SpiceParser::DIV)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(239);
      prefix_unary();
      setState(244);
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

//----------------- Prefix_unaryContext ------------------------------------------------------------------

SpiceParser::Prefix_unaryContext::Prefix_unaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::Postfix_unaryContext* SpiceParser::Prefix_unaryContext::postfix_unary() {
  return getRuleContext<SpiceParser::Postfix_unaryContext>(0);
}

tree::TerminalNode* SpiceParser::Prefix_unaryContext::NOT() {
  return getToken(SpiceParser::NOT, 0);
}

tree::TerminalNode* SpiceParser::Prefix_unaryContext::PLUS_PLUS() {
  return getToken(SpiceParser::PLUS_PLUS, 0);
}

tree::TerminalNode* SpiceParser::Prefix_unaryContext::MINUS_MINUS() {
  return getToken(SpiceParser::MINUS_MINUS, 0);
}


size_t SpiceParser::Prefix_unaryContext::getRuleIndex() const {
  return SpiceParser::RulePrefix_unary;
}


antlrcpp::Any SpiceParser::Prefix_unaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitPrefix_unary(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Prefix_unaryContext* SpiceParser::prefix_unary() {
  Prefix_unaryContext *_localctx = _tracker.createInstance<Prefix_unaryContext>(_ctx, getState());
  enterRule(_localctx, 50, SpiceParser::RulePrefix_unary);
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
    setState(246);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << SpiceParser::NOT)
      | (1ULL << SpiceParser::PLUS_PLUS)
      | (1ULL << SpiceParser::MINUS_MINUS))) != 0)) {
      setState(245);
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
    setState(248);
    postfix_unary();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Postfix_unaryContext ------------------------------------------------------------------

SpiceParser::Postfix_unaryContext::Postfix_unaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::Atomic_exprContext* SpiceParser::Postfix_unaryContext::atomic_expr() {
  return getRuleContext<SpiceParser::Atomic_exprContext>(0);
}

tree::TerminalNode* SpiceParser::Postfix_unaryContext::PLUS_PLUS() {
  return getToken(SpiceParser::PLUS_PLUS, 0);
}

tree::TerminalNode* SpiceParser::Postfix_unaryContext::MINUS_MINUS() {
  return getToken(SpiceParser::MINUS_MINUS, 0);
}


size_t SpiceParser::Postfix_unaryContext::getRuleIndex() const {
  return SpiceParser::RulePostfix_unary;
}


antlrcpp::Any SpiceParser::Postfix_unaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitPostfix_unary(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Postfix_unaryContext* SpiceParser::postfix_unary() {
  Postfix_unaryContext *_localctx = _tracker.createInstance<Postfix_unaryContext>(_ctx, getState());
  enterRule(_localctx, 52, SpiceParser::RulePostfix_unary);
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
    setState(250);
    atomic_expr();
    setState(252);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SpiceParser::PLUS_PLUS

    || _la == SpiceParser::MINUS_MINUS) {
      setState(251);
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

//----------------- Atomic_exprContext ------------------------------------------------------------------

SpiceParser::Atomic_exprContext::Atomic_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SpiceParser::ValueContext* SpiceParser::Atomic_exprContext::value() {
  return getRuleContext<SpiceParser::ValueContext>(0);
}

tree::TerminalNode* SpiceParser::Atomic_exprContext::LPAREN() {
  return getToken(SpiceParser::LPAREN, 0);
}

SpiceParser::Additive_exprContext* SpiceParser::Atomic_exprContext::additive_expr() {
  return getRuleContext<SpiceParser::Additive_exprContext>(0);
}

tree::TerminalNode* SpiceParser::Atomic_exprContext::RPAREN() {
  return getToken(SpiceParser::RPAREN, 0);
}


size_t SpiceParser::Atomic_exprContext::getRuleIndex() const {
  return SpiceParser::RuleAtomic_expr;
}


antlrcpp::Any SpiceParser::Atomic_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SpiceVisitor*>(visitor))
    return parserVisitor->visitAtomic_expr(this);
  else
    return visitor->visitChildren(this);
}

SpiceParser::Atomic_exprContext* SpiceParser::atomic_expr() {
  Atomic_exprContext *_localctx = _tracker.createInstance<Atomic_exprContext>(_ctx, getState());
  enterRule(_localctx, 54, SpiceParser::RuleAtomic_expr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(259);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SpiceParser::IDENTIFIER:
      case SpiceParser::INTEGER:
      case SpiceParser::DOUBLE:
      case SpiceParser::STRING:
      case SpiceParser::BOOL: {
        enterOuterAlt(_localctx, 1);
        setState(254);
        value();
        break;
      }

      case SpiceParser::LPAREN: {
        enterOuterAlt(_localctx, 2);
        setState(255);
        match(SpiceParser::LPAREN);
        setState(256);
        additive_expr();
        setState(257);
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

SpiceParser::Function_callContext* SpiceParser::ValueContext::function_call() {
  return getRuleContext<SpiceParser::Function_callContext>(0);
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
  enterRule(_localctx, 56, SpiceParser::RuleValue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(267);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(261);
      match(SpiceParser::STRING);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(262);
      match(SpiceParser::BOOL);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(263);
      match(SpiceParser::INTEGER);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(264);
      match(SpiceParser::DOUBLE);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(265);
      match(SpiceParser::IDENTIFIER);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(266);
      function_call();
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
  "entry", "function_def", "procedure_def", "for_loop", "for_each_loop", 
  "while_loop", "if_stmt", "stmt_lst", "param_lst", "stmt", "decl_stmt", 
  "function_call", "import_stmt", "return_stmt", "top_lvl_expr", "assignment", 
  "ternary", "logical_or_expr", "logical_and_expr", "bitwise_or_expr", "bitwise_and_expr", 
  "equality", "relational_expr", "additive_expr", "multiplicative_expr", 
  "prefix_unary", "postfix_unary", "atomic_expr", "value"
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
    0x3, 0x36, 0x110, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x40, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 
    0x43, 0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0x9, 0x3, 0x9, 0x7, 0x9, 0x7d, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x80, 0xb, 
    0x9, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x84, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xa, 0x5, 0xa, 0x89, 0xa, 0xa, 0x7, 0xa, 0x8b, 0xa, 0xa, 0xc, 0xa, 
    0xe, 0xa, 0x8e, 0xb, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x5, 0xb, 0x96, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 
    0x5, 0xc, 0x9b, 0xa, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 
    0x5, 0x11, 0xaf, 0xa, 0x11, 0x3, 0x11, 0x5, 0x11, 0xb2, 0xa, 0x11, 0x3, 
    0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x5, 0x12, 0xbc, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x7, 0x13, 0xc1, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0xc4, 0xb, 0x13, 0x3, 
    0x14, 0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 0xc9, 0xa, 0x14, 0xc, 0x14, 0xe, 
    0x14, 0xcc, 0xb, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x7, 0x15, 0xd1, 
    0xa, 0x15, 0xc, 0x15, 0xe, 0x15, 0xd4, 0xb, 0x15, 0x3, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x7, 0x16, 0xd9, 0xa, 0x16, 0xc, 0x16, 0xe, 0x16, 0xdc, 0xb, 
    0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0xe1, 0xa, 0x17, 0x3, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0xe6, 0xa, 0x18, 0x3, 0x19, 0x3, 
    0x19, 0x3, 0x19, 0x7, 0x19, 0xeb, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0xee, 
    0xb, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x7, 0x1a, 0xf3, 0xa, 0x1a, 
    0xc, 0x1a, 0xe, 0x1a, 0xf6, 0xb, 0x1a, 0x3, 0x1b, 0x5, 0x1b, 0xf9, 0xa, 
    0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0xff, 0xa, 
    0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 
    0x106, 0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x5, 0x1e, 0x10e, 0xa, 0x1e, 0x3, 0x1e, 0x2, 0x2, 0x1f, 0x2, 
    0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 
    0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 
    0x36, 0x38, 0x3a, 0x2, 0x9, 0x4, 0x2, 0x21, 0x24, 0x2f, 0x2f, 0x3, 0x2, 
    0x2d, 0x2e, 0x3, 0x2, 0x29, 0x2c, 0x3, 0x2, 0x25, 0x26, 0x3, 0x2, 0x27, 
    0x28, 0x3, 0x2, 0x1e, 0x20, 0x3, 0x2, 0x1f, 0x20, 0x2, 0x117, 0x2, 0x41, 
    0x3, 0x2, 0x2, 0x2, 0x4, 0x44, 0x3, 0x2, 0x2, 0x2, 0x6, 0x50, 0x3, 0x2, 
    0x2, 0x2, 0x8, 0x59, 0x3, 0x2, 0x2, 0x2, 0xa, 0x63, 0x3, 0x2, 0x2, 0x2, 
    0xc, 0x6b, 0x3, 0x2, 0x2, 0x2, 0xe, 0x71, 0x3, 0x2, 0x2, 0x2, 0x10, 
    0x7e, 0x3, 0x2, 0x2, 0x2, 0x12, 0x83, 0x3, 0x2, 0x2, 0x2, 0x14, 0x95, 
    0x3, 0x2, 0x2, 0x2, 0x16, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x18, 0x9f, 0x3, 
    0x2, 0x2, 0x2, 0x1a, 0xa4, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xa7, 0x3, 0x2, 
    0x2, 0x2, 0x1e, 0xaa, 0x3, 0x2, 0x2, 0x2, 0x20, 0xb1, 0x3, 0x2, 0x2, 
    0x2, 0x22, 0xb5, 0x3, 0x2, 0x2, 0x2, 0x24, 0xbd, 0x3, 0x2, 0x2, 0x2, 
    0x26, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x28, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x2a, 
    0xd5, 0x3, 0x2, 0x2, 0x2, 0x2c, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x2e, 0xe2, 
    0x3, 0x2, 0x2, 0x2, 0x30, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x32, 0xef, 0x3, 
    0x2, 0x2, 0x2, 0x34, 0xf8, 0x3, 0x2, 0x2, 0x2, 0x36, 0xfc, 0x3, 0x2, 
    0x2, 0x2, 0x38, 0x105, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x10d, 0x3, 0x2, 0x2, 
    0x2, 0x3c, 0x40, 0x5, 0x14, 0xb, 0x2, 0x3d, 0x40, 0x5, 0x4, 0x3, 0x2, 
    0x3e, 0x40, 0x5, 0x6, 0x4, 0x2, 0x3f, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x3f, 
    0x3d, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x40, 0x43, 
    0x3, 0x2, 0x2, 0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 0x3, 
    0x2, 0x2, 0x2, 0x42, 0x3, 0x3, 0x2, 0x2, 0x2, 0x43, 0x41, 0x3, 0x2, 
    0x2, 0x2, 0x44, 0x45, 0x7, 0x4, 0x2, 0x2, 0x45, 0x46, 0x7, 0x2a, 0x2, 
    0x2, 0x46, 0x47, 0x7, 0x3, 0x2, 0x2, 0x47, 0x48, 0x7, 0x29, 0x2, 0x2, 
    0x48, 0x49, 0x7, 0xd, 0x2, 0x2, 0x49, 0x4a, 0x7, 0x16, 0x2, 0x2, 0x4a, 
    0x4b, 0x5, 0x12, 0xa, 0x2, 0x4b, 0x4c, 0x7, 0x17, 0x2, 0x2, 0x4c, 0x4d, 
    0x7, 0x14, 0x2, 0x2, 0x4d, 0x4e, 0x5, 0x10, 0x9, 0x2, 0x4e, 0x4f, 0x7, 
    0x15, 0x2, 0x2, 0x4f, 0x5, 0x3, 0x2, 0x2, 0x2, 0x50, 0x51, 0x7, 0x5, 
    0x2, 0x2, 0x51, 0x52, 0x7, 0xd, 0x2, 0x2, 0x52, 0x53, 0x7, 0x16, 0x2, 
    0x2, 0x53, 0x54, 0x5, 0x12, 0xa, 0x2, 0x54, 0x55, 0x7, 0x17, 0x2, 0x2, 
    0x55, 0x56, 0x7, 0x14, 0x2, 0x2, 0x56, 0x57, 0x5, 0x10, 0x9, 0x2, 0x57, 
    0x58, 0x7, 0x15, 0x2, 0x2, 0x58, 0x7, 0x3, 0x2, 0x2, 0x2, 0x59, 0x5a, 
    0x7, 0x7, 0x2, 0x2, 0x5a, 0x5b, 0x5, 0x20, 0x11, 0x2, 0x5b, 0x5c, 0x7, 
    0x31, 0x2, 0x2, 0x5c, 0x5d, 0x5, 0x1e, 0x10, 0x2, 0x5d, 0x5e, 0x7, 0x31, 
    0x2, 0x2, 0x5e, 0x5f, 0x5, 0x1e, 0x10, 0x2, 0x5f, 0x60, 0x7, 0x14, 0x2, 
    0x2, 0x60, 0x61, 0x5, 0x10, 0x9, 0x2, 0x61, 0x62, 0x7, 0x15, 0x2, 0x2, 
    0x62, 0x9, 0x3, 0x2, 0x2, 0x2, 0x63, 0x64, 0x7, 0x8, 0x2, 0x2, 0x64, 
    0x65, 0x7, 0xd, 0x2, 0x2, 0x65, 0x66, 0x7, 0x32, 0x2, 0x2, 0x66, 0x67, 
    0x5, 0x3a, 0x1e, 0x2, 0x67, 0x68, 0x7, 0x14, 0x2, 0x2, 0x68, 0x69, 0x5, 
    0x10, 0x9, 0x2, 0x69, 0x6a, 0x7, 0x15, 0x2, 0x2, 0x6a, 0xb, 0x3, 0x2, 
    0x2, 0x2, 0x6b, 0x6c, 0x7, 0x9, 0x2, 0x2, 0x6c, 0x6d, 0x5, 0x1e, 0x10, 
    0x2, 0x6d, 0x6e, 0x7, 0x14, 0x2, 0x2, 0x6e, 0x6f, 0x5, 0x10, 0x9, 0x2, 
    0x6f, 0x70, 0x7, 0x15, 0x2, 0x2, 0x70, 0xd, 0x3, 0x2, 0x2, 0x2, 0x71, 
    0x72, 0x7, 0x6, 0x2, 0x2, 0x72, 0x73, 0x5, 0x1e, 0x10, 0x2, 0x73, 0x74, 
    0x7, 0x14, 0x2, 0x2, 0x74, 0x75, 0x5, 0x10, 0x9, 0x2, 0x75, 0x76, 0x7, 
    0x15, 0x2, 0x2, 0x76, 0xf, 0x3, 0x2, 0x2, 0x2, 0x77, 0x7d, 0x5, 0x14, 
    0xb, 0x2, 0x78, 0x7d, 0x5, 0x8, 0x5, 0x2, 0x79, 0x7d, 0x5, 0xa, 0x6, 
    0x2, 0x7a, 0x7d, 0x5, 0xc, 0x7, 0x2, 0x7b, 0x7d, 0x5, 0xe, 0x8, 0x2, 
    0x7c, 0x77, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7c, 
    0x79, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7b, 
    0x3, 0x2, 0x2, 0x2, 0x7d, 0x80, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x7c, 0x3, 
    0x2, 0x2, 0x2, 0x7e, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x11, 0x3, 0x2, 
    0x2, 0x2, 0x80, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x81, 0x84, 0x5, 0x16, 0xc, 
    0x2, 0x82, 0x84, 0x5, 0x20, 0x11, 0x2, 0x83, 0x81, 0x3, 0x2, 0x2, 0x2, 
    0x83, 0x82, 0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 0x3, 0x2, 0x2, 0x2, 0x84, 
    0x8c, 0x3, 0x2, 0x2, 0x2, 0x85, 0x88, 0x7, 0x33, 0x2, 0x2, 0x86, 0x89, 
    0x5, 0x16, 0xc, 0x2, 0x87, 0x89, 0x5, 0x20, 0x11, 0x2, 0x88, 0x86, 0x3, 
    0x2, 0x2, 0x2, 0x88, 0x87, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8b, 0x3, 0x2, 
    0x2, 0x2, 0x8a, 0x85, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8e, 0x3, 0x2, 0x2, 
    0x2, 0x8c, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 0x3, 0x2, 0x2, 0x2, 
    0x8d, 0x13, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8f, 
    0x96, 0x5, 0x16, 0xc, 0x2, 0x90, 0x96, 0x5, 0x20, 0x11, 0x2, 0x91, 0x96, 
    0x5, 0x18, 0xd, 0x2, 0x92, 0x96, 0x5, 0x1e, 0x10, 0x2, 0x93, 0x96, 0x5, 
    0x1a, 0xe, 0x2, 0x94, 0x96, 0x5, 0x1c, 0xf, 0x2, 0x95, 0x8f, 0x3, 0x2, 
    0x2, 0x2, 0x95, 0x90, 0x3, 0x2, 0x2, 0x2, 0x95, 0x91, 0x3, 0x2, 0x2, 
    0x2, 0x95, 0x92, 0x3, 0x2, 0x2, 0x2, 0x95, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x95, 0x94, 0x3, 0x2, 0x2, 0x2, 0x96, 0x97, 0x3, 0x2, 0x2, 0x2, 0x97, 
    0x98, 0x7, 0x31, 0x2, 0x2, 0x98, 0x15, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9b, 
    0x7, 0xa, 0x2, 0x2, 0x9a, 0x99, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9b, 0x3, 
    0x2, 0x2, 0x2, 0x9b, 0x9c, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x9d, 0x7, 0x3, 
    0x2, 0x2, 0x9d, 0x9e, 0x7, 0xd, 0x2, 0x2, 0x9e, 0x17, 0x3, 0x2, 0x2, 
    0x2, 0x9f, 0xa0, 0x7, 0xd, 0x2, 0x2, 0xa0, 0xa1, 0x7, 0x16, 0x2, 0x2, 
    0xa1, 0xa2, 0x5, 0x12, 0xa, 0x2, 0xa2, 0xa3, 0x7, 0x17, 0x2, 0x2, 0xa3, 
    0x19, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x7, 0xb, 0x2, 0x2, 0xa5, 0xa6, 
    0x7, 0x10, 0x2, 0x2, 0xa6, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x7, 
    0xc, 0x2, 0x2, 0xa8, 0xa9, 0x5, 0x1e, 0x10, 0x2, 0xa9, 0x1d, 0x3, 0x2, 
    0x2, 0x2, 0xaa, 0xab, 0x5, 0x20, 0x11, 0x2, 0xab, 0x1f, 0x3, 0x2, 0x2, 
    0x2, 0xac, 0xaf, 0x5, 0x16, 0xc, 0x2, 0xad, 0xaf, 0x7, 0xd, 0x2, 0x2, 
    0xae, 0xac, 0x3, 0x2, 0x2, 0x2, 0xae, 0xad, 0x3, 0x2, 0x2, 0x2, 0xaf, 
    0xb0, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb2, 0x9, 0x2, 0x2, 0x2, 0xb1, 0xae, 
    0x3, 0x2, 0x2, 0x2, 0xb1, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xb3, 0x3, 
    0x2, 0x2, 0x2, 0xb3, 0xb4, 0x5, 0x22, 0x12, 0x2, 0xb4, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0xb5, 0xbb, 0x5, 0x24, 0x13, 0x2, 0xb6, 0xb7, 0x7, 0x30, 0x2, 
    0x2, 0xb7, 0xb8, 0x5, 0x24, 0x13, 0x2, 0xb8, 0xb9, 0x7, 0x32, 0x2, 0x2, 
    0xb9, 0xba, 0x5, 0x24, 0x13, 0x2, 0xba, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xbb, 
    0xb6, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0x3, 0x2, 0x2, 0x2, 0xbc, 0x23, 
    0x3, 0x2, 0x2, 0x2, 0xbd, 0xc2, 0x5, 0x26, 0x14, 0x2, 0xbe, 0xbf, 0x7, 
    0x1a, 0x2, 0x2, 0xbf, 0xc1, 0x5, 0x26, 0x14, 0x2, 0xc0, 0xbe, 0x3, 0x2, 
    0x2, 0x2, 0xc1, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc0, 0x3, 0x2, 0x2, 
    0x2, 0xc2, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc3, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0xc4, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xca, 0x5, 0x28, 0x15, 0x2, 0xc6, 
    0xc7, 0x7, 0x1b, 0x2, 0x2, 0xc7, 0xc9, 0x5, 0x28, 0x15, 0x2, 0xc8, 0xc6, 
    0x3, 0x2, 0x2, 0x2, 0xc9, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xca, 0xc8, 0x3, 
    0x2, 0x2, 0x2, 0xca, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xcb, 0x27, 0x3, 0x2, 
    0x2, 0x2, 0xcc, 0xca, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xd2, 0x5, 0x2a, 0x16, 
    0x2, 0xce, 0xcf, 0x7, 0x1c, 0x2, 0x2, 0xcf, 0xd1, 0x5, 0x2a, 0x16, 0x2, 
    0xd0, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd1, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd2, 
    0xd0, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x3, 0x2, 0x2, 0x2, 0xd3, 0x29, 
    0x3, 0x2, 0x2, 0x2, 0xd4, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd5, 0xda, 0x5, 
    0x2c, 0x17, 0x2, 0xd6, 0xd7, 0x7, 0x1d, 0x2, 0x2, 0xd7, 0xd9, 0x5, 0x2c, 
    0x17, 0x2, 0xd8, 0xd6, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xdc, 0x3, 0x2, 0x2, 
    0x2, 0xda, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 
    0xdb, 0x2b, 0x3, 0x2, 0x2, 0x2, 0xdc, 0xda, 0x3, 0x2, 0x2, 0x2, 0xdd, 
    0xe0, 0x5, 0x2e, 0x18, 0x2, 0xde, 0xdf, 0x9, 0x3, 0x2, 0x2, 0xdf, 0xe1, 
    0x5, 0x2e, 0x18, 0x2, 0xe0, 0xde, 0x3, 0x2, 0x2, 0x2, 0xe0, 0xe1, 0x3, 
    0x2, 0x2, 0x2, 0xe1, 0x2d, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe5, 0x5, 0x30, 
    0x19, 0x2, 0xe3, 0xe4, 0x9, 0x4, 0x2, 0x2, 0xe4, 0xe6, 0x5, 0x30, 0x19, 
    0x2, 0xe5, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe6, 0x3, 0x2, 0x2, 0x2, 
    0xe6, 0x2f, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xec, 0x5, 0x32, 0x1a, 0x2, 0xe8, 
    0xe9, 0x9, 0x5, 0x2, 0x2, 0xe9, 0xeb, 0x5, 0x32, 0x1a, 0x2, 0xea, 0xe8, 
    0x3, 0x2, 0x2, 0x2, 0xeb, 0xee, 0x3, 0x2, 0x2, 0x2, 0xec, 0xea, 0x3, 
    0x2, 0x2, 0x2, 0xec, 0xed, 0x3, 0x2, 0x2, 0x2, 0xed, 0x31, 0x3, 0x2, 
    0x2, 0x2, 0xee, 0xec, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf4, 0x5, 0x34, 0x1b, 
    0x2, 0xf0, 0xf1, 0x9, 0x6, 0x2, 0x2, 0xf1, 0xf3, 0x5, 0x34, 0x1b, 0x2, 
    0xf2, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf6, 0x3, 0x2, 0x2, 0x2, 0xf4, 
    0xf2, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf5, 0x33, 
    0x3, 0x2, 0x2, 0x2, 0xf6, 0xf4, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf9, 0x9, 
    0x7, 0x2, 0x2, 0xf8, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xf9, 0x3, 0x2, 
    0x2, 0x2, 0xf9, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xfb, 0x5, 0x36, 0x1c, 
    0x2, 0xfb, 0x35, 0x3, 0x2, 0x2, 0x2, 0xfc, 0xfe, 0x5, 0x38, 0x1d, 0x2, 
    0xfd, 0xff, 0x9, 0x8, 0x2, 0x2, 0xfe, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xfe, 
    0xff, 0x3, 0x2, 0x2, 0x2, 0xff, 0x37, 0x3, 0x2, 0x2, 0x2, 0x100, 0x106, 
    0x5, 0x3a, 0x1e, 0x2, 0x101, 0x102, 0x7, 0x16, 0x2, 0x2, 0x102, 0x103, 
    0x5, 0x30, 0x19, 0x2, 0x103, 0x104, 0x7, 0x17, 0x2, 0x2, 0x104, 0x106, 
    0x3, 0x2, 0x2, 0x2, 0x105, 0x100, 0x3, 0x2, 0x2, 0x2, 0x105, 0x101, 
    0x3, 0x2, 0x2, 0x2, 0x106, 0x39, 0x3, 0x2, 0x2, 0x2, 0x107, 0x10e, 0x7, 
    0x10, 0x2, 0x2, 0x108, 0x10e, 0x7, 0x11, 0x2, 0x2, 0x109, 0x10e, 0x7, 
    0xe, 0x2, 0x2, 0x10a, 0x10e, 0x7, 0xf, 0x2, 0x2, 0x10b, 0x10e, 0x7, 
    0xd, 0x2, 0x2, 0x10c, 0x10e, 0x5, 0x18, 0xd, 0x2, 0x10d, 0x107, 0x3, 
    0x2, 0x2, 0x2, 0x10d, 0x108, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x109, 0x3, 
    0x2, 0x2, 0x2, 0x10d, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10b, 0x3, 
    0x2, 0x2, 0x2, 0x10d, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x3b, 0x3, 0x2, 
    0x2, 0x2, 0x1a, 0x3f, 0x41, 0x7c, 0x7e, 0x83, 0x88, 0x8c, 0x95, 0x9a, 
    0xae, 0xb1, 0xbb, 0xc2, 0xca, 0xd2, 0xda, 0xe0, 0xe5, 0xec, 0xf4, 0xf8, 
    0xfe, 0x105, 0x10d, 
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

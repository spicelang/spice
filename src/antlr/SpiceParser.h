
// Generated from D:/Dokumente/JustForFun GitHub Clones/spice/src\Spice.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"




class  SpiceParser : public antlr4::Parser {
public:
  enum {
    DATA_TYPE = 1, F = 2, P = 3, IF = 4, FOR = 5, FOREACH = 6, WHILE = 7, 
    CONST = 8, IMPORT = 9, RETURN = 10, IDENTIFIER = 11, INTEGER = 12, DOUBLE = 13, 
    STRING = 14, BOOL = 15, TRUE = 16, FALSE = 17, LBRACE = 18, RBRACE = 19, 
    LPAREN = 20, RPAREN = 21, LBRACKET = 22, RBRACKET = 23, LOGICAL_OR = 24, 
    LOGICAL_AND = 25, BITWISE_OR = 26, BITWISE_AND = 27, NOT = 28, PLUS_PLUS = 29, 
    MINUS_MINUS = 30, PLUS_EQUAL = 31, MINUS_EQUAL = 32, MUL_EQUAL = 33, 
    DIV_EQUAL = 34, PLUS = 35, MINUS = 36, MUL = 37, DIV = 38, GREATER = 39, 
    LESS = 40, GREATER_EQUAL = 41, LESS_EQUAL = 42, EQUAL = 43, NOT_EQUAL = 44, 
    ASSIGN_OP = 45, QUESTION_MARK = 46, SEMICOLON = 47, COLON = 48, COMMA = 49, 
    COMMENT = 50, LINE_COMMENT = 51, WS = 52
  };

  enum {
    RuleEntry = 0, RuleFunction_def = 1, RuleProcedure_def = 2, RuleFor_loop = 3, 
    RuleFor_each_loop = 4, RuleWhile_loop = 5, RuleIf_stmt = 6, RuleStmt_lst = 7, 
    RuleParam_lst = 8, RuleStmt = 9, RuleDecl_stmt = 10, RuleFunction_call = 11, 
    RuleImport_stmt = 12, RuleReturn_stmt = 13, RuleTop_lvl_expr = 14, RuleAssignment = 15, 
    RuleTernary = 16, RuleLogical_or_expr = 17, RuleLogical_and_expr = 18, 
    RuleBitwise_or_expr = 19, RuleBitwise_and_expr = 20, RuleEquality = 21, 
    RuleRelational_expr = 22, RuleAdditive_expr = 23, RuleMultiplicative_expr = 24, 
    RulePrefix_unary = 25, RulePostfix_unary = 26, RuleAtomic_expr = 27, 
    RuleValue = 28
  };

  explicit SpiceParser(antlr4::TokenStream *input);
  ~SpiceParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class EntryContext;
  class Function_defContext;
  class Procedure_defContext;
  class For_loopContext;
  class For_each_loopContext;
  class While_loopContext;
  class If_stmtContext;
  class Stmt_lstContext;
  class Param_lstContext;
  class StmtContext;
  class Decl_stmtContext;
  class Function_callContext;
  class Import_stmtContext;
  class Return_stmtContext;
  class Top_lvl_exprContext;
  class AssignmentContext;
  class TernaryContext;
  class Logical_or_exprContext;
  class Logical_and_exprContext;
  class Bitwise_or_exprContext;
  class Bitwise_and_exprContext;
  class EqualityContext;
  class Relational_exprContext;
  class Additive_exprContext;
  class Multiplicative_exprContext;
  class Prefix_unaryContext;
  class Postfix_unaryContext;
  class Atomic_exprContext;
  class ValueContext; 

  class  EntryContext : public antlr4::ParserRuleContext {
  public:
    EntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    std::vector<Function_defContext *> function_def();
    Function_defContext* function_def(size_t i);
    std::vector<Procedure_defContext *> procedure_def();
    Procedure_defContext* procedure_def(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EntryContext* entry();

  class  Function_defContext : public antlr4::ParserRuleContext {
  public:
    Function_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *F();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *DATA_TYPE();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    Param_lstContext *param_lst();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *LBRACE();
    Stmt_lstContext *stmt_lst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_defContext* function_def();

  class  Procedure_defContext : public antlr4::ParserRuleContext {
  public:
    Procedure_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *P();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    Param_lstContext *param_lst();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *LBRACE();
    Stmt_lstContext *stmt_lst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Procedure_defContext* procedure_def();

  class  For_loopContext : public antlr4::ParserRuleContext {
  public:
    For_loopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOR();
    AssignmentContext *assignment();
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);
    std::vector<Top_lvl_exprContext *> top_lvl_expr();
    Top_lvl_exprContext* top_lvl_expr(size_t i);
    antlr4::tree::TerminalNode *LBRACE();
    Stmt_lstContext *stmt_lst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  For_loopContext* for_loop();

  class  For_each_loopContext : public antlr4::ParserRuleContext {
  public:
    For_each_loopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOREACH();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();
    antlr4::tree::TerminalNode *LBRACE();
    Stmt_lstContext *stmt_lst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  For_each_loopContext* for_each_loop();

  class  While_loopContext : public antlr4::ParserRuleContext {
  public:
    While_loopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHILE();
    Top_lvl_exprContext *top_lvl_expr();
    antlr4::tree::TerminalNode *LBRACE();
    Stmt_lstContext *stmt_lst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  While_loopContext* while_loop();

  class  If_stmtContext : public antlr4::ParserRuleContext {
  public:
    If_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IF();
    Top_lvl_exprContext *top_lvl_expr();
    antlr4::tree::TerminalNode *LBRACE();
    Stmt_lstContext *stmt_lst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  If_stmtContext* if_stmt();

  class  Stmt_lstContext : public antlr4::ParserRuleContext {
  public:
    Stmt_lstContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    std::vector<For_loopContext *> for_loop();
    For_loopContext* for_loop(size_t i);
    std::vector<For_each_loopContext *> for_each_loop();
    For_each_loopContext* for_each_loop(size_t i);
    std::vector<While_loopContext *> while_loop();
    While_loopContext* while_loop(size_t i);
    std::vector<If_stmtContext *> if_stmt();
    If_stmtContext* if_stmt(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Stmt_lstContext* stmt_lst();

  class  Param_lstContext : public antlr4::ParserRuleContext {
  public:
    Param_lstContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Decl_stmtContext *> decl_stmt();
    Decl_stmtContext* decl_stmt(size_t i);
    std::vector<AssignmentContext *> assignment();
    AssignmentContext* assignment(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Param_lstContext* param_lst();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEMICOLON();
    Decl_stmtContext *decl_stmt();
    AssignmentContext *assignment();
    Function_callContext *function_call();
    Top_lvl_exprContext *top_lvl_expr();
    Import_stmtContext *import_stmt();
    Return_stmtContext *return_stmt();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StmtContext* stmt();

  class  Decl_stmtContext : public antlr4::ParserRuleContext {
  public:
    Decl_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATA_TYPE();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *CONST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Decl_stmtContext* decl_stmt();

  class  Function_callContext : public antlr4::ParserRuleContext {
  public:
    Function_callContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    Param_lstContext *param_lst();
    antlr4::tree::TerminalNode *RPAREN();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Function_callContext* function_call();

  class  Import_stmtContext : public antlr4::ParserRuleContext {
  public:
    Import_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPORT();
    antlr4::tree::TerminalNode *STRING();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Import_stmtContext* import_stmt();

  class  Return_stmtContext : public antlr4::ParserRuleContext {
  public:
    Return_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    Top_lvl_exprContext *top_lvl_expr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Return_stmtContext* return_stmt();

  class  Top_lvl_exprContext : public antlr4::ParserRuleContext {
  public:
    Top_lvl_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssignmentContext *assignment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Top_lvl_exprContext* top_lvl_expr();

  class  AssignmentContext : public antlr4::ParserRuleContext {
  public:
    AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TernaryContext *ternary();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    antlr4::tree::TerminalNode *PLUS_EQUAL();
    antlr4::tree::TerminalNode *MINUS_EQUAL();
    antlr4::tree::TerminalNode *MUL_EQUAL();
    antlr4::tree::TerminalNode *DIV_EQUAL();
    Decl_stmtContext *decl_stmt();
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentContext* assignment();

  class  TernaryContext : public antlr4::ParserRuleContext {
  public:
    TernaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Logical_or_exprContext *> logical_or_expr();
    Logical_or_exprContext* logical_or_expr(size_t i);
    antlr4::tree::TerminalNode *QUESTION_MARK();
    antlr4::tree::TerminalNode *COLON();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TernaryContext* ternary();

  class  Logical_or_exprContext : public antlr4::ParserRuleContext {
  public:
    Logical_or_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Logical_and_exprContext *> logical_and_expr();
    Logical_and_exprContext* logical_and_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOGICAL_OR();
    antlr4::tree::TerminalNode* LOGICAL_OR(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Logical_or_exprContext* logical_or_expr();

  class  Logical_and_exprContext : public antlr4::ParserRuleContext {
  public:
    Logical_and_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Bitwise_or_exprContext *> bitwise_or_expr();
    Bitwise_or_exprContext* bitwise_or_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOGICAL_AND();
    antlr4::tree::TerminalNode* LOGICAL_AND(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Logical_and_exprContext* logical_and_expr();

  class  Bitwise_or_exprContext : public antlr4::ParserRuleContext {
  public:
    Bitwise_or_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Bitwise_and_exprContext *> bitwise_and_expr();
    Bitwise_and_exprContext* bitwise_and_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BITWISE_OR();
    antlr4::tree::TerminalNode* BITWISE_OR(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Bitwise_or_exprContext* bitwise_or_expr();

  class  Bitwise_and_exprContext : public antlr4::ParserRuleContext {
  public:
    Bitwise_and_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EqualityContext *> equality();
    EqualityContext* equality(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BITWISE_AND();
    antlr4::tree::TerminalNode* BITWISE_AND(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Bitwise_and_exprContext* bitwise_and_expr();

  class  EqualityContext : public antlr4::ParserRuleContext {
  public:
    EqualityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Relational_exprContext *> relational_expr();
    Relational_exprContext* relational_expr(size_t i);
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *NOT_EQUAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EqualityContext* equality();

  class  Relational_exprContext : public antlr4::ParserRuleContext {
  public:
    Relational_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Additive_exprContext *> additive_expr();
    Additive_exprContext* additive_expr(size_t i);
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS_EQUAL();
    antlr4::tree::TerminalNode *GREATER_EQUAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Relational_exprContext* relational_expr();

  class  Additive_exprContext : public antlr4::ParserRuleContext {
  public:
    Additive_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Multiplicative_exprContext *> multiplicative_expr();
    Multiplicative_exprContext* multiplicative_expr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Additive_exprContext* additive_expr();

  class  Multiplicative_exprContext : public antlr4::ParserRuleContext {
  public:
    Multiplicative_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Prefix_unaryContext *> prefix_unary();
    Prefix_unaryContext* prefix_unary(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MUL();
    antlr4::tree::TerminalNode* MUL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DIV();
    antlr4::tree::TerminalNode* DIV(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Multiplicative_exprContext* multiplicative_expr();

  class  Prefix_unaryContext : public antlr4::ParserRuleContext {
  public:
    Prefix_unaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Postfix_unaryContext *postfix_unary();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *PLUS_PLUS();
    antlr4::tree::TerminalNode *MINUS_MINUS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Prefix_unaryContext* prefix_unary();

  class  Postfix_unaryContext : public antlr4::ParserRuleContext {
  public:
    Postfix_unaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Atomic_exprContext *atomic_expr();
    antlr4::tree::TerminalNode *PLUS_PLUS();
    antlr4::tree::TerminalNode *MINUS_MINUS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Postfix_unaryContext* postfix_unary();

  class  Atomic_exprContext : public antlr4::ParserRuleContext {
  public:
    Atomic_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueContext *value();
    antlr4::tree::TerminalNode *LPAREN();
    Additive_exprContext *additive_expr();
    antlr4::tree::TerminalNode *RPAREN();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Atomic_exprContext* atomic_expr();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *BOOL();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *IDENTIFIER();
    Function_callContext *function_call();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};


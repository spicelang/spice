
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
    RuleEntry = 0, RuleFunctionDef = 1, RuleProcedureDef = 2, RuleForLoop = 3, 
    RuleWhileLoop = 4, RuleIfStmt = 5, RuleStmtLst = 6, RuleParamLst = 7, 
    RuleStmt = 8, RuleDeclStmt = 9, RuleFunctionCall = 10, RuleImportStmt = 11, 
    RuleReturnStmt = 12, RuleTopLvlExpr = 13, RuleAssignment = 14, RuleTernary = 15, 
    RuleLogicalOrExpr = 16, RuleLogicalAndExpr = 17, RuleBitwiseOrExpr = 18, 
    RuleBitwiseAndExpr = 19, RuleEquality = 20, RuleRelationalExpr = 21, 
    RuleAdditiveExpr = 22, RuleMultiplicativeExpr = 23, RulePrefixUnary = 24, 
    RulePostfixUnary = 25, RuleAtomicExpr = 26, RuleValue = 27
  };

  explicit SpiceParser(antlr4::TokenStream *input);
  ~SpiceParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class EntryContext;
  class FunctionDefContext;
  class ProcedureDefContext;
  class ForLoopContext;
  class WhileLoopContext;
  class IfStmtContext;
  class StmtLstContext;
  class ParamLstContext;
  class StmtContext;
  class DeclStmtContext;
  class FunctionCallContext;
  class ImportStmtContext;
  class ReturnStmtContext;
  class TopLvlExprContext;
  class AssignmentContext;
  class TernaryContext;
  class LogicalOrExprContext;
  class LogicalAndExprContext;
  class BitwiseOrExprContext;
  class BitwiseAndExprContext;
  class EqualityContext;
  class RelationalExprContext;
  class AdditiveExprContext;
  class MultiplicativeExprContext;
  class PrefixUnaryContext;
  class PostfixUnaryContext;
  class AtomicExprContext;
  class ValueContext; 

  class  EntryContext : public antlr4::ParserRuleContext {
  public:
    EntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    std::vector<FunctionDefContext *> functionDef();
    FunctionDefContext* functionDef(size_t i);
    std::vector<ProcedureDefContext *> procedureDef();
    ProcedureDefContext* procedureDef(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EntryContext* entry();

  class  FunctionDefContext : public antlr4::ParserRuleContext {
  public:
    FunctionDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *F();
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *DATA_TYPE();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    ParamLstContext *paramLst();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *LBRACE();
    StmtLstContext *stmtLst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionDefContext* functionDef();

  class  ProcedureDefContext : public antlr4::ParserRuleContext {
  public:
    ProcedureDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *P();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    ParamLstContext *paramLst();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *LBRACE();
    StmtLstContext *stmtLst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProcedureDefContext* procedureDef();

  class  ForLoopContext : public antlr4::ParserRuleContext {
  public:
    ForLoopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FOR();
    AssignmentContext *assignment();
    std::vector<antlr4::tree::TerminalNode *> SEMICOLON();
    antlr4::tree::TerminalNode* SEMICOLON(size_t i);
    std::vector<TopLvlExprContext *> topLvlExpr();
    TopLvlExprContext* topLvlExpr(size_t i);
    antlr4::tree::TerminalNode *LBRACE();
    StmtLstContext *stmtLst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForLoopContext* forLoop();

  class  WhileLoopContext : public antlr4::ParserRuleContext {
  public:
    WhileLoopContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WHILE();
    TopLvlExprContext *topLvlExpr();
    antlr4::tree::TerminalNode *LBRACE();
    StmtLstContext *stmtLst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WhileLoopContext* whileLoop();

  class  IfStmtContext : public antlr4::ParserRuleContext {
  public:
    IfStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IF();
    TopLvlExprContext *topLvlExpr();
    antlr4::tree::TerminalNode *LBRACE();
    StmtLstContext *stmtLst();
    antlr4::tree::TerminalNode *RBRACE();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IfStmtContext* ifStmt();

  class  StmtLstContext : public antlr4::ParserRuleContext {
  public:
    StmtLstContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    std::vector<ForLoopContext *> forLoop();
    ForLoopContext* forLoop(size_t i);
    std::vector<WhileLoopContext *> whileLoop();
    WhileLoopContext* whileLoop(size_t i);
    std::vector<IfStmtContext *> ifStmt();
    IfStmtContext* ifStmt(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StmtLstContext* stmtLst();

  class  ParamLstContext : public antlr4::ParserRuleContext {
  public:
    ParamLstContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclStmtContext *> declStmt();
    DeclStmtContext* declStmt(size_t i);
    std::vector<AssignmentContext *> assignment();
    AssignmentContext* assignment(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParamLstContext* paramLst();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEMICOLON();
    DeclStmtContext *declStmt();
    AssignmentContext *assignment();
    FunctionCallContext *functionCall();
    TopLvlExprContext *topLvlExpr();
    ImportStmtContext *importStmt();
    ReturnStmtContext *returnStmt();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StmtContext* stmt();

  class  DeclStmtContext : public antlr4::ParserRuleContext {
  public:
    DeclStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DATA_TYPE();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *CONST();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclStmtContext* declStmt();

  class  FunctionCallContext : public antlr4::ParserRuleContext {
  public:
    FunctionCallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *LPAREN();
    ParamLstContext *paramLst();
    antlr4::tree::TerminalNode *RPAREN();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionCallContext* functionCall();

  class  ImportStmtContext : public antlr4::ParserRuleContext {
  public:
    ImportStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPORT();
    antlr4::tree::TerminalNode *STRING();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ImportStmtContext* importStmt();

  class  ReturnStmtContext : public antlr4::ParserRuleContext {
  public:
    ReturnStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    TopLvlExprContext *topLvlExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnStmtContext* returnStmt();

  class  TopLvlExprContext : public antlr4::ParserRuleContext {
  public:
    TopLvlExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssignmentContext *assignment();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TopLvlExprContext* topLvlExpr();

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
    DeclStmtContext *declStmt();
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentContext* assignment();

  class  TernaryContext : public antlr4::ParserRuleContext {
  public:
    TernaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LogicalOrExprContext *> logicalOrExpr();
    LogicalOrExprContext* logicalOrExpr(size_t i);
    antlr4::tree::TerminalNode *QUESTION_MARK();
    antlr4::tree::TerminalNode *COLON();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TernaryContext* ternary();

  class  LogicalOrExprContext : public antlr4::ParserRuleContext {
  public:
    LogicalOrExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LogicalAndExprContext *> logicalAndExpr();
    LogicalAndExprContext* logicalAndExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOGICAL_OR();
    antlr4::tree::TerminalNode* LOGICAL_OR(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogicalOrExprContext* logicalOrExpr();

  class  LogicalAndExprContext : public antlr4::ParserRuleContext {
  public:
    LogicalAndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BitwiseOrExprContext *> bitwiseOrExpr();
    BitwiseOrExprContext* bitwiseOrExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LOGICAL_AND();
    antlr4::tree::TerminalNode* LOGICAL_AND(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogicalAndExprContext* logicalAndExpr();

  class  BitwiseOrExprContext : public antlr4::ParserRuleContext {
  public:
    BitwiseOrExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BitwiseAndExprContext *> bitwiseAndExpr();
    BitwiseAndExprContext* bitwiseAndExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BITWISE_OR();
    antlr4::tree::TerminalNode* BITWISE_OR(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BitwiseOrExprContext* bitwiseOrExpr();

  class  BitwiseAndExprContext : public antlr4::ParserRuleContext {
  public:
    BitwiseAndExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EqualityContext *> equality();
    EqualityContext* equality(size_t i);
    std::vector<antlr4::tree::TerminalNode *> BITWISE_AND();
    antlr4::tree::TerminalNode* BITWISE_AND(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BitwiseAndExprContext* bitwiseAndExpr();

  class  EqualityContext : public antlr4::ParserRuleContext {
  public:
    EqualityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RelationalExprContext *> relationalExpr();
    RelationalExprContext* relationalExpr(size_t i);
    antlr4::tree::TerminalNode *EQUAL();
    antlr4::tree::TerminalNode *NOT_EQUAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EqualityContext* equality();

  class  RelationalExprContext : public antlr4::ParserRuleContext {
  public:
    RelationalExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AdditiveExprContext *> additiveExpr();
    AdditiveExprContext* additiveExpr(size_t i);
    antlr4::tree::TerminalNode *LESS();
    antlr4::tree::TerminalNode *GREATER();
    antlr4::tree::TerminalNode *LESS_EQUAL();
    antlr4::tree::TerminalNode *GREATER_EQUAL();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelationalExprContext* relationalExpr();

  class  AdditiveExprContext : public antlr4::ParserRuleContext {
  public:
    AdditiveExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MultiplicativeExprContext *> multiplicativeExpr();
    MultiplicativeExprContext* multiplicativeExpr(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditiveExprContext* additiveExpr();

  class  MultiplicativeExprContext : public antlr4::ParserRuleContext {
  public:
    MultiplicativeExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PrefixUnaryContext *> prefixUnary();
    PrefixUnaryContext* prefixUnary(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MUL();
    antlr4::tree::TerminalNode* MUL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DIV();
    antlr4::tree::TerminalNode* DIV(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MultiplicativeExprContext* multiplicativeExpr();

  class  PrefixUnaryContext : public antlr4::ParserRuleContext {
  public:
    PrefixUnaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PostfixUnaryContext *postfixUnary();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *PLUS_PLUS();
    antlr4::tree::TerminalNode *MINUS_MINUS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrefixUnaryContext* prefixUnary();

  class  PostfixUnaryContext : public antlr4::ParserRuleContext {
  public:
    PostfixUnaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AtomicExprContext *atomicExpr();
    antlr4::tree::TerminalNode *PLUS_PLUS();
    antlr4::tree::TerminalNode *MINUS_MINUS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PostfixUnaryContext* postfixUnary();

  class  AtomicExprContext : public antlr4::ParserRuleContext {
  public:
    AtomicExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ValueContext *value();
    antlr4::tree::TerminalNode *LPAREN();
    AssignmentContext *assignment();
    antlr4::tree::TerminalNode *RPAREN();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomicExprContext* atomicExpr();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *BOOL();
    antlr4::tree::TerminalNode *INTEGER();
    antlr4::tree::TerminalNode *DOUBLE();
    antlr4::tree::TerminalNode *IDENTIFIER();
    FunctionCallContext *functionCall();


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


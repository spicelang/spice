
// Generated from D:/Dokumente/JustForFun GitHub Clones/spice/src\Spice.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"




class  SpiceLexer : public antlr4::Lexer {
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

  explicit SpiceLexer(antlr4::CharStream *input);
  ~SpiceLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};


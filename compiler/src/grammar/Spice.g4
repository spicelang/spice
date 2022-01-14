// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

grammar Spice;

// Control structures
entry: (mainFunctionDef | functionDef | procedureDef | structDef | globalVarDef | importStmt | extDecl)*;
mainFunctionDef: F LESS TYPE_INT GREATER MAIN LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
functionDef: F LESS dataType GREATER (IDENTIFIER DOT)? IDENTIFIER LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
procedureDef: P (IDENTIFIER DOT)? IDENTIFIER LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
extDecl: EXT (LESS dataType GREATER)? IDENTIFIER LPAREN typeLst? RPAREN DLL? SEMICOLON;
structDef: TYPE IDENTIFIER STRUCT LBRACE fieldLst RBRACE;
globalVarDef: declSpecifiers? dataType IDENTIFIER (ASSIGN value)? SEMICOLON;
forLoop: FOR assignExpr SEMICOLON assignExpr SEMICOLON assignExpr LBRACE stmtLst RBRACE;
foreachLoop: FOREACH (foreachHead | LPAREN foreachHead RPAREN) LBRACE stmtLst RBRACE;
foreachHead: ((declStmt | assignExpr) COMMA)? declStmt COLON assignExpr;
whileLoop: WHILE assignExpr LBRACE stmtLst RBRACE;
ifStmt: IF assignExpr LBRACE stmtLst RBRACE elseStmt?;
elseStmt: ELSE ifStmt | ELSE LBRACE stmtLst RBRACE;

// Statements, declarations, definitions and lists
stmtLst: (stmt | forLoop | foreachLoop | whileLoop | ifStmt)*;
fieldLst: declStmt*;
typeLst: dataType (COMMA dataType)* ELLIPSIS?;
paramLstDef: (declStmt | assignExpr) (COMMA (declStmt | assignExpr))*;
paramLst: assignExpr (COMMA assignExpr)*;
stmt: (declStmt | assignExpr | returnStmt | breakStmt | continueStmt) SEMICOLON;
declStmt: declSpecifiers? dataType IDENTIFIER (ASSIGN assignExpr)?;
declSpecifiers: declSpecifier+;
declSpecifier: CONST | SIGNED | UNSIGNED;
importStmt: IMPORT STRING AS IDENTIFIER SEMICOLON;
returnStmt: RETURN assignExpr?;
breakStmt: BREAK INTEGER?;
continueStmt: CONTINUE INTEGER?;

// Builtin functions
builtinCall: printfCall | sizeOfCall;
printfCall: PRINTF LPAREN STRING (COMMA assignExpr)* RPAREN;
sizeOfCall: SIZEOF LPAREN assignExpr RPAREN;

// Expression loop
assignExpr: ternaryExpr | prefixUnaryExpr assignOp assignExpr;
ternaryExpr: logicalOrExpr (QUESTION_MARK logicalOrExpr COLON logicalOrExpr)?;
logicalOrExpr: logicalAndExpr (LOGICAL_OR logicalAndExpr)*;
logicalAndExpr: bitwiseOrExpr (LOGICAL_AND bitwiseOrExpr)*;
bitwiseOrExpr: bitwiseXorExpr (BITWISE_OR bitwiseXorExpr)*;
bitwiseXorExpr: bitwiseAndExpr (BITWISE_XOR bitwiseAndExpr)*;
bitwiseAndExpr: equalityExpr (BITWISE_AND equalityExpr)*;
equalityExpr: relationalExpr ((EQUAL | NOT_EQUAL) relationalExpr)?;
relationalExpr: shiftExpr ((LESS | GREATER | LESS_EQUAL | GREATER_EQUAL) shiftExpr)?;
shiftExpr: additiveExpr ((SHL | SHR) additiveExpr)?;
additiveExpr: multiplicativeExpr ((PLUS | MINUS) multiplicativeExpr)*;
multiplicativeExpr: castExpr ((MUL | DIV | REM) castExpr)*;
castExpr: prefixUnaryExpr | LPAREN dataType RPAREN prefixUnaryExpr;
prefixUnaryExpr: prefixUnaryOp* postfixUnaryExpr;
postfixUnaryExpr: atomicExpr (LBRACKET assignExpr RBRACKET | LPAREN paramLst? RPAREN | DOT postfixUnaryExpr | PLUS_PLUS | MINUS_MINUS)*;
atomicExpr: value | IDENTIFIER | builtinCall | LPAREN assignExpr RPAREN;

// Values and types
value: DOUBLE | INTEGER | CHAR | STRING | TRUE | FALSE | LBRACE paramLst? RBRACE | IDENTIFIER (DOT IDENTIFIER)* LBRACE paramLst? RBRACE | NIL LESS dataType GREATER;
dataType: baseDataType (MUL | LBRACKET INTEGER? RBRACKET)*;
baseDataType: TYPE_DOUBLE | TYPE_INT | TYPE_SHORT | TYPE_LONG | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | IDENTIFIER (DOT IDENTIFIER)*;

// Shorthands
assignOp: ASSIGN | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | REM_EQUAL | SHL_EQUAL | SHR_EQUAL | AND_EQUAL | OR_EQUAL | XOR_EQUAL;
prefixUnaryOp: PLUS | MINUS | PLUS_PLUS | MINUS_MINUS | NOT | BITWISE_NOT | MUL | BITWISE_AND;

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
SIGNED: 'signed';
UNSIGNED: 'unsigned';
F: 'f';
P: 'p';
IF: 'if';
ELSE: 'else';
FOR: 'for';
FOREACH: 'foreach';
WHILE: 'while';
CONST: 'const';
IMPORT: 'import';
BREAK: 'break';
CONTINUE: 'continue';
RETURN: 'return';
AS: 'as';
STRUCT: 'struct';
TYPE: 'type';
//NEW: 'new';
NIL: 'nil';
MAIN: 'main';
PRINTF: 'printf';
SIZEOF: 'sizeof';
EXT: 'ext';
DLL: 'dll';
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
SHL: '<<';
SHR: '>>';
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
ELLIPSIS: '...';

// Regex tokens
CHAR: '\'' (~['\\\r\n] | '\\' (. | EOF)) '\'';
STRING: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
INTEGER: NONZERO_DIGIT DIGIT*;
DOUBLE: DIGIT+ DOT DIGIT+;
IDENTIFIER: NONDIGIT (NONDIGIT | DIGIT)*;

fragment DIGIT: [0-9];
fragment NONZERO_DIGIT: [1-9];
fragment NONDIGIT: [a-zA-Z_];

// Skipped tokens
BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS: [ \t\r\n]+ -> skip;
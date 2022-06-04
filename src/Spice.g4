// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

grammar Spice;

// Control structures
entry: (mainFunctionDef | functionDef | procedureDef | structDef | genericTypeDef | globalVarDef | importStmt | extDecl)*;
mainFunctionDef: F LESS TYPE_INT GREATER MAIN LPAREN argLstDef? RPAREN LBRACE stmtLst RBRACE;
functionDef: declSpecifiers? F LESS dataType GREATER (IDENTIFIER DOT)? IDENTIFIER (LESS typeLst GREATER)? LPAREN argLstDef? RPAREN LBRACE stmtLst RBRACE;
procedureDef: declSpecifiers? P (IDENTIFIER DOT)? IDENTIFIER (LESS typeLst GREATER)? LPAREN argLstDef? RPAREN LBRACE stmtLst RBRACE;
extDecl: EXT (LESS dataType GREATER)? IDENTIFIER LPAREN typeLstEllipsis? RPAREN DLL? SEMICOLON;
genericTypeDef: declSpecifiers? TYPE IDENTIFIER typeAlts SEMICOLON;
structDef: declSpecifiers? TYPE IDENTIFIER (LESS typeLst GREATER)? STRUCT LBRACE field* RBRACE;
globalVarDef: declSpecifiers? dataType IDENTIFIER (ASSIGN MINUS? value)? SEMICOLON;
threadDef: THREAD LBRACE stmtLst RBRACE;
unsafeBlockDef: UNSAFE LBRACE stmtLst RBRACE;
forLoop: FOR (forHead | LPAREN forHead RPAREN) LBRACE stmtLst RBRACE;
forHead: declStmt SEMICOLON assignExpr SEMICOLON assignExpr;
foreachLoop: FOREACH (foreachHead | LPAREN foreachHead RPAREN) LBRACE stmtLst RBRACE;
foreachHead: (declStmt COMMA)? declStmt COLON assignExpr;
whileLoop: WHILE assignExpr LBRACE stmtLst RBRACE;
ifStmt: IF assignExpr LBRACE stmtLst RBRACE elseStmt?;
elseStmt: ELSE ifStmt | ELSE LBRACE stmtLst RBRACE;

// Statements, declarations, definitions and lists
stmtLst: (stmt | forLoop | foreachLoop | whileLoop | ifStmt | threadDef | unsafeBlockDef)*;
field: declSpecifiers? dataType IDENTIFIER;
typeLst: dataType (COMMA dataType)*;
typeLstEllipsis: typeLst ELLIPSIS?;
typeAlts: dataType (BITWISE_OR dataType)*;
argLstDef: declStmt (COMMA declStmt)*;
argLst: assignExpr (COMMA assignExpr)*;
stmt: (declStmt | assignExpr | returnStmt | breakStmt | continueStmt) SEMICOLON;
declStmt: declSpecifiers? dataType IDENTIFIER (ASSIGN assignExpr)?;
declSpecifiers: declSpecifier+;
declSpecifier: CONST | SIGNED | UNSIGNED | INLINE | PUBLIC;
importStmt: IMPORT STRING_LITERAL AS IDENTIFIER SEMICOLON;
returnStmt: RETURN assignExpr?;
breakStmt: BREAK INTEGER?;
continueStmt: CONTINUE INTEGER?;

// Builtin functions
builtinCall: printfCall | sizeOfCall | lenCall | tidCall | joinCall;
printfCall: PRINTF LPAREN STRING_LITERAL (COMMA assignExpr)* RPAREN;
sizeOfCall: SIZEOF LPAREN (assignExpr | TYPE dataType) RPAREN;
lenCall: LEN LPAREN assignExpr RPAREN;
tidCall: TID LPAREN RPAREN;
joinCall: JOIN LPAREN assignExpr (COMMA assignExpr)* RPAREN;

// Expression loop
assignExpr: prefixUnaryExpr assignOp assignExpr | ternaryExpr | threadDef;
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
postfixUnaryExpr: atomicExpr (LBRACKET assignExpr RBRACKET | LPAREN argLst? RPAREN | DOT postfixUnaryExpr | PLUS_PLUS | MINUS_MINUS)*;
atomicExpr: value | IDENTIFIER | builtinCall | LPAREN assignExpr RPAREN;

// Values and types
value: primitiveValue | LBRACE argLst? RBRACE | IDENTIFIER (DOT IDENTIFIER)* (LESS typeLst GREATER)? LBRACE argLst? RBRACE | NIL LESS dataType GREATER;
primitiveValue: DOUBLE | INTEGER | SHORT | LONG | CHAR_LITERAL | STRING_LITERAL | TRUE | FALSE;
dataType: baseDataType (MUL | LBRACKET INTEGER? RBRACKET)*;
baseDataType: TYPE_DOUBLE | TYPE_INT | TYPE_SHORT | TYPE_LONG | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | customDataType;
customDataType: IDENTIFIER (DOT IDENTIFIER)* (LESS typeLst GREATER)?;

// Shorthands
assignOp: ASSIGN | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | REM_EQUAL | SHL_EQUAL | SHR_EQUAL | AND_EQUAL | OR_EQUAL | XOR_EQUAL;
prefixUnaryOp: MINUS | PLUS_PLUS | MINUS_MINUS | NOT | BITWISE_NOT | MUL | BITWISE_AND | LOGICAL_AND; // Here, '&&' means the same as two times '&'

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
INLINE: 'inline';
PUBLIC: 'public';
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
THREAD: 'thread';
UNSAFE: 'unsafe';
//NEW: 'new';
NIL: 'nil';
MAIN: 'main';
PRINTF: 'printf';
SIZEOF: 'sizeof';
LEN: 'len';
TID: 'tid';
JOIN: 'join';
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
CHAR_LITERAL: '\'' (~['\\\r\n] | '\\' (. | EOF)) '\'';
STRING_LITERAL: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
INTEGER: NONZERO_DIGIT DIGIT* | ZERO;
DOUBLE: DIGIT+ DOT DIGIT+;
SHORT: INTEGER 's';
LONG: INTEGER 'l';
IDENTIFIER: NONDIGIT (NONDIGIT | DIGIT)*;

fragment ZERO: [0];
fragment DIGIT: [0-9];
fragment NONZERO_DIGIT: [1-9];
fragment NONDIGIT: [a-zA-Z_];

// Skipped tokens
BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS: [ \t\r\n]+ -> skip;
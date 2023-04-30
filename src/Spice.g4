// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

grammar Spice;

// Top level definitions and declarations
entry: (mainFunctionDef | functionDef | procedureDef | structDef | interfaceDef | enumDef | genericTypeDef | aliasDef | globalVarDef | importStmt | extDecl)*;
mainFunctionDef: F LESS TYPE_INT GREATER MAIN LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
functionDef: specifierLst? F LESS dataType GREATER fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
procedureDef: specifierLst? P fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
fctName: (IDENTIFIER DOT)? IDENTIFIER | OPERATOR overloadableOp;
structDef: specifierLst? TYPE IDENTIFIER (LESS typeLst GREATER)? STRUCT (COLON typeLst)? LBRACE field* RBRACE;
interfaceDef: specifierLst? TYPE IDENTIFIER (LESS typeLst GREATER)? INTERFACE LBRACE signature+ RBRACE;
enumDef: specifierLst? TYPE IDENTIFIER ENUM LBRACE enumItemLst RBRACE;
genericTypeDef: TYPE IDENTIFIER typeAltsLst SEMICOLON;
aliasDef: TYPE IDENTIFIER ALIAS dataType SEMICOLON;
globalVarDef: dataType IDENTIFIER (ASSIGN constant)? SEMICOLON;
extDecl: EXT (F LESS dataType GREATER | P) IDENTIFIER LPAREN (typeLst ELLIPSIS?)? RPAREN DLL? SEMICOLON;

// Control structures
threadDef: THREAD LBRACE stmtLst RBRACE;
unsafeBlockDef: UNSAFE LBRACE stmtLst RBRACE;
forLoop: FOR (forHead | LPAREN forHead RPAREN) LBRACE stmtLst RBRACE;
forHead: declStmt SEMICOLON assignExpr SEMICOLON assignExpr;
foreachLoop: FOREACH (foreachHead | LPAREN foreachHead RPAREN) LBRACE stmtLst RBRACE;
foreachHead: (declStmt COMMA)? declStmt COLON assignExpr;
whileLoop: WHILE assignExpr LBRACE stmtLst RBRACE;
doWhileLoop: DO LBRACE stmtLst RBRACE WHILE assignExpr SEMICOLON;
ifStmt: IF assignExpr LBRACE stmtLst RBRACE elseStmt?;
elseStmt: ELSE ifStmt | ELSE LBRACE stmtLst RBRACE;
anonymousBlockStmt: LBRACE stmtLst RBRACE;

// Statements, declarations, definitions and lists
stmtLst: (stmt | forLoop | foreachLoop | whileLoop | doWhileLoop | ifStmt | assertStmt | threadDef | unsafeBlockDef | anonymousBlockStmt)*;
typeLst: dataType (COMMA dataType)*;
typeAltsLst: dataType (BITWISE_OR dataType)*;
paramLst: declStmt (COMMA declStmt)*;
argLst: assignExpr (COMMA assignExpr)*;
enumItemLst: enumItem (COMMA enumItem)*;
enumItem: IDENTIFIER (ASSIGN INT_LIT)?;
field: dataType IDENTIFIER;
signature: specifierLst? (F LESS dataType GREATER | P) IDENTIFIER (LESS typeLst GREATER)? LPAREN typeLst? RPAREN SEMICOLON;
stmt: (declStmt | assignExpr | returnStmt | breakStmt | continueStmt) SEMICOLON;
declStmt: dataType IDENTIFIER (ASSIGN assignExpr)?;
specifierLst: specifier+;
specifier: CONST | SIGNED | UNSIGNED | INLINE | PUBLIC | HEAP;
importStmt: IMPORT STRING_LIT (AS IDENTIFIER)? SEMICOLON;
returnStmt: RETURN assignExpr?;
breakStmt: BREAK INT_LIT?;
continueStmt: CONTINUE INT_LIT?;
assertStmt: ASSERT assignExpr SEMICOLON;

// Builtin functions
builtinCall: printfCall | sizeOfCall | alignOfCall | lenCall | tidCall | joinCall;
printfCall: PRINTF LPAREN STRING_LIT (COMMA assignExpr)* RPAREN;
sizeOfCall: SIZEOF LPAREN (assignExpr | TYPE dataType) RPAREN;
alignOfCall: ALIGNOF LPAREN (assignExpr | TYPE dataType) RPAREN;
lenCall: LEN LPAREN assignExpr RPAREN;
tidCall: TID LPAREN RPAREN;
joinCall: JOIN LPAREN assignExpr (COMMA assignExpr)* RPAREN;

// Expression loop
assignExpr: prefixUnaryExpr assignOp assignExpr | ternaryExpr | threadDef;
ternaryExpr: logicalOrExpr (QUESTION_MARK logicalOrExpr? COLON logicalOrExpr)?;
logicalOrExpr: logicalAndExpr (LOGICAL_OR logicalAndExpr)*;
logicalAndExpr: bitwiseOrExpr (LOGICAL_AND bitwiseOrExpr)*;
bitwiseOrExpr: bitwiseXorExpr (BITWISE_OR bitwiseXorExpr)*;
bitwiseXorExpr: bitwiseAndExpr (BITWISE_XOR bitwiseAndExpr)*;
bitwiseAndExpr: equalityExpr (BITWISE_AND equalityExpr)*;
equalityExpr: relationalExpr ((EQUAL | NOT_EQUAL) relationalExpr)?;
relationalExpr: shiftExpr ((LESS | GREATER | LESS_EQUAL | GREATER_EQUAL) shiftExpr)?;
shiftExpr: additiveExpr ((LESS LESS | GREATER GREATER) additiveExpr)?;
additiveExpr: multiplicativeExpr ((PLUS | MINUS) multiplicativeExpr)*;
multiplicativeExpr: castExpr ((MUL | DIV | REM) castExpr)*;
castExpr: LPAREN dataType RPAREN prefixUnaryExpr | prefixUnaryExpr;
prefixUnaryExpr: postfixUnaryExpr | prefixUnaryOp prefixUnaryExpr;
postfixUnaryExpr: atomicExpr | postfixUnaryExpr LBRACKET assignExpr RBRACKET | postfixUnaryExpr DOT IDENTIFIER | postfixUnaryExpr PLUS_PLUS | postfixUnaryExpr MINUS_MINUS;
atomicExpr: constant | value | IDENTIFIER (SCOPE_ACCESS IDENTIFIER)* | builtinCall | LPAREN assignExpr RPAREN;

// Values
value: functionCall | arrayInitialization | structInstantiation | NIL LESS dataType GREATER;
constant: DOUBLE_LIT | INT_LIT | SHORT_LIT | LONG_LIT | CHAR_LIT | STRING_LIT | TRUE | FALSE;
functionCall: IDENTIFIER (SCOPE_ACCESS IDENTIFIER)* (DOT IDENTIFIER)* (LESS typeLst GREATER)? LPAREN argLst? RPAREN;
arrayInitialization: LBRACE argLst? RBRACE;
structInstantiation: IDENTIFIER (SCOPE_ACCESS IDENTIFIER)* (LESS typeLst GREATER)? LBRACE argLst? RBRACE;

// Types
dataType: specifierLst? baseDataType (MUL | BITWISE_AND | LBRACKET (INT_LIT | IDENTIFIER)? RBRACKET)*;
baseDataType: TYPE_DOUBLE | TYPE_INT | TYPE_SHORT | TYPE_LONG | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | customDataType;
customDataType: IDENTIFIER (SCOPE_ACCESS IDENTIFIER)* (LESS typeLst GREATER)?;

// Shorthands
assignOp: ASSIGN | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | REM_EQUAL | SHL_EQUAL | SHR_EQUAL | AND_EQUAL | OR_EQUAL | XOR_EQUAL;
prefixUnaryOp: MINUS | PLUS_PLUS | MINUS_MINUS | NOT | BITWISE_NOT | MUL | BITWISE_AND | LOGICAL_AND; // Here, '&&' means the same as two times '&'
overloadableOp: PLUS | MINUS | MUL | DIV | EQUAL | NOT_EQUAL | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | PLUS_PLUS | MINUS_MINUS;

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
CONST: 'const';
SIGNED: 'signed';
UNSIGNED: 'unsigned';
INLINE: 'inline';
PUBLIC: 'public';
HEAP: 'heap';
F: 'f';
P: 'p';
IF: 'if';
ELSE: 'else';
ASSERT: 'assert';
FOR: 'for';
FOREACH: 'foreach';
DO: 'do';
WHILE: 'while';
IMPORT: 'import';
BREAK: 'break';
CONTINUE: 'continue';
RETURN: 'return';
AS: 'as';
STRUCT: 'struct';
INTERFACE: 'interface';
TYPE: 'type';
ENUM: 'enum';
OPERATOR: 'operator';
ALIAS: 'alias';
THREAD: 'thread';
UNSAFE: 'unsafe';
NIL: 'nil';
MAIN: 'main';
PRINTF: 'printf';
SIZEOF: 'sizeof';
ALIGNOF: 'alignof';
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
SCOPE_ACCESS: '::';
ELLIPSIS: '...';

// Regex tokens
DOUBLE_LIT: [-]?[0-9]*[.][0-9]+([eE][+-]?[0-9]+)?;
INT_LIT: NUM_LIT;
SHORT_LIT: NUM_LIT 's';
LONG_LIT: NUM_LIT 'l';
CHAR_LIT: '\'' (~['\\\r\n] | '\\' (. | EOF)) '\'';
STRING_LIT: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;

fragment NUM_LIT: NUM_LIT_S | NUM_LIT_U;
fragment NUM_LIT_S: [-](DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT);
fragment NUM_LIT_U: (DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT)[u]?;
fragment DEC_LIT: ([0][dD])?[0-9]+;
fragment BIN_LIT: [0][bB][01]+;
fragment HEX_LIT: [0][xXhH][0-9a-fA-F]+;
fragment OCT_LIT: [0][oO][0-7]+;

// Skipped tokens
DOC_COMMENT: '/**' .*? '*/' -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS: [ \t\r\n]+ -> skip;
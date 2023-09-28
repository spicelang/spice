// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

grammar Spice;

// Top level definitions and declarations
entry: (mainFunctionDef | functionDef | procedureDef | structDef | interfaceDef | enumDef | genericTypeDef | aliasDef | globalVarDef | importStmt | extDecl | modAttr)*;
mainFunctionDef: fctAttr? F LESS TYPE_INT GREATER MAIN LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
functionDef: fctAttr? specifierLst? F LESS dataType GREATER fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
procedureDef: fctAttr? specifierLst? P fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
fctName: (TYPE_IDENTIFIER DOT)? IDENTIFIER | OPERATOR overloadableOp;
structDef: specifierLst? TYPE TYPE_IDENTIFIER (LESS typeLst GREATER)? STRUCT (COLON typeLst)? LBRACE fieldLst RBRACE;
interfaceDef: specifierLst? TYPE TYPE_IDENTIFIER (LESS typeLst GREATER)? INTERFACE LBRACE signature+ RBRACE;
enumDef: specifierLst? TYPE TYPE_IDENTIFIER ENUM LBRACE enumItemLst RBRACE;
genericTypeDef: TYPE TYPE_IDENTIFIER typeAltsLst SEMICOLON;
aliasDef: TYPE TYPE_IDENTIFIER ALIAS dataType SEMICOLON;
globalVarDef: dataType TYPE_IDENTIFIER (ASSIGN constant)? SEMICOLON;
extDecl: fctAttr? EXT (F LESS dataType GREATER | P) (IDENTIFIER | TYPE_IDENTIFIER) LPAREN (typeLst ELLIPSIS?)? RPAREN SEMICOLON;

// Control structures
unsafeBlock: UNSAFE LBRACE stmtLst RBRACE;
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
stmtLst: (stmt | forLoop | foreachLoop | whileLoop | doWhileLoop | ifStmt | assertStmt | unsafeBlock | anonymousBlockStmt)*;
typeLst: dataType (COMMA dataType)*;
typeAltsLst: dataType (BITWISE_OR dataType)*;
paramLst: declStmt (COMMA declStmt)*;
argLst: assignExpr (COMMA assignExpr)*;
enumItemLst: enumItem (COMMA enumItem)*;
enumItem: TYPE_IDENTIFIER (ASSIGN INT_LIT)?;
fieldLst: field*;
field: dataType IDENTIFIER (ASSIGN ternaryExpr)?;
signature: specifierLst? (F LESS dataType GREATER | P) IDENTIFIER (LESS typeLst GREATER)? LPAREN typeLst? RPAREN SEMICOLON;
stmt: (declStmt | assignExpr | returnStmt | breakStmt | continueStmt) SEMICOLON;
declStmt: dataType IDENTIFIER (ASSIGN assignExpr)?;
specifierLst: specifier+;
specifier: CONST | SIGNED | UNSIGNED | INLINE | PUBLIC | HEAP;
modAttr: MOD_ATTR_PREAMBLE LBRACKET attrLst RBRACKET;
fctAttr: FCT_ATTR_PREAMBLE LBRACKET attrLst RBRACKET;
attrLst: attr (COMMA attr)*;
attr: IDENTIFIER (DOT IDENTIFIER)* ASSIGN constant;
importStmt: IMPORT STRING_LIT (AS IDENTIFIER)? SEMICOLON;
returnStmt: RETURN assignExpr?;
breakStmt: BREAK INT_LIT?;
continueStmt: CONTINUE INT_LIT?;
assertStmt: ASSERT assignExpr SEMICOLON;

// Builtin functions
builtinCall: printfCall | sizeOfCall | alignOfCall | lenCall | panicCall;
printfCall: PRINTF LPAREN STRING_LIT (COMMA assignExpr)* RPAREN;
sizeOfCall: SIZEOF LPAREN (assignExpr | TYPE dataType) RPAREN;
alignOfCall: ALIGNOF LPAREN (assignExpr | TYPE dataType) RPAREN;
lenCall: LEN LPAREN assignExpr RPAREN;
panicCall: PANIC LPAREN assignExpr RPAREN;

// Expression loop
assignExpr: prefixUnaryExpr assignOp assignExpr | ternaryExpr;
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
prefixUnaryExpr: postfixUnaryExpr | (MINUS | PLUS_PLUS | MINUS_MINUS | NOT | BITWISE_NOT | MUL | BITWISE_AND) prefixUnaryExpr;
postfixUnaryExpr: atomicExpr | postfixUnaryExpr (LBRACKET assignExpr RBRACKET | DOT IDENTIFIER | PLUS_PLUS | MINUS_MINUS);
atomicExpr: constant | value | ((IDENTIFIER | TYPE_IDENTIFIER) SCOPE_ACCESS)* (IDENTIFIER | TYPE_IDENTIFIER) | builtinCall | LPAREN assignExpr RPAREN;

// Values
value: fctCall | arrayInitialization | structInstantiation| lambdaFunc | lambdaProc | lambdaExpr | NIL LESS dataType GREATER;
constant: DOUBLE_LIT | INT_LIT | SHORT_LIT | LONG_LIT | CHAR_LIT | STRING_LIT | TRUE | FALSE;
fctCall: (IDENTIFIER SCOPE_ACCESS)* (IDENTIFIER DOT)* (IDENTIFIER | TYPE_IDENTIFIER) (LESS typeLst GREATER)? LPAREN argLst? RPAREN;
arrayInitialization: LBRACKET argLst? RBRACKET;
structInstantiation: (IDENTIFIER SCOPE_ACCESS)* TYPE_IDENTIFIER (LESS typeLst GREATER)? LBRACE argLst? RBRACE;
lambdaFunc: F LESS dataType GREATER LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
lambdaProc: P LPAREN paramLst? RPAREN LBRACE stmtLst RBRACE;
lambdaExpr: LPAREN paramLst? RPAREN ARROW assignExpr;

// Types
dataType: specifierLst? baseDataType (MUL | BITWISE_AND | LBRACKET (INT_LIT | TYPE_IDENTIFIER)? RBRACKET)*;
baseDataType: TYPE_DOUBLE | TYPE_INT | TYPE_SHORT | TYPE_LONG | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | customDataType | functionDataType;
customDataType: (IDENTIFIER SCOPE_ACCESS)* TYPE_IDENTIFIER (LESS typeLst GREATER)?;
functionDataType: (P | F LESS dataType GREATER) LPAREN typeLst? RPAREN;

// Shorthands
assignOp: ASSIGN | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | REM_EQUAL | SHL_EQUAL | SHR_EQUAL | AND_EQUAL | OR_EQUAL | XOR_EQUAL;
overloadableOp: PLUS | MINUS | MUL | DIV | EQUAL | NOT_EQUAL | LESS LESS | GREATER GREATER | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | PLUS_PLUS | MINUS_MINUS;

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
UNSAFE: 'unsafe';
NIL: 'nil';
MAIN: 'main';
PRINTF: 'printf';
SIZEOF: 'sizeof';
ALIGNOF: 'alignof';
LEN: 'len';
PANIC: 'panic';
EXT: 'ext';
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
ARROW: '->';
SCOPE_ACCESS: '::';
ELLIPSIS: '...';
FCT_ATTR_PREAMBLE: '#';
MOD_ATTR_PREAMBLE: '#!';

// Regex tokens
DOUBLE_LIT: [-]?[0-9]*[.][0-9]+([eE][+-]?[0-9]+)?;
INT_LIT: NUM_LIT;
SHORT_LIT: NUM_LIT 's';
LONG_LIT: NUM_LIT 'l';
CHAR_LIT: '\'' (~['\\\r\n] | '\\' (. | EOF)) '\'';
STRING_LIT: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
IDENTIFIER: [a-z_][a-zA-Z0-9_]*;
TYPE_IDENTIFIER: [A-Z][a-zA-Z0-9_]*;

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
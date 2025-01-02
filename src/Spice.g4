// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

grammar Spice;

// Top level definitions and declarations
entry: (mainFunctionDef | functionDef | procedureDef | structDef | interfaceDef | enumDef | genericTypeDef | aliasDef | globalVarDef | importDef | extDecl | modAttr)* EOF;
mainFunctionDef: topLevelDefAttr? F LESS TYPE_INT GREATER MAIN LPAREN paramLst? RPAREN stmtLst;
functionDef: topLevelDefAttr? specifierLst? F LESS dataType GREATER fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN stmtLst;
procedureDef: topLevelDefAttr? specifierLst? P fctName (LESS typeLst GREATER)? LPAREN paramLst? RPAREN stmtLst;
fctName: (TYPE_IDENTIFIER DOT)? IDENTIFIER | OPERATOR overloadableOp;
structDef: topLevelDefAttr? specifierLst? TYPE TYPE_IDENTIFIER (LESS typeLst GREATER)? STRUCT (COLON typeLst)? LBRACE field* RBRACE;
interfaceDef: topLevelDefAttr? specifierLst? TYPE TYPE_IDENTIFIER (LESS typeLst GREATER)? INTERFACE LBRACE signature* RBRACE;
enumDef: specifierLst? TYPE TYPE_IDENTIFIER ENUM LBRACE enumItemLst RBRACE;
genericTypeDef: TYPE TYPE_IDENTIFIER typeAltsLst SEMICOLON;
aliasDef: specifierLst? TYPE TYPE_IDENTIFIER ALIAS dataType SEMICOLON;
globalVarDef: dataType TYPE_IDENTIFIER (ASSIGN constant)? SEMICOLON;
extDecl: topLevelDefAttr? EXT (F LESS dataType GREATER | P) (IDENTIFIER | TYPE_IDENTIFIER) LPAREN (typeLst ELLIPSIS?)? RPAREN SEMICOLON;
importDef: IMPORT STRING_LIT (AS IDENTIFIER)? SEMICOLON;

// Control structures
unsafeBlock: UNSAFE stmtLst;
forLoop: FOR (forHead | LPAREN forHead RPAREN) stmtLst;
forHead: declStmt SEMICOLON assignExpr SEMICOLON assignExpr;
foreachLoop: FOREACH (foreachHead | LPAREN foreachHead RPAREN) stmtLst;
foreachHead: (declStmt COMMA)? declStmt COLON assignExpr;
whileLoop: WHILE assignExpr stmtLst;
doWhileLoop: DO stmtLst WHILE assignExpr SEMICOLON;
ifStmt: IF assignExpr stmtLst elseStmt?;
elseStmt: ELSE ifStmt | ELSE stmtLst;
switchStmt: SWITCH assignExpr LBRACE caseBranch* defaultBranch? RBRACE;
caseBranch: CASE caseConstant (COMMA caseConstant)* COLON stmtLst;
defaultBranch: DEFAULT COLON stmtLst;
anonymousBlockStmt: stmtLst;

// Statements, declarations, definitions and lists
stmtLst: LBRACE (stmt | forLoop | foreachLoop | whileLoop | doWhileLoop | ifStmt | switchStmt | assertStmt | unsafeBlock | anonymousBlockStmt)* RBRACE;
typeLst: dataType (COMMA dataType)*;
typeAltsLst: dataType (BITWISE_OR dataType)*;
paramLst: declStmt (COMMA declStmt)*;
argLst: assignExpr (COMMA assignExpr)*;
enumItemLst: enumItem (COMMA enumItem)*;
enumItem: TYPE_IDENTIFIER (ASSIGN INT_LIT)?;
field: dataType IDENTIFIER (ASSIGN ternaryExpr)?;
signature: specifierLst? (F LESS dataType GREATER | P) IDENTIFIER (LESS typeLst GREATER)? LPAREN typeLst? RPAREN SEMICOLON;
stmt: (declStmt | exprStmt | returnStmt | breakStmt | continueStmt | fallthroughStmt) SEMICOLON;
declStmt: dataType IDENTIFIER (ASSIGN assignExpr)?;
exprStmt: assignExpr;
specifierLst: specifier+;
specifier: CONST | SIGNED | UNSIGNED | INLINE | PUBLIC | HEAP | COMPOSE;
modAttr: MOD_ATTR_PREAMBLE LBRACKET attrLst RBRACKET;
topLevelDefAttr: TOPLEVEL_ATTR_PREAMBLE LBRACKET attrLst RBRACKET;
lambdaAttr: LBRACKET LBRACKET attrLst RBRACKET RBRACKET;
attrLst: attr (COMMA attr)*;
attr: IDENTIFIER (DOT IDENTIFIER)* (ASSIGN constant)?;
caseConstant: constant | (IDENTIFIER SCOPE_ACCESS)? TYPE_IDENTIFIER (SCOPE_ACCESS TYPE_IDENTIFIER)*;
returnStmt: RETURN assignExpr?;
breakStmt: BREAK INT_LIT?;
continueStmt: CONTINUE INT_LIT?;
fallthroughStmt: FALLTHROUGH;
assertStmt: ASSERT assignExpr SEMICOLON;

// Builtin functions
builtinCall: printfCall | sizeOfCall | alignOfCall | lenCall | panicCall | sysCall;
printfCall: PRINTF LPAREN STRING_LIT (COMMA assignExpr)* RPAREN;
sizeOfCall: SIZEOF LPAREN (assignExpr | TYPE dataType) RPAREN;
alignOfCall: ALIGNOF LPAREN (assignExpr | TYPE dataType) RPAREN;
lenCall: LEN LPAREN assignExpr RPAREN;
panicCall: PANIC LPAREN assignExpr RPAREN;
sysCall: SYSCALL LPAREN assignExpr (COMMA assignExpr)* RPAREN;

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
castExpr: (LPAREN dataType RPAREN)? prefixUnaryExpr;
prefixUnaryExpr: postfixUnaryExpr | (MINUS | PLUS_PLUS | MINUS_MINUS | NOT | BITWISE_NOT | MUL | BITWISE_AND) prefixUnaryExpr;
postfixUnaryExpr: atomicExpr | postfixUnaryExpr (LBRACKET assignExpr RBRACKET | DOT IDENTIFIER | PLUS_PLUS | MINUS_MINUS);
atomicExpr: constant | value | (IDENTIFIER | TYPE_IDENTIFIER) (SCOPE_ACCESS (IDENTIFIER | TYPE_IDENTIFIER))* | builtinCall | LPAREN assignExpr RPAREN;

// Values
value: fctCall | arrayInitialization | structInstantiation | lambdaFunc | lambdaProc | lambdaExpr | NIL LESS dataType GREATER;
constant: DOUBLE_LIT | INT_LIT | SHORT_LIT | LONG_LIT | CHAR_LIT | STRING_LIT | TRUE | FALSE;
fctCall: (IDENTIFIER SCOPE_ACCESS)* (IDENTIFIER DOT)* (IDENTIFIER | TYPE_IDENTIFIER) (LESS typeLst GREATER)? LPAREN argLst? RPAREN;
arrayInitialization: LBRACKET argLst? RBRACKET;
structInstantiation: (IDENTIFIER SCOPE_ACCESS)* TYPE_IDENTIFIER (LESS typeLst GREATER)? LBRACE argLst? RBRACE;
lambdaFunc: F LESS dataType GREATER LPAREN paramLst? RPAREN lambdaAttr? stmtLst;
lambdaProc: P LPAREN paramLst? RPAREN lambdaAttr? stmtLst;
lambdaExpr: LPAREN paramLst? RPAREN ARROW assignExpr;

// Types
dataType: specifierLst? baseDataType (MUL | BITWISE_AND | LBRACKET (INT_LIT | TYPE_IDENTIFIER)? RBRACKET)*;
baseDataType: TYPE_DOUBLE | TYPE_INT | TYPE_SHORT | TYPE_LONG | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | customDataType | functionDataType;
customDataType: (IDENTIFIER SCOPE_ACCESS)* TYPE_IDENTIFIER (LESS typeLst GREATER)?;
functionDataType: (F LESS dataType GREATER | P) LPAREN typeLst? RPAREN;

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
COMPOSE: 'compose';
F: 'f';
P: 'p';
IF: 'if';
ELSE: 'else';
SWITCH: 'switch';
CASE: 'case';
DEFAULT: 'default';
ASSERT: 'assert';
FOR: 'for';
FOREACH: 'foreach';
DO: 'do';
WHILE: 'while';
IMPORT: 'import';
BREAK: 'break';
CONTINUE: 'continue';
FALLTHROUGH: 'fallthrough';
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
SYSCALL: 'syscall';
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
TOPLEVEL_ATTR_PREAMBLE: '#';
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

fragment NUM_LIT: [-]?(DEC_LIT | BIN_LIT | HEX_LIT | OCT_LIT)[u]?;
fragment DEC_LIT: ([0][dD])?[0-9]+;
fragment BIN_LIT: [0][bB][01]+;
fragment HEX_LIT: [0][xXhH][0-9a-fA-F]+;
fragment OCT_LIT: [0][oO][0-7]+;

// Skipped tokens
DOC_COMMENT: '/**' .*? '*/' -> skip;
BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS: [ \t\r\n]+ -> skip;
grammar Spice;

// Control structures
entry: (mainFunctionDef | functionDef | procedureDef | structDef | globalVarDef | importStmt | extDecl)*;
mainFunctionDef: F LESS TYPE_INT GREATER MAIN LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
functionDef: F LESS dataType GREATER IDENTIFIER LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
procedureDef: P IDENTIFIER LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
extDecl: EXT (LESS dataType GREATER)? IDENTIFIER LPAREN typeLst? RPAREN SEMICOLON;
structDef: TYPE IDENTIFIER STRUCT LBRACE fieldLst RBRACE;
globalVarDef: CONST? dataType IDENTIFIER (ASSIGN_OP value)? SEMICOLON;
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
stmt: (declStmt | assignExpr | newStmt | arrayInitStmt | functionCall | builtinCall | returnStmt | breakStmt | continueStmt) SEMICOLON;
declStmt: CONST? dataType IDENTIFIER;
functionCall: IDENTIFIER (DOT IDENTIFIER)* LPAREN paramLst? RPAREN;
newStmt: CONST? dataType IDENTIFIER ASSIGN_OP NEW IDENTIFIER LBRACE paramLst? RBRACE;
arrayInitStmt: CONST? dataType LBRACKET value RBRACKET IDENTIFIER (ASSIGN_OP LBRACE paramLst? RBRACE)?;
importStmt: IMPORT STRING AS IDENTIFIER SEMICOLON;
returnStmt: RETURN assignExpr?;
breakStmt: BREAK INTEGER?;
continueStmt: CONTINUE INTEGER?;

// Builtin functions
builtinCall: printfCall | sizeOfCall;
printfCall: PRINTF LPAREN STRING (COMMA assignExpr)* RPAREN;
sizeOfCall: SIZEOF LPAREN assignExpr RPAREN;

// Expression loop
assignExpr: ((declStmt | idenValue) (ASSIGN_OP | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL | SHL_EQUAL | SHR_EQUAL))? ternaryExpr;
ternaryExpr: logicalOrExpr (QUESTION_MARK logicalOrExpr ':' logicalOrExpr)?;
logicalOrExpr: logicalAndExpr (LOGICAL_OR logicalAndExpr)*;
logicalAndExpr: bitwiseOrExpr (LOGICAL_AND bitwiseOrExpr)*;
bitwiseOrExpr: bitwiseAndExpr (BITWISE_OR bitwiseAndExpr)*;
bitwiseAndExpr: equalityExpr (BITWISE_AND equalityExpr)*;
equalityExpr: relationalExpr ((EQUAL | NOT_EQUAL) relationalExpr)?;
relationalExpr: shiftExpr ((LESS | GREATER | LESS_EQUAL | GREATER_EQUAL) shiftExpr)?;
shiftExpr: additiveExpr ((SHL | SHR) additiveExpr)?;
additiveExpr: multiplicativeExpr ((PLUS | MINUS) multiplicativeExpr)*;
multiplicativeExpr: prefixUnaryExpr ((MUL | DIV | REM) prefixUnaryExpr)*;
prefixUnaryExpr: (NOT | PLUS_PLUS | MINUS_MINUS)? postfixUnaryExpr;
postfixUnaryExpr: atomicExpr (PLUS_PLUS | MINUS_MINUS)?;
atomicExpr: value | idenValue | functionCall | builtinCall | LPAREN assignExpr RPAREN;

// Values and types
idenValue: (BITWISE_AND | MUL)? IDENTIFIER (LBRACKET assignExpr RBRACKET)? (DOT IDENTIFIER (LBRACKET assignExpr RBRACKET)?)*;
value: DOUBLE | INTEGER | CHAR | STRING | TRUE | FALSE;
dataType: (TYPE_DOUBLE | TYPE_INT | TYPE_BYTE | TYPE_CHAR | TYPE_STRING | TYPE_BOOL | TYPE_DYN | IDENTIFIER) MUL? (LBRACKET RBRACKET)?;

// Terminal tokens
TYPE_DOUBLE: 'double';
TYPE_INT: 'int';
TYPE_BYTE: 'byte';
TYPE_CHAR: 'char';
TYPE_STRING: 'string';
TYPE_BOOL: 'bool';
TYPE_DYN: 'dyn';
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
NEW: 'new';
MAIN: 'main';
PRINTF: 'printf';
SIZEOF: 'sizeof';
EXT: 'ext';
ELLIPSIS: '...';
TRUE: 'true';
FALSE: 'false';
CHAR: '\'' (~['\\\r\n] | '\\' (. | EOF)) '\'';
STRING: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
INTEGER: '-'? [1-9]+[0-9]* | '0';
DOUBLE: '-'? [0-9]+.[0-9]+;
IDENTIFIER: [_a-zA-Z][_a-zA-Z0-9]*;
LBRACE: '{';
RBRACE: '}';
LPAREN: '(';
RPAREN: ')';
LBRACKET: '[';
RBRACKET: ']';
LOGICAL_OR: '||';
LOGICAL_AND: '&&';
BITWISE_OR: '|';
BITWISE_AND: '&';
NOT: '!';
PLUS_PLUS: '++';
MINUS_MINUS: '--';
PLUS_EQUAL: '+=';
MINUS_EQUAL: '-=';
MUL_EQUAL: '*=';
DIV_EQUAL: '/=';
SHL_EQUAL: '<<=';
SHR_EQUAL: '>>=';
SHL: '<<';
SHR: '>>';
PLUS: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
REM: '%';
GREATER: '>';
LESS: '<';
GREATER_EQUAL: '>=';
LESS_EQUAL: '<=';
EQUAL: '==';
NOT_EQUAL: '!=';
ASSIGN_OP: '=';
QUESTION_MARK: '?';
SEMICOLON: ';';
COLON: ':';
COMMA: ',';
DOT: '.';

BLOCK_COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' ~[\r\n]* -> skip;
WS: [ \t\r\n]+ -> channel(HIDDEN);
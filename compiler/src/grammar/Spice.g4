grammar Spice;

entry: (stmt | mainFunctionDef | functionDef | procedureDef | structDef)*;
mainFunctionDef: F LESS TYPE_INT GREATER MAIN LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
functionDef: F LESS dataType GREATER IDENTIFIER LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
procedureDef: P IDENTIFIER LPAREN paramLstDef? RPAREN LBRACE stmtLst RBRACE;
structDef: TYPE IDENTIFIER STRUCT LBRACE fieldLst RBRACE;
forLoop: FOR assignExpr SEMICOLON assignExpr SEMICOLON assignExpr LBRACE stmtLst RBRACE;
//foreachLoop: FOREACH IDENTIFIER COLON assignment LBRACE stmtLst RBRACE;
whileLoop: WHILE assignExpr LBRACE stmtLst RBRACE;
ifStmt: IF assignExpr LBRACE stmtLst RBRACE elseStmt?;
elseStmt: ELSE ifStmt | ELSE LBRACE stmtLst RBRACE;

stmtLst: (stmt | forLoop | /*foreachLoop |*/ whileLoop | ifStmt)*;
fieldLst: declStmt*;
paramLstDef: (declStmt | assignExpr) (COMMA (declStmt | assignExpr))*;
paramLst: assignExpr (COMMA assignExpr)*;
stmt: (declStmt | assignExpr | functionCall | importStmt | returnStmt | breakStmt | continueStmt | printfStmt) SEMICOLON;
declStmt: CONST? dataType IDENTIFIER;
functionCall: IDENTIFIER (DOT IDENTIFIER)* LPAREN paramLst? RPAREN;
newStmt: NEW IDENTIFIER LBRACE paramLst? RBRACE;
arrayInitStmt: dataType LBRACKET assignExpr RBRACKET (LBRACE paramLst? RBRACE)?;
importStmt: IMPORT STRING AS IDENTIFIER;
returnStmt: RETURN assignExpr;
breakStmt: BREAK INTEGER?;
continueStmt: CONTINUE INTEGER?;
printfStmt: PRINTF LPAREN STRING (COMMA assignExpr)* RPAREN;

assignExpr: ((declStmt | MUL? IDENTIFIER (DOT IDENTIFIER)* (LBRACKET assignExpr RBRACKET)?)
            (ASSIGN_OP | PLUS_EQUAL | MINUS_EQUAL | MUL_EQUAL | DIV_EQUAL))?
            (ternaryExpr | newStmt | arrayInitStmt);
ternaryExpr: logicalOrExpr (QUESTION_MARK logicalOrExpr ':' logicalOrExpr)?;
logicalOrExpr: logicalAndExpr (LOGICAL_OR logicalAndExpr)*;
logicalAndExpr: bitwiseOrExpr (LOGICAL_AND bitwiseOrExpr)*;
bitwiseOrExpr: bitwiseAndExpr (BITWISE_OR bitwiseAndExpr)*;
bitwiseAndExpr: equalityExpr (BITWISE_AND equalityExpr)*;
equalityExpr: relationalExpr ((EQUAL | NOT_EQUAL) relationalExpr)?;
relationalExpr: additiveExpr ((LESS | GREATER | LESS_EQUAL | GREATER_EQUAL) additiveExpr)?;
additiveExpr: multiplicativeExpr ((PLUS | MINUS) multiplicativeExpr)*;
multiplicativeExpr: prefixUnaryExpr ((MUL | DIV) prefixUnaryExpr)*;
prefixUnaryExpr: (NOT | PLUS_PLUS | MINUS_MINUS)? postfixUnaryExpr;
postfixUnaryExpr: atomicExpr (PLUS_PLUS | MINUS_MINUS)?;
atomicExpr: value | idenValue | functionCall | LPAREN assignExpr RPAREN;

idenValue: (BITWISE_AND | MUL)? IDENTIFIER (DOT IDENTIFIER)* (LBRACKET assignExpr RBRACKET)?;
value: STRING | TRUE | FALSE | INTEGER | DOUBLE;
dataType: (TYPE_DOUBLE | TYPE_INT | TYPE_STRING | TYPE_BOOL | TYPE_DYN | IDENTIFIER) MUL? (LBRACKET RBRACKET)?;

TYPE_DOUBLE: 'double';
TYPE_INT: 'int';
TYPE_STRING: 'string';
TYPE_BOOL: 'bool';
TYPE_DYN: 'dyn';
F: 'f';
P: 'p';
IF: 'if';
ELSE: 'else';
FOR: 'for';
//FOREACH: 'foreach';
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
TRUE: 'true';
FALSE: 'false';
STRING: '"' (~["\\\r\n] | '\\' (. | EOF))* '"';
INTEGER: '-'* [1-9]+[0-9]* | '0';
DOUBLE: '-'* [0-9]+.[0-9]+;
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
PLUS: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
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
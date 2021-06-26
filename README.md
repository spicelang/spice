# Spice Programming Language

## Grammar
```
ENTRY                --> (TOP_LVL_EXPR | FUNCTION_DEF | PROCEDURE_DEF)*
FUNCTION_DEF         --> 'f' '<' DATA_TYPE '>' IDENTIFIER '(' PARAM_LST ')' '{' STMT_LST '}'
PROCEDURE_DEF        --> 'p' IDENTIFIER '(' PARAM_LST ')' '{' STMT_LST '}'
FOR_LOOP             --> 'for' ASSIGNMENT ';' TOP_LVL_EXPR ';' (INTEGER | DOUBLE) '{' STMT_LST '}'
FOR_EACH_LOOP        --> 'foreach' IDENTIFIER ':' VALUE '{' STMT_LST '}'
WHILE_LOOP           --> 'while' TOP_LVL_EXPR '{' STMT_LST '}'
IF_STMT              --> 'if' TOP_LVL_EXPR '{' STMT_LST '}'

STMT_LST             --> (STMT | FOR_LOOP | FOR_EACH_LOOP | WHILE_LOOP | IF_STMT)*
PARAM_LST            --> (DECL_STMT | ASSIGNMENT)? (',' (DECL_STMT | ASSIGNMENT))*
STMT                 --> (DECL_STMT | ASSIGNMENT | FUNCTION_CALL | TOP_LVL_EXPR | IMPORT_STMT | RETURN_STMT) ';'
DECL_STMT            --> 'const'? DATA_TYPE IDENTIFIER
FUNCTION_CALL        --> IDENTIFIER '(' PARAM_LST ')'
IMPORT_STMT          --> 'import' STRING
RETURN_STMT          --> 'return' TOP_LVL_EXPR

DATA_TYPE            --> 'double' | 'int' | 'string' | 'bool' | 'dyn'

TOP_LVL_EXPR         --> ASSIGNMENT
ASSIGNMENT           --> ((DECL_STMT | IDENTIFIER) ('=' | '+=' | '-=' | '*=' | '/='))? TERNARY
TERNARY              --> LOGICAL_OR ('?' LOGICAL_OR ':' LOGICAL_OR)?
LOGICAL_OR           --> LOGICAL_AND ('||' LOGICAL_AND)*
LOGICAL_AND          --> BITWISE_OR ('&&' BITWISE_OR)*
BITWISE_OR           --> BITWISE_AND ('|' BITWISE_AND)*
BITWISE_AND          --> EQUALITY ('&' EQUALITY)*
EQUALITY             --> RELATIONAL_EXPR (('==' | '!=') RELATIONAL_EXPR)?
RELATIONAL_EXPR      --> ADDITIVE_EXPR (('<' | '>' | '<=' | '>=') ADDITIVE_EXPR)?
ADDITIVE_EXPR        --> MUL_DIV_EXPR (('+' | '-') MUL_DIV_EXPR)*
MULTIPLICATIVE_EXPR  --> PREFIX_UNARY (('*' | '/') PREFIX_UNARY)*
PREFIX_UNARY         --> ('!' | '++' | '--')? POSTFIX_UNARY
POSTFIX_UNARY        --> ATOMIC_EXPR ('++' | '--')?
ATOMIC_EXPR          --> VALUE | '(' ASSIGNMENT ')'

COMMENT_LINE         --> //([^\n])*
COMMENT_BLOCK        --> \/\*.*\*\/

VALUE                --> STRING | BOOL | INTEGER | DOUBLE | IDENTIFIER | FUNCTION_CALL
IDENTIFIER           --> [_a-zA-Z][_a-zA-Z0-9]*
STRING               --> "([^\\"]|\\\\|\\")*"
BOOL                 --> 'true' | 'false'
INTEGER              --> -*[1-9]+[0-9]*|0
DOUBLE               --> -*[0-9]+.[0-9]+
```
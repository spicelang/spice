# Spice Programming Language

## Grammar
```
TOP_LVL              --> 
FCTN_DEF             --> 'f' LANGLEBRACKET DATA_TYPE RANGLEBRACKET IDENTIFIER LPAREN PARAM_LST RPAREN LBRACE STMT_LST RBRACE
PCRE_DEF             --> 'p' IDENTIFIER LBRACE STMT_LST RBRACE
FOR_LOOP             --> 'for'
FOR_EACH_LOOP        --> 'for'
WHILE_LOOP           --> 'while' COND_LST LBRACE STMT_LST RBRACE
IF_STMT              --> 'if' COND_LST LBRACE STMT_LST RBRACE

STMT_LST             --> STMT*
PARAM_LST            --> (DECL_STMT | DEF_STMT)*
STMT                 --> (ASSIGN_STMT | DECL_STMT | DEF_STMT | FCTN_CALL |) ';'
ASSIGN_STMT          --> DECL_STMT '=' VALUE | IDENTIFIER '=' VALUE
DECL_STMT            --> DATA_TYPE IDENTIFIER
DEF_STMT             --> DECL '=' VALUE
FCTN_CALL            --> IDENTIFIER LPAREN PARAM_LST RPAREN
RETURN_STMT          --> 'return' VALUE

DATA_TYPE            --> 'double' | 'int' | 'string' | 'bool' | 'auto'
IDENTIFIER           --> [_a-zA-Z][_a-zA-Z0-9]
VALUE                --> CONST | IDENFIER | FCTN_CALL
CONST                --> STRING | BOOL | INTEGER | DOUBLE

COND_LST             --> COND*

ASSIGNMENT           --> 
LOGICAL_OR           -->
LOGICAL_AND          --> 
BITWISE_OR           --> 
BITWISE_AND          --> 
RELATIONAL_EXPR      --> 
ADDITIVE_EXPR        --> MUL_DIV_EXPR ('+' MUL_DIV_EXPR)* | MUL_DIV_EXPR ('-' MUL_DIV_EXPR)*
MULTIPLICATIVE_EXPR  --> NEG_EXPR ('*' NEG_EXPR)* | NEG_EXPR ('/' NEG_EXPR)*
PREFIX_UNARY         --> 
POSTFIX_UNARY        -->
ATOMIC_EXPR          --> INTEGER | DOUBLE | IDENTIFIER | LPAREN ADD_SUB_EXPR RPAREN

STRING               --> "([^\\"]|\\\\|\\")*"
BOOL                 --> 'true' | 'false'
INTEGER              --> -*[1-9]+[0-9]*|0
DOUBLE               --> -*[0-9]+.[0-9]+

LBRACE               --> '{'
RBRACE               --> '}'
LPAREN               --> '('
RPAREN               --> ')'
LBRACKET             --> '['
RBRACKET             --> '['
LANGLEBRACKET        --> '<'
RANGLEBRACKET        --> '>'
```
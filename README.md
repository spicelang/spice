# Spice Programming Language

## Grammar
```
TOP_LVL              --> (TOP_LVL_EXPR | FCTN_DEF)*
FCTN_DEF             --> 'f' LANGLEBRACKET DATA_TYPE RANGLEBRACKET IDENTIFIER LPAREN PARAM_LST RPAREN LBRACE STMT_LST RBRACE
PCRE_DEF             --> 'p' IDENTIFIER LBRACE STMT_LST RBRACE
FOR_LOOP             --> 'for' DEF_STMT ';' TOP_LVL_EXPR ';' (INTEGER | DOUBLE) LBRACE STMT_LST RBRACE
FOR_EACH_LOOP        --> 'foreach' DATA_TYPE IDENTIFIER ':' VALUE LBRACE STMT_LST RBRACE
WHILE_LOOP           --> 'while' TOP_LVL_EXPR LBRACE STMT_LST RBRACE
IF_STMT              --> 'if' TOP_LVL_EXPR LBRACE STMT_LST RBRACE

STMT_LST             --> STMT*
PARAM_LST            --> ((DECL_STMT | DEF_STMT) ',')*
STMT                 --> (DECL_STMT | DEF_STMT | FCTN_CALL | TOP_LVL_EXPR | IMPORT_STMT | RETURN_STMT) ';'
DECL_STMT            --> 'const'? DATA_TYPE IDENTIFIER
FCTN_CALL            --> IDENTIFIER LPAREN PARAM_LST RPAREN
IMPORT_STMT          --> 'import' STRING
RETURN_STMT          --> 'return' VALUE

DATA_TYPE            --> 'double' | 'int' | 'string' | 'bool' | 'dyn'
IDENTIFIER           --> [_a-zA-Z][_a-zA-Z0-9]
VALUE                --> STRING | BOOL | INTEGER | DOUBLE | IDENFIER | FCTN_CALL

TOP_LVL_EXPR         --> ASSIGNMENT
ASSIGNMENT           --> ((DECL | IDENTIFIER) ('=' | '+=' | '-=' | '*=' | '/='))? TERNARY
TERNARY              --> LOGICAL_OR ('?' LOGICAL_OR ':' LOGICAL_OR)?
LOGICAL_OR           --> LOGICAL_AND ('||' LOGICAL_AND)*
LOGICAL_AND          --> BITWISE_OR ('&&' BITWISE_OR)*
BITWISE_OR           --> BITWISE_AND ('|' BITWISE_AND)*
BITWISE_AND          --> EQUALITY ('&' EQUALITY)*
EQUALITY             --> RELATIONAL_EXPR (('==' | '!=') RELATIONAL_EXPR)?
RELATIONAL_EXPR      --> ADDITIVE_EXPR (('<' | '>' | '<=' | '>=') ADDITIVE_EXPR)?
ADDITIVE_EXPR        --> MUL_DIV_EXPR (('+' | '-') MUL_DIV_EXPR)*
MULTIPLICATIVE_EXPR  --> PREFIX_UNARY (('*' | '/') PREFIX_UNARY)*
PREFIX_UNARY         --> POSTFIX_UNARY ('++' | '--')?
POSTFIX_UNARY        --> ('!' | '+' | '-' | '++' | '--')? ATOMIC_EXPR
ATOMIC_EXPR          --> INTEGER | DOUBLE | IDENTIFIER | FCTN_CALL | LPAREN ADD_SUB_EXPR RPAREN

STRING               --> "([^\\"] | \\\\ | \\")*"
BOOL                 --> 'true' | 'false'
INTEGER              --> -*[1-9]+[0-9]* | 0
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
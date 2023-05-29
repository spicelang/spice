```mermaid
graph LR;
    lexer["Lexing"]-->
    parser["Parsing"]-->
    semanticAnalysis["Semantic analysis"]-->
    typeChecking["Type checking"]-->
    irGeneration["IR generation"]-->
    optimization["Optimization"]-->
    objectEmitting["Object file emission"]-->
    linkage["Linkage"];
```
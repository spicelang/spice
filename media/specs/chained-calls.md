# Technical Specification for chained function calls in Spice

## Implementation steps:

- [x] Parse method calls and calls on the result of arbitrary postfix expressions
- [x] Type-check method calls on receiver expressions and calls of callee expressions
- [x] Generate IR for both kinds of calls
- [x] Add tests and docs
- [ ] Call function pointer fields without parentheses (`holder.callback()`)

## Syntax

Calls are no longer restricted to name paths like `ns::fct()` or `a.b.method()`. A method can be called on the result of
any postfix expression, and the result of any postfix expression can be called, if it is a function or procedure:

```spice
counter.increment().increment().get();  // Method call on a returned reference
makeCounter().get();                    // Method call on a temporary
counters[1].get();                      // Method call on a subscript
(*counterPtr).get();                    // Method call on a parenthesized expression
getCounter()!.get();                    // Method call on the result of an error propagation
getOperation()(21);                     // Call of a returned function
(button.onClick)();                     // Call of a function pointer field
```

Grammar of the postfix unary expression:

```
postfixUnaryExpr: atomicExpr
                | postfixUnaryExpr ( LBRACKET assignExpr RBRACKET
                                   | DOT IDENTIFIER (LESS typeLst GREATER)? LPAREN argLst? RPAREN
                                   | DOT IDENTIFIER
                                   | LPAREN argLst? RPAREN
                                   | PLUS_PLUS | MINUS_MINUS | NOT );
```

A member access is a method call, if it is followed by an optional template type list and an argument list. The same
lookahead decides between generic calls and comparisons as for name paths, so `getCell().value < 5` stays a comparison.

## Implementation

Name path calls keep their existing representation. Calls on expressions reuse `ASTFctCallNode` with one of two new
children, which wraps the expression parsed so far:

- `receiverExpr`: the expression, that a method is called on. `functionNameFragments` only holds the method name.
- `calleeExpr`: the expression, that is called. It has to evaluate to a function or procedure.

`ASTFctCallNode::isExprBasedCall()` tells both apart from name path calls. Such calls are never builtin calls, so a
method named like a builtin (e.g. `panic` or `len`) is not mistaken for one.

### Type checker

The receiver or callee expression is visited before the args, matching the evaluation order. A receiver has to be a
struct or interface after dereferencing pointers and references, and becomes the `this` type of a method call, which is
resolved like for name paths. A callee has to be a function or procedure after dereferencing. Its type is stored in
`FctCallData::fctPtrType`, which both kinds of function pointer calls use for their param and return types.

A call, that is the receiver or callee of another call, uses its return value. So it neither gets an unused return value
warning, nor counts as a discarded `Result<T>` with explicit error handling.

### IR generator

The `this` pointer of a method call on an expression is the address of the receiver result. Results without an address
(e.g. a returned pointer) are stored to a temporary slot. The fat pointer of a call of an expression is obtained the
same way. Temporary receivers are destructed like any other temporary value.

## Limitations

- A function pointer field has to be wrapped in parentheses to call it (`(holder.callback)()`), because
  `holder.callback()` is a method call.
- Methods can only be called through interface pointers and references, so `(*shapePtr).area()` is rejected like a
  method call on an interface value.

# Spice Programming Language

## Grammar
The Spice grammar can be found [here](./compiler/src/grammar/Spice.g4) as a ANTLR grammar configuration file.

## Semantic analysis checks
- [x] Data type compatibility
	- [x] Function/procedure call matches function def
	- [x] Assignments
	- [x] If condition is bool
	- [x] While condition is bool
	- [x] For condition is bool
- [x] Check for usage before defined
	- [x] Variables
	- [x] Functions/Procedures

## Special language features
- Something like `"Test" * 3` is valid and will evaluate to `"TestTestTest"`
- Alternatively to the return statement in a function, you can also assign variable `result` with a value, which was automatically declared by the function header
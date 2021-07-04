# Spice Programming Language

## Grammar
The Spice grammar can be found [here](./compiler/src/grammar/Spice.g4) as a ANTLR grammar configuration file.

## Semantic analysis checks
- Data type compatibility
	- Function call matches function def
	- Procedure call matches procedure def
	- Assignments
	- If condition is bool
	- While condition is bool
	- For condition is bool
	- Foreach is list
- Check for usage of undefined
	- Variables
	- Functions
	- Procedures

## Special language features
- Something like `"Test" * 3` is valid and will evaluate to `"TestTestTest"`
- Alternatively to the return statement in a function, you can also assign variable `result` with a value, which was automatically declared by the function header
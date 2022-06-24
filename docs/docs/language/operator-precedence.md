---
title: Operator Precedence
---

The following table shows how Spice prioritizes the supported operators. The list is sorted top to bottom in descending precedence.

| Precedence | Operator | Description                          | Associativity | Modifying |
| ---------- | -------- | ------------------------------------ | ------------- | --------- |
| 1          | `a()`    | Function/procedure call              | left to right | no        |
| 1          | `.`      | Member access operator               | left to right | no        |
| 1          | `a[]`    | Subscript operator                   | left to right | no        |
| 1          | `a++`    | Postfix increment operator           | left to right | yes       |
| 1          | `a--`    | Postfix decrement operator           | left to right | yes       |
| 2          | `-`      | Unary minus operator                 | right to left | no        |
| 2          | `++a`    | Prefix increment operator            | right to left | yes       |
| 2          | `--a`    | Prefix decrement operator            | right to left | yes       |
| 2          | `!`      | Logical not operator                 | right to left | no        |
| 2          | `~`      | Bitwise not operator                 | right to left | no        |
| 2          | `*`      | De-referencing operator              | right to left | no        |
| 2          | `&`      | Referencing operator                 | right to left | no        |
| 3          | `(type)` | Type cast operator                   | right to left | no        |
| 4          | `*`      | Multiplication                       | left to right | no        |
| 4          | `/`      | Division                             | left to right | no        |
| 4          | `%`      | Remainder                            | left to right | no        |
| 5          | `+`      | Addition                             | left to right | no        |
| 5          | `-`      | Subtraction                          | left to right | no        |
| 6          | `<<`     | Bitwise left shift operator          | left to right | no        |
| 6          | `>>`     | Bitwise right shift operator         | left to right | no        |
| 7          | `<`      | Relational less operator             | left to right | no        |
| 7          | `>`      | Relational greater operator          | left to right | no        |
| 7          | `<=`     | Relational less equal operator       | left to right | no        |
| 7          | `>=`     | Relational greater equal operator    | left to right | no        |
| 8          | `==`     | Equal operator                       | left to right | no        |
| 8          | `!=`     | Not equal operator                   | left to right | no        |
| 9          | `&`      | Bitwise and operator                 | left to right | no        |
| 10         | `^`      | Bitwise or operator                  | left to right | no        |
| 11         | `|`      | Bitwise or operator                  | left to right | no        |
| 12         | `&&`     | Logical and operator                 | left to right | no        |
| 13         | `||`     | Logical or operator                  | left to right | no        |
| 14         | `a?b:c`  | Ternary conditional                  | right to left | no        |
| 15         | `=`      | Assign operator                      | right to left | yes       |
| 15         | `+=`     | Compound sum assign operator         | right to left | yes       |
| 15         | `-=`     | Compound difference assign operator  | right to left | yes       |
| 15         | `*=`     | Compound product assign operator     | right to left | yes       |
| 15         | `/=`     | Compound quotient assign operator    | right to left | yes       |
| 15         | `%=`     | Compound remainder assign operator   | right to left | yes       |
| 15         | `<<=`    | Compound shift left assign operator  | right to left | yes       |
| 15         | `>>=`    | Compound shift right assign operator | right to left | yes       |
| 15         | `&=`     | Compound bitwise and assign operator | right to left | yes       |
| 15         | `|=`     | Compound bitwise or assign operator  | right to left | yes       |
| 15         | `^=`     | Compound bitwise xor assign operator | right to left | yes       |
| 16         | `,`      | Comma                                | left to right | no        |
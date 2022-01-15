---
title: Operator Precedence
---

The following table shows how Spice prioritizes the supported operators. The list is sorted top to bottom in descending precedence.

| Precedence | Operator | Description                          |
| ---------- | -------- | ------------------------------------ |
| 1          | `.`      | Member access operator               |
| 1          | `a[]`    | Subscript operator                   |
| 1          | `a()`    | Function/procedure call              |
| 1          | `a++`    | Postfix increment operator           |
| 1          | `a--`    | Postfix decrement operator           |
| 2          | `-`      | Unary minus operator                 |
| 2          | `++a`    | Prefix increment operator            |
| 2          | `--a`    | Prefix decrement operator            |
| 2          | `!`      | Logical not operator                 |
| 2          | `~`      | Bitwise not operator                 |
| 2          | `*`      | De-referencing operator              |
| 2          | `&`      | Referencing operator                 |
| 3          | `(type)` | Type cast operator                   |
| 4          | `*`      | Multiplication                       |
| 4          | `/`      | Division                             |
| 4          | `%`      | Remainder                            |
| 5          | `+`      | Addition                             |
| 5          | `-`      | Subtraction                          |
| 6          | `<<`     | Bitwise left shift operator          |
| 6          | `>>`     | Bitwise right shift operator         |
| 7          | `<`      | Relational less operator             |
| 7          | `>`      | Relational greater operator          |
| 7          | `<=`     | Relational less equal operator       |
| 7          | `>=`     | Relational greater equal operator    |
| 8          | `==`     | Equal operator                       |
| 8          | `!=`     | Not equal operator                   |
| 9          | `&`      | Bitwise and operator                 |
| 10         | `^`      | Bitwise or operator                  |
| 11         | `|`      | Bitwise or operator                  |
| 12         | `&&`     | Logical and operator                 |
| 13         | `||`     | Logical or operator                  |
| 14         | `a?b:c`  | Ternary conditional                  |
| 15         | `=`      | Assign operator                      |
| 15         | `+=`     | Compound sum assign operator         |
| 15         | `-=`     | Compound difference assign operator  |
| 15         | `*=`     | Compound product assign operator     |
| 15         | `/=`     | Compound quotient assign operator    |
| 15         | `%=`     | Compound remainder assign operator   |
| 15         | `<<=`    | Compound shift left assign operator  |
| 15         | `>>=`    | Compound shift right assign operator |
| 15         | `&=`     | Compound bitwise and assign operator |
| 15         | `|=`     | Compound bitwise or assign operator  |
| 15         | `^=`     | Compound bitwise xor assign operator |
| 16         | `,`      | Comma                                |
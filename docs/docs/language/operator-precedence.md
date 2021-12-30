---
title: Operator Precedence
---

The following table shows how Spice prioritizes the supported operators. The list is sorted top to bottom in descending precedence.

| Precedence | Operator | Description                          |
| ---------- | -------- | ------------------------------------ |
| 1          | `.`      | Member access operator               |
| 1          | `*`      | De-referencing operator              |
| 1          | `&`      | Referencing operator                 |
| 1          | `a[]`    | Subscript operator                   |
| 1          | `a()`    | Function/procedure call              |
| 2          | `(type)` | Type cast operator                   |
| 3          | `a++`    | Postfix increment operator           |
| 3          | `a--`    | Postfix decrement operator           |
| 4          | `++a`    | Prefix increment operator            |
| 4          | `--a`    | Prefix decrement operator            |
| 4          | `!`      | Logical not operator                 |
| 5          | `*`      | Multiplication                       |
| 5          | `/`      | Division                             |
| 5          | `%`      | Remainder                            |
| 6          | `+`      | Addition                             |
| 6          | `-`      | Subtraction                          |
| 7          | `<<`     | Bitwise left shift operator          |
| 7          | `>>`     | Bitwise right shift operator         |
| 8          | `<`      | Relational less operator             |
| 8          | `>`      | Relational greater operator          |
| 8          | `<=`     | Relational less equal operator       |
| 8          | `>=`     | Relational greater equal operator    |
| 9          | `==`     | Equal operator                       |
| 9          | `!=`     | Not equal operator                   |
| 10         | `&`      | Bitwise and operator                 |
| 11         | `|`      | Bitwise or operator                  |
| 12         | `&&`     | Logical and operator                 |
| 13         | `||`     | Logical or operator                  |
| 14         | `a?b:c`  | Ternary conditional                  |
| 15         | `=`      | Assign operator                      |
| 15         | `+=`     | Compound sum assign operator         |
| 15         | `-=`     | Compound difference assign operator  |
| 15         | `*=`     | Compound product assign operator     |
| 15         | `/=`     | Compound quotient assign operator    |
| 15         | `<<=`    | Compound shift left assign operator  |
| 15         | `>>=`    | Compound shift right assign operator |
| 16         | `,`      | Comma                                |
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
| 2          | `a++`    | Postfix increment operator           |
| 2          | `a--`    | Postfix decrement operator           |
| 3          | `++a`    | Prefix increment operator            |
| 3          | `--a`    | Prefix decrement operator            |
| 3          | `!`      | Logical not operator                 |
| 4          | `*`      | Multiplication                       |
| 4          | `/`      | Division                             |
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
| 10         | `|`      | Bitwise or operator                  |
| 11         | `&&`     | Logical and operator                 |
| 12         | `||`     | Logical or operator                  |
| 13         | `a?b:c`  | Ternary conditional                  |
| 14         | `=`      | Assign operator                      |
| 14         | `+=`     | Compound sum assign operator         |
| 14         | `-=`     | Compound difference assign operator  |
| 14         | `*=`     | Compound product assign operator     |
| 14         | `/=`     | Compound quotient assign operator    |
| 14         | `<<=`    | Compound shift left assign operator  |
| 14         | `>>=`    | Compound shift right assign operator |
| 15         | `,`      | Comma                                |
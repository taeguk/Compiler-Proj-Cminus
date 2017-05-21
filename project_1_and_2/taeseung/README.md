# Compielr for C- language
## Description
Design and Development of Compiler for C-(C minus) Language  
Modify [Tiny Compiler](http://www.cs.sjsu.edu/faculty/louden/cmptext/) and develop a compiler for C-Minus(a subset of C).

## Required Program
* gcc
* flex (fast lex)
* bison

## Project 1: Design and Implementation of Lexical Analyzer - Build Scanner
### Usage
```
$ make
$ ./20141570 <filename>
```
### Test
```
$ bash lexeme_error_check.bash
```
If nothing appears after this test, there is no error in the file.

### Lexical conventions of C-
#### Keywords
 - else
 - if
 - int
 - return
 - void
 - while
#### Symbols
 - Arithmetic Symbols: \+ \- \* /
 - Logical Symbols: < <= \> \>= == \!=
 - Assignment Symbol: =
 - Parenthesis Symbols: ( ) [ ] { }
 - Comment Symbols: /* */
#### Tokens (Regular Expression)
 - letter = [a-zA-Z]
 - digit = [0-9]
 - ID = letter+
 - NUM = digit+

## Project 2: Design and Implementation of Parser
### Usage
```
$ make
$ ./20141570 <filename>
```
### Usage exmaple
```
$ ./20141570 resources/error.cm
$ ./20141570 resources/errorfree.cm
```

### Description
Using bison, make cm.h and cm.c. After that, I implement Parser.

### BNF grammar for C-
1. program -> declaration-list
2. declaration-list -> declaration-list declaration | declaration
3. declaration -> var-declaration | fun-declaration
4. var-declaration -> type-specifier ID ; | type-specifier ID [ NUM ] ;
5. type-specifie -> int | void
6. fun-declaration -> type-specifier ID ( params ) compound-stmt
7. params -> param-list | void
8. param-list -> param-list , param | param
9. param -> type-specifier ID | type-specifier ID [  ]
10. compound-stmt -> { local-declarations statement-list }
11. local-declarations -> local-declarations var-declaration | empty
12. statement-list -> statement-list statement | empty
13. statement -> expression-stmt | compound-stmt | selection-stmt |
 iteration-stmt | return-stmt
14. expression-stmt -> expression ; | ;
15. selection-stmt -> if ( expression ) statement  
                     | if ( expression ) statement else statement
16. iteration-stmt -> while ( expression ) statement
17. return-stmt -> return ; | return expression;
18. expression -> var = expression | simple-expression
19. var -> ID | ID [ expression ]
20. simple-expression -> additive-expression relop additive-expression |  
                         additive-expression
21. relop -> <= | < | > | >= | == | !=
22. additive-expression -> additive-expression addop term | term
23. addop -> + | -
24. term -> term mulop factor | factor
25. mulop -> * | /
26. factor -> ( expression ) | var | call | NUM
27. call -> ID ( args )
28. args -> arg-list | empty
29. arg-list -> arg-list , expression | expression

## Author
 * Taeseung Lee

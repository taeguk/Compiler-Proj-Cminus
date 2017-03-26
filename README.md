# Compielr for C- language
## Description
Design and Development of Compiler for C-(C minus) Language  
Modify [Tiny Compiler](http://www.cs.sjsu.edu/faculty/louden/cmptext/) and develop a compiler for C-Minus(a subset of C).

## Required Program
* gcc
* flex (fast lex)

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

## Author
 * Taeseung Lee

/****************************************************/
/* File: cm.y                                       */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void); // added 11/2/11 to ensure no conflict with lex

%}

%token ID NUM
%token LE_OP GE_OP EQ_OP NE_OP
%token INT VOID
%token IF ELSE WHILE RETURN
%token ERROR 

%%
/* 1 */
program: declaration_list
       ;
/* 2 */
declaration_list: declaration_list declaration
                | declaration
                ;
/* 3 */
declaration: var_declaration
           | fun_declaration
           ;
/* 4 */
var_declaration: type_specifier ID ';'
               | type_specifier ID '[' NUM ']' ';'
               ;
/* 5 */
type_specifier: INT | VOID
              ;
/* 6 */
fun_declaration: type_specifier ID '(' params ')' compound_stmt
               ;
/* 7 */
params: param_list
      | VOID
      ;
/* 8 */
param_list: param_list ',' param
          | params
          ;
/* 9 */
param: type_specifier ID
     | type_specifier ID '[' ']'
     ;
/* 10 */
compound_stmt: '{' local_declarations statement_list '}'
             ;
/* 11 */
local_declarations: local_declarations var_declaration
                  |
                  ;
/* 12 */
statement_list: statement_list statement
              | 
              ;
/* 13 */
statement: expression_stmt
         | compound_stmt
         | selection_stmt
         | iteration_stmt
         | return_stmt
         ;
/* 14 */
expression_stmt: expression ';'
               | ';'
               ;
/* 15 */
selection_stmt: IF '(' expression ')' statement
              | IF '(' expression ')' statement ELSE statement
              ;
/* 16 */
iteration_stmt: WHILE '(' expression ')' statement_list
              ;
/* 17 */
return_stmt: RETURN ';'
           | RETURN expression ';'
           ;
/* 18 */
expression: var '=' expression
          | simple_expression
          ;
/* 19 */
var: ID
   | ID '[' expression ']'
   ;
/* 20 */
simple_expression: additive_expression relop additive_expression
                 | additive_expression
                 ;
/* 21 */
relop: LE_OP
     | '<'
     | '>'
     | GE_OP
     | EQ_OP
     | NE_OP
     ;
/* 22 */
additive_expression: additive_expression addop term
                   | term
                   ;
/* 23 */
addop: '+'
     | '-'
     ;
/* 24 */
term: term mulop factor
    | factor
    ;
/* 25 */
mulop: '*'
     | '/'
     ;
/* 26 */
factor: '(' expression ')'
      | var
      | call
      | NUM
      ;
/* 27 */
call: ID '(' args ')'
    ;
/* 28 */
args: arg_list
    |
    ;
/* 29 */
arg_list: arg_list ',' expression
        | expression
        ;




%%

int yyerror(char * message)
{
  fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;

  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{
  return getToken();
}

TreeNode * parse(void)
{
  yyparse();
  return savedTree;
}


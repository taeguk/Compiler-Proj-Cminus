/****************************************************/
/* File: cm.y                                       */
/* The C- Yacc/Bison specification file             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Modified by Kwanghee Choi                        */
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

int yylex(void);
int yyerror(char*);
%}

%token ID NUM
%token ELSE IF INT RETURN VOID WHILE
%token PLUS MINUS TIMES OVER
%token LT LE GT GE EQ NE
%token ASSIGN SEMI COMMA
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token ENDFILE ERROR

%start program

%% /* Grammar for C- */

program
        : declaration_list
        ;

declaration_list
        : declaration_list declaration
        | declaration
        ;

declaration
        : var_declaration
        | fun_declaration
        ;

var_declaration
        : type_specifier ID SEMI
        | type_specifier ID LBRACK NUM RBRACK SEMI
        ;

type_specifier
        : INT
        | VOID
        ;

fun_declaration
        : type_specifier ID LPAREN params RPAREN compound_stmt
        ;

params
        : param_list
        | VOID
        ;

param_list
        : param_list COMMA param
        | param
        ;

param
        : type_specifier ID
        | type_specifier ID LBRACK RBRACK
        ;

compound_stmt
        : LBRACE local_declarations statement_list RBRACE
        ;

local_declarations
        : local_declarations var_declaration
        | /* empty */
        ;

statement_list
        : statement_list statement
        | /* empty */
        ;

statement
        : expression_stmt
        | compound_stmt
        | selection_stmt
        | iteration_stmt
        | return_stmt
        ;

expression_stmt
        : expression SEMI
        | SEMI
        ;

selection_stmt
        : IF LPAREN expression RPAREN statement
        | IF LPAREN expression RPAREN statement ELSE statement
        ;

iteration_stmt
        : WHILE LPAREN expression RPAREN statement
        ;

return_stmt
        : RETURN SEMI
        | RETURN expression SEMI
        ;

expression
        : var ASSIGN expression | simple_expression
        ;

var
        : ID
        | ID LBRACK expression RBRACK
        ;

simple_expression
        : additive_expression relop additive_expression
        | additive_expression
        ;

relop
        : LT
        | LE
        | GT
        | GE
        | EQ
        | NE
        ;

additive_expression
        : additive_expression addop term | term
        ;

addop
        : PLUS
        | MINUS
        ;

term
        : term mulop factor
        | factor
        ;

mulop
        : TIMES
        | OVER
        ;

factor
        : LPAREN expression RPAREN
        | var_declaration
        | call
        | NUM
        ;

call
        : ID LPAREN args RPAREN
        ;

args
        : arg_list
        | /* empty */
        ;

arg_list
        : arg_list COMMA expression
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

TreeNode * parse(void)
{
  yyparse();
  return savedTree;
}


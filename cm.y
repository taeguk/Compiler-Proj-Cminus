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
          { savedTree = $1; }
        ;

declaration_list
        : declaration_list declaration
          { // $$ = addSibling($1, $2);
            /*
            YYSTYPE node = $1;
            if(node == NULL) $$ = $2;
            else {
              while(node->sibling != NULL) node = node->sibling;
              node->sibling = $2;
              $$ = $1;
            }
            */
          }
        | declaration
          { $$ = $1; }
        ;

declaration
        : var_declaration
          { $$ = $1; }
        | fun_declaration
          { $$ = $1; }
        ;

var_declaration
        : type_specifier ID SEMI
          { // $$ = newVariableDeclarationNode(type_specifier, ID, SINGLE, 0); }
        | type_specifier ID LBRACK NUM RBRACK SEMI
          { // $$ = newVariableDeclarationNode(type_specifier, ID, ARRAY, NUM); }
          /* exception handling for num! */
        ;

type_specifier
        : INT
          { $$ = INT; }
        | VOID
          { $$ = VOID; }
        ;

fun_declaration
        : type_specifier ID LPAREN params RPAREN compound_stmt
          { // $$ = newFunctionDeclaratonNode(type_specifier, ID, params, compound_stmt); }
        ;

params
        : param_list
          { $$ = $1; }
        | VOID
          { $$ = NULL; }
        ;

param_list
        : param_list COMMA param
          { // $$ = addSibling($1, $3); }
        | param
          { // $$ = newParameterListNode(param); }
        ;

param
        : type_specifier ID
          { // $$ = newParameterNode(type_specifier, SINGLE, ID); }
        | type_specifier ID LBRACK RBRACK
          { // $$ = newParameterNode(type_specifier, ARRAY, ID); }
        ;

compound_stmt
        : LBRACE local_declarations statement_list RBRACE
          { // $$ = newCompoundStatementNode(local_declarations, statement_list); }
        ;

local_declarations
        : local_declarations var_declaration
          { // $$ = addSibling($1, $2); }
        | /* empty */
          { $$ = NULL; }
        ;

statement_list
        : statement_list statement
          { // $$ = addSibling($1, $2); }
        | /* empty */
          { $$ = NULL; }
        ;

statement
        : expression_stmt
          { $$ = $1; }
        | compound_stmt
          { $$ = $1; }
        | selection_stmt
          { $$ = $1; }
        | iteration_stmt
          { $$ = $1; }
        | return_stmt
          { $$ = $1; }
        ;

expression_stmt
        : expression SEMI
          { // $$ = newExpressionStatementNode(expression); }
        | SEMI
          { // $$ = newExpressionStatementNode(NULL); }
        ;

selection_stmt
        : IF LPAREN expression RPAREN statement
          { // $$ = newSelectionStatementNode(expression, statement, NULL); }
        | IF LPAREN expression RPAREN statement ELSE statement
          { // $$ = newSelectionStatementNode(expression, statement, statement); }
        ;

iteration_stmt
        : WHILE LPAREN expression RPAREN statement
          { // $$ = newIterationStatementNode(expression, statement); }
        ;

return_stmt
        : RETURN SEMI
          { // $$ = newReturnStatementNode(NULL); }
        | RETURN expression SEMI
          { // $$ = newReturnStatementNode(expression); }
        ;

expression
        : var ASSIGN expression
         { // $$ = newAssignExpressionNode(var, expression); }
        | simple_expression
         { $$ = $1; }
        ;

var
        : ID
          { // $$ = newVariableNode(ID, NULL); }
        | ID LBRACK expression RBRACK
          { // $$ = newVariableNode(ID, expression); }
        ;

simple_expression
        : additive_expression relop additive_expression
          { // $$ = newComparisonExpressionNode(additive_expression, relop, additive_expression); }
        | additive_expression
          { $$ = $1; }
        ;

relop
        : LT
          { $$ = LT; }
        | LE
          { $$ = LE; }
        | GT
          { $$ = GT; }
        | GE
          { $$ = GE; }
        | EQ
          { $$ = EQ; }
        | NE
          { $$ = NE; }
        ;

additive_expression
        : additive_expression addop term
          { // $$ = newAdditiveExpressionNode(additive_expression, addop, term); }
        | term
          { $$ = $1; }
        ;

addop
        : PLUS
          { $$ = PLUS; }
        | MINUS
          { $$ = MINUS; }
        ;

term
        : term mulop factor
          { // $$ = newMultiplicativeExpressionNode(additive_expression, addop, term); }
        | factor
          { $$ = $1; }
        ;

mulop
        : TIMES
          { $$ = TIMES; }
        | OVER
          { $$ = OVER; }
        ;

factor
        : LPAREN expression RPAREN
          { $$ = $2; }
        | var_declaration
          { $$ = $1; }
        | call
          { $$ = $1; }
        | NUM
          { // $$ = newConstantExpressionNode(NUM); }
        ;

call
        : ID LPAREN args RPAREN
          { // $$ = newCallNode(ID, args); }
        ;

args
        : arg_list
          { $$ = $1; }
        | /* empty */
          { $$ = NULL; }
        ;

arg_list
        : arg_list COMMA expression
          { $$ = addSibling($1, $3); }
        | expression
          { $$ = $1; }
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


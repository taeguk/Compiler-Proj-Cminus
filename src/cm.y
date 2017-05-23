/****************************************************/
/* File: cm.y                                       */
/* The C- Yacc/Bison specification file             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Modified by Kwanghee Choi                        */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */
/* because cm.tab.c is located in ./build/ directory */
#include "../globals.h"
#include "../util.h"
#include "../scan.h"
#include "../parse.h"

#define YYSTYPE TreeNode *

static TreeNode * savedTree; /* stores syntax tree for later return */

static int yylex(void);
int yyerror(char*);
%}

%token MINIMUM_TOKEN
%token ID NUM
%token ELSE IF INT RETURN VOID WHILE
%token PLUS MINUS TIMES OVER
%token LT LE GT GE EQ NE
%token ASSIGN SEMI COMMA
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token ENDFILE ERROR
%token MAXIMUM_TOKEN

%nonassoc LOWER_ELSE
%nonassoc ELSE

%start program

%% /* Grammar for C- */

program
        : declaration_list
          { savedTree = $1; }
        ;

declaration_list
        : declaration_list declaration
          { $$ = addSibling($1, $2); }
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
        : type_specifier _id SEMI
          { $$ = newVariableDeclarationNode($1, $2); }
        | type_specifier _id LBRACK _num RBRACK SEMI
          { $$ = newArrayDeclarationNode($1, $2, $4); }
        ;

type_specifier
        : INT
          { $$ = newTokenTypeNode(INT); }
        | VOID
          { $$ = newTokenTypeNode(VOID); }

fun_declaration
        : type_specifier _id LPAREN params RPAREN compound_stmt
          { $$ = newFunctionDeclarationNode($1, $2, $4, $6); }
        ;

params
        : param_list
          { $$ = $1; }
        | VOID
          { $$ = NULL; }
        ;

param_list
        : param_list COMMA param
          { $$ = addSibling($1, $3); }
        | param
          { $$ = $1; }
        ;

param
        : type_specifier _id
          { $$ = newVariableParameterNode($1, $2); }
        | type_specifier _id LBRACK RBRACK
          { $$ = newArrayParameterNode($1, $2); }
        ;

compound_stmt
        : LBRACE local_declarations statement_list RBRACE
          { $$ = newCompoundStatementNode($2, $3); }
        ;

local_declarations
        : local_declarations var_declaration
          { $$ = addSibling($1, $2); }
        | /* empty */
          { $$ = NULL; }
        ;

statement_list
        : statement_list statement
          { $$ = addSibling($1, $2); }
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
          { $$ = newExpressionStatementNode($1); }
        | SEMI
          { $$ = NULL; }
        ;

selection_stmt
        : IF LPAREN expression RPAREN statement
          { $$ = newSelectionStatementNode($3, $5, NULL); }
          %prec LOWER_ELSE
        | IF LPAREN expression RPAREN statement ELSE statement
          { $$ = newSelectionStatementNode($3, $5, $7); }
        ;

iteration_stmt
        : WHILE LPAREN expression RPAREN statement
          { $$ = newIterationStatementNode($3, $5); }
        ;

return_stmt
        : RETURN SEMI
          { $$ = newReturnStatementNode(NULL); }
        | RETURN expression SEMI
          { $$ = newReturnStatementNode($2); }
        ;

expression
        : var ASSIGN expression
         { $$ = newAssignExpressionNode($1, $3); }
        | simple_expression
         { $$ = $1; }
        ;

var
        : _id
          { $$ = $1; }
        | _id LBRACK expression RBRACK
          { $$ = newArrayNode($1, $3); }
        ;

simple_expression
        : additive_expression relop additive_expression
          { $$ = newComparisonExpressionNode($1, $2, $3); }
        | additive_expression
          { $$ = $1; }
        ;

relop
        : LT
          { $$ = newTokenTypeNode(LT); }
        | LE
          { $$ = newTokenTypeNode(LE); }
        | GT
          { $$ = newTokenTypeNode(GT); }
        | GE
          { $$ = newTokenTypeNode(GE); }
        | EQ
          { $$ = newTokenTypeNode(EQ); }
        | NE
          { $$ = newTokenTypeNode(NE); }
        ;

additive_expression
        : additive_expression addop term
          { $$ = newAdditiveExpressionNode($1, $2, $3); }
        | term
          { $$ = $1; }
        ;

addop
        : PLUS
          { $$ = newTokenTypeNode(PLUS); }
        | MINUS
          { $$ = newTokenTypeNode(MINUS); }

term
        : term mulop factor
          { $$ = newMultiplicativeExpressionNode($1, $2, $3); }
        | factor
          { $$ = $1; }
        ;

mulop
        : TIMES
          { $$ = newTokenTypeNode(TIMES); }
        | OVER
          { $$ = newTokenTypeNode(OVER); }
        ;

factor
        : LPAREN expression RPAREN
          { $$ = $2; }
        | var
          { $$ = $1; }
        | call
          { $$ = $1; }
        | _num
          { $$ = $1; }
        ;

call
        : _id LPAREN args RPAREN
          { $$ = newCallNode($1, $3); }
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

_id
        : ID
          { $$ = newVariableNode(tokenString); }
        ;

_num
        : NUM
          { $$ = newConstantNode(tokenString); }
        ;

%%

int yyerror(char * message)
{
  fprintf(listing,
          "Syntax error at line %d: %s\n",
          lineno,
          message);
  fprintf(listing, "Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

static int yylex(void)
{
  TokenType tok = getToken();
  if (tok == ENDFILE) return 0;
  if (tok == ERROR)
    {
      fprintf(listing,
              "Lexical analyze error at line %d\n",
              lineno);
      fprintf(listing,
              "Current token: %s",
              tokenString);
      Error = TRUE;
      return 0;
    }
  return tok;
}

TreeNode * parse(void)
{
  yyparse();
  return savedTree;
}


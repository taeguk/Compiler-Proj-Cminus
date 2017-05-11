/****************************************************/
/* File: cm.y                                       */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "../globals.h"
#include "../util.h"
#include "../scan.h"
#include "../parse.h"

#define YYSTYPE TreeNode *
static char * saved_name; /* for use in assignments as a name*/
static int saved_number; /* for use in assignments as a number */
static int saved_line_no;  /* ditto */
static TreeNode * saved_tree; /* stores syntax tree for later return */
static int yylex(void); // added 11/2/11 to ensure no conflict with lex
int yyerror(char * message);

%}

%token ELSE IF INT VOID WHILE RETURN
%token ID NUM
%token LT_OP GT_OP LE_OP GE_OP EQ_OP NE_OP ASSIGN SEMI
%token LPAREN RPAREN LBRACE RBRACE
%token ERROR ENDFILE
%right RPAREN ELSE
%%
/* 1 */
program : declaration_list
        {
          saved_tree = $1;
        }
       ;
/* 2 */
declaration_list  : declaration_list declaration
                  {
                    YYSTYPE t = $1;
                    if (t)
                    {
                      while (t->sibling)
                        t = t->sibling;
                      t->sibling = $2;
                      $$ = $1;
                    }
                    else
                      $$ = $2;
                  }
                  | declaration
                  {
                    $$ = $1;
                  }
                  ;
/* 3 */
declaration : var_declaration
            {
              $$ = $1;
            }
            | fun_declaration
            {
              $$ = $1;
            }
           ;

number: NUM
      {
        saved_number = atoi(token_string);
        saved_line_no = lineno;
      }
      ;
identifier: ID
          {
            $$ = new_exp_node(IdK);
            $$->attr.name = copy_string(token_string);
            saved_line_no = lineno;
          }
          ;


/* 4 */
var_declaration : type_specifier identifier SEMI
                {
                  $$ = new_decl_node(VarK);
                  $$->child[0] = $1;
                  $$->child[1] = $2;
                  $$->lineno = lineno;
                }
                | type_specifier identifier '[' number ']' SEMI
                {
                  $$ = new_decl_node(ArrK);
                  $$->child[0] = $1;
                  $$->child[1] = $2;
                  $$->lineno = lineno;
                  $$->attr.arr_attr.size = saved_number;
                }
               ;
/* 5 */
type_specifier: INT
              {
                $$ = new_type_node(TypeNameK);
                $$->attr.type = INT;
              }
              | VOID
              {
                $$ = new_type_node(TypeNameK);
                $$->attr.type = VOID;
              }
              ;
/* 6 */
fun_declaration : type_specifier identifier 
                    {
                      $$ = new_decl_node(FuncK);
                      $$->lineno = lineno;
                      $$->child[0] = $2;
                    }
                  LPAREN params RPAREN compound_stmt
                    {
                      $$ = $3;
                      $$->child[0] = $1; /* type */
                      $$->child[1] = $5; /* parameters */
                      $$->child[2] = $7; /* body */
                    }
               ;
/* 7 */
params: param_list
      {
        $$ = $1;
      }
      | VOID
      {
        $$ = new_type_node(TypeNameK);
        $$->attr.type = VOID;
      }
      ;
/* 8 */
param_list: param_list ',' param
          {
            YYSTYPE t = $1;
            /* TODO: Check NULL */
            if (t != NULL)
            {
              while (t->sibling)
                t = t->sibling;
              t->sibling = $3;
              $$ = $1;
            }
            else
              $$ = $3;
          }
          | param
              {
                $$ = $1;
              }
          ;
/* 9 */
param : type_specifier identifier
      {
        $$ = new_param_node(ParamVarK);
        $$->child[0] = $1; /* type */
        $$->child[1] = $2;
      }
      | type_specifier identifier '[' ']'
      {
        $$ = new_param_node(ParamArrK);
        $$->child[0] = $1; /* type */
        $$->child[1] = $2;
      }
     ;
/* 10 */
compound_stmt: LBRACE local_declarations statement_list RBRACE
                {
                  $$ = new_stmt_node(CompK);
                  $$->child[0] = $2;
                  $$->child[1] = $3;
                }
             ;
/* 11 */
local_declarations: local_declarations var_declaration
                      {
                        YYSTYPE t = $1;
                        if (t != NULL)
                        {
                          while (t->sibling)
                            t = t->sibling;
                          t->sibling = $2;
                          $$ = $1;
                        }
                        else
                          $$ = $2;
                      }
                  | /* empty */
                  {
                    $$ = NULL;
                  }
                  ;
/* 12 */
statement_list: statement_list statement
                  {
                    YYSTYPE t = $1;
                    if (t)
                    {
                      while (t->sibling)
                        t = t->sibling;
                      t->sibling = $2;
                      $$ = $1;
                    }
                    else
                      $$ = $2;
                  }
              | /* empty */
                {
                  $$ = NULL;
                }
              ;
/* 13 */
statement : expression_stmt
              {
                $$ = $1;
              }
         | compound_stmt
              {
                $$ = $1;
              }
         | selection_stmt
              {
                $$ = $1;
              }
        | iteration_stmt
              {
                $$ = $1;
              }
        | return_stmt
              {
                $$ = $1;
              }
      ;
/* 14 */
expression_stmt: expression SEMI
                  {
                    $$ = $1;
                  }
               | SEMI
                  {
                    $$ = NULL;
                  }
               ;
/* 15 */
selection_stmt: IF LPAREN expression RPAREN statement
                  {
                    $$ = new_stmt_node(IfK);
                    $$->child[0] = $3;
                    $$->child[1] = $5;
                  }
              | IF LPAREN expression RPAREN statement ELSE statement
                  {
                    $$ = new_stmt_node(IfK);
                    $$->child[0] = $3;
                    $$->child[1] = $5;
                    $$->child[2] = $7;
                  }
              ;
/* 16 */
iteration_stmt: WHILE LPAREN expression RPAREN statement
                  {
                    $$ = new_stmt_node(IterK);
                    $$->child[0] = $3;
                    $$->child[1] = $5;
                  }
              ;
/* 17 */
return_stmt: RETURN SEMI
              {
                $$ = new_stmt_node(RetK);
              }
           | RETURN expression SEMI
              {
                $$ = new_stmt_node(RetK);
                $$->child[0] = $2;
              }
           ;
/* 18 */
expression: var ASSIGN expression
              {
                $$ = new_exp_node(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
          | simple_expression
              {
                $$ = $1;
              }
          ;
/* 19 */
var: identifier
      {
        $$ = $1;
      }
   | identifier '[' expression ']'
      {
        $$ = new_exp_node(ArrIdK);
        $$->attr.arr_attr.name = $1->attr.name;
        $$->child[1] = $3;
        free($1);
      }
   ;
/* 20 */
simple_expression: additive_expression relop additive_expression
                    {
                      $$ = $2;
                      $$->child[0] = $1;
                      $$->child[1] = $3;
                    }
                 | additive_expression
                    {
                      $$ = $1;
                    }
                 ;
/* 21 */
relop: LE_OP
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = LE_OP;
        }
     | LT_OP
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = LT_OP;
        }
     | GT_OP
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = GT_OP;
        }
     | GE_OP
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = GE_OP;
        }
     | EQ_OP
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = EQ_OP;
        }
     | NE_OP
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = NE_OP;
        }
     ;
/* 22 */
additive_expression: additive_expression addop term
                      {
                        $$ = $2;
                        $$->child[0] = $1;
                        $$->child[1] = $3;
                      }
                   | term
                      {
                        $$ = $1;
                      }
                   ;
/* 23 */
addop: '+'
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = '+';
        }
     | '-'
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = '-';
        }
     ;
/* 24 */
term: term mulop factor
        {
          $$ = $2;
          $$->child[0] = $1;
          $$->child[1] = $3;
        }
    | factor
        {
          $$ = $1;
        }
    ;
/* 25 */
mulop: '*'
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = '*';
        }
     | '/'
        {
          $$ = new_exp_node(OpK);
          $$->attr.op = '/';
        }
     ;
/* 26 */
factor: LPAREN expression RPAREN
          {
            $$ = $2;
          }
      | var
          {
            $$ = $1;
          }
      | call
          {
            $$ = $1;
          }
      | number
          {
            $$ = new_exp_node(ConstK);
            $$->attr.val = saved_number;
          }
      ;
/* 27 */
call: identifier
        {
          $$ = new_exp_node(CallK);
          $$->attr.name = $1->attr.name;
          free($1);
        }
      LPAREN args RPAREN
        {
          $$ = $2;
          $$->child[0] = $4;
        }
      ;
/* 28 */
args: arg_list
        {
          $$ = $1;
        }
    | /* empty */
        {
          $$ = NULL;
        }
    ;
/* 29 */
arg_list: arg_list ',' expression
            {
              YYSTYPE t = $1;
              if (t)
              {
                while (t->sibling)
                  t = t->sibling;
                t->sibling = $3;
                $$ = $1;
              }
              else
                $$ = $3;
            }
        | expression
            {
                $$ = $1;
            }
        ;
%%

int yyerror(char * message)
{
  fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar, token_string);
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
  return saved_tree;
}


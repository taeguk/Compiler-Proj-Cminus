/*****************************************************/
/* File: cminus.y                                    */
/* The C- language Yacc/Bison specification file     */
/* Compiler Construction: Principles and Practice    */
/* Kenneth C. Louden & Taeguk Kwon (http://taeguk.me)*/
/*****************************************************/
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

static int yylex(void);
%}

/* reserved words */
%token IF ELSE INT RETURN VOID WHILE
/* multicharacter tokens */
%token ID NUM 
/* special symbols */
%token PLUS MINUS TIMES OVER 
%token LT LTEQ GT GTEQ EQ NOTEQ 
%token ASSIGN 
%token SEMI COMMA 
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET  

%token ERROR
%token COMMENT COMMENT_ERROR

/* for resolving shift-reduce conflict */
%nonassoc RPAREN 
%nonassoc ELSE

%start program

%% /* Grammar for C- language */

program     : decl_list { savedTree = $1; }
            ;
save_id     : ID { savedName = copyString(tokenString); }
            ;
decl_list   : decl_list decl
                { YYSTYPE t = $1;
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
            | decl
                { $$ = $1; }
            ;
decl        : var_decl { $$ = $1; }
            | func_decl { $$ = $1; }
            ;
var_decl    : type_spec save_id SEMI
                { $$ = newDeclNode(VarK);
                  $$->attr.name = savedName;
                  $$->child[0] = $1;
                }
            | type_spec save_id 
                { $$ = newDeclNode(VarArrK);
                  $$->attr.arrAttr.name = savedName;
                  $$->child[0] = $1;
                }
              LBRACKET NUM 
                { $$ = $3;
                  $$->attr.arrAttr.len = atoi(tokenString); 
                }
              RBRACKET SEMI
                { $$ = $6; }
            ;
type_spec   : INT 
                { $$ = newTypeNode(TypeSpecK);
                  $$->attr.typeSpec = INT;
                }
            | VOID 
                { $$ = newTypeNode(TypeSpecK);
                  $$->attr.typeSpec = VOID;
                }
            ;
func_decl   : type_spec save_id 
                { $$ = newDeclNode(FuncK);
                  $$->attr.name = savedName;
                }
              LPAREN params RPAREN comp_stmt
                { $$ = $3;
                  $$->child[0] = $1;
                  $$->child[1] = $5;
                  $$->child[2] = $7;
                }
            ;
params      : param_list
                { $$ = $1; }
            | VOID
                { $$ = newTypeNode(TypeSpecK);
                  $$->attr.typeSpec = VOID;
                }
            ;
param_list  : param_list COMMA param
                { YYSTYPE t = $1;
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
                { $$ = $1; }
            ;
param       : type_spec save_id
                { $$ = newDeclNode(VarK);
                  $$->attr.name = savedName;
                  $$->child[0] = $1;
                }
            | type_spec save_id LBRACKET RBRACKET
                { $$ = newDeclNode(VarArrK);
                  $$->attr.arrAttr.name = savedName;
                  $$->attr.arrAttr.len = -1;
                }
            ;
comp_stmt   : LBRACE local_decls stmt_list RBRACE
                { $$ = newStmtNode(CompK);
                  $$->child[0] = $2;
                  $$->child[1] = $3;
                }
            ;
local_decls : local_decls var_decl
                { YYSTYPE t = $1;
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
            | empty
                { $$ = $1; }
            ;
stmt_list   : stmt_list stmt
                { YYSTYPE t = $1;
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
            | empty
                { $$ = $1; }
            ;
stmt        : expr_stmt   { $$ = $1; }
            | comp_stmt   { $$ = $1; }
            | select_stmt { $$ = $1; }
            | iter_stmt   { $$ = $1; }
            | ret_stmt    { $$ = $1; }
            ;
expr_stmt   : expr SEMI { $$ = $1; }
            | SEMI      { $$ = NULL; }
            ;
select_stmt : IF LPAREN expr RPAREN stmt
                { $$ = newStmtNode(SelectK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                }
            | IF LPAREN expr RPAREN stmt ELSE stmt
                { $$ = newStmtNode(SelectK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                  $$->child[2] = $7;
                }
            ;
iter_stmt   : WHILE LPAREN expr RPAREN stmt
                { $$ = newStmtNode(IterK);
                  $$->child[0] = $3;
                  $$->child[1] = $5;
                }
            ;
ret_stmt    : RETURN SEMI
                { $$ = newStmtNode(RetK); }
            | RETURN expr SEMI
                { $$ = newStmtNode(RetK);
                  $$->child[0] = $2;
                }
            ;
expr        : var ASSIGN expr
                { $$ = newExpNode(AssignK);
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | simple_expr
                { $$ = $1; }
            ;
var         : save_id
                { $$ = newExpNode(IdK);
                  $$->attr.name = savedName;
                }
            | save_id 
                { $$ = newExpNode(IdArrK);
                  $$->attr.name = savedName;
                }
              LBRACKET expr RBRACKET
                { $$ = $2;
                  $$->child[0] = $4; 
                }
            ;
simple_expr : add_expr relop add_expr
                { $$ = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | add_expr
                { $$ = $1; }
            ;
relop       : LTEQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = LTEQ;
                }
            | LT
                { $$ = newExpNode(OpK);
                  $$->attr.op = LT;
                }
            | GT
                { $$ = newExpNode(OpK);
                  $$->attr.op = GT;
                }
            | GTEQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = GTEQ;
                }
            | EQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = EQ;
                }
            | NOTEQ
                { $$ = newExpNode(OpK);
                  $$->attr.op = NOTEQ;
                }
            ;
add_expr    : add_expr addop term
                { $$ = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | term
                { $$ = $1; }
            ;
addop       : PLUS
                { $$ = newExpNode(OpK);
                  $$->attr.op = PLUS;
                }
            | MINUS
                { $$ = newExpNode(OpK);
                  $$->attr.op = MINUS;
                }
            ;
term        : term mulop factor
                { $$ = $2;
                  $$->child[0] = $1;
                  $$->child[1] = $3;
                }
            | factor
                { $$ = $1; }
            ;
mulop       : TIMES
                { $$ = newExpNode(OpK);
                  $$->attr.op = TIMES;
                }
            | OVER
                { $$ = newExpNode(OpK);
                  $$->attr.op = OVER;
                }
            ;
factor      : LPAREN expr RPAREN
                { $$ = $2; }
            | var
                { $$ = $1; }
            | call
                { $$ = $1; }
            | NUM
                { $$ = newExpNode(ConstK);
                  $$->attr.val = atoi(tokenString); /* TODO: error handling */
                }
            ;
call        : save_id
                { $$ = newExpNode(CallK);
                  $$->attr.name = savedName;
                }
              LBRACE args RBRACE
                { $$ = $2;
                  $$->child[0] = $4; 
                }
            ;
args        : arg_list  { $$ = $1; }
            | empty     { $$ = $1; }
            ;
arg_list    : arg_list COMMA expr
                { YYSTYPE t = $1;
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
            | expr
                { $$ = $1; }
            ;
empty       : /* empty */ { $$ = NULL; }
            ;
%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
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
/*
  int token = getToken();
  fprintf(listing,"Debug: %c %s\n",yychar, tokenString);
  printToken(yychar,tokenString);
  return token;
  */

  int token = getToken();
  while (token == COMMENT)
    token = getToken();
  return token;
}

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}


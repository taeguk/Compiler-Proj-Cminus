/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for C- compiler            */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Modified by Kwanghee Choi                        */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER
#include "build/cm.tab.h"
#endif

#include "log_debugger.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 8

typedef int TokenType;

/*
// TokenType values

// book-keeping tokens
ENDFILE ERROR

// multicharacter tokens
ID NUM

// reserved words
ELSE IF INT RETURN VOID WHILE

// special symbols
PLUS(+) MINUS(-) TIMES(*) OVER(/)
LT(<) LE (<=) GT(>) GE(>=) EQ(==) NE(!=)
ASSIGN(=) SEMI(;) COMMA(,)
LPAREN(() RPAREN()) LBRACK([) RBRACK(]) LBRACE({) RBRACE(})

*/

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {
    ErrorK,

    VariableDeclarationK,
    ArrayDeclarationK,
    FunctionDeclarationK,

    VariableParameterK,
    ArrayParameterK,

    CompoundStatementK,
    ExpressionStatementK,
    SelectionStatementK,
    IterationStatementK,
    ReturnStatementK,

    AssignExpressionK,
    ComparisonExpressionK,
    AdditiveExpressionK,
    MultiplicativeExpressionK,

    VariableK,
    ArrayK,
    CallK,

    ConstantK,
    TokenTypeK,
} NodeKind;

/* ExpType is used for type checking */
/* TODO: naming issue */
typedef enum {
    NotResolvedT,
    ErrorT,
    NoneT,
    VoidT,
    IntT,
    IntArrayT,
    FuncT,
} NodeType;

typedef struct
{
  NodeType nodeType;
  union {
      // VariableDeclarationK
      struct {
      } intInfo;

      // ArrayDeclarationK
      struct {
          int len;
      } arrInfo;

      // FunctionDeclarationK
      struct {
          NodeType retType;
          int len;
          NodeType * paramTypeList;
      } funcInfo;
  } attr;
} SymbolInfo;

typedef struct treeNode {
  struct treeNode *sibling;
  int lineno;
  NodeKind nodeKind;
  NodeType nodeType;
  SymbolInfo * symbolInfo;

  union {
      // VariableDeclarationK
      struct {
          struct treeNode *type_spec;
          struct treeNode *_id;
      } varDecl;

      // ArrayDeclarationK
      struct {
          struct treeNode *type_spec;
          struct treeNode *_id;
          struct treeNode *_num;
      } arrDecl;

      // FunctionDeclarationK
      struct {
          struct treeNode *type_spec;
          struct treeNode *_id;
          struct treeNode *params;
          struct treeNode *cmpd_stmt;
      } funcDecl;

      // VariableParameterK
      struct {
          struct treeNode *type_spec;
          struct treeNode *_id;
      } varParam;

      // ArrayParameterK
      struct {
          struct treeNode *type_spec;
          struct treeNode *_id;
      } arrParam;
      
      // CompoundStatementK
      struct {
          struct treeNode *local_decl;
          struct treeNode *stmt_list;
      } cmpdStmt;

      // ExpressionStatementK
      struct {
          struct treeNode *expr;
      } exprStmt;

      // SelectionStatementK
      struct {
          struct treeNode *expr;
          struct treeNode *if_stmt;
          struct treeNode *else_stmt;
      } selectStmt;

      // IterationStatementK
      struct {
          struct treeNode *expr;
          struct treeNode *loop_stmt;
      } iterStmt;

      // ReturnStatementK
      struct {
          struct treeNode *expr;
      } retStmt;

      // AssignExpressionK
      struct {
          struct treeNode *expr;
          struct treeNode *_var;
      } assignStmt;

      // ComparisonExpressionK
      struct {
          struct treeNode *lexpr;
          struct treeNode *op;
          struct treeNode *rexpr;
      } cmpExpr;

      // AdditiveExpressionK
      struct {
          struct treeNode *lexpr;
          struct treeNode *op;
          struct treeNode *rexpr;
      } addExpr;

      // MultiplicativeExpressionK
      struct {
          struct treeNode *lexpr;
          struct treeNode *op;
          struct treeNode *rexpr;
      } multExpr;

      // ArrayK
      struct {
          struct treeNode *_id;
          struct treeNode *arr_expr;
      } arr;

      // CallK
      struct {
          struct treeNode *_id;
          struct treeNode *expr_list;
      } call;

      // VariableK
      struct {
          char *ID;
      };

      // ConstantK
      struct {
          int NUM;
      };

      // TokenTypeK
      struct {
          TokenType TOK;
      };
  } attr;
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif

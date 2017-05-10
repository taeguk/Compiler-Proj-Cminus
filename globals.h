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
#include "cm.tab.h"
#endif

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

    // Error codes
    ParameterRange,
    StatementRange,
    ExpressionRange,
    VariableRange,
} NodeKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

typedef struct treeNode
{
  struct treeNode* sibling;
  int lineno;
  NodeKind nodeKind;
  union {
      // VariableDeclarationK
      // ArrayDeclarationK
      // FunctionDeclarationK
      // VariableParameterK
      // ArrayParameterK
      // CallK
      // ArrayK
      struct {
          struct treeNode *type_specifier;
          struct treeNode *_id;
          union {
              struct treeNode *_num; // ArrayDeclarationK
              struct { // FunctionDeclarationK
                  struct treeNode *params;
                  struct treeNode *compound_stmt;
              };
              struct treeNode *expression_list; // CallK
              struct treeNode *array_expression; // ArrayK
          };
      };

      // CompoundStatementK
      struct {
          struct treeNode *local_declarations;
          struct treeNode *statement_list;
      };

      // ExpressionStatementK
      // SelectionStatementK
      // IterationStatementK
      // ReturnStatementK
      // AssignExpressionK
      struct {
          struct treeNode *expression;
          union {
              struct { // SelectionStatementK
                  struct treeNode *if_statement;
                  struct treeNode *else_statement;
              };
              struct treeNode *loop_statement; // IterationStatementK
              struct treeNode *var; // AssignExpressionK
          };
      };

      // ComparisonExpressionK
      // AdditiveExpressionK
      // MultiplicativeExpressionK
      struct {
          struct treeNode *left_expression;
          struct treeNode *operator_specifier;
          struct treeNode *right_expression;
      };

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
          TokenType token_type;
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

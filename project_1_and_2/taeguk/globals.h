/****************************************************/
/* File: globals.h                                  */
/* Global types and vars for TINY compiler          */
/* must come before other include files             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define YYDEBUG 1

#ifndef YYPARSER
#include "cminus.tab.h"
#define ENDFILE 0
#endif

typedef int TokenType;

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 6

/*
#define YYTOKENTYPE
typedef enum yytokentype 
    // book-keeping tokens 
   {ENDFILE,ERROR,
    // reserved words
    IF,ELSE,INT,RETURN,VOID,WHILE,
    // multicharacter tokens 
    ID,NUM,
    // special symbols
    PLUS,MINUS,TIMES,OVER,
    LT,LTEQ,GT,GTEQ,EQ,NOTEQ,
    ASSIGN,
    SEMI,COMMA,
    LPAREN,RPAREN,LBRACE,RBRACE,LBRACKET,RBRACKET, // ( ) { } [ ]
    COMMENT, COMMENT_ERROR
   } TokenType;
*/

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {StmtK, ExpK, DeclK, TypeK} NodeKind;
typedef enum {CompK, SelectK, IterK, RetK} StmtKind;
typedef enum {AssignK, OpK, ConstK, IdK, IdArrK, CallK} ExpKind;
typedef enum {FuncK, VarK, VarArrK} DeclKind;
typedef enum {TypeSpecK} TypeKind;

/* ExpType is used for type checking */
typedef enum {Void, Integer} ExpType;

#define MAXCHILDREN 3

typedef struct arrayAttr
  {
    char *name;
    int len; /* -1 means an array in parameter. */
  } ArrayAttr;

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt;
             ExpKind exp; 
             DeclKind decl; 
             TypeKind type; } kind;
     union { TokenType op;
             int val;
             char * name;
             TokenType typeSpec;
             ArrayAttr arrAttr; } attr;
     ExpType type; /* for type checking of exps */
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

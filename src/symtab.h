/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

/* the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;


/* The record in the bucket lists for
 * each variable, including name,
 * assigned memory location, and
 * the list of line numbers in which
 * it appears in the source code
 */
typedef struct BucketListRec
   {
     char * name;
     LineList lines;
     TreeNode *tree_node;
     SymbolInfo *symbolInfo;
     int scope_level;
     // TODO: 'memloc' will be considered in project 4.
     int memloc; /* memory location for variable */

     struct BucketListRec * next;
   } * BucketList;


int st_push_scope(void);
int st_pop_scope(void);

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */

void st_register(char * name, int lineno, SymbolInfo * symbolInfo);
void st_refer(char * name, int lineno);

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
SymbolInfo* st_lookup ( char * name, int * is_cur_scope /* 0 or 1 */ );

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing);

#endif

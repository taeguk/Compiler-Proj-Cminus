/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler*/
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

#define MAX_SCOPE_LEVEL 100

#define VALID_HASH_ARRAY_SIZE (MAX_SCOPE_LEVEL * 200)

/* SHIFT is the power of two used as multiplier
   in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* the hash table */

static BucketList hashTable[SIZE];
static int valid_hash_arr[VALID_HASH_ARRAY_SIZE];
static int valid_hash_arr_base[MAX_SCOPE_LEVEL];  // scope level -> valid_hash_arr base index.
static int valid_hash_arr_cnt;
static int cur_scope_level = 0;

int st_push_scope(void)
{
  if (cur_scope_level + 1 >= MAX_SCOPE_LEVEL)
    return -1;

  valid_hash_arr_base[++cur_scope_level] = valid_hash_arr_cnt;

  return 0;
}

int st_pop_scope(void)
{
  if (cur_scope_level - 1 < 0)
    return -1;

  int i;
  for (i = valid_hash_arr_base[cur_scope_level];
       i < valid_hash_arr_cnt; ++i)
    {
      int h = valid_hash_arr[i];
      BucketList l = hashTable[h];
      
      while ((l != NULL) && l->scope_level >= cur_scope_level)
        {
          struct BucketListRec *tmp = l;
          l = l->next;

          free(tmp);
        }
      hashTable[h] = l;
    }

  valid_hash_arr_cnt = valid_hash_arr_base[cur_scope_level--];

  return 0;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_register(char * name, int lineno, SymbolInfo * symbolInfo)
{
  assert(name != NULL);
  assert(symbolInfo != NULL);

  int h = hash(name);
  BucketList l =  hashTable[h];

  while (l != NULL && l->scope_level == cur_scope_level && 
         strcmp(name,l->name) != 0)
    l = l->next;

  assert(l == NULL || l->scope_level < cur_scope_level);

  MALLOC(l, sizeof(struct BucketListRec));
  l->name = name;
  MALLOC(l->lines, sizeof(struct LineListRec));
  l->lines->lineno = lineno;
  l->symbolInfo = symbolInfo;
  l->lines->next = NULL;
  l->next = hashTable[h];
  l->scope_level = cur_scope_level;
  valid_hash_arr[valid_hash_arr_cnt++] = h;
  hashTable[h] = l;
}

void st_refer(char * name, int lineno)
{
  assert(name != NULL);

  int h = hash(name);
  BucketList l =  hashTable[h];

  while (l != NULL && strcmp(name,l->name) != 0)
    l = l->next;

  assert(l != NULL);

  LineList t = l->lines;
  while (t->next != NULL) 
    t = t->next;
  MALLOC(t->next, sizeof(struct LineListRec));
  t->next->lineno = lineno;
  t->next->next = NULL;
}

/* Function st_lookup returns the memory
 * location of a variable or -1 if not found
 */
SymbolInfo* st_lookup ( char * name, int * is_cur_scope /* 0 or 1 */ )
{
  int h = hash(name);
  BucketList l =  hashTable[h];

  while ((l != NULL) && (strcmp(name,l->name) != 0))
    {
      l = l->next;
    }

  if (l != NULL)
    {
      *is_cur_scope = (cur_scope_level == l->scope_level);
    }

  return l ? l->symbolInfo : NULL;
}

typedef enum { VAR, PAR, FUNC } ID_TYPE;
typedef enum { DT_VOID, DT_INT, DT_ARRAY, DT_INVALID } DATA_TYPE;

DATA_TYPE 
set_data_type (TokenType op)
{
  switch (op)
    {
    case INT:
      return DT_INT;
      break;
    case VOID:
      return DT_VOID;
      break;
    default:
      DONT_OCCUR_PRINT;
      return DT_INVALID;
      break;
    }
}

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
void printSymTab(FILE * listing)
{
  int i;
  fprintf(listing,
          "Name\tScope\tLoc\tV/P/F\tArray?\tArrSize\tType\tLine Numbers\n"
          "--------------------------------------------------------------------\n");

  for (i=0;i<SIZE;++i)
    {
      BucketList l;

      // skip symbols in deeper scopes.
      for (l = hashTable[i]; 
           l && l->scope_level > cur_scope_level;
           l = l->next);

      for (; 
           l && l->scope_level == cur_scope_level; 
           l = l->next)
        {
          SymbolInfo* symbolInfo = l->symbolInfo;
          int is_arr, size_arr = 0;


          /* print name */
          fprintf(listing, "%-7s ", l->name);

          /* print scope */
          fprintf(listing, "%-8d", l->scope_level);

          /* print Memory Location */
          fprintf(listing, "%-6d  ", l->memloc);

          /* print ID Type, V/P/F = 0,1,2 */
          switch (symbolInfo->nodeType)
            {
            case IntT:
              if (symbolInfo->attr.intInfo.isParam)
                fprintf(listing, "%-8s", "Par");
              else
                fprintf(listing, "%-8s", "Var");

              fprintf(listing, "%-8s%-8c%-5s", "No", '-', "int");
              break;
            case IntArrayT:
              if (symbolInfo->attr.arrInfo.isParam)
                fprintf(listing, "%-8s", "Par");
              else
                fprintf(listing, "%-8s", "Var");

              fprintf(listing, "%-8s%-8d%-8s",
                      "Array", symbolInfo->attr.arrInfo.len, "array");
              break;
            case FuncT: /* Function */
              fprintf(listing, "%-8s%-8s%-8c", "Func", "No", '-');
              if (symbolInfo->attr.funcInfo.retType == IntT)
                fprintf(listing, "%-5s", "int");
              else if (symbolInfo->attr.funcInfo.retType == VoidT)
                fprintf(listing, "%-5s", "void");
              else
                DONT_OCCUR_PRINT;
              break;
            default:
              DONT_OCCUR_PRINT;
              break;
            }

          /* line numbers */
          LineList t = l->lines;
          while (t != NULL)
            {
              fprintf(listing,"%8d",t->lineno);
              t = t->next;
            }
          fprintf(listing, "\n");
        }
    }
  fprintf(listing, "\n");
} /* printSymTab */

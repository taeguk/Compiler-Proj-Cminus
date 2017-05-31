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
     int scope_level;
     // TODO: 'memloc' will be considered in project 4.
     int memloc; /* memory location for variable */ 
     
     struct BucketListRec * next;
   } * BucketList;

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

          // TODO: print some infomations.
          free(tmp);
        }
    }

  valid_hash_arr_cnt = valid_hash_arr_base[cur_scope_level--];

  return 0;
}

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
void st_insert( char * name, int lineno, TreeNode *node, int loc )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* variable not yet in table */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->tree_node = node;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = hashTable[h];
    l->scope_level = cur_scope_level;
    valid_hash_arr[valid_hash_arr_cnt++] = h;
    hashTable[h] = l; }
  else /* found in table, so just add line number */
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */
int st_lookup ( char * name, int * is_cur_scope /* 0 or 1 */ )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else 
    {
      *is_cur_scope = (cur_scope_level == l->scope_level);
      return l->memloc;
    }
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
      for (l = hashTable[i]; 
           l && l->scope_level == cur_scope_level; 
           l = l->next)
        {
          TreeNode *node = l->tree_node;

          if (!node)
            continue;

          int is_arr, size_arr = 0;
          ID_TYPE id_type = -1; /* (Var,Par,Func) (0,1,2) */
          DATA_TYPE data_type; /* (void, int, array) (0,1,2) */

          switch (node->nodeKind)
            {
              /* Declaration */
            case VariableDeclarationK:
              is_arr = FALSE;
              id_type = VAR;
              data_type = set_data_type(node->attr.varDecl.type_spec->attr.TOK);
              break; /* VariableDeclarationK */

            case ArrayDeclarationK:
              is_arr = TRUE;
              id_type = VAR;
              size_arr = node->attr.arrDecl._num->attr.NUM;
              data_type = DT_ARRAY;
              break; /* ArrayDeclarationK */

            case FunctionDeclarationK:
              is_arr = FALSE;
              id_type = FUNC;
              data_type = set_data_type(node->attr.funcDecl.type_spec->attr.TOK);
              break; /* FunctionDeclarationK */

              /* Parameter */
            case VariableParameterK:
              is_arr = FALSE;
              id_type = PAR;
              data_type = set_data_type(node->attr.varParam.type_spec->attr.TOK);
              break; /* VariableParameterK */

            case ArrayParameterK:
              is_arr = TRUE;
              id_type = PAR;
              size_arr = 0;
              data_type = DT_ARRAY;
              break;
            default:
              fprintf(listing, "No nodeKind, %d\n", node->nodeKind);
            }

          /* print name */
          fprintf(listing, "%-7s ", l->name);

          /* print scope */
          fprintf(listing, "%-8d", l->scope_level);

          /* print Memory Location */
          fprintf(listing, "%-6d  ", l->memloc);

          /* print ID Type, V/P/F = 0,1,2 */
          switch (id_type)
            {
            case VAR: /* variable */
              fprintf(listing, "%-8s", "Var");
              break;
            case PAR: /* Parameter */
              fprintf(listing, "%-8s", "Par");
              break;
            case FUNC: /* Function */
              fprintf(listing, "%-8s", "Func");
              break;
            default:
              fprintf(listing, "No id_type, %-8d", id_type);
              break;
            }

          /* is_arr with Array size if it is array */
          if (is_arr)
            fprintf(listing, "%-8s%-8d", "Array", size_arr);
          else
            fprintf(listing, "%-8s%-8c", "No", '-');

          /* data type of ID: void, int, array(0, 1, 2) */
          switch (data_type)
            {
            case DT_VOID:
              fprintf(listing, "%-5s", "void");
              break;
            case DT_INT:
              fprintf(listing, "%-5s", "int");
              break;
            case DT_ARRAY:
              fprintf(listing, "%-5s", "array");
              break;
            case DT_INVALID:
              fprintf(listing, "%-5s", "Invalid");
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

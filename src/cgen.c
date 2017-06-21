#include "globals.h"
#include "cgen.h"

static void localCodeGen(TreeNode *, FILE *, int);
static int globalMemAlloc(int);
static int localMemAlloc(int);
static int funcLabelAlloc(void);

// Global decls
void codeGen(TreeNode *syntaxTree, FILE *codeStream)
{
  int i;
  TreeNode *t;
  for(t = syntaxTree;
      t != NULL;
      t = t->sibling)
    {
      if(t->nodeKind == VariableDeclarationK)
        t->symbolInfo->attr.intInfo.globalmemloc = globalMemAlloc(sizeof(int));
      else if(t->nodeKind == ArrayDeclarationK)
        t->symbolInfo->attr.arrInfo.globalmemloc = globalMemAlloc(sizeof(int) * t->symbolInfo->attr.arrInfo.arrLen);
      else if(t->nodeKind == FunctionDeclarationK)
        {
          // Function labeling
          t->symbolInfo->attr.funcInfo.label = funcLabelAlloc();
          fprintf(codeStream, "# Function declaration\n");
          fprintf(codeStream, "f%08x:", t->symbolInfo->attr.funcInfo.label);

          // Function parameters total memory
          TreeNode *param;
          int accLoc = 0;
          for(param = t->attr.funcDecl.params;
              param != NULL;
              param = param->sibling)
            {
              if(param->symbolInfo->nodeType == IntT)
                {
                  param->symbolInfo->attr.intInfo.memloc = accLoc;
                  accLoc += sizeof(int);
                }
              else if(param->symbolInfo->nodeType == IntArrayT)
                {
                  param->symbolInfo->attr.arrInfo.memloc = accLoc;
                  accLoc += sizeof(int) * param->symbolInfo->attr.arrInfo.arrLen;
                }
              else
                {
                  DONT_OCCUR_PRINT;
                }
            }

          // allocate stack
          fprintf(codeStream, "# Allocate stack\n");
          fprintf(codeStream, "addiu $sp, $sp, (-%d)\n", accLoc + 9*4);

          // save registers $s0~$s7
          fprintf(codeStream, "# Save registers s0~s7\n");
          for(i=0; i<8; ++i)
            fprintf(codeStream, "sw $s%d, %d($sp)\n", i, accLoc + i*4);

          // save register $ra
          fprintf(codeStream, "# Save register ra\n");
          fprintf(codeStream, "sw $ra, %d($sp)\n", accLoc + 8*4);

          // cmpd statement generation
          fprintf(codeStream, "# Compound statement for function\n");
          localCodeGen(t->attr.funcDecl.cmpd_stmt, codeStream, accLoc + 9*4);

          // cleanup
          // load registers $s0~$s7
          fprintf(codeStream, "# Load registers s0~s7\n");
          for(i=0; i<8; ++i)
            fprintf(codeStream, "lw $s%d, %d($sp)\n", i, accLoc + i*4);

          // load register $ra
          fprintf(codeStream, "# Load register ra\n");
          fprintf(codeStream, "lw $ra, %d($sp)\n", accLoc + 8*4);

          // free stack
          fprintf(codeStream, "# Free stack\n");
          fprintf(codeStream, "addiu $sp, $sp, (%d)\n", accLoc + 9*4);

          // return
          fprintf(codeStream, "# Return to caller\n");
          fprintf(codeStream, "jr $ra\n");
        }
      else
        DONT_OCCUR_PRINT;
    }
}

// Global memory always starts with 0x10000000
static int globalMemAlloc(int size)
{
  if (size <= 0) DONT_OCCUR_PRINT;
  static int addr = 0x10000000;
  addr += size;
  return addr-size;
}

// Local memory always starts with 0x7fffffff
static int localMemAlloc(int size)
{
  static int addr = 0x7fffffff;
  addr -= size;
  return addr;
}

// Function label generator
static int funcLabelAlloc(void)
{
  static int label = 0x00000000;
  return label++;
}

// Local decls
static void localCodeGen(TreeNode *syntaxTree, FILE *codeStream, int currStack)
{
  TreeNode *t;
  for(t = syntaxTree;
      t != NULL;
      t = t->sibling)
    {
      switch (t->nodeKind)
        {
        }
    }
}
